#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <fstream>

#define CAMERA_DEBUG 1

#define MAX_AREAS 100

//OilRigs/Tools
#define UNDEFINED	0
#define RECTANGLE	1
#define TRIANGLE	2
#define CIRCLE		3

using namespace cv;
using namespace std;

int trackbarsEnabled = 0;
int showThresholded = 0;
int showMismatch = 0;
int showMoments = 0;

char key;

//Flame Parameters
double flameAverage[] =
{
    5.15E+08,
    1.37E+08,
    9.52E+08,
    1.76E+09,
    -5.26E+07,
    -3.43E+09,
    -2.45E+09
};

double flameDev[] =
{
    1.07E+08,
    3.89E+07,
    1.44E+08,
    4.43E+08,
    3.48E+08,
    7.98E+08,
    2.01E+08
};

//Figure Parameters
//Circle
double circleAverage[] =
{
    4.07E+10,
    -2.36E+08,
    3.99E+10,
    6.39E+09,
    -2.67E+08,
    -6.70E+09,
    3.47E+07
};

double circleDev[] =
{
    5.91E+09,
	4.58E+08,
	5.74E+09,
	2.70E+10,
	1.94E+10,
	2.78E+10,
	1.84E+10
};

//Triangle
double triangleAverage[] =
{
    1.60E+10,
    -3.28E+07,
    1.56E+10,
    8.93E+09,
    1.04E+11,
    -1.34E+10,
    -8.85E+10

};

double triangleDev[] =
{
    1.31E+09,
    6.60E+08,
    1.13E+09,
    2.48E+11,
    3.41E+11,
    2.40E+11,
    3.39E+11
};

//Rectangle
double rectangleHAverage[] =
{
	4.95E+10,
	-6.07E+08,
	2.99E+10,
	6.41E+10,
	5.88E+09,
	-5.94E+10,
	1.46E+10
};

double rectangleHDev[] =
{
	1.64E+09,
	4.28E+09,
	1.21E+09,
	5.03E+10,
	4.47E+10,
	4.40E+10,
	4.34E+10
};

double rectangleVAverage[] =
{
	3.49E+10,
	-1.64E+09,
	4.71E+10,
	-2.15E+10,
	7.44E+09,
	1.13E+10,
	-3.88E+09
};

double rectangleVDev[] =
{
	3.44E+09,
	6.90E+09,
	2.60E+09,
	6.19E+10,
	3.54E+10,
	6.60E+10,
	3.72E+10
};

/*
CvCapture* initCapture()
{
    return cvCaptureFromCAM(CV_CAP_ANY);  //Capture using any camera connected to your system
}
*/

VideoCapture initCapture()
{
	//VideoCapture cap0(0);
	VideoCapture cap(-1);

	return cap;
}

/*
void releaseCapture(CvCapture* &capture)
{
	cvReleaseCapture(&capture); //Release capture.
}
*/

void releaseCapture(VideoCapture capture)
{
	//cvReleaseCapture(&capture); //Release capture.
	return;
}

