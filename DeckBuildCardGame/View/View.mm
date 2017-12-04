////////////////////////////////////////
// View
// メモ：あとでクラス化していくつもり・・・
////////////////////////////////////////
#include <iostream>
#include <map>
#include "General.hpp"
#include "View.hpp"

constexpr int CARDW = 6;

View::View(SKScene *scene):_scene(scene){
    cout << "create View" << endl;
};

View::~View(){
    cout << "delete View" << endl;
};

void View::addObserver(Observer_forController *observer){
    _observer = observer;
}

void View::update(EVENT_MtoV *event){
    // サプライのカード描画
    drawSupplyCards(&event->supplyCards);
    // 廃棄置き場の描画
    drawTrashCards(event->trashCards.size());
    // 山札、手札、捨札、プレイエリアのカード描画
    drawPlayerCards(&event->playerCards);
    // アクション、購入、コインのラベル描画
    drawABCcount(&event->abcCount);
    // アクション終了、購入終了、ターン終了のSW描画
    drawPhaseSW(event->phase);
    // 完了、キャンセルSWの描画
    drawOtherSW(event->phase);
    // ポップアップの描画
    //    drawPopup(event->popup);
}

// 一度だけ描画するもの達
void View::init(){
    // プレイエリアの描画
    CGFloat w = _scene.frame.size.width;
    CGFloat h = _scene.frame.size.height / CARDW;
    SKSpriteNode *playArea = [SKSpriteNode spriteNodeWithColor:SKColor.grayColor size:CGSizeMake(w, h)];
    playArea.anchorPoint = CGPointMake(0.0, 0.0);
    playArea.position = CGPointMake(0, (_scene.frame.size.height * 3.1) / 9);
    playArea.name = [NSString stringWithFormat:@"%s", PLAYAREA.c_str()];
    [_scene addChild:playArea];
}

// アクション数、購入数、コイン数の描画
void View::drawABCcount(ABCCOUNT *abcCount){
    {
        // まず前回描画したノードを削除
        vector<const string*> tmp = {&NODE_ACTIONCOUNT, &NODE_BUYCOUNT, &NODE_COINCOUNT};
        delNode(&tmp);
    }
    {
        // action
        SKLabelNode *action = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        action.text = [NSString stringWithFormat:@"a:%d", (int)abcCount->action];
        action.fontSize = FONTSIZE;
        action.fontColor = SKColor.whiteColor;
        action.position = CGPointMake(_scene.frame.size.width - 135, (_scene.frame.size.height * 2) / 8);
        action.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeRight;
        action.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        action.name = [NSString stringWithFormat:@"%s", NODE_ACTIONCOUNT.c_str()];
        [_scene addChild:action];
    }
    {
        // buy
        SKLabelNode *buy = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        buy.text = [NSString stringWithFormat:@"b:%d", (int)abcCount->buy];
        buy.fontSize = FONTSIZE;
        buy.fontColor = SKColor.whiteColor;
        buy.position = CGPointMake(_scene.frame.size.width - 135, ((_scene.frame.size.height  * 2) / 8) - 20);
        buy.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeRight;
        buy.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        buy.name = [NSString stringWithFormat:@"%s", NODE_BUYCOUNT.c_str()];
        [_scene addChild:buy];
    }
    {
        // coin
        SKLabelNode *coin = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        coin.text = [NSString stringWithFormat:@"c:%d", (int)abcCount->coin];
        coin.fontSize = FONTSIZE;
        coin.fontColor = SKColor.whiteColor;
        coin.position = CGPointMake(_scene.frame.size.width - 135, ((_scene.frame.size.height * 2) / 8) - 40);
        coin.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeRight;
        coin.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        coin.name = [NSString stringWithFormat:@"%s", NODE_COINCOUNT.c_str()];
        [_scene addChild:coin];
    }
}

