////////////////////////////////////////
// Controller
////////////////////////////////////////
#ifndef Controller_hpp
#define Controller_hpp

#include <vector>
#include <string>
#include "Observer.hpp"
#include "EventVtoC.hpp"
#import <SpriteKit/SpriteKit.h>

enum DRAGFLG : bool{
    DRAGGING    = true,
    NODRAGGING  = false,
};

//////////////////////////////
// タッチ情報（1ノード）
//////////////////////////////
typedef struct{
    const std::string   touchFrameName; // タッチフレーム名
    TOUCHMOVE           moveFlg;        // なぞり操作可能か？
    const std::string   parentName;     // なぞり操作で動かす親ノード名
    void                (^blk)(void);   // ブロック処理（ドラッグなら完了後、タップなら押下時）
}TOUCHFRAMEINFO;

//////////////////////////////
// ドラッグ情報
//////////////////////////////
typedef struct{
    DRAGFLG             dragFlg;        // ドラッグ中ならtrue
    const std::string   *dragNode;      // ドラッグ中のノード
    CGPoint             oldLocation;    // 前回座標
    NSTimeInterval      oldTimestamp;   // 前回時間
}DRAGINFO;

//////////////////////////////
// Controllerクラス
//////////////////////////////
class Controller : virtual public Observer_forController{
public:
    //////////////////////////////
    // パブリックメソッド
    //////////////////////////////
    Controller(SKScene *scene);
    ~Controller();
    void addObserver(Observer_forModel *observer);
    void update(EVENT_VtoC *event);
    void touchesBegan(NSSet *touches, UIEvent *event);
    void touchesMoved(NSSet *touches, UIEvent *event);
    void touchesEnded(NSSet *touches, UIEvent *event);
    void otherEvent(NSData *data);

private:
    //////////////////////////////
    // プライベートメソッド
    //////////////////////////////
    void notify(const std::string *opeNord, TAPORDRAG TapOrDrag, const std::string *moveTo);

    //////////////////////////////
    // プライベート変数
    //////////////////////////////
    Observer_forModel           *m_observer;        // Modelへのupdate通知用
    id                          m_scene;            // ルートノード（シーン）
    DRAGINFO                    m_dragInfo;         // ドラッグ情報管理用
    std::vector<TOUCHFRAMEINFO> m_touchFrameInfo;   // タッチ情報管理用
};

#endif /* Controller_hpp */