int oilDrop(const Mat &imgSource, Mat &imgMarked)
{
    int colorID = 0;
    
    int state = 1;  //Variable for state machine for exploring
    int factor;     // Variable to explore in spiral
    int delta;

    Point centroid[MAX_AREAS];
    imgMarked = Mat::zeros(imgSource.size(), CV_8UC1);  //Start with the empty image
    
    for(int nAreas = 0; nAreas < 100; nAreas++) { //color 100 areas momentsMax
        
        int seedCol = imgSource.cols / 2;
        int seedRow = imgSource.rows / 2;
        Point seed = Point(seedCol, seedRow);   //Place first seed in the middle of the image

        //int attemptsLeft = 30; //Number of attempts to replace the seed
        int attemptsLeft = 20; //Number of attempts to replace the seed
        
        factor = imgSource.cols / 20;
        delta = factor;

        do {
            switch(state)
            {   
                case 1:
                seedRow += delta;  //move right
                delta += factor;   //increase delta
                state = 2;
                break;
                
                case 2:
                seedCol -= delta;  //move up
                delta += factor;   //increase delta
                state = 3;
                break;
                
                case 3:
                seedRow -= delta;  //move left
                delta += factor;   //increase delta
                state = 4;
                break;
                
                case 4:
                seedCol += delta;  //move down
                delta += factor;   //increase delta
                state = 1;
                break;
                
                case 5:
                seedCol = rand() % imgSource.cols;    //place seed in a random location
                seedRow = rand() % imgSource.rows;
                attemptsLeft--;
                state = 1;
                //cout << "Seeding Attempts Left = " << attemptsLeft << endl;
                break;
            }

            if(seedRow >= imgSource.rows || seedRow < 0)
            {
                seedRow = imgSource.rows / 2;
                state = 5;  //Change to random
            }

            if(seedCol >= imgSource.cols || seedCol < 0)
            {
                seedCol = imgSource.cols / 2;
                state = 5;  //Change to random
            }

            seed = Point(seedCol, seedRow); //Generate seed
      
        } while ( (imgSource.at<uchar>(seed) != 255 || (imgMarked.at<uchar>(seed) != 0)) && attemptsLeft > 0);
        //Find a white starting point, if it doesn't find one in c chances, skip

        if (attemptsLeft > 0) {
            
            if(colorID < MAX_AREAS)
                colorID++;

            queue<Point> fifo;
            fifo.push(seed);

            imgMarked.at<uchar>(seed) = colorID;   //Color the seed pixel

            Point coord;
            
            while (!fifo.empty()) {
                for (int state = 0; state < 4; state++) {   //check all 4 sides
                    switch (state) {
                    case 0: //north
                        coord.x = fifo.front().x;
                        coord.y = fifo.front().y - 1;
                        break;
                    case 1: //west
                        coord.x = fifo.front().x - 1;
                        coord.y = fifo.front().y;
                        break;
                    case 2: //south
                        coord.x = fifo.front().x;
                        coord.y = fifo.front().y + 1;
                        break;
                    case 3: //east
                        coord.x = fifo.front().x + 1;
                        coord.y = fifo.front().y;
                        break;
                    }

                    if (coord.y >= 0 && coord.y < imgSource.rows && coord.x >= 0 && coord.x < imgSource.cols) { //is in range
                        unsigned char temp = imgMarked.at<uchar>(coord);
                        if (temp != colorID) {                            //AND is not colored in the destination yet
                            if (imgSource.at<uchar>(coord) != 0) {        //AND is not 0 in the binary image
                                imgMarked.at<uchar>(coord) = colorID;      //color the destination
                                //cout << "Coloured - " << colorID << endl;
                                fifo.push(Point(coord));            //enqueue this point for analysis
                            }
                        }
                    }
                }

                fifo.pop();
            }
        }
    }

    return colorID; //return number of areas;
}

double compareWithCircle(Moments oMoments)
{
    double momentFactors[7];
    double factor;
    
    momentFactors[0] = abs((oMoments.mu20  - circleAverage[0]) / circleDev[0]);
    momentFactors[1] = abs((oMoments.mu11  - circleAverage[1]) / circleDev[1]);
    momentFactors[2] = abs((oMoments.mu02  - circleAverage[2]) / circleDev[2]);
    momentFactors[3] = abs((oMoments.mu30  - circleAverage[3]) / circleDev[3]);
    momentFactors[4] = abs((oMoments.mu21  - circleAverage[4]) / circleDev[4]);
    momentFactors[5] = abs((oMoments.mu12  - circleAverage[5]) / circleDev[5]);
    momentFactors[6] = abs((oMoments.mu03  - circleAverage[6]) / circleDev[6]);

    factor = 0;
    factor += momentFactors[0] / 7.0;
    factor += momentFactors[1] / 7.0;
    factor += momentFactors[2] / 7.0;
    factor += momentFactors[3] / 7.0;
    factor += momentFactors[4] / 7.0;
    factor += momentFactors[5] / 7.0;
    factor += momentFactors[6] / 7.0;

    //cout << "circle factor:" << factor << endl;
    
    return factor;
}