// フェーズ関連SWの描画
void View::drawPhaseSW(Phase phase){
    {
        // まず前回描画したノードを削除
        vector<const string*> tmp = {&PHASE};
        delNode(&tmp);
    }
    // フェーズ関連SWの親ノード作っておく
    SKSpriteNode *phase_ = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
    phase_.anchorPoint = CGPointMake(0.0, 0.0);
    phase_.name = [NSString stringWithFormat:@"%s", PHASE.c_str()];
    [_scene addChild:phase_];
    
    if(phase == PHASE_ACTION){
        // アクション終了SW描画
        // とりあえずべた塗りのRectを描画
        SKSpriteNode *rect = [SKSpriteNode spriteNodeWithColor:SKColor.greenColor size:CGSizeMake(100, 20)];
        rect.anchorPoint = CGPointMake(0.0, 0.0);
        rect.position = CGPointMake(10, (_scene.frame.size.height * 2) / 7.5);
        [phase_ addChild:rect];
        
        // 文字を描画
        SKLabelNode *endAction = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        endAction.text = [NSString stringWithFormat:@"アクション終了"];
        endAction.fontSize = FONTSIZE;
        endAction.fontColor = SKColor.blackColor;
        endAction.position = CGPointMake(rect.frame.size.width / 2, rect.frame.size.height / 2);
        endAction.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeCenter;
        endAction.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        [rect addChild:endAction];
        
        // タッチ用フレーム描画
        SKSpriteNode *touchFrame = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(100, 20)];
        touchFrame.anchorPoint = CGPointMake(0.0, 0.0);
        touchFrame.name = [NSString stringWithFormat:@"%s%s", PHASE.c_str(), PHASESWENDACTION.c_str()];
        [rect addChild:touchFrame];
        
        // 通知データ
        const string name = touchFrame.name.UTF8String;
        EVENT_VtoC tmp = {};
        createNotifyData(&name, TOUCHMOVE_DISABLE, &STRNOTHING, &tmp);
        _observer->update(&tmp);
    }
    if(phase == PHASE_BUY){
        // 購入終了SW描画
        // とりあえずべた塗りのRectを描画
        SKSpriteNode *rect = [SKSpriteNode spriteNodeWithColor:SKColor.greenColor size:CGSizeMake(100, 20)];
        rect.anchorPoint = CGPointMake(0.0, 0.0);
        rect.position = CGPointMake(10, ((_scene.frame.size.height * 2) / 7.5) - 30);
        [phase_ addChild:rect];
        
        // 文字を描画
        SKLabelNode *endBuy = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        endBuy.text = [NSString stringWithFormat:@"購入終了"];
        endBuy.fontSize = FONTSIZE;
        endBuy.fontColor = SKColor.blackColor;
        endBuy.position = CGPointMake(rect.frame.size.width / 2, rect.frame.size.height / 2);
        endBuy.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeCenter;
        endBuy.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        [rect addChild:endBuy];
        
        // タッチ用フレーム描画
        SKSpriteNode *touchFrame = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(100, 20)];
        touchFrame.anchorPoint = CGPointMake(0.0, 0.0);
        touchFrame.name = [NSString stringWithFormat:@"%s%s", PHASE.c_str(), PHASESWENDBUY.c_str()];
        [rect addChild:touchFrame];
        
        // 通知データ
        const string name = touchFrame.name.UTF8String;
        EVENT_VtoC tmp = {};
        createNotifyData(&name, TOUCHMOVE_DISABLE, &STRNOTHING, &tmp);
        _observer->update(&tmp);
    }
    if(phase != PHASE_OTHERPLAYER){
        // とりあえずべた塗りのRectを描画
        SKSpriteNode *rect = [SKSpriteNode spriteNodeWithColor:SKColor.greenColor size:CGSizeMake(100, 20)];
        rect.anchorPoint = CGPointMake(0.0, 0.0);
        rect.position = CGPointMake(10, ((_scene.frame.size.height * 2) / 7.5) - 60);
        [phase_ addChild:rect];
        
        // 文字を描画
        SKLabelNode *endTurn = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        endTurn.text = [NSString stringWithFormat:@"ターン終了"];
        endTurn.fontSize = FONTSIZE;
        endTurn.fontColor = SKColor.blackColor;
        endTurn.position = CGPointMake(rect.frame.size.width / 2, rect.frame.size.height / 2);
        endTurn.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeCenter;
        endTurn.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        [rect addChild:endTurn];
        
        // タッチ用フレーム描画
        SKSpriteNode *touchFrame = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(100, 20)];
        touchFrame.anchorPoint = CGPointMake(0.0, 0.0);
        touchFrame.name = [NSString stringWithFormat:@"%s%s", PHASE.c_str(), PHASESWENDTURN.c_str()];
        [rect addChild:touchFrame];
        
        // 通知データ
        const string name = touchFrame.name.UTF8String;
        EVENT_VtoC tmp = {};
        createNotifyData(&name, TOUCHMOVE_DISABLE, &STRNOTHING, &tmp);
        _observer->update(&tmp);
    }
}

