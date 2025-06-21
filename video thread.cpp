#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>

std::atomic<bool> running(true);
std::mutex frame_mutex;
cv::Mat current_frame;

// Function to save a snapshot in a separate thread
void save_snapshot(const cv::Mat& frame) {
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    std::string filename = "snapshot_" + std::to_string(timestamp) + ".jpg";
    cv::imwrite(filename, frame);
    std::cout << "Saved snapshot: " << filename << std::endl;
}

// Main function
int main() {
    cv::VideoCapture cap(0); // 0 for default camera
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open the camera." << std::endl;
        return -1;
    }

    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter video("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30,
                          cv::Size(frame_width, frame_height));

    if (!video.isOpened()) {
        std::cerr << "Error: Cannot open the video file for writing." << std::endl;
        return -1;
    }

    std::cout << "Press 's' to save a snapshot, 'q' to quit." << std::endl;

    while (running) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        {
            std::lock_guard<std::mutex> lock(frame_mutex);
            current_frame = frame.clone();
        }

        video.write(frame);
        cv::imshow("Live Video", frame);

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 'q') {
            running = false;
        } else if (key == 's') {
            std::thread(save_snapshot, current_frame).detach();
        }
    }

    cap.release();
    video.release();
    cv::destroyAllWindows();
    return 0;
}

