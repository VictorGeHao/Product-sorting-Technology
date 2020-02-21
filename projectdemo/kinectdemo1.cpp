////////////Kinect深度直方图提取特定物品视频
////#include "stdafx.h"
//#include <stdio.h>
//#include <Kinect.h>
//#include <windows.h>
//#include <opencv\highgui.h>
//#include <opencv\cv.h>
//#include <opencv2/opencv.hpp>
//#include <stdlib.h>
//#include <time.h>
//#include <string.h>
//#include <strstream>
//
//using namespace cv;
//using namespace std;
//
//// 转换depth图像到cv::Mat
//Mat ConvertMat(const UINT16* pBuffer, int nWidth, int nHeight)
//{
//	Mat img(nHeight, nWidth, CV_8UC1);
//	//指向头指针
//	uchar* p_mat = img.data;
//	//指向最后一个元素的指针
//	const UINT16* pBufferEnd = pBuffer + (nWidth * nHeight);
//	while (pBuffer < pBufferEnd)  //16位最大值为65536
//	{
//		//将16位数据转换到8位
//		*p_mat = *pBuffer / 65536.0 * 256;
//		pBuffer++;
//		p_mat++;
//	}
//	return img;
//}
//
//void getForeground()
//{
//	IKinectSensor*          m_pKinectSensor;
//	IDepthFrameReader*      m_pDepthFrameReader;
//	IDepthFrameSource*      pDepthFrameSource = NULL;
//	IColorFrameSource*      pColorFrameSource;
//	IColorFrameReader*      m_pColorFrameReader;
//	IFrameDescription*      depthFrameDescription = NULL;
//	IFrameDescription*      colorFrameDescription = NULL;
//	ColorImageFormat        imageFormat = ColorImageFormat_None;
//	ICoordinateMapper*      coordinateMapper = NULL;
//
//	GetDefaultKinectSensor(&m_pKinectSensor);      //获取默认kinect传感器
//	printf("打开kinect传感器成功\n");
//	//打开传感器
//	m_pKinectSensor->Open();
//
//	//获得深度信息传感器
//	m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
//	//获得彩色信息传感器  
//	m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);
//	//打开深度信息帧读取器
//	pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
//	//打开彩色信息帧读取器  
//	pColorFrameSource->OpenReader(&m_pColorFrameReader);
//
//	while (true)
//	{
//		IColorFrame*       pColorFrame = NULL;
//		IDepthFrame*       pDepthFrame = NULL;
//
//		while (pDepthFrame == NULL) {
//			//由于有时候获取不到，因此循环获取最近的帧
//			m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);
//		}
//		pDepthFrame->get_FrameDescription(&depthFrameDescription);
//		int depth_width, depth_height;
//		//获取帧的像素信息（宽和高）
//		depthFrameDescription->get_Width(&depth_width);
//		depthFrameDescription->get_Height(&depth_height);
//		printf("width=%d height=%d\n", depth_width, depth_height);
//		UINT nBufferSize_depth = 0;
//		UINT16 *pBuffer_depth = NULL;
//		//获取图像像素个数和指向图像的指针
//		pDepthFrame->AccessUnderlyingBuffer(&nBufferSize_depth, &pBuffer_depth);
//		//转换为8位的mat   
//		Mat depthImg_show = ConvertMat(pBuffer_depth, depth_width, depth_height);
//		//均衡化，为了提高显示效果    
//		equalizeHist(depthImg_show, depthImg_show);
//
//
//
//		//获取彩色图像
//		while (pColorFrame == NULL) {
//			//由于有时候获取不到，因此循环获取最近的帧
//			m_pColorFrameReader->AcquireLatestFrame(&pColorFrame);
//		}
//		//获取图片描述信息
//		pColorFrame->get_FrameDescription(&colorFrameDescription);
//		int nWidth, nHeight;
//		uchar *pBuffer = NULL;
//		UINT nBufferSize = 0;
//		colorFrameDescription->get_Width(&nWidth);
//		colorFrameDescription->get_Height(&nHeight);
//		cout << "width=" << nWidth << endl;
//		cout << "Height=" << nHeight << endl;
//		pColorFrame->get_RawColorImageFormat(&imageFormat);
//		//输出结果为 ColorImageFormat_Yuy2    = 5，为Yuy2格式   
//		cout << "imageformat is " << imageFormat << endl;
//		//新建一个mat对象，用于保存读入的图像,注意参数的高在前，宽在后
//		Mat colorImg(nHeight, nWidth, CV_8UC4);
//		pBuffer = colorImg.data;
//		nBufferSize = colorImg.rows*colorImg.step;
//		pColorFrame->CopyConvertedFrameDataToArray(nBufferSize, reinterpret_cast<BYTE*>(pBuffer), ColorImageFormat_Bgra);
//
//
//		//将深度帧映射到彩色空间 
//		m_pKinectSensor->get_CoordinateMapper(&coordinateMapper);
//		ColorSpacePoint* colorSpacePoint = new ColorSpacePoint[depth_width*depth_height];
//		coordinateMapper->MapDepthFrameToColorSpace(depth_width*depth_height, pBuffer_depth, depth_width*depth_height, colorSpacePoint);
//
//
//		UINT16 *depthData = NULL;
//		depthData = pBuffer_depth;
//		//创建一个空Mat对象保存最后得到的前景图像
//		Mat resultImg(depth_height, depth_width, CV_8UC4, Scalar::all(0));
//		for (int i = 0; i < depth_height; i++)
//		{
//			for (int j = 0; j < depth_width; j++)
//			{
//				unsigned int index = i * depth_width + j;
//				ColorSpacePoint csp = colorSpacePoint[index];
//				int colorX = static_cast<int>(floor(csp.X + 0.5));
//				int colorY = static_cast<int>(floor(csp.Y + 0.5));
//				//选取落在彩色图像上的点并对前景背景进行阈值分割,再次更改阈值大小
//				if (colorX >= 0 && colorX < nWidth && colorY >= 0 && colorY < nHeight && *depthData <650)
//				{
//					//拷贝彩色信息
//					resultImg.at<cv::Vec4b>(i, j) = colorImg.at<cv::Vec4b>(colorY, colorX);
//				}
//				depthData++;
//			}
//
//		}
//
//		//显示图像
//		cv::imshow("colorImg", colorImg);
//		cv::imshow("depthImg", depthImg_show);
//		cv::imshow("result", resultImg);
//
//		colorImg.release();
//		depthImg_show.release();
//		pDepthFrame->Release();
//		pColorFrame->Release();
//		delete[] colorSpacePoint;
//		if (27 == waitKey(50))
//			break;
//	}
//}
//
//int main()
//{
//	getForeground();
//}





