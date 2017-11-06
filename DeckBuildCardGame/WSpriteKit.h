#ifndef SpriteKit_h
#define SpriteKit_h

#import <SpriteKit/SpriteKit.h>

@interface WSKScene : SKScene
// オーバーライドメソッド
-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
-(void)update:(NSTimeInterval)currentTime;
-(void)dealloc;

// 自作メソッド
-(void)setup;
+(void)addEventQue:(NSData*)data;
@end

#endif /* SpriteKit_h */
