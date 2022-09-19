#include <opencv2/opencv.hpp>

#define FONT FONT_HERSHEY_COMPLEX
#define FONTSCALAR 0.5

// 캠을 동전과 수직으로 놓은 후 위치에 맞게 사이즈 값을 변경해야한다.
// 100원짜리 동전이 없어서 사이즈를 정하지 못했다... 
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

// 이미지 로드와 에러 체크를 함께 하기 위한 함수를 만들었다.. 캡슐화를 통해 main 함수를 짧게 하고 싶었다.
// 하지만 메인에서 return을 시키는것이 아니기 때문에 
// 에러가 발생하여도 코드가 계속 진행된다. 따라서 별로 좋은 선택이라고 생각되지 않는다.
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

    // Open_Cam 함수에서 리턴한 값에 따라 반복문을 실행시켜 오류 발생시 작동하지 않도록 바꾸었다.
    while (err_check)
    {
        cam.read(frame_or_Img);

        Mat frame;

        GrayNblurr(frame_or_Img, &frame);
        
        // 입력 영상과 축적 배열의 크기 비율
        // 인접한 원 중심의 최소 거리, 
        // 케니 에지 검출기 높은 임계값 100,
        // 축적 배열에서 원 검출을 위한 임계값 100,
        // 검출할 원의 최소 반지름 0,
        // 검출할 원의 최대 반지름 0
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