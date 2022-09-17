#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Point pt1;
Point pt2;
Mat src;
Mat dst;
Rect box;
bool bool_box;

int pill = 3;

void myMouse_4(int event, int x, int y, int flags, void*);

int main()
{
	src = imread("field.bmp");
	if (src.empty())
	{
		cerr << "Image load failed!";
		return 0;
	}

	
	dst = src.clone();

	imshow("dst", dst);
	setMouseCallback("dst", myMouse_4);
	while (true) {
		int myKey = waitKey();
		if (myKey == 't' || myKey == 'T')
		{
			pill = pill * (-1);
		}
		else if (myKey == 'c' || myKey == 'C')
		{
			src = imread("field.bmp");
			dst = src.clone();
			imshow("dst", dst);
		}
	}

	return 0;
}


void myMouse_4(int event, int x, int y, int flags, void*) {
	switch (event) {
	case EVENT_LBUTTONDOWN:
		bool_box = true;
		pt1 = Point(x, y);
		break;
	case EVENT_LBUTTONUP:
		bool_box = false;
		src = dst.clone();
		break;
	case EVENT_MOUSEMOVE:
		if (bool_box) {
			if (flags & EVENT_FLAG_CTRLKEY) {
				pt2 = Point(x, y);
				dst = src.clone();
				rectangle(dst, pt1, pt2, Scalar(255, 255, 255), pill);
				imshow("dst", dst);
			}
			else
			{
				line(dst, pt1, Point(x, y), Scalar(0, 255, 255), 2);
				imshow("dst", dst);
				pt1 = Point(x, y);
			}
		}

		break;
	case EVENT_RBUTTONDOWN:
		break;
	default:
		break;
	}
}