#ifndef P2P_hpp
#define P2P_hpp

#import <MultipeerConnectivity/MultipeerConnectivity.h>
#import "ViewController.h"

@interface P2P : NSObject <MCBrowserViewControllerDelegate, MCSessionDelegate>

-(void)drawBrowser:(ViewController*)view;
-(void)setup;
+(void)sendData:(NSData*)data;

@end

#endif /* P2P_hpp */