double compareWithTriangle(Moments oMoments)
{
    double momentFactors[7];
    double factor;
    
    momentFactors[0] = abs((oMoments.mu20  - triangleAverage[0]) / triangleDev[0]);
    momentFactors[1] = abs((oMoments.mu11  - triangleAverage[1]) / triangleDev[1]);
    momentFactors[2] = abs((oMoments.mu02  - triangleAverage[2]) / triangleDev[2]);
    momentFactors[3] = abs((oMoments.mu30  - triangleAverage[3]) / triangleDev[3]);
    momentFactors[4] = abs((oMoments.mu21  - triangleAverage[4]) / triangleDev[4]);
    momentFactors[5] = abs((oMoments.mu12  - triangleAverage[5]) / triangleDev[5]);
    momentFactors[6] = abs((oMoments.mu03  - triangleAverage[6]) / triangleDev[6]);

    factor = 0;
    factor += momentFactors[0] / 7.0;
    factor += momentFactors[1] / 7.0;
    factor += momentFactors[2] / 7.0;
    factor += momentFactors[3] / 7.0;
    factor += momentFactors[4] / 7.0;
    factor += momentFactors[5] / 7.0;
    factor += momentFactors[6] / 7.0;

    //cout << "triangle factor:" << factor << endl;
    
    return factor;
}

double compareWithRectangleH(Moments oMoments)
{
    double momentFactors[7];
    double factor;
    
    momentFactors[0] = abs((oMoments.mu20  - rectangleHAverage[0]) / rectangleHDev[0]);
    momentFactors[1] = abs((oMoments.mu11  - rectangleHAverage[1]) / rectangleHDev[1]);
    momentFactors[2] = abs((oMoments.mu02  - rectangleHAverage[2]) / rectangleHDev[2]);
    momentFactors[3] = abs((oMoments.mu30  - rectangleHAverage[3]) / rectangleHDev[3]);
    momentFactors[4] = abs((oMoments.mu21  - rectangleHAverage[4]) / rectangleHDev[4]);
    momentFactors[5] = abs((oMoments.mu12  - rectangleHAverage[5]) / rectangleHDev[5]);
    momentFactors[6] = abs((oMoments.mu03  - rectangleHAverage[6]) / rectangleHDev[6]);

    factor = 0;
    factor += momentFactors[0] / 7.0;
    factor += momentFactors[1] / 7.0;
    factor += momentFactors[2] / 7.0;
    factor += momentFactors[3] / 7.0;
    factor += momentFactors[4] / 7.0;
    factor += momentFactors[5] / 7.0;
    factor += momentFactors[6] / 7.0;

    //cout << "rectangle factor:" << factor << endl;
    
    return factor;
}

double compareWithRectangleV(Moments oMoments)
{
    double momentFactors[7];
    double factor;
    
    momentFactors[0] = abs((oMoments.mu20  - rectangleVAverage[0]) / rectangleVDev[0]);
    momentFactors[1] = abs((oMoments.mu11  - rectangleVAverage[1]) / rectangleVDev[1]);
    momentFactors[2] = abs((oMoments.mu02  - rectangleVAverage[2]) / rectangleVDev[2]);
    momentFactors[3] = abs((oMoments.mu30  - rectangleVAverage[3]) / rectangleVDev[3]);
    momentFactors[4] = abs((oMoments.mu21  - rectangleVAverage[4]) / rectangleVDev[4]);
    momentFactors[5] = abs((oMoments.mu12  - rectangleVAverage[5]) / rectangleVDev[5]);
    momentFactors[6] = abs((oMoments.mu03  - rectangleVAverage[6]) / rectangleVDev[6]);

    factor = 0;
    factor += momentFactors[0] / 7.0;
    factor += momentFactors[1] / 7.0;
    factor += momentFactors[2] / 7.0;
    factor += momentFactors[3] / 7.0;
    factor += momentFactors[4] / 7.0;
    factor += momentFactors[5] / 7.0;
    factor += momentFactors[6] / 7.0;

    //cout << "rectangle factor:" << factor << endl;
    
    return factor;
}

