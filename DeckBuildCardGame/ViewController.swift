import UIKit

class ViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    private var tableView: UITableView!
    private var items = [
        NSLocalizedString("play", comment: ""),
        NSLocalizedString("deck editing", comment: ""),
        NSLocalizedString("settings", comment: ""),
    ]

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.items.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = self.tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath)
        cell.textLabel!.text = self.items[indexPath.row]
        return cell
    }

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch self.items[indexPath.row] {
        case NSLocalizedString("play", comment: ""):
            print(NSLocalizedString("play", comment: ""))
        case NSLocalizedString("deck editing", comment: ""):
            print(NSLocalizedString("deck editing", comment: ""))
        case NSLocalizedString("settings", comment: ""):
            print(NSLocalizedString("settings", comment: ""))
        default:
            break
        }
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.

        self.view.backgroundColor = UIColor.cyan

        self.tableView = UITableView(frame: self.view.bounds, style: UITableView.Style.plain)
        self.tableView.register(UITableViewCell.self, forCellReuseIdentifier: "Cell")
        self.tableView.autoresizingMask = [UIView.AutoresizingMask.flexibleWidth, UIView.AutoresizingMask.flexibleHeight]
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.view.addSubview(self.tableView)

        let label = UILabel(frame: CGRect(origin: CGPoint(x: 0, y: 0), size: CGSize(width: 200, height: 50)))
        label.text = "Test"
        label.textColor = UIColor.black
//        self.view.addSubview(label)
        self.tableView.addSubview(label)

//        let languages = NSLocale.preferredLanguages
//        print(languages)

//        let locale = Locale.current.languageCode ?? "en"
//        print(locale)

//        print(NSLocalizedString("play", comment: ""))
    }

/*    override func loadView() {
    }
*/
    override func viewWillAppear(_ animated: Bool) {
    }
}

