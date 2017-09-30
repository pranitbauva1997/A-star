#include <opencv2/opencv.hpp>
#include <stack>
#include <math.h>

using namespace std;
using namespace cv;

struct bgr_color {
    int b_low;
    int b_high;
    int g_low;
    int g_high;
    int r_low;
    int r_high;
};

void init_bgrcolor(struct bgr_color &green, struct bgr_color &red) {
    green = {0, 10, 200, 255, 0, 10};
    red = {0, 10, 0, 10, 100, 255};
}

void extract_color(Mat &img, Mat &start_img, Mat &end_img,
                   vector<vector<Point> > &start_points,
                   vector<vector<Point> > &end_points,
                   struct bgr_color &green, struct bgr_color &red) {

    vector<Vec4i> start_hierarchy, end_hierarchy;
    inRange(img, Scalar(green.b_low, green.g_low, green.r_low),
            Scalar(green.b_high, green.g_high, green.r_high), start_img);

    inRange(img, Scalar(red.b_low, red.g_low, red.r_low),
            Scalar(red.b_high, red.g_high, red.r_high), end_img);

    findContours(start_img, start_points, start_hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    findContours(end_img, end_points, end_hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
}

void display_images(Mat &img, Mat &start_img, Mat &end_img) {
    imshow("Area", img);
    imshow("Start", start_img);
    imshow("End", end_img);
}

Point get_centre(vector<Point> v) {
    double x = 0, y = 0;
    int s = v.size();
    for (int i = 0; i < v.size(); i++) {
        x += v[i].x * 100/s;
        y += v[i].y * 100/s;
    }
    return Point((int) x/100, (int) y/100);
}

bool is_obstacle(Mat &img, int x, int y) {
    if (!(img.at<Vec3b>(x, y)[0] >= 0 && img.at<Vec3b>(x, y)[0] <= 10))
        return false;

    if (!(img.at<Vec3b>(x, y)[1] >= 0 && img.at<Vec3b>(x, y)[1] <= 0))
        return false;

    if (!(img.at<Vec3b>(x, y)[2] >= 0 && img.at<Vec3b>(x, y)[2] <= 10))
        return false;

    return true;
}

double dist(Point p1, Point p2) {
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

bool is_end(Point pt, Point end) {
    if (dist(pt, end) > 10)
        return false;
    else
        return true;
}

void init_visited(int **visited, int rows, int cols) {
    visited = new int*[rows];
    for (int i = 0; i < rows; i++) {
        visited[i] = new int[cols];
        for (int j = 0;  j < cols; j++) {
            visited[i][j] = 0;
        }
    }
}

void free_visited(int **visited, int rows, int cols) {
    for (int i = 0; i < rows; i++)
        delete [] visited[i];

    delete [] visited;
}

void dfs(Mat &img, Point s, Point e, stack<Point> &st) {
    int **visited;

    init_visited(visited, img.rows, img.cols);
    cout << visited[61][38] << endl;
    st.push(s);
    visited[s.y][s.x] = 1;
    while(!st.empty()) {
        Point temp = st.top();
        st.pop();

        if (is_end(temp, e)) {
            return;
        }

        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (temp.x + i < 0 || temp.x + i >= img.cols)
                    continue;
                if (temp.y + j < 0 || temp.y + j >= img.rows)
                    continue;
                if (!is_obstacle(img, temp.y + j, temp.x + i) &&
                    !visited[temp.y + j][temp.x + i]) {
                    st.push(Point(temp.x + i, temp.y + j));
                    cout << temp.x + i << ", " << temp.y + j << endl;
                    visited[temp.y + j][temp.x + i] = 1;
                }
            }
        }
    }
    free_visited(visited, img.rows, img.cols);
}

void paint_path(Mat &img, stack<Point> &st) {
    while(!st.empty()) {
        Point temp = st.top();
        st.pop();
        img.at<Vec3b>(temp.y, temp.x)[0] = 255;
        img.at<Vec3b>(temp.y, temp.x)[2] = 255;
        img.at<Vec3b>(temp.y, temp.x)[1] = 0;
    }
}

int main() {
    Mat img, start_img, end_img;
    vector<vector<Point> > start_points, end_points;
    Point start, end;
    stack<Point> path;
    struct bgr_color green, red;

    img = imread("a-star-image.jpg", CV_LOAD_IMAGE_COLOR);

    init_bgrcolor(green, red);
    extract_color(img, start_img, end_img, start_points, end_points,
                  green, red);

    start = get_centre(start_points[0]);
    end = get_centre(end_points[0]);

    dfs(img, start, end, path);
    paint_path(img, path);

    display_images(img, start_img, end_img);
    waitKey(0);

    return 0;
}
