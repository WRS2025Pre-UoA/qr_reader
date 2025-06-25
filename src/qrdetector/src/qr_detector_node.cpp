#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <filesystem>

using std::placeholders::_1;
using namespace cv;
using namespace zbar;

class QRDetectorNode : public rclcpp::Node {
public:
    QRDetectorNode() : Node("qr_detector_node"), save_count_(1) {
        // サブスクライバの作成
        subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/image_raw", 10, std::bind(&QRDetectorNode::image_callback, this, _1));

        // 保存ディレクトリ作成
        output_dir_ = std::string(std::getenv("HOME")) + "/QR_success";
        if (!std::filesystem::exists(output_dir_)) {
            std::filesystem::create_directories(output_dir_);
        }

        // ZBarスキャナの初期化
        scanner_.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);

        RCLCPP_INFO(this->get_logger(), "QR Detector Node started");
    }

private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg) {
        // ROS → OpenCV画像へ変換
        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
        } catch (cv_bridge::Exception& e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }

        Mat frame = cv_ptr->image;
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // ZBar形式に変換してスキャン
        Image zbarImage(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
        int n = scanner_.scan(zbarImage);

        if (n > 0) {
            for (auto symbol = zbarImage.symbol_begin(); symbol != zbarImage.symbol_end(); ++symbol) {
                std::string data = symbol->get_data();
                RCLCPP_INFO(this->get_logger(), "QRコード検出: %s", data.c_str());

                // 保存処理
                std::string filename = output_dir_ + "/No" + std::to_string(save_count_++) + ".png";
                imwrite(filename, frame);
                RCLCPP_INFO(this->get_logger(), "保存しました: %s", filename.c_str());
                break;
            }
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
    ImageScanner scanner_;
    std::string output_dir_;
    int save_count_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<QRDetectorNode>());
    rclcpp::shutdown();
    return 0;
}
