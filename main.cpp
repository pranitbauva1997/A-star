#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

struct bgr_color {
    int b_low;
    int b_high;
    int g_low;
    int g_high;
    int r_low;
    int r_high;
} green, red;

Mat img, start_img, end_img;

void init_bgrcolor() {
    green = {0, 10, 200, 255, 0, 10};
    red = {0, 10, 0, 10, 100, 255};
}

void extract_color() {
    inRange(img, Scalar(green.b_low, green.g_low, green.r_low),
            Scalar(green.b_high, green.g_high, green.r_high), start_img);

    inRange(img, Scalar(red.b_low, red.g_low, red.r_low),
            Scalar(red.b_high, red.g_high, red.r_high), end_img);
}

void display_images() {
    imshow("Area", img);
    imshow("Start", start_img);
    imshow("End", end_img);
}

int main() {
    img = imread("a-star-image.jpg", CV_LOAD_IMAGE_COLOR);

    init_bgrcolor();
    extract_color();

    display_images();
    waitKey(0);

    return 0;
}
