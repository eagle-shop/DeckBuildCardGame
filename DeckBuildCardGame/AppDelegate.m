#import "AppDelegate.h"
#import "ViewController.h"
#import "P2P.h"

@implementation AppDelegate{
    P2P *g_p2p;  // 暫定
}

-(BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions{
    // 一番初めに来るやつ
    // 全画面サイズ取得（[[UIScreen mainScreen] bounds]）
    // UIWindow生成（[UIWindow alloc] initWithFrame:）
    self.window = [UIWindow.alloc initWithFrame:(UIScreen.mainScreen).bounds];
    // ViewController生成
    id viewController = ViewController.new;
    // viewControllerをrootViewControllerに指定
    self.window.rootViewController = viewController;
    // 背景を白色に設定
    self.window.backgroundColor = UIColor.whiteColor;
    // key windowの表示
    [self.window makeKeyAndVisible];
    // P2Pセットアップ
    g_p2p = P2P.new;
    [g_p2p setup];

    return YES;
}

-(void)applicationWillResignActive:(UIApplication*)application{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

-(void)applicationDidEnterBackground:(UIApplication*)application{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

-(void)applicationWillEnterForeground:(UIApplication*)application{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

-(void)applicationDidBecomeActive:(UIApplication*)application{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    // アクティブ時に来るやつ
    [g_p2p drawBrowser:(ViewController*)self.window.rootViewController]; // 暫定
}

-(void)applicationWillTerminate:(UIApplication*)application{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
@end