void View::drawOtherSW(Phase phase){
    {
        // まず前回描画したノードを削除
        vector<const string*> tmp = {&OTHER};
        delNode(&tmp);
    }
    if(phase != PHASE_ACTIONWAIT){
        return;
    }
    // その他SWの親ノード作っておく
    SKSpriteNode *phase_ = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
    phase_.anchorPoint = CGPointMake(0.0, 0.0);
    phase_.name = [NSString stringWithFormat:@"%s", OTHER.c_str()];
    [_scene addChild:phase_];
    
    {
        // とりあえずべた塗りのRectを描画
        SKSpriteNode *rect = [SKSpriteNode spriteNodeWithColor:SKColor.greenColor size:CGSizeMake(80, 20)];
        rect.anchorPoint = CGPointMake(0.0, 0.0);
        rect.position = CGPointMake(120, ((_scene.frame.size.height * 2) / 7));
        [phase_ addChild:rect];
        
        // 文字を描画
        SKLabelNode *endTurn = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        endTurn.text = [NSString stringWithFormat:@"完了"];
        endTurn.fontSize = FONTSIZE;
        endTurn.fontColor = SKColor.blackColor;
        endTurn.position = CGPointMake(rect.frame.size.width / 2, rect.frame.size.height / 2);
        endTurn.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeCenter;
        endTurn.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        [rect addChild:endTurn];
        
        // タッチ用フレーム描画
        SKSpriteNode *touchFrame = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(80, 20)];
        touchFrame.anchorPoint = CGPointMake(0.0, 0.0);
        touchFrame.name = [NSString stringWithFormat:@"%s%s", OTHER.c_str(), OTHERSWCOMPLETE.c_str()];
        [rect addChild:touchFrame];
        
        // 通知データ
        const string name = touchFrame.name.UTF8String;
        EVENT_VtoC tmp = {};
        createNotifyData(&name, TOUCHMOVE_DISABLE, &STRNOTHING, &tmp);
        _observer->update(&tmp);
    }
    {
        // とりあえずべた塗りのRectを描画
        SKSpriteNode *rect = [SKSpriteNode spriteNodeWithColor:SKColor.greenColor size:CGSizeMake(80, 20)];
        rect.anchorPoint = CGPointMake(0.0, 0.0);
        rect.position = CGPointMake(120, ((_scene.frame.size.height * 2) / 7) - 30);
        [phase_ addChild:rect];
        
        // 文字を描画
        SKLabelNode *endTurn = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
        endTurn.text = [NSString stringWithFormat:@"キャンセル"];
        endTurn.fontSize = FONTSIZE;
        endTurn.fontColor = SKColor.blackColor;
        endTurn.position = CGPointMake(rect.frame.size.width / 2, rect.frame.size.height / 2);
        endTurn.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeCenter;
        endTurn.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
        [rect addChild:endTurn];
        
        // タッチ用フレーム描画
        SKSpriteNode *touchFrame = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(80, 20)];
        touchFrame.anchorPoint = CGPointMake(0.0, 0.0);
        touchFrame.name = [NSString stringWithFormat:@"%s%s", OTHER.c_str(), OTHERSWCHANSEL.c_str()];
        [rect addChild:touchFrame];
        
        // 通知データ
        const string name = touchFrame.name.UTF8String;
        EVENT_VtoC tmp = {};
        createNotifyData(&name, TOUCHMOVE_DISABLE, &STRNOTHING, &tmp);
        _observer->update(&tmp);
    }
}
/*
 void View::drawPopup(string popup){
 }
 */
