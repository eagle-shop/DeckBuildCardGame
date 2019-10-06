#include <iostream>
#include "General.hpp"
#include "Controller.hpp"
#include "EventVtoC.hpp"
#import "P2P.h"

using namespace std;

// コンストラクタ
Controller::Controller(SKScene *scene):
    _observer(NULL),
    _scene(scene),
    _dragInfo({NODRAGGING, &STRNOTHING, {}, 0}),
    _touchFrameInfo({}){
    cout << "create Controller" << endl;
}

Controller::~Controller(){
    cout << "delete Controller" << endl;
};

// オブザーバ登録
void Controller::addObserver(Observer_forModel *observer){
    _observer = observer;
}

////////////////////////////////////////////////////////////
// Viewにタッチフレームを設定するたびに通知される
// メモ：タッチ情報削除はControllerへの通知前に行う
////////////////////////////////////////////////////////////
void Controller::update(EVENT_VtoC *event){
    // 引数そのままプッシュ
    _touchFrameInfo.push_back({*(event->touchFrameName), event->moveFlg, *(event->parentName), event->blk});
}

////////////////////////////////////////////////////////////
// メモ：なぞり対象でなければONトリガ
//      なぞり対象ならタッチOFFトリガ
//      長押しは未定・・・
////////////////////////////////////////////////////////////

// タッチON
void Controller::touchesBegan(NSSet *touches, UIEvent *event){
    // タッチ座標とタッチノードを取得
    CGPoint toucheLocation = [touches.anyObject locationInNode:_scene];
    SKNode *toucheNode = [_scene nodeAtPoint:toucheLocation];

    // 名前無しノードなら何もせず終了
    if(toucheNode.name == nil){
        return;
    }

    // タッチノード名をNSString(objc) -> string(c++)へ変換
    const string tmpNodeName = toucheNode.name.UTF8String;

    // タッチノードが登録済みのタッチフレームか探す
    for(auto& touchFrameInfo : _touchFrameInfo){
        // なぞり対象でない場合
        if((tmpNodeName == touchFrameInfo.touchFrameName) && (touchFrameInfo.moveFlg == TOUCHMOVE_DISABLE)){
            cout << "touchesBegan : " << tmpNodeName << endl;
            // Modelへタッチ通知
            notify(&(touchFrameInfo.touchFrameName), TAP, &STRNOTHING);
            return;
        }
        // なぞり対象の場合
        if((tmpNodeName == touchFrameInfo.touchFrameName) && (touchFrameInfo.moveFlg == TOUCHMOVE_ENABLE)){
            // ドラッグに備えて各種情報を保持
            _dragInfo.dragNode = &(touchFrameInfo.parentName);  // なぞり操作で動かす親ノード名
            _dragInfo.oldLocation = toucheLocation;             // タッチ座標
            _dragInfo.oldTimestamp = event.timestamp;           // タッチ時間
            _dragInfo.dragFlg = NODRAGGING;                     // まだドラッグの判断できないのでfalse
            return;
        }
    }
}

// なぞり
void Controller::touchesMoved(NSSet *touches, UIEvent *event){
    // 親ノード名が未設定なら何もしない
    if(*(_dragInfo.dragNode) == STRNOTHING){
        return;
    }

    // ドラッグ中
    _dragInfo.dragFlg = DRAGGING;

    // タッチ座標を取得
    CGPoint toucheLocation = [touches.anyObject locationInNode:_scene];

    // ドラッグアニメ作成。相対位置に相対時間かけて移動する。
    SKAction *dragAnime = [SKAction moveByX:(toucheLocation.x - _dragInfo.oldLocation.x) y:(toucheLocation.y - _dragInfo.oldLocation.y) duration:(event.timestamp - _dragInfo.oldTimestamp)];

    // 親ノードを検索
    SKNode *node = [_scene childNodeWithName:[NSString stringWithFormat:@"//%s", _dragInfo.dragNode->c_str()]];
    // 親ノードを一時的に前面に描画
    node.zPosition = 99.0f;
    // アニメ実行
    [node runAction:dragAnime];
    // 次のドラッグに備えて情報保持
    _dragInfo.oldLocation = toucheLocation;
    _dragInfo.oldTimestamp = event.timestamp;
}

