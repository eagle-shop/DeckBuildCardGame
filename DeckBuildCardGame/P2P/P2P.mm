#include <iostream>
#include "General.hpp"
#include "Model.hpp"
#include "EventCtoM.hpp"
#import <MultipeerConnectivity/MultipeerConnectivity.h>
#import "ViewController.h"
#import "WSpriteKit.h"
#import "P2P.h"

using namespace std;

MCSession *_session = nil;

@implementation P2P{
    ////////////////////////////////////////
    // P2P用変数保持
    ////////////////////////////////////////
    MCPeerID *_peerID;
    MCBrowserViewController *_browser;
    MCAdvertiserAssistant *_assistant;
}

-(void)setup{
    // セッション設定
    _peerID = [MCPeerID.alloc initWithDisplayName:UIDevice.currentDevice.name];
    _session = [MCSession.alloc initWithPeer:_peerID];
    _session.delegate = self;
    // ブラウザ設定
    _browser = [MCBrowserViewController.alloc initWithServiceType:@"p2pTest" session:_session];
    _browser.delegate = self;
    // アシスタント設定
    _assistant = [MCAdvertiserAssistant.alloc initWithServiceType:@"p2pTest" discoveryInfo:nil session:_session];
    [_assistant start];
}

-(void)drawBrowser:(ViewController*)view{
    // 未接続の場合
    if(_session.connectedPeers.count <= 0){
        // ブラウザ表示
        [view presentViewController:_browser animated:true completion:nil];
    }
}

// ブラウザ完了SW
-(void)browserViewControllerDidFinish:(MCBrowserViewController*)browserViewController{
    [browserViewController dismissViewControllerAnimated:true completion:nil];
}

// ブラウザキャンセルSW
-(void)browserViewControllerWasCancelled:(MCBrowserViewController*)browserViewController{
    [browserViewController dismissViewControllerAnimated:true completion:nil];
}

// NSData受信（とりあえず1対1前提）
-(void)session:(MCSession*)session didReceiveData:(NSData*)data fromPeer:(MCPeerID*)peerID{
    // 同一セッション、かつピアIDがセッションに存在する場合
    if((_session == session) && ([_session.connectedPeers containsObject:peerID])){
        cout << "session didReceiveData" << endl;
        // キューにイベントを積む
        [WSKScene addEventQue:data];
    }
}

// NSURL受信開始（未使用）
-(void)session:(MCSession*)session didStartReceivingResourceWithName:(NSString*)resourceName fromPeer:(MCPeerID*)peerID withProgress:(NSProgress*)progress{
}

// NSURL受信完了（未使用）
-(void)session:(MCSession*)session didFinishReceivingResourceWithName:(NSString*)resourceName fromPeer:(MCPeerID*)peerID atURL:(NSURL*)localURL withError:(NSError*)error{
}

// NSInputStream受信（未使用）
-(void)session:(MCSession*)session didReceiveStream:(NSInputStream*)stream withName:(NSString*)streamName fromPeer:(MCPeerID*)peerID{
}

// 状態変化
-(void)session:(MCSession*)session peer:(MCPeerID*)peerID didChangeState:(MCSessionState)state{
    switch(state){
        // 未接続
        case MCSessionStateNotConnected:
            cout << "session MCSessionStateNotConnected" << endl;
            break;
        // 接続中
        case MCSessionStateConnecting:
            cout << "session MCSessionStateConnecting" << endl;
            break;
        // 接続完了
        case MCSessionStateConnected:
            cout << "session MCSessionStateConnected" << endl;
            {
                EVENT_CtoM tmp = {};
                tmp.eventKind = EVENT_P2P_CONNECTED;
                tmp.receiveData.phase = PHASE_UNKNOWN;
                // MCPeerIDを元に、プレイヤー番号（先行後攻）を決める -> イマイチ上手くできない・・・
/*
                if(_session.connectedPeers[0] > _peerID){
                    tmp.playerNo = 0;   // 先行
                }
                else{
                    tmp.playerNo = 1;   // 後攻
                }
*/
                TIME systime = NSDate.timeIntervalSinceReferenceDate;
                tmp.time = systime;
                Model::setTime(systime);
                [P2P sendData:([NSData dataWithBytes:&tmp length:sizeof(tmp)])];
            }
            break;
        // nop
        default:
            break;
    }
}

// NSData送信
+(void)sendData:(NSData*)data{
    NSError *error = nil;
    [_session sendData:data toPeers:_session.connectedPeers withMode:MCSessionSendDataReliable error:&error];
}

@end
