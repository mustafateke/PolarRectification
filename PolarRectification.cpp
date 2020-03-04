// PolarRectification.cpp: implementation of the PolarRectification class.
//
//////////////////////////////////////////////////////////////////////

#include "PolarRectification.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#include "PolarRectification.h"

#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include <math.h>

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

double CalculateDistance(CvPoint2D32f pt1, CvPoint2D32f pt2)
{
	return sqrt((pt1.x - pt2.x)*(pt1.x-pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y));
}

double CalculateSlope(CvPoint2D32f pt1, CvPoint2D32f pt2)
{
	return (pt1.y - pt2.y)/(pt1.x-pt2.x);
}

PolarRectification::PolarRectification()
{

}

PolarRectification::~PolarRectification()
{

}

void PolarRectification::LoadImages()
{
	ImageProps[0].Image = Images[0] = cvLoadImage( "digcam1.jpg" );
	ImageProps[1].Image = Images[1] = cvLoadImage( "digcam2.jpg" );
}

int PolarRectification::FindEpipoleRegion( CvPoint2D32f epipole)
{
	CvPoint region;
	
	if(epipole.x < ImageProps[0].yOffset-Images[0]->height/2)
	{
		region.x = 0;
	}
	else if(epipole.x < ImageProps[0].yOffset+Images[0]->height/2)
	{
		region.x = 1;
	}
	else
		region.x = 2;
		
	if(epipole.y < ImageProps[0].yOffset-Images[0]->height/2)
	{
		region.y = 0;
	}
	else if(epipole.y < ImageProps[0].yOffset+Images[0]->height/2)
	{
		region.y = 1;
	}
	else
		region.y = 2;	

	int Region = region.x + 3*region.y;
	
	return Region;
}


int PolarRectification::FindOffSetAngle(ImageProperties &imProp)
{
	switch(imProp.EpipoleRegion)
	{
		case 0:
		case 3:
		case 6:
			imProp.EpipoleOffsetAngle = 360;
			break;
		case 2:
		case 5:
		case 8:
			imProp.EpipoleOffsetAngle = 180;
			break;
		case 1:
			imProp.EpipoleOffsetAngle = 270;
			break;
		case 7:
			imProp.EpipoleOffsetAngle = 90;
				break;
		}
		
		return 0;

}

int PolarRectification::initialize()
{
	LoadFandEMatrices("f.txt");
	ImageProps[0].xOffset = 0;//ImageProps[0].Image->width/2;
	ImageProps[0].yOffset = 0;
	
	ImageProps[1].xOffset = 0;//ImageProps[0].Image->width/2;
	ImageProps[1].yOffset = 0;
	
	ImageProps[0].xOffsetImagePlane = ImageProps[1].xOffsetImagePlane = ImageProps[0].Image->width/2;
	ImageProps[0].yOffsetImagePlane = ImageProps[1].yOffsetImagePlane = ImageProps[0].Image->height/2;
	
	ImageProps[0].EpipoleRegion = FindEpipoleRegion(ImageProps[0].Epipole);
	ImageProps[1].EpipoleRegion = FindEpipoleRegion(ImageProps[1].Epipole);
	
	FindOffSetAngle(ImageProps[0]);
	FindOffSetAngle(ImageProps[1]);
	
	findImageParams(ImageProps[0]);
	findImageParams(ImageProps[1]);
	
	return 0;
}