/////////////实时canny边缘检测
////包含头文件
//#include <opencv2/highgui/highgui.hpp>  
//#include <opencv2/imgproc/imgproc.hpp>  
//#include <opencv2/core/core.hpp>  
//
////使用命名空间
//using namespace cv;
//
//int main()
//{
//	VideoCapture cap(0);//VideoCapture类用法：1.VideoCapture(const string& filename)打开视频文件；2.VideoCapture(int device)打开设备。
//	if (!cap.isOpened())//检测VideoCapture类是否打开成功
//	{
//		return -1;//打开失败返回值
//	}
//
//	//定义变量类
//	Mat mFrame;
//	Mat mEdges;
//
//	//定义控制变量
//	bool bStop = false;//控制变量初始化
//	while (!bStop)//开始
//	{
//		cap >> mFrame;
//
//		cvtColor(mFrame, mEdges, CV_BGR2GRAY);//图像转换为灰度
//		GaussianBlur(mEdges, mEdges, Size(7, 7), 1.5, 1.5);//高斯滤波
//		Canny(mEdges, mEdges, 0, 30, 3);//边缘检测
//
//		imshow("camera", mEdges);//显示
//
//		if (waitKey(30) >= 0)//检测用户按键
//		{
//			bStop = true;//更改控制变量
//		}
//	}
//	return 0;//结束返回
//}





//////////直出深度信息进行Canny检测和标记
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>  
#include <Kinect.h>


using namespace cv;
using namespace std;

void on_ContoursChange(int, void*);

#define WINDOW_NAME1 "OriginDepthImage"
#define WINDOW_NAME2 "FliterImage"
#define WINDOW_NAME3 "CannyTest"
#define WINDOW_NAME4 "ContoursShow"        

//多边形包围准备
Mat g_srcImage;
Mat g_grayImage;
int g_nThresh = 50;//阈值
int g_nMaxThresh = 255;//阈值最大值
RNG g_rng(12345);//随机数生成器
int g_FilterValue = 3;//滤波参数
cv::Mat mFliterImg;//滤波效果图

////轨迹条回调函数声明
//static void on_BoxFilter(int, void *);		//方框滤波
//static void on_MeanBlur(int, void *);		//均值块滤波器
//static void on_GaussianBlur(int, void *);		//高斯滤波器
//static void on_MedianBlur(int, void *);		//中值滤波器
//static void on_BilateralFilter(int, void *);		//双边滤波器

