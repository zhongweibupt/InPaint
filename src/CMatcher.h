#ifndef CMATCHER_H
#define CMATCHER_H

#include "cv.h"

class CMatcher
{
private:
	char* src = "";

	float edgeThresh = 120;
	float radius = 10;
	int lo = 80;
	int hi = 254;

	CvMemStorage * storageSrc = cvCreateMemStorage(0);
	CvMemStorage * storageTemp = cvCreateMemStorage(0);
	CvMemStorage * storage = cvCreateMemStorage(0);


	IplImage* img = 0; 
	IplImage* imgRGB = 0;
	IplImage* imgMask = 0;
	IplImage* imgInpaint = 0;
	IplImage* imgWrite = 0;

	IplImage* imgF = 0; 
	IplImage* imgContour = 0;
	IplImage* imgResult = 0;
	IplImage* imgSrc = 0; 
	IplImage* imgTemp = 0; 
	IplImage* imgContourSrc = 0; 
	IplImage* imgContourTemp = 0; 




public:
	int setSrc(char* src);
	int setParameter(float EdgeThresh, float Radius, int Lo, int Hi);
	int match(char* model);
	int inPaint(void);
	IplImage* getResult(void);
	int write(char* dst);
	int releaseImage(void);
};

#endif