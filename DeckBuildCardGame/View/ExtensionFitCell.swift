import UIKit

extension UIImage {
    // Square images only
    func fitCell(height: CGFloat) -> UIImage {
        let resizedSize = CGSize(width: (height * 0.5), height: (height * 0.5))
        UIGraphicsBeginImageContextWithOptions(resizedSize, false, 0.0)
        draw(in: CGRect(x: 0, y: 0, width: resizedSize.width, height: resizedSize.height))
        let resizedImage = UIGraphicsGetImageFromCurrentImageContext() ?? UIImage()
        UIGraphicsEndImageContext()
        return resizedImage
    }
}