double compareWithFlame(Moments oMoments)
{
    double momentFactors[7];
    double factor;
    
    momentFactors[0] = abs((oMoments.mu20  - flameAverage[0]) / flameDev[0]);
    momentFactors[1] = abs((oMoments.mu11  - flameAverage[1]) / flameDev[1]);
    momentFactors[2] = abs((oMoments.mu02  - flameAverage[2]) / flameDev[2]);
    momentFactors[3] = abs((oMoments.mu30  - flameAverage[3]) / flameDev[3]);
    momentFactors[4] = abs((oMoments.mu21  - flameAverage[4]) / flameDev[4]);
    momentFactors[5] = abs((oMoments.mu12  - flameAverage[5]) / flameDev[5]);
    momentFactors[6] = abs((oMoments.mu03  - flameAverage[6]) / flameDev[6]);

    factor = 0;
    factor += momentFactors[0] / 7.0;
    factor += momentFactors[1] / 7.0;
    factor += momentFactors[2] / 7.0;
    factor += momentFactors[3] / 7.0;
    factor += momentFactors[4] / 7.0;
    factor += momentFactors[5] / 7.0;
    factor += momentFactors[6] / 7.0;

    //cout << "flame factor:" << factor << endl;
    
    return factor;
}


bool scanFlame(Mat &imgOriginal, Point &location)
{
	//Color filter parameters
	const int iLowH = 0;
	const int iHighH = 255;

	const int iLowS = 0; 
	const int iHighS = 225;

	const int iLowV = 0;
	const int iHighV = 110;
	
	Mat imgHSV;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    Mat imgThresholded;

    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
          
    //Morphological opening (removes small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

    //Morphological closing (removes small holes from the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //Segmentation
    Mat imgMarked, imgArea;
    int nareas;
    nareas = oilDrop(imgThresholded, imgMarked);

    double currentFactor, bestFactor;
    
    bestFactor = 10000;

    for(int i = 1; i <= nareas; i++)
    {
        inRange(imgMarked, i, i, imgArea); //Threshold the image

        //Identification
        //Calculate the moments of the thresholded image
        Moments oMoments = moments(imgArea);
        
        double dArea = oMoments.m00;
		
        if (dArea > 1000000 && dArea < 4000000)	///Flame area - according to tests
        {
            currentFactor = compareWithFlame(oMoments);
			
            if(currentFactor < bestFactor)
            {
                bestFactor = currentFactor;

                int posX = oMoments.m10 / oMoments.m00;
                int posY = oMoments.m01 / oMoments.m00;
                location = Point(posX, posY);
            }
        }

    }

    if(bestFactor < 3.2)
    {  
        return true;
    }
    else
    {
        location = Point(-1,-1);
        return false;
    }
}

int scanFigure(Mat &imgOriginal, Point &location)
{
	//Color filter parameters
	const int iLowH = 0;
	const int iHighH = 255;

	const int iLowS = 0; 
	const int iHighS = 225;

	const int iLowV = 0;
	const int iHighV = 62;
	
	Mat imgHSV;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    Mat imgThresholded;

    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

    //Morphological opening (removes small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) ); 

    //Morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) ); 
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

    //Morphological opening (removes small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)) ); 

    //Morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(25, 25)) ); 
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(25, 25)) );

    //Segmentation
    Mat imgMarked, imgArea;
    int nareas;
    nareas = oilDrop(imgThresholded, imgMarked);

    double currentFactor, bestFactor;
    char figure;
    
    bestFactor = 10000;
    
    for(int i = 1; i <= nareas; i++)
    {
        inRange(imgMarked, i, i, imgArea); //Threshold the image

        //Identification
        //Calculate the moments of the thresholded image
        Moments oMoments = moments(imgArea);
        
        double dArea = oMoments.m00;
		
        if (dArea > 5500000)	//Figure area - according to tests
        {
		currentFactor = compareWithCircle(oMoments);         
            if(currentFactor < bestFactor)
            {
                bestFactor = currentFactor;
                figure = CIRCLE;

                int posX = oMoments.m10 / oMoments.m00;
                int posY = oMoments.m01 / oMoments.m00;
                location = Point(posX, posY);
            }
            
            currentFactor = compareWithTriangle(oMoments);           
            if(currentFactor < bestFactor)
            {
                bestFactor = currentFactor;
                figure = TRIANGLE;

                int posX = oMoments.m10 / oMoments.m00;
                int posY = oMoments.m01 / oMoments.m00;
                location = Point(posX, posY);
            }

            currentFactor = compareWithRectangleH(oMoments) * 1.05;           
            if(currentFactor < bestFactor)
            {
                bestFactor = currentFactor;
                figure = RECTANGLE;

                int posX = oMoments.m10 / oMoments.m00;
                int posY = oMoments.m01 / oMoments.m00;
                location = Point(posX, posY);
            }

            currentFactor = compareWithRectangleV(oMoments) * 1.05;           
            if(currentFactor < bestFactor)
            {
                bestFactor = currentFactor;
                figure = RECTANGLE;

                int posX = oMoments.m10 / oMoments.m00;
                int posY = oMoments.m01 / oMoments.m00;
                location = Point(posX, posY);
            }
            
        }

    }

    if(bestFactor > 3.8)
    {
		location = Point(-1,-1);
        figure = UNDEFINED;
    }

    return figure;
}