// タッチOFF
void Controller::touchesEnded(NSSet *touches, UIEvent *event){
    // 親ノード名が未設定なら、既にタッチ消化済みorタッチフレームをタッチONしなかったので何もしない
    if(*(_dragInfo.dragNode) == STRNOTHING){
        return;
    }

    // タッチ座標とタッチノードを取得
    CGPoint toucheLocation = [touches.anyObject locationInNode:_scene];
    SKNode *toucheNode = [_scene nodeAtPoint:toucheLocation];

    // 名前無しノードなら終了（ここには来ないはずだが、フェール）
    if(toucheNode.name == nil){
        // Modelへ通知
        notify(&STRNOTHING, NOTHING, &STRNOTHING);
        return;
    }

    // ドラッグしてからのタッチOFF
    if(_dragInfo.dragFlg == true){
        cout << "touchesMovedEnded : " << *(_dragInfo.dragNode) << endl;
/*        // test 試しにここでプレイエリア判定してみる
        SKNode *playArea = [_scene childNodeWithName:[NSString stringWithFormat:@"//%s", PLAYAREA.c_str()]];
        SKNode *trashArea = [_scene childNodeWithName:[NSString stringWithFormat:@"//%s", TRASHAREA.c_str()]];
        string moveTo;
        if([playArea containsPoint:location] == true){
            moveTo = PLAYAREA;
        }
        else if([trashArea containsPoint:location] == true){
            moveTo = TRASHAREA;
        }
 */
        // ドロップエリア判定（containsPointは重なってるノードで上手く使えないのでコレで・・）
        const string *moveTo = &STRNOTHING;
        {
            // ドロップしたノード配列を取得
            NSArray<SKNode*> *tmpDropNodes = [_scene nodesAtPoint:toucheLocation];
            for(auto i = 0; i < tmpDropNodes.count; i++){
                if(tmpDropNodes[i].name == nil){
                    continue;
                }
                const string tmpDropNode = tmpDropNodes[i].name.UTF8String;
                if(tmpDropNode == PLAYAREA){
                    moveTo = &PLAYAREA;
                }
                else if(tmpDropNode == TRASHAREA){
                    moveTo = &TRASHAREA;
                }
            }
        }
        // Modelへ通知
        notify(_dragInfo.dragNode, DRAG, moveTo);
        return;
    }

    // ドラッグせずにタッチOFF
    if(_dragInfo.dragFlg == false){
        // タッチノード名をNSString(objc) -> string(c++)へ変換
        const string tmpNodeName = toucheNode.name.UTF8String;

        // タッチノードが登録済みのタッチフレームか探す
        for(auto& touchFrameInfo : _touchFrameInfo){
            if(tmpNodeName == touchFrameInfo.touchFrameName){
                cout << "touchesEnded : " << tmpNodeName << endl;
                // Modelへ通知
                notify(&(touchFrameInfo.touchFrameName), TAP, &STRNOTHING);
                return;
            }
        }
        // Modelへ通知
        notify(&STRNOTHING, NOTHING, &STRNOTHING);
        return;
    }
}

void Controller::otherEvent(NSData *data){
    cout << "otherEvent" << endl;

    EVENT_CtoM tmp = {};
    [data getBytes:&tmp length:sizeof(tmp)];

    // Modelへ通知
    _observer->update(&tmp);
}

void Controller::notify(const string *opeNord, TAPORDRAG TapOrDrag, const string *moveTo){
    // 通知データ作成
    EVENT_CtoM tmp = {};
    tmp.self.TapOrDrag = TapOrDrag;
    tmp.self.dragMoveTo = moveTo;
    tmp.self.opeIndex = 0;
    tmp.eventKind = EVENT_SELF;

    // ノード名からエリアを判定
    const string *tmpArea = &STRNOTHING;
    {
        vector<const string*> tmpPartString = ALLAREA;
        getString(&tmpArea, opeNord, &tmpPartString);
        tmp.self.area = tmpArea;
    }
    // ノード名から操作対象を判定
    const string *tmpOpeElement = &STRNOTHING;
    {
        vector<const string*> tmpPartString = ALLELE;
        getString(&tmpOpeElement, opeNord, &tmpPartString);
        tmp.self.opeElement = tmpOpeElement;
    }
    // 操作対象のカード番号を取得
    // 親ノードを検索
    SKNode *node = [_scene childNodeWithName:[NSString stringWithFormat:@"//%s", _dragInfo.dragNode->c_str()]];
    if(node != nil){
        const string s = node.name.UTF8String;
        // スペースの次からカード番号なので、それを抜き出す
        auto num = s.find(" ");
        if(num != string::npos){
            tmp.self.opeIndex = stoi(s.substr(num, s.size() - num));
        }
    }

    // 親ノードは初期化（他のはしなくて大丈夫なはず）
    _dragInfo.dragNode = &STRNOTHING;
    // タッチ情報初期化
    _touchFrameInfo.clear();

    // Modelへ通知
    tmp.receiveData.phase = _observer->update(&tmp);

    // P2P接続先に通知
    tmp.eventKind = EVENT_P2P_RECEIVEDATA;
    [P2P sendData:([NSData dataWithBytes:&tmp length:sizeof(tmp)])];
}
