#ifndef View_hpp
#define View_hpp

#include "Observer.hpp"
#include "EventVtoC.hpp"
#import <SpriteKit/SpriteKit.h>

constexpr int CIRCLESIZE        = 9;        // カード枚数の円のサイズ
constexpr int FONTSIZE          = 12;       // フォントサイズ

const string FONTSET            = "American Typewriter Bold";   // フォント
const string TOUCH              = "touch";

// ノード名定義
const string NODE_ACTIONCOUNT   = "actionCount";
const string NODE_BUYCOUNT      = "buyCount";
const string NODE_COINCOUNT     = "coinCount";

class View : virtual public Observer_forView{
public:
    View(SKScene *scene);
    ~View();
    void addObserver(Observer_forController *observer);
    void update(EVENT_MtoV *event);
    void init();
    
private:
    void drawABCcount(ABCCOUNT *abcCount);
    void drawPhaseSW(Phase phase);
    void drawOtherSW(Phase phase);
    //    void drawPopup(string popup);
    void drawSupplyCards(vector<SUPPLYCARDS> *supplyCards);
    void drawTrashCards(CARDNUM cardNum);
    void drawPlayerCards(PLAYERCARDS *playerCards);
    void delNode(vector<const string*> *name);
    void createNotifyData(const string *touchFrameName, TOUCHMOVE moveFlg, const string *parentName, EVENT_VtoC *event);
    SKSpriteNode* drawCard(const string *cardName, CGPoint position, CGPoint anchor, SKNode *drawNode, const string *nodeName, DrawState drawState);
    SKShapeNode* drawCircleNum(int num, SKNode *drawNode);
    
    //////////////////////////////
    // プライベート変数
    //////////////////////////////
    Observer_forController  *_observer;
    SKScene                 *_scene;
};

#endif /* View_hpp */

