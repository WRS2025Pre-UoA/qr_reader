#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "zbar_detector.hpp"

namespace fs = std::filesystem;
using namespace std;
using namespace cv;

int main() {
    string input_dir = "../QR_image/20250908_QR";  // あなたの画像フォルダ
    int total_count = 0;
    int success_count = 0;

    vector<String> image_paths;
    glob(input_dir + "/*.jpg", image_paths, false);

    for (const auto& path : image_paths) {
        Mat img = imread(path);
        if (img.empty()) continue;

        total_count++;
        string image_name = fs::path(path).filename().string();

        string data = detect_qrcode(img);  // ← 統合関数を使う
        if (!data.empty()) {
            cout << "[" << image_name << "] ✅ QR検出成功: " << data << endl;
            success_count++;
        } else {
            cout << "[" << image_name << "] ❌ QR検出失敗" << endl;
        }
    }

    cout << "\n📊 QRコード検出結果" << endl;
    cout << "  成功数: " << success_count << " / " << total_count << endl;
    cout << "  成功率: " << (100.0 * success_count / total_count) << "%" << endl;

    return 0;
}
