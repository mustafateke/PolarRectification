// PolarRectification.h: interface for the PolarRectification class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLARRECTIFICATION_H__D967EB05_3161_459C_BBE4_8C005847EA89__INCLUDED_)
#define AFX_POLARRECTIFICATION_H__D967EB05_3161_459C_BBE4_8C005847EA89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageProperties.h"

#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include <math.h>

class PolarRectification  
{
public:
	PolarRectification();
	virtual ~PolarRectification();
	void SaveImages();
	double fromRadianToDegree(double Angle);
	double fromDegreeToRadian(double Angle);
	CvSize CalculateImageSize();
	void constructImages();
	int LoadFandEMatrices(char* FileName);
	int initialize();
	int FindOffSetAngle(ImageProperties &imProp);
	int FindEpipoleRegion(CvPoint2D32f epipole);
	void FindCommonRegion(CvPoint2D32f epipole);
	void LoadImages();

private:
	IplImage* Images[2];
	IplImage* RectifiedImage[2];
	ImageProperties ImageProps[2];
	CvMat* FMatrix;


};

#endif // !defined(AFX_POLARRECTIFICATION_H__D967EB05_3161_459C_BBE4_8C005847EA89__INCLUDED_)
