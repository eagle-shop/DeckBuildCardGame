#include <iostream>
#include <queue>
#include <mutex>
#include "Model.hpp"
#include "View.hpp"
#include "Controller.hpp"
#import "ViewController.h"
#import "WSpriteKit.h"
#import "P2P.h"

using namespace std;

NSMutableArray<NSData*> *_eventQue = NSMutableArray.array;// イベントキュー。描画スレッドとP2Pの受信スレッドで排他必須
mutex _mtx;

@implementation WSKScene{
    ////////////////////////////////////////
    // MVCの保持
    ////////////////////////////////////////
    Model       *_model;
    View        *_view;
    Controller  *_controller;
}

-(void)setup{
    _model      = new Model();
    _view       = new View(self);
    _controller = new Controller(self);

    _model->addObserver(_view);
    _view->addObserver(_controller);
    _controller->addObserver(_model);

    _view->init();
    _model->init();
}

+(void)addEventQue:(NSData*)data{
    _mtx.lock();
    [_eventQue addObject:data];
    _mtx.unlock();
}
////////////////////////////////////////
// タッチイベント処理
////////////////////////////////////////
-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event{
    _controller->touchesBegan(touches, event);
}

-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event{
    _controller->touchesMoved(touches, event);
}

-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event{
    _controller->touchesEnded(touches, event);
}

////////////////////////////////////////
// タッチイベント以外はここで受ける
////////////////////////////////////////
-(void)update:(NSTimeInterval)currentTime{
    _mtx.lock();
    if(_eventQue.count > 0){
        // controllerで処理実施
        _controller->otherEvent(_eventQue[0]);
        // イベント消費
        [_eventQue removeObjectAtIndex:0];
    }
    _mtx.unlock();
}

// 解放処理（呼ばれる？）
-(void)dealloc{
    cout << "WSKScene dealloc" << endl;
    _mtx.lock();
    _eventQue = NSMutableArray.array;
    _mtx.unlock();
    delete _model;
    delete _view;
    delete _controller;
}
@end
