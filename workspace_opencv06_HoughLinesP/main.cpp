#include "opencv2/opencv.hpp"

//#define DEBUG1

using namespace cv;
using namespace std;

String filePath = "C:/Users/ssssv/source/repos/workspace_opencv00/workspace_opencv00/";
String cardImg = filePath + "card.bmp";
String coinImg = filePath + "coin.jpg";
vector<vector<Point>> point_s(2, vector<Point>(4));

void Set_img(Mat* img, String path);
void Canny_GRAY(Mat* dst, Mat src);
Mat Set_ROI_img(Mat src, Point p1, Point p2, Point p3, Point p4);
void DrawLine(Mat img, vector<Vec4i>line);

int main()
{
	Mat img, edge_img;

	Set_img(&img, cardImg);

	Canny_GRAY(&edge_img, img);

	Mat img1 = Set_ROI_img(edge_img, Point(200, 060), Point(465, 060), Point(465, 200), Point(200, 200));
	Mat img2 = Set_ROI_img(edge_img, Point(390, 110), Point(610, 110), Point(610, 320), Point(390, 320));

	vector<Vec4i>lines_1, lines_2;
	cv::HoughLinesP(img1, lines_1, 1, CV_PI / 180, 18, 25,4);
	cv::HoughLinesP(img2, lines_2, 1, CV_PI / 180, 80, 90, 8);

	DrawLine(img, lines_1);
	DrawLine(img, lines_2);

	cv::imshow("img", img);
#ifdef DEBUG1
	cv::imshow("img1", img1);
	cv::imshow("img2", img2);
#endif // DEBUG1

	

	waitKey(0);
	destroyAllWindows();

	return 0;
}

void Set_img(Mat* img, String path)
{
	*img = imread(path);
	
	if (img->empty())
	{
		cerr << "이미지 로드 실패!";
		return;
	}
}

void Canny_GRAY(Mat* dst, Mat src)
{
	cv::cvtColor(src, *dst, COLOR_BGR2GRAY);
	cv::Canny(*dst, *dst, 480, 1000);
}

Mat Set_ROI_img(Mat src, Point p1, Point p2, Point p3, Point p4)
{
	static int i = 0;
	point_s[i][0] = p1;
	point_s[i][1] = p2;
	point_s[i][2] = p3;
	point_s[i][3] = p4;

	Mat img(480, 640, CV_8UC3, Scalar(0, 0, 0));
	cv::fillPoly(img, { point_s[i] }, Scalar(255, 255, 255), LINE_AA);

	Mat dst;
	cv::cvtColor(src, src, COLOR_GRAY2BGR);
	bitwise_and(src, img, dst);

	i++;

	cv::cvtColor(dst, dst, COLOR_BGR2GRAY);
	return dst;
}

void DrawLine(Mat img, vector<Vec4i>line)
{
	for (Vec4i l : line) {
		cv::line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(125, 125, 255), 2, LINE_AA);
	}
}