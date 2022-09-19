#include <opencv2/opencv.hpp>

#define FONT FONT_HERSHEY_COMPLEX
#define FONTSCALAR 0.5

// ķ�� ������ �������� ���� �� ��ġ�� �°� ������ ���� �����ؾ��Ѵ�.
// 100��¥�� ������ ��� ����� ������ ���ߴ�... 
enum COIN_SIZE
{
    COIN_10_MIN = 10,
    COIN_10_MAX = 12,
    COIN_50_MIN = 14,
    COIN_50_MAX,
    COIN_100_MIN,
    COIN_100_MAX,
    COIN_500_MIN = 16,
    C0IN_500_MAX = 20
};

enum ERROR_CODE
{
    ERROR_CANNOT_OPEN_THE_CAMERA = -2,
    ERROR_IMAGE_LOAD_FAILED,
    SUCCES = 1,
};

using namespace cv;
using namespace std;

VideoCapture cam;
vector<Vec3f> circles;

// �̹��� �ε�� ���� üũ�� �Բ� �ϱ� ���� �Լ��� �������.. ĸ��ȭ�� ���� main �Լ��� ª�� �ϰ� �;���.
// ������ ���ο��� return�� ��Ű�°��� �ƴϱ� ������ 
// ������ �߻��Ͽ��� �ڵ尡 ��� ����ȴ�. ���� ���� ���� �����̶�� �������� �ʴ´�.
ERROR_CODE Open_Cam(VideoCapture* cam)
{
    cam->open(0);
    if (!cam->isOpened())
    {
        cerr << "Error! Cannot open the camera" << endl;
        return ERROR_CANNOT_OPEN_THE_CAMERA;
    }
    return SUCCES;
}

ERROR_CODE Read_img(Mat* img, String path, int type = 1)
{
    *img = imread(path, type);

    if (img->empty())
    {
        cerr << "Error! Image load failed" << endl;
        return ERROR_IMAGE_LOAD_FAILED;
    }

    return SUCCES;
}

void GrayNblurr(Mat input, Mat* output)
{
    cvtColor(input, input, COLOR_RGB2GRAY);
    blur(input, *output, Size(4, 4));
}

void drowCircle(Mat* img)
{
    for (Vec3f c : circles) {
        Point center(cvRound(c[0]), cvRound(c[1]));
        int radius = cvRound(c[2]);
        circle(*img, center, radius, Scalar(0, 0, 255), 2, LINE_AA);

        String text = format("%d", cvRound(c[2]));
        putText(*img, text, Point(cvRound(c[0]), cvRound(c[1])), FONT, FONTSCALAR, Scalar(0, 0, 255), 1);
    }
}

int count_money()
{
    int sum = 0;
    for (Vec3f c : circles) {
        int radius = cvRound(c[2]);
        if (radius >= COIN_10_MIN && radius <= COIN_10_MAX) {
            sum += 10;
        }
        else if (radius >= COIN_50_MIN && radius <= COIN_50_MAX)
        {
            sum += 50;
        }
        else if (radius >= COIN_500_MIN && radius < C0IN_500_MAX)
        {
            sum += 500;
        }
    }
    return sum;
}

void print_sum(Mat* img)
{
    int sum = count_money();
    String text = format("SUM : % d", sum);
    putText(*img, text, Point(10, 15), FONT, FONTSCALAR, Scalar(255, 255, 0), 1);
}

int main()
{
    Mat frame_or_Img;

    ERROR_CODE err_check = Open_Cam(&cam);

    // Open_Cam �Լ����� ������ ���� ���� �ݺ����� ������� ���� �߻��� �۵����� �ʵ��� �ٲپ���.
    while (err_check)
    {
        cam.read(frame_or_Img);

        Mat frame;

        GrayNblurr(frame_or_Img, &frame);
        
        // �Է� ����� ���� �迭�� ũ�� ����
        // ������ �� �߽��� �ּ� �Ÿ�, 
        // �ɴ� ���� ����� ���� �Ӱ谪 100,
        // ���� �迭���� �� ������ ���� �Ӱ谪 100,
        // ������ ���� �ּ� ������ 0,
        // ������ ���� �ִ� ������ 0
        HoughCircles(frame, circles, HOUGH_GRADIENT, 1, 10, 130, 25, 5, 20);

        drowCircle(&frame_or_Img);
        print_sum(&frame_or_Img);

        imshow("Display", frame_or_Img);

        if (waitKey(1) >= 0) {
            break;
        }
    }

    return 0;
}