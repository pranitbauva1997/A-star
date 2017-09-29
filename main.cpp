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
vector<vector<Point> > start_points, end_points;
vector<Vec4i> start_hierarchy, end_hierarchy;

void init_bgrcolor() {
    green = {0, 10, 200, 255, 0, 10};
    red = {0, 10, 0, 10, 100, 255};
}

void extract_color() {
    inRange(img, Scalar(green.b_low, green.g_low, green.r_low),
            Scalar(green.b_high, green.g_high, green.r_high), start_img);

    inRange(img, Scalar(red.b_low, red.g_low, red.r_low),
            Scalar(red.b_high, red.g_high, red.r_high), end_img);

    findContours(start_img, start_points, start_hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    findContours(end_img, end_points, end_hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
}

void display_images() {
    imshow("Area", img);
    imshow("Start", start_img);
    imshow("End", end_img);
}

Point get_centre(vector<Point> v) {
    double x = 0, y = 0;
    int s = v.size();
    for (int i = 0; i < v.size(); i++) {
        cout << v[i].x << ", " << v[i].y << endl;
        x += v[i].x * 100/s;
        y += v[i].y * 100/s;
    }
    return Point((int) x/100, (int) y/100);
}

int main() {
    Point start, end;
    img = imread("a-star-image.jpg", CV_LOAD_IMAGE_COLOR);

    init_bgrcolor();
    extract_color();

    start = get_centre(start_points[0]);
    end = get_centre(end_points[0]);

    display_images();
    waitKey(0);

    return 0;
}
