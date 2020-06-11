import UIKit

class DeckEditingViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    private var m_delegate: ViewDelegate? = nil
    private var m_tableView: UITableView!
    private let m_items = [
        NSLocalizedString("test", comment: ""),
    ]

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return m_items.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = m_tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath)
        cell.textLabel!.text = m_items[indexPath.row]
        return cell
    }

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch m_items[indexPath.row] {
        case NSLocalizedString("test", comment: ""):
            dismiss(animated: true, completion: nil)

        default:
            break
        }
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        self.view.backgroundColor = UIColor.cyan

        // Table Initialization
        m_tableView = UITableView(frame: self.view.bounds, style: UITableView.Style.plain)
        m_tableView.register(UITableViewCell.self, forCellReuseIdentifier: "Cell")
        m_tableView.autoresizingMask = [UIView.AutoresizingMask.flexibleWidth, UIView.AutoresizingMask.flexibleHeight]
        m_tableView.delegate = self
        m_tableView.dataSource = self
        self.view.addSubview(m_tableView)

//        let label = UILabel(frame: CGRect(origin: CGPoint(x: 0, y: 0), size: CGSize(width: 200, height: 50)))
//        label.text = "Test"
//        label.textColor = UIColor.black

//        self.view.addSubview(label)
//        m_tableView.addSubview(label)

//        let languages = NSLocale.preferredLanguages
//        print(languages)

//        let locale = Locale.current.languageCode ?? "en"
//        print(locale)

//        print(NSLocalizedString("play", comment: ""))

        // NavigationBar Initialization
        self.navigationItem.title = "test2"
    }

/*    override func loadView() {
    }
*/
    override func viewWillAppear(_ animated: Bool) {
    }
}

