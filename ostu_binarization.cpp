// g++ -std=c++17 -o otsu_nichika otsu_nichika.cpp `pkg-config --cflags --libs opencv4`
// ./otsu_nichika


#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>  // ✅ 必須
namespace fs = std::filesystem;

using namespace std;
using namespace cv;

int main() {
    string input_dir = "../dark_flash";

    // 画像パス取得
    vector<String> image_paths;
    glob(input_dir + "/*.png", image_paths, false);

    QRCodeDetector qr_detector;

    int success_count = 0;
    int total_count = 0;

    string output_dir = "output_otsu_only";
    fs::create_directory(output_dir);  // ✅ 修正: fs:: を使う

    for (const auto& path : image_paths) {
        Mat img = imread(path);
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);

        Mat bin_img;
        threshold(gray, bin_img, 0, 255, THRESH_BINARY | THRESH_OTSU);

        string data = qr_detector.detectAndDecode(bin_img);
        total_count++;
        string image_name = fs::path(path).filename().string();  // ✅ 修正: fs:: を使う

        if (!data.empty()) {
            cout << "[" << image_name << "] ✅ QR検出成功: " << data << endl;
            success_count++;
        } else {
            cout << "[" << image_name << "] ❌ QR検出失敗" << endl;
        }

        string out_path = output_dir + "/" + image_name;
        imwrite(out_path, bin_img);
    }

    cout << "\n📊 QRコード検出結果（大津のみ）" << endl;
    cout << "  成功数: " << success_count << " / " << total_count << endl;
    cout << "  成功率: " << (static_cast<double>(success_count) / total_count) * 100 << "%" << endl;

    return 0;
}
