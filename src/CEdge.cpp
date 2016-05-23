#include "CEdge.h"
#include <cstdio>
#include <iostream>
#include "cv.h"
#include "highgui.h"

int CEdge::setEdgeThresh(int thresh)
{
	this->edgeThresh = thresh;
	return 1;
}

int CEdge::extractEdge(char* str)
{
	this->fileName = str;
	if ((this->image = cvLoadImage(this->fileName, 1)) == 0)
		return 0;
	this->cEdge = cvCreateImage(cvSize(this->image->width, this->image->height), IPL_DEPTH_8U, 3);
	this->gray = cvCreateImage(cvSize(this->image->width, this->image->height), IPL_DEPTH_8U, 1);
	this->edge = cvCreateImage(cvSize(this->image->width, this->image->height), IPL_DEPTH_8U, 1);
	cvCvtColor(this->image, this->gray, CV_BGR2GRAY);

	cvSmooth(this->gray, this->edge, CV_BLUR, 3, 3, 0, 0);
	cvNot(this->gray, this->edge);

	cvCanny(this->gray, this->edge, (float)this->edgeThresh, (float)this->edgeThresh * 3, 3);

	cvZero(this->cEdge);
	cvCopy(this->image, this->cEdge, this->edge);

	return 1;
}

int CEdge::cEdgeShow(void)
{
	char wndName[] = "ColorEdge";
	cvNamedWindow(wndName, 1);
	cvShowImage(wndName, this->cEdge);
	cvWaitKey(0);
	cvDestroyWindow(wndName);

	return 1;
}

int CEdge::edgeShow(void)
{
	char wndName[] = "Edge";
	cvNamedWindow(wndName, 1);
	cvShowImage(wndName, this->edge);
	cvWaitKey(0);
	cvDestroyWindow(wndName);

	return 1;
}

IplImage* CEdge::getCEdge(void)
{
	return this->cEdge;
}

IplImage* CEdge::getEdge(void)
{
	return this->edge;
}

int CEdge::releaseImage(void)
{
	if(0 != this->image)
		cvReleaseImage(&this->image);
	if(0 != this->gray)
		cvReleaseImage(&this->gray);
	if(0 != this->edge)
		cvReleaseImage(&this->edge);
	if(0 != this->cEdge)
		cvReleaseImage(&this->cEdge);
	return 1;
}

/*
int main(int argc, char** argv)
{
	char* fileName = argc == 2 ? argv[1] : (char*)"test.jpg";

	Edge edge;

	if (edge.extractEdge(fileName) == 0)
	{
		return -1;
	}

	edge.edgeShow();
	edge.releaseImage();

	return 1;
}*/