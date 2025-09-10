// zbar_detector.cpp

// g++ -std=c++17 -o zbar_qr_detect main.cpp zbar_detector.cpp \
//      `pkg-config --cflags --libs opencv4` \
//      -I/opt/homebrew/include \
//      /opt/homebrew/lib/libzbar.dylib

#include <opencv2/opencv.hpp>
#include </opt/homebrew/include/zbar.h>
#include "zbar_detector.hpp"

using namespace cv;
using namespace zbar;

// ===== OpenCV用 前処理 =====
cv::Mat preprocess_for_opencv(const cv::Mat& src) {
    cv::Mat gray, eq, resized;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, eq);
    resize(eq, resized, Size(), 2.0, 2.0);  // リサイズで歪みに強く
    return resized;
}

// ===== ZBarのみ（前処理なし） =====
std::string try_zbar(const cv::Mat& image) {
    cv::Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);  // ★前処理なし！

    ImageScanner scanner;
    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);

    Image zbar_img(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
    scanner.scan(zbar_img);

    for (Image::SymbolIterator symbol = zbar_img.symbol_begin(); symbol != zbar_img.symbol_end(); ++symbol) {
        return symbol->get_data();
    }

    return "";
}

// ===== OpenCVのみ（前処理あり） =====
std::string try_opencv(const cv::Mat& image) {
    QRCodeDetector detector;
    std::string result = detector.detectAndDecode(image);
    return result;
}

// ===== 統合関数 =====
std::string detect_qrcode(cv::Mat& image) {
    std::string result = try_zbar(image);
    if (!result.empty()) return result;

    // ZBarで失敗したらOpenCVで試す（湾曲用に前処理）
    cv::Mat processed = preprocess_for_opencv(image);
    result = try_opencv(processed);
    return result;
}
