#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;



int angle = 0;
float w = 680;
float h = 480;

Mat src;
Mat _dst2;

void on_mouse5(int event, int x, int y, int flags, void*);
void affine_rotation();



int main(void)
{
	affine_rotation();

	return 0;
}

void on_mouse5(int event, int x, int y, int flags, void*)
{
	if (event == EVENT_LBUTTONDOWN) {
		if (angle >= 360)
			angle = 0;
		angle += 40;
		cout << angle << endl;
	}
	Point2f cp(src.cols / 2.f, src.rows / 2.f);
	Mat M = getRotationMatrix2D(cp, angle, 1);


	vector<Point2f> p_src = { Point2f(src.cols - 1, 0), Point2f(src.cols - 1, src.rows - 1), Point2f(0, src.rows - 1),Point2f(0, 0) };
	vector<Point2f> p_dst;

	transform(p_src, p_dst, M); // 회전시 구해질 사각형의 크기를 미리 구하기 위해서 만든 M 객체이다.


	if (angle < 90 && angle > 0)
	{
		w = p_dst[1].x - p_dst[3].x;
		h = p_dst[2].y - p_dst[0].y;
	}
	else if (angle == 180 || angle == 360 || angle == 0)
	{
		w = 640;
		h = 480;
	}
	else if (angle == 90 || angle == 270)
	{
		w = 480;
		h = 640;
	}
	else if (angle > 90 && angle < 180)
	{
		w = p_dst[2].x - p_dst[0].x;
		h = p_dst[3].y - p_dst[1].y;
	}
	else if (angle > 180 && angle < 270)
	{
		w = p_dst[3].x - p_dst[1].x;
		h = p_dst[0].y - p_dst[2].y;
	}
	else if (angle > 270 && angle < 360)
	{
		w = p_dst[0].x - p_dst[2].x;
		h = p_dst[1].y - p_dst[3].y;
	}

	Mat _dst;
	double pust_w = (w - 640);
	double pust_h = (h - 480);

	if (angle == 90 || angle == 270)
	{
		Mat _move = Mat_<double>({ 2, 3 }, { 1, 0, 40, 0, 1, 120 }); // 회전하기 전 미리 그림을 이동시킨다.
		warpAffine(src, _dst, _move, Size(src.cols + 80, src.cols + 80));  // 그림의 배경을 크게 만든다.

		Point2f _cp(_dst.cols / 2, _dst.rows / 2);
		Mat _M = getRotationMatrix2D(_cp, angle, 1);
		warpAffine(_dst, _dst2, _M, Size());
		cout << endl << _dst2.size << endl;

		_move = Mat_<double>({ 2, 3 }, { 1, 0, -120, 0, 1, -40 });
		warpAffine(_dst2, _dst2, _move, Size(src.rows, src.cols)); // 회전한 그림 사이즈를 맞춘다.

	}
	else
	{
		Mat _move = Mat_<double>({ 2, 3 }, { 1, 0, (pust_w / 2) , 0, 1, (pust_h / 2) });
		warpAffine(src, _dst, _move, Size(src.cols + pust_w, src.rows + pust_h)); // 회전하기 전 미리 그림을 이동시킨다.

		Point2f _cp(_dst.cols / 2, _dst.rows / 2);
		Mat _M = getRotationMatrix2D(_cp, angle, 1);

		warpAffine(_dst, _dst2, _M, Size(w, h));
	}


	imshow("_dst2", _dst2);

	return;
}

void affine_rotation()
{
	src = imread("tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed!" << endl;
		return;
	}

	src.copyTo(_dst2);

	namedWindow("_dst2");
	setMouseCallback("_dst2", on_mouse5);

	imshow("_dst2", _dst2);


	waitKey();
	destroyAllWindows();
}