// サプライの描画
void View::drawSupplyCards(vector<SUPPLYCARDS> *supplyCards){
    {
        // まず前回描画したノードを削除
        vector<const string*> tmp = {&SUPPLY};
        delNode(&tmp);
    }
    // サプライの親ノード作っておく
    SKSpriteNode *supply = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
    supply.anchorPoint = CGPointMake(0.0, 0.0);
    supply.name = [NSString stringWithFormat:@"%s", SUPPLY.c_str()];
    [_scene addChild:supply];
    
    CGFloat w = _scene.frame.size.width / CARDW;
    CGFloat h = 0;
    
    // とりあえず左上から詰めて表示、CARDW枚で折り返す
    auto countY = ((supplyCards->size() % CARDW) > 0 ? 1 : 0) + (supplyCards->size() / CARDW);
    for(auto i = 0; i < countY; i++){
        SKSpriteNode *tmpNode;
        auto countX = (((supplyCards->size() - (CARDW * i)) / CARDW) != 0 ? CARDW : (supplyCards->size() % CARDW));
        for(auto j = 0; j < countX; j++){
            // サプライカード・タッチフレーム描画
            const string nodeName = SUPPLY + (*((*supplyCards)[(i * CARDW) + j].cardName));
            CGPoint position = CGPointMake(j * w, (_scene.frame.size.height - 20) - (h * i));
            CGPoint anchor = CGPointMake(0, 1.0);
            tmpNode = drawCard(&(*(*supplyCards)[(i * CARDW) + j].cardName), position, anchor, supply, &nodeName, DRAW_NOMAL);
            
            SKSpriteNode *circlePoint = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
            circlePoint.position = CGPointMake(tmpNode.size.width / tmpNode.xScale - CIRCLESIZE, (-1) * tmpNode.size.height / tmpNode.yScale + CIRCLESIZE);
            [tmpNode addChild:circlePoint];
            drawCircleNum((int)(*supplyCards)[(i * CARDW) + j].num, circlePoint);
            
            // 通知データ
            const string name = tmpNode.name.UTF8String;
            EVENT_VtoC tmp = {};
            createNotifyData(&name, TOUCHMOVE_DISABLE, &STRNOTHING, &tmp);
            _observer->update(&tmp);
        }
        h = tmpNode.size.height / tmpNode.yScale;
    }
}

void View::drawTrashCards(CARDNUM cardNum){
    {
        // まず前回描画したノードを削除
        vector<const string*> tmp = {&TRASHCARD};
        delNode(&tmp);
    }
    
    // 廃棄置き場の描画
    const string cardName = TRASHCARD;
    CGPoint position = CGPointMake(_scene.frame.size.width * ((CARDW - 2.0) / CARDW), (_scene.frame.size.height * 2) / 12);
    CGPoint anchor = CGPointMake(0.0, 0.0);
    SKSpriteNode *trashCard = drawCard(&cardName, position, anchor, _scene, &cardName, DRAW_NOMAL);
    
    // 山札の残り枚数を描画
    SKSpriteNode *circlePoint = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
    circlePoint.position = CGPointMake(trashCard.size.width / trashCard.xScale - CIRCLESIZE, CIRCLESIZE);
    [trashCard addChild:circlePoint];
    drawCircleNum((int)cardNum, circlePoint);
    
    // 通知データ
    EVENT_VtoC tmp = {};
    const string name = trashCard.name.UTF8String;
    createNotifyData(&name, TOUCHMOVE_DISABLE, &cardName, &tmp);
    _observer->update(&tmp);
}