int main(void)
{
	// 获取感应器
	IKinectSensor* pSensor = nullptr;
	GetDefaultKinectSensor(&pSensor);

	// 打开感应器
	pSensor->Open();

	// 取得深度数据
	IDepthFrameSource* pFrameSource = nullptr;
	pSensor->get_DepthFrameSource(&pFrameSource);

	// 取得深度数据的描述信息（宽、高）
	int        iWidth = 0;
	int        iHeight = 0;
	
	IFrameDescription* pFrameDescription = nullptr;
	pFrameSource->get_FrameDescription(&pFrameDescription);
	pFrameDescription->get_Width(&iWidth);
	pFrameDescription->get_Height(&iHeight);
	pFrameDescription->Release();
	pFrameDescription = nullptr;

	// 取得感应器的“可靠深度”的最大、最小距离
	UINT16 uDepthMin = 0, uDepthMax = 0;
	pFrameSource->get_DepthMinReliableDistance(&uDepthMin);
	pFrameSource->get_DepthMaxReliableDistance(&uDepthMax);
	cout << "Reliable Distance: "
		<< uDepthMin << " – " << uDepthMax << endl;

	// 建立图像矩阵，mDepthImg用来存储16位的图像数据，直接用来显示会接近全黑
	//不方便观察，用mImg8bit转换成8位再显示
	cv::Mat mDepthImg(iHeight, iWidth, CV_16UC1);
	cv::Mat mImg8bit(iHeight, iWidth, CV_8UC1);
	cv::Mat mCanny;
	
	//cv::namedWindow("DepthImage");
	cv::namedWindow("CannyTest");
	


	// 打开深度数据阅读器
	IDepthFrameReader* pFrameReader = nullptr;
	pFrameSource->OpenReader(&pFrameReader);

	// 主循环
	while (1)
	{
		// 取得最新数据
		IDepthFrame* pFrame = nullptr;
		if (pFrameReader->AcquireLatestFrame(&pFrame) == S_OK)
		{
			// 把数据存入16位图像矩阵中
			pFrame->CopyFrameDataToArray(iWidth * iHeight,
				reinterpret_cast<UINT16*>(mDepthImg.data));// 强制转换数据类型，把16位转换成8位;要改变显示的颜色和效果，就改变从mDepthImg到mImg8bit的转换
			
			mDepthImg.convertTo(mImg8bit, CV_8U, 255.0f / uDepthMax);// converto()第一个参数是输出矩阵，第二个是转换类型，第三个是缩放因子，其中4500是深度数据的最大距离
			
			g_grayImage = mImg8bit;
			

			//设置滤波效果窗口滚动条（多种滤波共用）
			//createTrackbar("滤波内核", WINDOW_NAME2, &g_FilterValue, 40, bilateralFilter);
			////方框滤波
			//boxFilter(g_grayImage, mFliterImg, -1, Size(g_FilterValue + 1, g_FilterValue + 1));
			////均值滤波
			//blur(g_grayImage, mFliterImg, Size(g_FilterValue + 1, g_FilterValue + 1), Point(-1, -1));
			////高斯滤波
			//GaussianBlur(g_grayImage, mFliterImg, Size(g_FilterValue * 2 + 1, g_FilterValue * 2 + 1), 0, 0);
			////中值滤波
			//medianBlur(g_grayImage, mFliterImg, g_FilterValue * 2 + 1);
			//双边滤波
			bilateralFilter(g_grayImage, mFliterImg, g_FilterValue, g_FilterValue * 2, g_FilterValue / 2);



			Canny(mFliterImg, mCanny, 0, 30, 3);// 处理后图像进行边缘检测

			cv::namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
			cv::namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);
			cv::namedWindow(WINDOW_NAME3, WINDOW_AUTOSIZE);
			cv::imshow(WINDOW_NAME1, mImg8bit);
			cv::imshow(WINDOW_NAME2, mFliterImg);
			cv::imshow(WINDOW_NAME3, mCanny);
			


			// 改变console字体颜色
			system("color 1F");

			// 设置滚动条并调用一次回调函数
			createTrackbar(" 阈值:", WINDOW_NAME1, &g_nThresh, g_nMaxThresh, on_ContoursChange);
			on_ContoursChange(0, 0);
  
			
			pFrame->Release();
		}

		if (cv::waitKey(30) == VK_ESCAPE) {
			break;
		}
	}

	// 释放变量pFrameReader
	pFrameReader->Release();
	pFrameReader = nullptr;

	// 释放变量pFramesSource
	pFrameSource->Release();
	pFrameSource = nullptr;

	// 关闭感应器
	pSensor->Close();

	// 释放感应器
	pSensor->Release();
	pSensor = nullptr;

	return 0;
}

//////////////////////////////// 声明函数描述
void on_ContoursChange(int, void*)
{
	//定义一些参数
	Mat threshold_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// 使用Threshold检测边缘
	threshold(g_grayImage, threshold_output, g_nThresh, 255, THRESH_BINARY);

	// 找出轮廓
	findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	//一个循环，遍历所有部分，进行本程序最核心的操作
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);//用指定精度逼近多边形曲线 
		boundRect[i] = boundingRect(Mat(contours_poly[i]));//计算点集的最外面（up-right）矩形边界
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);//对给定的 2D点集，寻找最小面积的包围圆形 
	}

	// 绘制多边形轮廓 + 包围的矩形框 + 圆形框
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int unsigned i = 0; i<contours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));//随机设置颜色
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());//绘制轮廓
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);//绘制矩形
	}

	// 显示效果图窗口
	cv::namedWindow(WINDOW_NAME4, WINDOW_AUTOSIZE);
	cv::imshow(WINDOW_NAME4, drawing);
}
