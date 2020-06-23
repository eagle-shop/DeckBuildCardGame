import UIKit

class DeckEditingViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    private var m_tableView: UITableView!
    private var m_items: [String] = []
    private var m_alert: UIAlertController!
    private var m_alertOkButton: UIAlertAction!
    private var m_model: Model!

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        let test: NSMutableArray = []
        m_model.getSetsData(test)
        m_items = test as! [String]
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

        m_model = Model()

        // Table Initialization
        m_tableView = UITableView(frame: self.view.bounds, style: UITableView.Style.plain)
        m_tableView.register(UITableViewCell.self, forCellReuseIdentifier: "Cell")
        m_tableView.autoresizingMask = [UIView.AutoresizingMask.flexibleWidth, UIView.AutoresizingMask.flexibleHeight]
        m_tableView.delegate = self
        m_tableView.dataSource = self
        self.view.addSubview(m_tableView)

        // NavigationBar Initialization
        self.navigationItem.title = NSLocalizedString("set", comment: "")
        self.navigationItem.rightBarButtonItem = UIBarButtonItem(image: UIImage(named: "Add")?.fitNavigationBar(height: (self.navigationController?.navigationBar.frame.height)!), style: UIBarButtonItem.Style.plain, target: self, action: #selector(addButton))

        // Alert Initialization
        m_alert = UIAlertController(title: NSLocalizedString("please enter a set name", comment: ""), message: nil, preferredStyle: UIAlertController.Style.alert)
        m_alert.addTextField()
        m_alert.textFields?.first?.addTarget(self, action: #selector(changeText), for: UIControl.Event.allEvents)

        m_alertOkButton = UIAlertAction(title: "OK", style: UIAlertAction.Style.default){ action in
//            let text = self.m_alert.textFields?.first?.text ?? ""

            // Creating a set (handled by the Model)
            self.m_model.createSet(self.m_alert.textFields?.first?.text ?? "")
            self.m_tableView.reloadData()

            self.m_alert.textFields?.first?.text = ""
        }
        m_alert.addAction(m_alertOkButton)

        let cancel = UIAlertAction(title: "Cancel", style: UIAlertAction.Style.cancel, handler: nil)
        m_alert.addAction(cancel)
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        if let selectedRow = m_tableView.indexPathForSelectedRow {
            m_tableView.deselectRow(at: selectedRow, animated: true)
        }
    }

    @objc internal func addButton() {
        self.present(m_alert, animated: true, completion: nil)
    }

    @objc internal func changeText() {
        if m_alert.textFields?.first?.text?.count ?? 0 > 0 {
            m_alertOkButton.isEnabled = true
        } else {
            m_alertOkButton.isEnabled = false
        }
    }
}