// プレイヤーカードの描画
void View::drawPlayerCards(PLAYERCARDS *playerCards){
    {
        // まず前回描画したノードを削除
        vector<const string*> tmp = {&DECK, &HAND, &PLAY, &DISCARD};
        delNode(&tmp);
    }
    const CGFloat w = _scene.frame.size.width / CARDW;
    {
        // 山札の親ノード作っておく
        SKSpriteNode *deck = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
        deck.anchorPoint = CGPointMake(0.0, 0.0);
        deck.name = [NSString stringWithFormat:@"%s", DECK.c_str()];
        [_scene addChild:deck];
        
        // 山札の描画
        const string cardName = BACKCARD;
        CGPoint position = CGPointMake(w * (CARDW - 1.0), (_scene.frame.size.height * 2) / 12);
        CGPoint anchor = CGPointMake(0.0, 0.0);
        SKSpriteNode *deckCard = drawCard(&cardName, position, anchor, deck, &cardName, DRAW_NOMAL);
        
        // 山札の残り枚数を描画
        SKSpriteNode *circlePoint = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
        circlePoint.position = CGPointMake(deckCard.size.width / deckCard.xScale - CIRCLESIZE, CIRCLESIZE);
        [deckCard addChild:circlePoint];
        drawCircleNum((int)playerCards->deckCardNum, circlePoint);
    }
    {
        // 手札の親ノード作っておく
        SKSpriteNode *hand = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
        hand.anchorPoint = CGPointMake(0.0, 0.0);
        hand.name = [NSString stringWithFormat:@"%s", HAND.c_str()];
        [_scene addChild:hand];
        
        // CARDW枚より多い場合は、間隔を詰める
        CGFloat n = w;
        if(playerCards->handCards.handCardsName.size() > CARDW){
            n = (_scene.frame.size.width - w) / (playerCards->handCards.handCardsName.size() - 1);
        }
        
        // 手札描画
        for(auto i = 0; i < playerCards->handCards.handCardsName.size(); i++){
            // 手札・タッチフレーム描画
            string nodeName = HAND + (*(playerCards->handCards.handCardsName[i]));
            nodeName += (" " + to_string(i));
            CGPoint position = CGPointMake(i * n, 2);
            CGPoint anchor = CGPointMake(0.0, 0.0);
            SKSpriteNode *tmpNode = drawCard(playerCards->handCards.handCardsName[i], position, anchor, hand, &nodeName, playerCards->handCards.drawState[i]);
            
            // 通知データ
            const string name = tmpNode.name.UTF8String;
            EVENT_VtoC tmp = {};
            createNotifyData(&name, TOUCHMOVE_ENABLE, &nodeName, &tmp);
            _observer->update(&tmp);
        }
    }
    {
        // プレイエリアを取得
        SKNode *playArea = [_scene childNodeWithName:[NSString stringWithFormat:@"//%s", PLAYAREA.c_str()]];
        
        // プレイエリアの親ノード作っておく
        SKSpriteNode *play = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(0, 0)];
        play.anchorPoint = CGPointMake(0.0, 0.0);
        play.name = [NSString stringWithFormat:@"%s", PLAY.c_str()];
        [playArea addChild:play];
        
        // CARDW枚より多い場合は、間隔を詰める
        CGFloat n = w;
        if(playerCards->playCardsName.size() > CARDW){
            n = (_scene.frame.size.width - w) / (playerCards->playCardsName.size() - 1);
        }
        
        // プレイエリアカード描画
        for(auto i = 0; i < playerCards->playCardsName.size(); i++){
            // プレイエリアカード・タッチフレーム描画
            string nodeName = PLAY + (*(playerCards->playCardsName[i]));
            nodeName += i;
            CGPoint position;
            position = CGPointMake(i * n, playArea.frame.size.height / 2);
            CGPoint anchor = CGPointMake(0.0, 0.5);
            drawCard(playerCards->playCardsName[i], position, anchor, play, &nodeName, DRAW_NOMAL);
        }
    }
}

