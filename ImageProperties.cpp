// ImageProperties.cpp: implementation of the ImageProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "ImageProperties.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageProperties::ImageProperties()
{
	Epipole.x = 0;
	Epipole.y = 0;
	AngleStepSize = 1;
	EpipoleRegionSgin = 1;
	xOffsetImagePlane = 0;
	yOffsetImagePlane = 0;
}

ImageProperties::~ImageProperties()
{

}
