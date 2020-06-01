import UIKit

class ViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    private var tableView: UITableView!
    private let items = ["1", "2", "3"]

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
        case "1":
            print("1")
        case "2":
            print("2")
        case "3":
            print("3")
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

        let languages = NSLocale.preferredLanguages
        print(languages)

        let locale = Locale.current.languageCode ?? "en"
        print(locale)
        let test = Locale.current.localizedString(forLanguageCode: locale)!
        print(test)
    }

/*    override func loadView() {
    }
*/
    override func viewWillAppear(_ animated: Bool) {
    }
}