// ノード削除
void View::delNode(vector<const string*> *name){
    for(auto x : *name){
        // nilでも大丈夫
        SKNode *del = [_scene childNodeWithName:[NSString stringWithFormat:@"//%s", x->c_str()]];
        [del removeFromParent];
    }
}

void View::createNotifyData(const string *touchFrameName, TOUCHMOVE moveFlg, const string *parentName, EVENT_VtoC *event){
    event->touchFrameName = touchFrameName;
    event->moveFlg = moveFlg;
    event->parentName = parentName;
}

// 指定した名前のカードを、指定したノードと座標に描画
// メモ：サイズは画面幅の(1/CARDW)で固定
SKSpriteNode* View::drawCard(const string *cardName, CGPoint position, CGPoint anchor, SKNode *drawNode, const string *nodeName, DrawState drawState){
    CardInfo &cardInfo = CardInfo::getInstance();
    SKSpriteNode *sprite = [SKSpriteNode spriteNodeWithImageNamed:[NSString stringWithFormat:@"%s", cardInfo._cardInfo[*cardName].fileName.c_str()]];
    sprite.anchorPoint = anchor;
    sprite.position = position;
    sprite.xScale = (_scene.frame.size.width / sprite.size.width) / CARDW;
    sprite.yScale = sprite.xScale;
    sprite.name = [NSString stringWithFormat:@"%s", nodeName->c_str()];
    // 選択状態（赤色）
    if(drawState == DRAW_SELECT){
        sprite.color = SKColor.redColor;
        sprite.colorBlendFactor = 1.0f;
    }
    // トーンダウン状態（灰色）
    else if(drawState == DRAW_TONEDOWN){
        sprite.color = SKColor.grayColor;
        sprite.colorBlendFactor = 1.0f;
    }
    [drawNode addChild:sprite];
    
    // タッチ用フレーム描画
    SKSpriteNode *touchFrame = [SKSpriteNode spriteNodeWithColor:SKColor.clearColor size:CGSizeMake(sprite.size.width, sprite.size.height)];
    touchFrame.anchorPoint = anchor;
    touchFrame.xScale = 1.0 / sprite.xScale;
    touchFrame.yScale = 1.0 / sprite.yScale;
    touchFrame.name = [NSString stringWithFormat:@"%s%s", TOUCH.c_str(), nodeName->c_str()];
    touchFrame.zPosition = 2.0f;
    [sprite addChild:touchFrame];
    
    return touchFrame;
}

SKShapeNode* View::drawCircleNum(int num, SKNode *drawNode){
    SKShapeNode *circle = [SKShapeNode shapeNodeWithCircleOfRadius:CIRCLESIZE];
    circle.fillColor = SKColor.redColor;
    circle.zPosition = 1.0f;
    circle.userInteractionEnabled = NO;
    [drawNode addChild:circle];
    
    SKLabelNode *label = [SKLabelNode labelNodeWithFontNamed:[NSString stringWithFormat:@"%s", FONTSET.c_str()]];
    label.text = [NSString stringWithFormat:@"%d", num];
    label.fontSize = FONTSIZE;
    label.fontColor = SKColor.whiteColor;
    label.position = CGPointMake(0, 0);
    label.horizontalAlignmentMode = SKLabelHorizontalAlignmentModeCenter;
    label.verticalAlignmentMode = SKLabelVerticalAlignmentModeCenter;
    label.zPosition = 1.0f;
    label.userInteractionEnabled = NO;
    [circle addChild:label];
    
    return circle;
}

// Blocksテスト
// メモ：ここでできるのはノードのパラメータ変更のみ。
// アクションの実施はOK、ノードの削除は禁止。
// このとき描画更新を止めないと、ノードが削除されそう
void (^blk)(void) = ^void(void){
    cout << "blk" << endl;
};

