#include <stdlib.h>
#include <iostream>
#include <string>
#include "OpenNI.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Windows.h"
#include <Shlobj.h>
#include "NuiApi.h"
#include <sstream>
 
 
#include <opencv2/opencv.hpp>  
 
using namespace std;  
using namespace cv;  
using namespace openni;
 
 
int frameNo = 1;
 
int main(int argc, char *argv[])
{
	Mat colorImage;
	Mat depthImage;
	colorImage.create(480, 640, CV_8UC3);
	depthImage.create(480,640,CV_8UC1);

	HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH); 
	if (FAILED(hr)) 
	{ 
		cout<<"NuiInitialize failed"<<endl; 
		return hr; 
	} 
 

	HANDLE nextColorFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	HANDLE colorStreamHandle = NULL; //保存彩色图像数据流的句柄，用以提取数据
 
	HANDLE nextDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	HANDLE depthStreamHandle = NULL;//保存深度图像数据流的句柄，用以提取数据
 
	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 
		0, 2, nextColorFrameEvent, &colorStreamHandle); 
	if( FAILED( hr ) )
	{ 
		cout<<"Could not open color image stream video"<<endl; 
		NuiShutdown(); 
		return hr; 
	}
	namedWindow("colorImage", CV_WINDOW_AUTOSIZE);
 
	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480,
		0, 2, nextDepthFrameEvent, &depthStreamHandle);
	if( FAILED( hr ) )
	{
		cout<<"Could not open depth image stream video"<<endl;
		NuiShutdown();
		return hr;
	}
	namedWindow("depthImage", CV_WINDOW_AUTOSIZE);
 
	//读取彩色图数据 
	while(1) 
	{ 
		const NUI_IMAGE_FRAME * pColorImageFrame = NULL; 
		const NUI_IMAGE_FRAME * pDepthImageFrame = NULL;
 
		string colorPathPrefix = "colorImage";
		string depthPathPrefix = "depthImage";
		string commonPathSuffix = ".jpg";
 
		//等待新的彩色图像数据
		if (WaitForSingleObject(nextColorFrameEvent, INFINITE)==0) 
		{
			
			stringstream ss;
			ss<<colorPathPrefix;
			ss<<frameNo;
			ss<<commonPathSuffix;
			string colorImagePath = ss.str();
 
			hr = NuiImageStreamGetNextFrame(colorStreamHandle, 0, &pColorImageFrame); 
			if (FAILED(hr))
			{
				cout<<"Could not get color image"<<endl; 
				NuiShutdown();
				return -1;
			}
 
			INuiFrameTexture * pTexture = pColorImageFrame->pFrameTexture;
			NUI_LOCKED_RECT LockedRect;
 
			pTexture->LockRect(0, &LockedRect, NULL, 0); 
			//确认获得的数据是否有效
			if( LockedRect.Pitch != 0 ) 
			{ 
				for (int i=0; i<colorImage.rows; i++) 
				{
					uchar *ptr = colorImage.ptr<uchar>(i); 
 
					uchar *pBuffer = (uchar*)(LockedRect.pBits) + i * LockedRect.Pitch;
					for (int j=0; j<colorImage.cols; j++) 
					{ 
						ptr[3*j] = pBuffer[4*j]; 
						ptr[3*j+1] = pBuffer[4*j+1]; 
						ptr[3*j+2] = pBuffer[4*j+2]; 
					} 
				} 
				imwrite(colorImagePath,colorImage);
				imshow("colorImage", colorImage);
			} 
			else 
			{ 
				cout<<"Buffer length of received texture is bogus\r\n"<<endl; 
			}
 
			pTexture->UnlockRect(0);

			NuiImageStreamReleaseFrame(colorStreamHandle, pColorImageFrame ); 
		}
 

		if (WaitForSingleObject(nextDepthFrameEvent, INFINITE)==0)
		{
			stringstream ss;
			ss<<depthPathPrefix;
			ss<<frameNo;
			ss<<commonPathSuffix;
			string depthImagePath = ss.str();
			frameNo++;

			hr = NuiImageStreamGetNextFrame(depthStreamHandle, 0, &pDepthImageFrame);
			if (FAILED(hr))
			{
				cout<<"Could not get color image"<<endl;
				NuiShutdown();
				return -1;
			}
			INuiFrameTexture * pTexture = pDepthImageFrame->pFrameTexture;
			NUI_LOCKED_RECT LockedRect;

			pTexture->LockRect(0, &LockedRect, NULL, 0);

			if( LockedRect.Pitch != 0 )
			{
				for (int i=0; i<depthImage.rows; i++)
				{
					uchar *ptr = depthImage.ptr(i);  
					uchar *pBuffer = (uchar*)(LockedRect.pBits) + i * LockedRect.Pitch;
					USHORT *pBufferRun = (USHORT *)pBuffer;
					 for (int j=0; j<depthImage.cols; j++)  
					{
						ptr[j] = 255-(BYTE)(256*pBufferRun[j]/0x1fff); 
					}
				}
				imwrite(depthImagePath,depthImage);
				imshow("depthImage", depthImage); 
				
			}
			else
			{
				cout<<"Buffer length of received texture is bogus\r\n"<<endl;
			}

			pTexture->UnlockRect(0);

			NuiImageStreamReleaseFrame(depthStreamHandle, pDepthImageFrame );
			if(frameNo == 10)
				break;
		}
 
		if (cvWaitKey(20) == 27) 
			break; 
	} 

	NuiShutdown(); 
	return 0;
}