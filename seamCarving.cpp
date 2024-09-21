#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat computeGradient(Mat &img)
{
    vector<Mat> bgr_channels;
    split(img, bgr_channels);

    Mat grad_x_b, grad_x_g, grad_x_r;
    Mat grad_y_b, grad_y_g, grad_y_r;
    Mat abs_grad_x_b, abs_grad_x_g, abs_grad_x_r;
    Mat abs_grad_y_b, abs_grad_y_g, abs_grad_y_r;

    Sobel(bgr_channels[0], grad_x_b, CV_32F, 1, 0, 3);
    Sobel(bgr_channels[0], grad_y_b, CV_32F, 0, 1, 3);

    Sobel(bgr_channels[1], grad_x_g, CV_32F, 1, 0, 3);
    Sobel(bgr_channels[1], grad_y_g, CV_32F, 0, 1, 3);

    Sobel(bgr_channels[2], grad_x_r, CV_32F, 1, 0, 3);
    Sobel(bgr_channels[2], grad_y_r, CV_32F, 0, 1, 3);

    convertScaleAbs(grad_x_b, abs_grad_x_b);
    convertScaleAbs(grad_y_b, abs_grad_y_b);
    convertScaleAbs(grad_x_g, abs_grad_x_g);
    convertScaleAbs(grad_y_g, abs_grad_y_g);
    convertScaleAbs(grad_x_r, abs_grad_x_r);
    convertScaleAbs(grad_y_r, abs_grad_y_r);

    Mat grad_mag_b, grad_mag_g, grad_mag_r;
    Mat grad_mag;

    Mat mag_b, mag_g, mag_r;
    magnitude(grad_x_b, grad_y_b, mag_b);
    magnitude(grad_x_g, grad_y_g, mag_g);
    magnitude(grad_x_r, grad_y_r, mag_r);

    Mat avg_mag = (mag_b + mag_g + mag_r) / 3.0;
    convertScaleAbs(avg_mag, grad_mag);

    return grad_mag;
}

vector<int> findVerticalSeam(Mat &energy)
{
    int rows = energy.rows;
    int cols = energy.cols;

    Mat dp = Mat(rows, cols, CV_32F, Scalar(INT_MAX));
    vector<int> seam(rows);

    for (int j = 0; j < cols; ++j)
    {
        dp.at<int>(0, j) = energy.at<uchar>(0, j);
    }

    for (int i = 1; i < rows; ++i)
    {

        for (int j = 0; j < cols; ++j)
        {
            dp.at<int>(i, j) = energy.at<uchar>(i, j) + dp.at<int>(i - 1, j); 
            if (j > 0)
            {
                dp.at<int>(i, j) = min(dp.at<int>(i, j), energy.at<uchar>(i, j) + dp.at<int>(i - 1, j - 1)); 
            }
            if (j < cols - 1)
            {
                dp.at<int>(i, j) = min(dp.at<int>(i, j), energy.at<uchar>(i, j) + dp.at<int>(i - 1, j + 1)); 
            }
        }
    }

    int minSeamCol = 0;
    int minSeamValue = dp.at<int>(rows - 1, 0);
    for (int j = 1; j < cols; ++j)
    {
        if (dp.at<int>(rows - 1, j) < minSeamValue)
        {
            minSeamValue = dp.at<int>(rows - 1, j);
            minSeamCol = j;
        }
    }
    seam[rows - 1] = minSeamCol;
    for (int i = rows - 2; i >= 0; --i)
    {
        int prevCol = seam[i + 1];
        int bestCol = prevCol;

        if (prevCol > 0 && dp.at<int>(i, prevCol - 1) < dp.at<int>(i, bestCol))
        {
            bestCol = prevCol - 1;
        }
        if (prevCol < cols - 1 && dp.at<int>(i, prevCol + 1) < dp.at<int>(i, bestCol))
        {
            bestCol = prevCol + 1;
        }

        seam[i] = bestCol;
    }

    return seam;
}

void removeVerticalSeam(Mat &img, vector<int> &seam)
{
    for (int i = 0; i < img.rows; ++i)
    {
        for (int j = seam[i]; j < img.cols - 1; ++j)
        {
            img.at<Vec3b>(i, j) = img.at<Vec3b>(i, j + 1);
        }
    }

    img = img(Rect(0, 0, img.cols - 1, img.rows));
}

vector<int> findHorizontalSeam(Mat &energy)
{
    int rows = energy.rows;
    int cols = energy.cols;

    Mat dp = Mat(cols, rows, CV_32F, Scalar(INT_MAX));
    vector<int> seam(cols);

    for (int j = 0; j < rows; ++j)
    {
        dp.at<int>(j, 0) = energy.at<uchar>(j, 0);
    }

    for (int i = 1; i < cols; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            dp.at<int>(j, i) = energy.at<uchar>(j, i) + dp.at<int>(j, i - 1); 
            if (j > 0)
            {
                dp.at<int>(j, i) = min(dp.at<int>(j, i), energy.at<uchar>(j, i) + dp.at<int>(j - 1, i - 1)); 
            }
            if (j < rows - 1)
            {
                dp.at<int>(j, i) = min(dp.at<int>(j, i), energy.at<uchar>(j, i) + dp.at<int>(j + 1, i + -1)); 
            }
        }
    }

    int minSeamCol = 0;
    int minSeamValue = dp.at<int>(0, cols - 1);
    for (int j = 1; j < rows; ++j)
    {
        if (dp.at<int>(j, cols - 1) < minSeamValue)
        {
            minSeamValue = dp.at<int>(j, cols - 1);
            minSeamCol = j;
        }
    }

    seam[cols - 1] = minSeamCol;
    for (int i = cols - 2; i >= 0; --i)
    {
        int prevRow = seam[i + 1];
        int bestRow = prevRow;

        if (prevRow > 0 && dp.at<int>(prevRow - 1, i) < dp.at<int>(bestRow, i))
        {
            bestRow = prevRow - 1;
        }
        if (prevRow < rows - 1 && dp.at<int>(prevRow + 1, i) < dp.at<int>(bestRow, i))
        {
            bestRow = prevRow + 1;
        }

        seam[i] = bestRow;
    }

    return seam;
}

void removeHorizontalSeam(Mat &img, vector<int> &seam)
{
    for (int i = 0; i < img.cols; ++i)
    {
        for (int j = seam[i]; j < img.rows - 1; ++j)
        {
            img.at<Vec3b>(j, i) = img.at<Vec3b>(j + 1, i);
        }
    }

    img = img(Rect(0, 0, img.cols, img.rows - 1));
}


int main()
{
    int h, w;
    string filepath;
    cout<<"Enter image filepath: ";
    cin>>filepath;
    cout << "Enter resized image size: ";
    cin >> w >> h;
    Mat img = imread(filepath, IMREAD_COLOR);
    if (img.empty())
    {
        cout << "Image not found";
        return 1;
    }

    while (img.rows > h)
    {
        Mat grad_img = computeGradient(img);
        vector<int> horizontalSeam = findHorizontalSeam(grad_img);
        removeHorizontalSeam(img, horizontalSeam);
    }

    while (img.cols > w)
    {
        Mat grad_img = computeGradient(img);
        vector<int> verticalSeam = findVerticalSeam(grad_img);
        removeVerticalSeam(img, verticalSeam);
    }

    imwrite("/home/aarathi/IIITH/DSAPS/2024201083_A1/out.png", img);
    return 0;
}