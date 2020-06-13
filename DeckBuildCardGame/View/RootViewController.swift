import UIKit

class RootViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    private var m_delegate: ViewDelegate? = nil
    private var m_tableView: UITableView!
    private let m_items = [
        NSLocalizedString("play", comment: ""),
        NSLocalizedString("deck editing", comment: ""),
        NSLocalizedString("settings", comment: ""),
    ]

    init(delegate: ViewDelegate) {
        super.init(nibName: nil, bundle: nil)
        m_delegate = delegate
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return m_items.count
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = m_tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath)
        cell.textLabel?.text = m_items[indexPath.row]
//        cell.textLabel?.font = UIFont.boldSystemFont(ofSize: UIFont.labelFontSize)
        cell.accessoryType = UITableViewCell.AccessoryType.disclosureIndicator

        let imageView = cell.imageView
        switch m_items[indexPath.row] {
        case NSLocalizedString("play", comment: ""):
            imageView?.image = UIImage(named: "Play")?.fitCell(height: cell.frame.height)

        case NSLocalizedString("deck editing", comment: ""):
            imageView?.image = UIImage(named: "Edit")?.fitCell(height: cell.frame.height)

        case NSLocalizedString("settings", comment: ""):
            imageView?.image = UIImage(named: "Settings")?.fitCell(height: cell.frame.height)

        default:
            break
        }

        return cell
    }

/*    func tableView(_ tableView: UITableView, willDisplay cell: UITableViewCell, forRowAt indexPath: IndexPath) {
        print(m_tableView.rowHeight)
    }
*/
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        switch m_items[indexPath.row] {
        case NSLocalizedString("play", comment: ""):
            m_delegate?.play()

        case NSLocalizedString("deck editing", comment: ""):
//            let nextVC = DeckEditingViewController()
//            nextVC.modalTransitionStyle = UIModalTransitionStyle.flipHorizontal
//            nextVC.modalPresentationStyle = UIModalPresentationStyle.currentContext
//            self.present(nextVC, animated: true, completion: nil)
//            self.show(nextVC, sender: nil)
            m_delegate?.deckEditing()

        case NSLocalizedString("settings", comment: ""):
            m_delegate?.settings()

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
        m_tableView.remembersLastFocusedIndexPath = true
        self.view.addSubview(m_tableView)

        // NavigationBar Initialization
        self.navigationItem.title = NSLocalizedString("top", comment: "")
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        if let selectedRow = m_tableView.indexPathForSelectedRow {
            m_tableView.deselectRow(at: selectedRow, animated: true)
        }
    }
}
