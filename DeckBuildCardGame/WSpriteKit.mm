#include <iostream>
#include <mutex>
#include "Model.hpp"
#include "View.hpp"
#include "Controller.hpp"
#import "ViewController.h"
#import "WSpriteKit.h"
#import "P2P.h"

using namespace std;

NSMutableArray<NSData*> *g_eventQue = NSMutableArray.array;// イベントキュー。描画スレッドとP2Pの受信スレッドで排他必須
mutex g_mtx;

@implementation WSKScene{
    ////////////////////////////////////////
    // MVCの保持
    ////////////////////////////////////////
    Model       *m_model;
    View        *m_view;
    Controller  *m_controller;
}

-(void)setup{
    m_model      = new Model();
    m_view       = new View(self);
    m_controller = new Controller(self);

    m_model->addObserver(m_view);
    m_view->addObserver(m_controller);
    m_controller->addObserver(m_model);

    m_model->init();
}

+(void)addEventQue:(NSData*)data{
    g_mtx.lock();
    [g_eventQue addObject:data];
    g_mtx.unlock();
}
////////////////////////////////////////
// タッチイベント処理
////////////////////////////////////////
-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event{
    m_controller->touchesBegan(touches, event);
}

-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event{
    m_controller->touchesMoved(touches, event);
}

-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event{
    m_controller->touchesEnded(touches, event);
}

////////////////////////////////////////
// タッチイベント以外はここで受ける
////////////////////////////////////////
-(void)update:(NSTimeInterval)currentTime{
    g_mtx.lock();
    if(g_eventQue.count > 0){
        // controllerで処理実施
        m_controller->otherEvent(g_eventQue[0]);
        // イベント消費
        [g_eventQue removeObjectAtIndex:0];
    }
    g_mtx.unlock();
}

// 解放処理（呼ばれる？）
-(void)dealloc{
    cout << "WSKScene dealloc" << endl;
    g_mtx.lock();
    g_eventQue = NSMutableArray.array;
    g_mtx.unlock();
    delete m_model;
    delete m_view;
    delete m_controller;
}
@end
