#include <opencv2/core/core.hpp> 
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <Kinect.h>

using namespace cv;
using namespace std;

#define WINDOW_NAME1 "OriginDepthImage"
#define WINDOW_NAME2 "grayImage"
#define WINDOW_NAME3 "dstImage"

int main()
{
	Mat originImg;
	Mat grayImage;
	Mat dstImage;

	originImg = imread("1.jpg");

	cv::namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	cv::namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);
	cv::namedWindow(WINDOW_NAME3, WINDOW_AUTOSIZE);

	cvtColor(originImg, grayImage, COLOR_BGR2GRAY);
	equalizeHist(grayImage, dstImage);

	cv::imshow(WINDOW_NAME1, originImg);
	cv::imshow(WINDOW_NAME2, grayImage);
	cv::imshow(WINDOW_NAME3, dstImage);

	waitKey(0);
	return 0;
}