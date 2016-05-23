#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "CEdge.h"
#include "CMatcher.h"

int CMatcher::setSrc(char* src)
{
	this->img = cvLoadImage(src, 0);
	this->imgRGB = cvLoadImage(src, 1);
	return 1;
}

int CMatcher::setParameter(float EdgeThresh, float Radius, int Lo, int Hi)
{
	this->edgeThresh = EdgeThresh;
	this->radius = Radius;
	this->lo = Lo;
	this->hi = Hi;
	return 1;
}


int CMatcher::match(char* model)
{
	CEdge edgeSrc;
	edgeSrc.setEdgeThresh(this->edgeThresh);
	edgeSrc.extractEdge(this->src);
	this->img = edgeSrc.getEdge();

	this->imgSrc = this->img;
	this->imgTemp = cvLoadImage(model, 0);
	this->imgContourSrc = cvCreateImage(cvGetSize(this->imgSrc), IPL_DEPTH_32F, 1);
	this->imgContourTemp = cvCreateImage(cvGetSize(this->imgTemp), IPL_DEPTH_32F, 1);
	cvZero(this->imgContourSrc);
	cvZero(this->imgContourTemp);

	CvSeq * contoursSrc = 0;
	this->storageSrc = cvCreateMemStorage(0);

	cvFindContours(this->imgSrc, this->storageSrc, &contoursSrc, sizeof(CvContour),
		CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	cvDrawContours(this->imgContourSrc, contoursSrc,
		CV_RGB(0, 0, 255), CV_RGB(255, 0, 0),
		2, -1, 8);

	CvSeq * contoursTemp = 0;
	this->storageTemp = cvCreateMemStorage(0);

	cvFindContours(this->imgTemp, this->storageTemp, &contoursTemp, sizeof(CvContour),
		CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	cvDrawContours(this->imgContourTemp, contoursTemp,
		CV_RGB(0, 0, 255), CV_RGB(255, 0, 0),
		2, -1, 8);


	CvSize sizeSrc = cvGetSize(this->imgSrc);
	CvSize sizeTemp = cvGetSize(this->imgTemp);
	CvSize sizeResult = cvSize(sizeSrc.width - sizeTemp.width + 1, sizeSrc.height - sizeTemp.height + 1);
	this->imgResult = cvCreateImage(sizeResult, IPL_DEPTH_32F, 1);
	cvMatchTemplate(this->imgContourSrc, this->imgContourTemp, imgResult, 4);

	float dMax = 0.;
	CvPoint point = cvPoint(0, 0);

	for (int cx = 0; cx < sizeResult.width; cx++)
	{
		for (int cy = 0; cy < sizeResult.height; cy++)
		{
			float fTemp = CV_IMAGE_ELEM(imgResult, float, cy, cx);
			if (dMax < fTemp)
			{
				dMax = fTemp;
				point = cvPoint(cx, cy);
			}
		}
	}

	CvPoint point2 = cvPoint(point.x + sizeTemp.width, point.y + sizeTemp.height);
	cvRectangle(this->img, point, point2, cvScalar(255));

	this->imgF = cvLoadImage(model, 0);
	this->imgContour = cvCreateImage(cvGetSize(imgF), 32, 1);

	cvThreshold(imgF, imgF, lo, hi, CV_THRESH_BINARY);
	cvConvertScale(imgF, imgContour, 1 / 255.);

	CvSeq * contours = 0;
	CvMemStorage * storage = cvCreateMemStorage(0);

	cvFindContours(this->imgTemp, storage, &contours, sizeof(CvContour),
		CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	cvDrawContours(imgContour, contours,
		CV_RGB(255, 255, 255), CV_RGB(255, 255, 255),
		2, 2, 8);

	int channels = imgRGB->nChannels;
	for (int cx = 0; cx < sizeTemp.width; cx++)
	{
		for (int cy = 0; cy < sizeTemp.height; cy++)
		{
			int fTemp = CV_IMAGE_ELEM(imgContour, int, cy, cx);
			if (0 < fTemp)
			{
				int j = point.x + cx;
				int i = point.y + cy;
				(imgRGB->imageData + i*imgRGB->widthStep)[j* channels + 0] = 255;
				(imgRGB->imageData + i*imgRGB->widthStep)[j* channels + 1] = 255;
				(imgRGB->imageData + i*imgRGB->widthStep)[j* channels + 2] = 255;
			}
		}
	}

	this->imgMask = cvCreateImage(cvGetSize(this->img), IPL_DEPTH_8U, 1);

	for (int cx = 0; cx < sizeTemp.width; cx++)
	{
		for (int cy = 0; cy < sizeTemp.height; cy++)
		{
			int fTemp = CV_IMAGE_ELEM(imgContour, int, cy, cx);
			if (0 < fTemp)
			{
				int j = point.x + cx;
				int i = point.y + cy;
				cvSet2D(this->imgMask, i, j, 255);
			}
		}
	}
	return 1;
}

int CMatcher::inPaint(void)
{
	cvInpaint(this->imgRGB, this->imgMask, this->imgInpaint, this->radius, CV_INPAINT_TELEA);
	return 1;
}

IplImage* CMatcher::getResult(void)
{
	return this->imgInpaint;
}

int CMatcher::write(char* dst)
{
	this->imgWrite = cvCreateImage(cvGetSize(this->img), IPL_DEPTH_8U, 3);
	cvConvertScale(this->imgInpaint, this->imgWrite);
	cvSaveImage(dst, this->imgWrite);
	return 1;
}

int CMatcher::releaseImage(void)
{
	cvReleaseMemStorage(&this->storageSrc);
	cvReleaseMemStorage(&this->storageTemp);
	
	cvReleaseImage(&this->imgSrc);
	cvReleaseImage(&this->imgTemp);
	cvReleaseImage(&this->imgContourSrc);
	cvReleaseImage(&this->imgContourTemp);
	cvReleaseImage(&this->imgContour);
	cvReleaseImage(&this->imgF);
	cvReleaseImage(&this->imgResult);

	cvReleaseImage(&this->img);
	cvReleaseImage(&this->imgRGB);
	cvReleaseImage(&this->imgMask);
	cvReleaseImage(&this->imgInpaint);
	cvReleaseImage(&this->imgWrite);

	return 1;
}