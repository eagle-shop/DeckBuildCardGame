#ifndef ModelBridgeObjC_h
#define ModelBridgeObjC_h

#import <Foundation/Foundation.h>

@interface Model : NSObject

-(void)action;
-(void)getSetsData : (NSMutableArray*) out_data;
-(bool)createSet : (NSString*) name;
@end

#endif /* ModelBridgeObjC_h */
