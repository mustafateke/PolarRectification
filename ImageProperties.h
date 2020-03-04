// ImageProperties.h: interface for the ImageProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROPERTIES_H__3409EC03_BA83_4CEF_96EA_4CD529ED0B4B__INCLUDED_)
#define AFX_IMAGEPROPERTIES_H__3409EC03_BA83_4CEF_96EA_4CD529ED0B4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PolarRectification.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "cxcore.h"
#include "cv.h"
#include "highgui.h"

class ImageProperties
{
public:
	ImageProperties();
	virtual ~ImageProperties();
	int RectImageHeight;
	int EpipoleRegionSgin;
	int yOffset;
	int xOffset;
	int yOffsetImagePlane;
	int xOffsetImagePlane;
	double AngleStepSize;
	double Kosegen;
	double startAngle;
	double endAngle;
	double maxDistancetoImage;
	double minDistancetoImage;
	double EpipoleOffsetAngle;
	int EpipoleRegion;
	CvPoint2D32f Epipole;
	IplImage* Image;

};

#endif // !defined(AFX_IMAGEPROPERTIES_H__3409EC03_BA83_4CEF_96EA_4CD529ED0B4B__INCLUDED_)