//int scanBurningOilRigP1(CvCapture* capture)
int scanBurningOilRigP1(VideoCapture capture)
{
	Mat imgOriginal;
	Point flameLocation;
	bool located;
	
	#if CAMERA_DEBUG
	cout << "Waiting for the camera to start correctly..." << endl;
	#endif
	
	//Wait for the camera to start correctly
	for(int i = 0; i < 5; i++)
	{
		//imgOriginal = cvQueryFrame(capture); //Create image frames from capture
		capture >> imgOriginal;
		sleep(1);
	}
	
	//12 attempts max
	for(int i = 0; i < 12; i++)
	{
		#if CAMERA_DEBUG
		cout << "Scanning burning oil - attempt " << i + 1 << endl;
		#endif
		
		//imgOriginal = cvQueryFrame(capture); //Create image frames from capture
		capture >> imgOriginal;
		located = scanFlame(imgOriginal, flameLocation);
		
		if(located)
			break;
		
		usleep(200000);  // wait for 0.2 seconds
	}

	if(located)
	{
		if(flameLocation.x > 0 && flameLocation.x < 320)
			return RECTANGLE;
		else if(flameLocation.x > 320 && flameLocation.x < 640)
			return TRIANGLE;
		else
			return UNDEFINED;
	}
	else
	{
		return UNDEFINED;
	}
}

//int scanBurningOilRigP2(CvCapture* capture)
int scanBurningOilRigP2(VideoCapture capture)
{
	Mat imgOriginal;
	Point flameLocation;
	bool located;
	
	#if CAMERA_DEBUG
	cout << "Waiting for the camera to start correctly..." << endl;
	#endif
	
	//Wait for the camera to start correctly
	for(int i = 0; i < 5; i++)
	{
		//imgOriginal = cvQueryFrame(capture); //Create image frames from capture
		capture >> imgOriginal;
		sleep(1);
	}
	
	//12 attempts max
	for(int i = 0; i < 12; i++)
	{
		#if CAMERA_DEBUG
		cout << "Scanning burning oil - attempt " << i + 1 << endl;
		#endif
		
		//imgOriginal = cvQueryFrame(capture); //Create image frames from capture
		capture >> imgOriginal;
		located = scanFlame(imgOriginal, flameLocation);
		
		if(located)
			break;
		
		usleep(200000);  // wait for 0.2 seconds
	}

	if(located)
	{
		if(flameLocation.x > 0 && flameLocation.x < 320)
			return TRIANGLE;
		else if(flameLocation.x > 320 && flameLocation.x < 640)
			return CIRCLE;
		else
			return UNDEFINED;
	}
	else
	{
		return UNDEFINED;
	}
}

//int scanThisFigure(CvCapture* capture)
int scanThisFigure(VideoCapture capture)
{
	Mat imgOriginal;
	Point figureLocation;
	int figure;
	
	#if CAMERA_DEBUG
	cout << "Waiting for the camera to start correctly..." << endl;
	#endif
	
	//Wait for the camera to start correctly
	for(int i = 0; i < 5; i++)
	{
		//imgOriginal = cvQueryFrame(capture); //Create image frames from capture
		capture >> imgOriginal;
		sleep(1);
	}
	
	//12 attempts max
	for(int i = 0; i < 12; i++)
	{
		#if CAMERA_DEBUG
		cout << "Scanning figure - attempt " << i + 1 << endl;
		#endif
		
		//imgOriginal = cvQueryFrame(capture); //Create image frames from capture
		capture >> imgOriginal;
		figure = scanFigure(imgOriginal, figureLocation);
		
		if(figure != UNDEFINED)
			break;
		
		usleep(200000);  // wait for 0.2 seconds
	}

	return figure;
}

//