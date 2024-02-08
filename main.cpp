#include <opencv2/opencv.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "nadjieb/mjpeg_streamer.hpp"





// for convenience
using MJPEGStreamer = nadjieb::MJPEGStreamer;

int main() {
   // cv::VideoCapture cap(0);
  //  if (!cap.isOpened()) {
  //      std::cerr << "VideoCapture not opened\n";
  //      exit(EXIT_FAILURE);
  //  }

    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};

    MJPEGStreamer streamer;

    // By default "/shutdown" is the target to graceful shutdown the streamer
    // if you want to change the target to graceful shutdown:
    //      streamer.setShutdownTarget("/stop");

    // By default std::thread::hardware_concurrency() workers is used for streaming
    // if you want to use 4 workers instead:
    //      streamer.start(8080, 4);
    streamer.start(8080);
int i=0;

    // Visit /shutdown or another defined target to stop the loop and graceful shutdown
    while (streamer.isRunning()) {
        cv::Mat frame;

      //  cap >> frame;
       /* if (frame.empty()) {
            std::cerr << "frame not grabbed\n";
            exit(EXIT_FAILURE);
        }
*/



        // http://localhost:8080/bgr
        std::vector<uchar> buff_bgr;
i=i+1;
        frame = cv::imread( "D:\\QtProect\\video\\imgLeft_1004.bmp" );

        cv::putText(frame, //target image
                    std::to_string(i), //text
                    cv::Point(10, frame.rows / 5), //top-left position
                    cv::FONT_HERSHEY_DUPLEX,
                    1.0,
                    CV_RGB(0, 0, 0), //font color
                    2);

        cv::imencode(".jpg", frame, buff_bgr, params);
        streamer.publish("/bgr", std::string(buff_bgr.begin(), buff_bgr.end()));




        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // http://localhost:8080/hsv
        std::vector<uchar> buff_hsv;
        cv::imencode(".jpg", hsv, buff_hsv, params);
        streamer.publish("/hsv", std::string(buff_hsv.begin(), buff_hsv.end()));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    streamer.stop();
}
