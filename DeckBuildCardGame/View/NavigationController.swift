import UIKit

class NavigationController: UINavigationController, ViewDelegate{
    func play() {
        print(NSLocalizedString("play", comment: ""))
    }
    
    func deckEditing() {
        print(NSLocalizedString("deck editing", comment: ""))
        self.pushViewController(DeckEditingViewController(), animated: true)
    }
    
    func settings() {
        print(NSLocalizedString("settings", comment: ""))
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.backgroundColor = UIColor.cyan
        self.pushViewController(RootViewController(delegate: self), animated: true)
    }
}
