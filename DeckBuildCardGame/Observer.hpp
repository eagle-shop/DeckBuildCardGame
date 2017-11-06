#ifndef Observer_hpp
#define Observer_hpp

#include "EventMtoV.hpp"
#include "EventVtoC.hpp"
#include "EventCtoM.hpp"

// とりあえずなので手抜きオブザーバ

// Model用のオブザーバ抽象クラス
class Observer_forModel{
public:
    virtual ~Observer_forModel(){};
    virtual Phase update(EVENT_CtoM *event) = 0;  // 通知データは要検討
};

// View用のオブザーバ抽象クラス
class Observer_forView{
public:
    virtual ~Observer_forView(){};
    virtual void update(EVENT_MtoV *event) = 0;
};

// Controller用のオブザーバ抽象クラス
class Observer_forController{
public:
    virtual ~Observer_forController(){};
    virtual void update(EVENT_VtoC *event) = 0;
};

#endif /* Observer_hpp */