void PolarRectification::findImageParam(ImageProperties &imProp)
{
	CvSize size;
	CvPoint2D32f Corners[4];
	double Distances[4];
	double Slopes[4];
	double tempSlopes[4];
	vector<double> Angles;
	int maxID = 0, minID = 0;
	double minDistance = 1000000, maxDistance = 0;
	double minAngle = 360, maxAngle = -360;
	if(imProp.Image != NULL)
	{
		size = cvGetSize(imProp.Image);
	}
	Corners[0] = cvPoint2D32f(imProp.xOffset - size.width/2,
								imProp.yOffset + size.height/2);
								
	Corners[1] = cvPoint2D32f(imProp.xOffset + size.width/2,
								imProp.yOffset + size.height/2);

	Corners[2] = cvPoint2D32f(imProp.xOffset - size.width/2,
								imProp.yOffset - size.height/2);

	Corners[3] = cvPoint2D32f(imProp.xOffset + size.width/2,
								imProp.yOffset - size.height/2);	

	for(int i = 0; i < 4 ; i++)
	{
		Distances[i] = (CalculateDistance(imProp.Epipole,Corners[i]));
	}
	
	for(i = 0; i < 4; i++)
	{
		if(Distances[i] < minDistance)
		{
			minDistance = Distances[i];
			minID = i;
		}
		if(Distances[i] > maxDistance)
		{
			maxDistance = Distances[i];
			maxID = i;
		}
	}
	
	imProp.Kosegen = CalculateDistance(Corners[0],Corners[3]);
	imProp.minDistancetoImage = distances[ maxID ] - imProp.Kosegen;
	imProp.maxDistancetoImage = Distances[ maxID ];
	
	for( i = 0; i < 4; i++ )
	{
		tempSlopes[ i ] = CalculateSlope(imProp.Epipole, Corners[i];
		if(imProp.EpipoleRegion == 1 || imProp.EpipoleRegion == 7)
		{
			tempSlopes[ i ] = -1/tempSlopes[ i ];
		}
		Slopes[ i ] = atan( tempSlopes[ i ]);
		Angles.push_back( imProp.EpipoleOffsetAngle + fromRadiantoDegree( Slopes[ i ] ));
	}
	
	minID = 0;
	maxID = 0;
	
	for( i = 0; i < 4; i++)
	{
		double see = Angles[ i ];
		if(Angles[i] < minAngle)
		{
			minAngle = Angles[i];
			minID = i;
		}
		if(Angles[i] > maxAngle)
		{
			maxAngle = Angles[i];
			maxID = i;
		}
	}

	imProp.startAngle = Angles[ maxID ];
	imProp.endAngle = Angles [ minID ];

	if(imProp.EpipoleRegion == 0 || imProp.EpipoleRegion == 3 
		|| imProp.EpipoleRegion == 6)
	{
		double temp = imProp.startAngle;
		imProp.startAngle = imProp.endAngle;
		imProp.endAngle = imProp.startAngle;
		imProp.EpipoleRegionSgin = -1;
	}

	typedef vector<double> IntVector;
	typedef IntVector::iterator IntVectorIt;

	IntVector Numbers(4);

	IntVectorIt start,end,it;

	start = Numbers.begin();
	end = Numbers.end();

	sort(start,end);

	i = 0;

	double tempOrder[4];
	for(it = start; it != end; it++)
	{
		cout << *it << endl;
		tempOrder[ i++ ] = *it;
	}

	double AngleDifference = tempOrder[2] - tempOrder[1];
	double farDifference = CalculateDistance(Corners[ minID ],
											 Corners[ maxID ]
											 );

	imProp.AngleStepSize = AngleDifference/farDifference;
	imProp.RectImageHeight = (tempOrder[ 3 ] - tempOrder[ 0 ])
								/ imProp.AngleStepSize;

}

int PolarRectification::LoadFandEMatrices(char* FileName)
{

	return 0;
}
void PolarRectification::constructImages()
{
	CvSize MaxImageSize = CalculateImageSize();
	RectifiedImage[0] = cvCreateImage(
		MaxImageSize,
		Images[0]->depth,
		Images[0]->nChannels
		);

	RectifiedImage[1] = cvCreateImage(
		MaxImageSize,
		Images[0]->depth,
		Images[0]->nChannels
		);

	int x,y;
	CvPoint2D32f EgimTestNoktasi;

	vector <double> Acilar;
	double Acilar2[2000];

	int lowerLimitX,lowerLimitY, upperLimitX, upperLimitY;

	lowerLimitX = (int)(ImageProps[0].xOffset - ImageProps[0].Image->width/2);
	upperLimitX = (int)(ImageProps[0].xOffset + ImageProps[0].Image->width/2);
	lowerLimity = (int)(ImageProps[0].yOffset - ImageProps[0].Image->height/2);
	upperLimitY = (int)(ImageProps[0].yOffset + ImageProps[0].Image->height/2);


	for(int index = 0; index < (int)ImageProps[0].RectImageHeight; index++)
	{
		for(int j = (int)ImageProps[0].minDistancetoImage; j < ImageProps[0].maxDistancetoImage; j++)
		{
			x = (int)(ImageProps[0].Epipole.x + 
						j*cos(fromDegreeToRadian(ImageProps[0].startAngle - 
												 ImageProps[0].EpipoleRegionSgin*index*ImageProps[0].AngleStepSize) ) );

			y = (int)(ImageProps[0].Epipole.y + 
						j*sin(fromDegreeToRadian(ImageProps[0].startAngle - 
												 ImageProps[0].EpipoleRegionSgin*index*ImageProps[0].AngleStepSize) ) );



			if(((( x > lowerLimitX ) && ( x < upperLimitX ))
				&&
			  (( ( y > lowerLimitX ) && ( y < upperLimitY )))
			{
				EgimTestNoktasi = cvPoint2D32f(x,y);

