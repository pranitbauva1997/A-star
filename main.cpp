#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat img;
    img = imread("a-star-image.jpg", CV_LOAD_IMAGE_COLOR);
    imshow("Arena", img);
    waitKey(0);
    return 0;
}
