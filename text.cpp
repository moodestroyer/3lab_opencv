#include <opencv2/opencv.hpp>

using namespace cv;

int main() {

    Mat img = imread("C:/Users/User/Downloads/text.jpg");
    Mat image_gray, gauss, edges;

    cvtColor(img, image_gray, COLOR_BGR2GRAY);
    GaussianBlur(image_gray, gauss, Size(5, 5), 0);
    Canny(image_gray, edges, 40, 230);

    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;

    findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    Mat result = Mat::zeros(img.size(), CV_8UC3);

    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(result, contours, i, Scalar(255, 255, 255), 1, LINE_8, hierarchy, 0);
    }

    resize(img, img, Size(1080, 720));
    imshow("original", img);
    resize(result, result, Size(1080, 720));
    imshow("result", result);
    waitKey(0);

    return 0;
}