#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

Mat src;
Mat _src;
Point2f srcQuad[4], _srcQuad[4], dstQuad[4];

double Distance(const Point2f p1, const Point2f p2);
void point_algorithm();
void on_mouse(int event, int x, int y, int flags, void* userdata);

int main()
{
	src = imread("card.bmp");
	_src = src.clone();

	if (src.empty())
	{
		cerr << "Image load fail!" << endl;
		return 0;
	}

	namedWindow("src");
	setMouseCallback("src", on_mouse);
	while (true) {
		imshow("src", src);
		waitKey();
	}
	

	return 0;
}

void on_mouse(int event, int x, int y, int flags, void*)
{
	static int cnt = 0;

	if (event == EVENT_LBUTTONDOWN)
	{
		if (cnt < 4) {
			srcQuad[cnt++] = Point2f(x, y);

			circle(src, Point(x, y), 5, Scalar(0, 0, 255), -1);
			imshow("src", src);

			if (cnt == 4) {
				point_algorithm();
				int w = 400, h = 600;

				dstQuad[0] = Point2f(0, 0);
				dstQuad[1] = Point2f(w - 1, 0);
				dstQuad[2] = Point2f(w - 1, h - 1);
				dstQuad[3] = Point2f(0, h - 1);

				Mat pers = getPerspectiveTransform(_srcQuad, dstQuad);

				Mat dst;
				warpPerspective(src, dst, pers, Size(w, h));

				imshow("dst", dst);
				
				int key = waitKey();
				if (key == 'c' || key == 'C')
				{
					destroyWindow("dst");
					src = _src.clone();
					imshow("src", src);
				}
				else if (key == 't' || key == 'T')
				{
					Point2f temp;
					temp = _srcQuad[0];
					_srcQuad[0] = _srcQuad[3];
					_srcQuad[3] = _srcQuad[2];
					_srcQuad[2] = _srcQuad[1];
					_srcQuad[1] = temp;
					pers = getPerspectiveTransform(_srcQuad, dstQuad);
					warpPerspective(src, dst, pers, Size(w, h));
					imshow("dst", dst);

					int key = waitKey();
					if (key == 'c' || key == 'C')
					{
						destroyWindow("dst");
						src = _src.clone();
						imshow("src", src);
					}

				}

				cnt = 0;
			}
		}
	}
}


void point_algorithm()
{

	Point2f _long;
	Point2f _long2;
	Point2f _long3;
	Point2f _long4;

	Point2f temp;
	srcQuad[0].x > srcQuad[1].x ? _long = srcQuad[0] : _long = srcQuad[1];
	if (srcQuad[2].x > _long.x) _long = srcQuad[2];
	if (srcQuad[3].x > _long.x) _long = srcQuad[3];


	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			if (srcQuad[i] != _long)
				_long2 = srcQuad[0];
			else
				_long2 = srcQuad[1];
		}
	
		if ((i > 0) && (srcQuad[i] != _long) && (_long2.x < srcQuad[i].x))
		{
			_long2 = srcQuad[i];
		}
	}
			
	if (_long.y > _long2.y)
	{
		_srcQuad[2] = _long, _srcQuad[1] = _long2;
	}
	else
	{
		_srcQuad[2] = _long2, _srcQuad[1] = _long;
	}

	for (int i = 0; i < 4; i++) {
		if (i == 0 && srcQuad[i] != _long && srcQuad[i] != _long2)
		{
			_long3 = srcQuad[0];
		}
		else if(i == 0 && srcQuad[1] != _long && srcQuad[1] != _long2)
		{
			_long3 = srcQuad[1];
		}
		else if (i == 0 && srcQuad[2] != _long && srcQuad[2] != _long2)
		{
			_long3 = srcQuad[2];
		}
		else if (i == 0 && srcQuad[3] != _long && srcQuad[3] != _long2)
		{
			_long3 = srcQuad[3];
		}

		if (i > 0 && srcQuad[i] != _long && srcQuad[i] != _long2)
		{
			if (srcQuad[i].x > _long3.x) {
				_long4 = _long3;
				_long3 = srcQuad[i];
			}
			else _long4 = srcQuad[i];
		}
	}

	 if (_long3.y > _long4.y)
	 {
		 _srcQuad[3] = _long3, _srcQuad[0] = _long4;
	 }
	 else
	 {
		 _srcQuad[3] = _long4, _srcQuad[0] = _long3;
	 }


	if (_srcQuad[3].y < _srcQuad[0].y)
	{
		temp = _srcQuad[3];
		_srcQuad[3] = _srcQuad[0];
		_srcQuad[0] = temp;
	}

	if (_srcQuad[2].y < _srcQuad[1].y)
	{
		temp = _srcQuad[2];
		_srcQuad[2] = _srcQuad[1];
		_srcQuad[1] = temp;
	}

	if (_srcQuad[0].y > _srcQuad[2].y)
	{
		temp = _srcQuad[0];
		_srcQuad[0] = _srcQuad[2];
		_srcQuad[2] = temp;
	}

	if (_srcQuad[1].y > _srcQuad[3].y)
	{
		temp = _srcQuad[1];
		_srcQuad[1] = _srcQuad[3];
		_srcQuad[3] = temp;
	}
	
	double x = Distance(_srcQuad[0], _srcQuad[1]);
	double y = Distance(_srcQuad[2], _srcQuad[3]);
}

double Distance(const Point2f p1, const Point2f p2) {
	double distance;
	return distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}