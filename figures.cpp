#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
using namespace cv;

int main() {

	setlocale(LC_ALL, "Russian");
	Mat img = imread("C:/Users/User/Downloads/123123123.png");
	Mat blur_img, blur_img2, gray, edges, edges2;
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	std::string shapeName;
	RNG rng(1234);

	if (img.empty()) {
		std::cout << "Не удалось загрузить изображение " << std::endl;
		return -1;
	}

	cvtColor(img, gray, COLOR_RGBA2GRAY, 0);
	GaussianBlur(gray, blur_img, Size(3, 3), 0);
	Canny(blur_img, edges, 50, 100);
	findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	Mat drawing = Mat::zeros(edges.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
		Moments M = moments(contours[i]);
		Point centroid(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
		double peri = arcLength(contours[i], true);
		std::vector<Point> approx;
		approxPolyDP(contours[i], approx, 0.04 * peri, true);
		int vertices = static_cast<int>(approx.size());

		if (vertices == 3) {
			shapeName = "Triangle";
			putText(drawing, shapeName, centroid, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
		}
		else if (vertices == 4) {
			Rect rect = boundingRect(contours[i]);
			double aspectRatio = static_cast<double>(rect.width) / rect.height;
			shapeName = (aspectRatio >= 0.95 && aspectRatio <= 1.05) ? "Square" : "Rectangle";
			putText(drawing, shapeName, centroid, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
		}
		else {
			shapeName = "Circle";
			putText(drawing, shapeName, centroid, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
		}
	}

	imshow("the first but not player", img);
	imshow("Contours", drawing);

	waitKey(0);

	return 0;
}