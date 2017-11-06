#import "ViewController.h"
#import "WSpriteKit.h"

@implementation ViewController

-(void)loadView{
    CGRect applicationFrame = (UIScreen.mainScreen).bounds;
    SKView *skView = [SKView.alloc initWithFrame:applicationFrame];
    self.view = skView;
}

-(void)viewDidLoad{
    [super viewDidLoad];

    SKView *skView = (SKView*)self.view;
    skView.showsDrawCount = YES;
    skView.showsNodeCount = YES;
    skView.showsFPS = YES;

    WSKScene *scene = [WSKScene sceneWithSize:skView.bounds.size];
    scene.name = @"scene";
    scene.backgroundColor = SKColor.blackColor;
    [skView presentScene:scene];
    [scene setup];
}

-(void)didReceiveMemoryWarning{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
@end
