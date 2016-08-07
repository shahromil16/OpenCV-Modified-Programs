// Red_Ball_Detectino.cpp

// OpenCV header files
#include<opencv\cvaux.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>

// C++ Header files
#include<stdio.h>
#include<stdlib.h>
#include "windows.h"
#include "iostream"

// DLL files fro Console Audio
#using <mscorlib.dll>

using namespace::std;
using namespace System;

int main(int argc, char* argv[])
{
	CvSize size640x480 = cvSize(640, 480);		//Makeing window of 640 * 480
	
	CvCapture* p_capwebcam;			// Webcam assigned to this variable

	IplImage* p_imgOriginal;		// Input image from webcam
	IplImage* p_imgProcessed;		// Pointer to image processed

	CvMemStorage* p_strStorage;		// necessary variable to pass in to CvHough Circles()

	CvSeq* p_seqCircles;				// An OpenCV sequence will be returned by CvHough Circles() and will contain all circles
									// CvGetSeqElem(p_seqCircles. i) will return 3 element array of the ith circle (see next variable)
	
	float* p_fltXYRadius;				// Pointer to a 3 element array of flots
									// [0] => X Position of Detected Object
									// [1] => Y Position of Detected Object
									// [2] => Radius of the detected Object

	int i;							// Loop counter
	char charCheckForESC;			// Char for checking key Press (ESC exits the program)

	p_capwebcam = cvCaptureFromCAM(0);			// Use 1st WebCam.

	if(p_capwebcam == NULL)
	{
		printf("Error: Camera Output is null \n");
		getchar();
		return(-1);					//Exit from a program
	}

	// Decleration of 2 Windoes

	cvNamedWindow("Original Window", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Processed Window", CV_WINDOW_AUTOSIZE);

	p_imgProcessed = cvCreateImage(size640x480,				// 640 x 480 pixels
								   IPL_DEPTH_8U,			// Depth 8 bit unsigned
								   1);						// Channel = 1 or Black and White image

	while(1)
	{
		p_imgOriginal = cvQueryFrame(p_capwebcam);

		if(p_imgOriginal == NULL)
		{
			printf("Error: Original Frame is not captured successfully \n");
			getchar();
			break;
		}

		/*********************************************
		BLUE Color DETECTION
		**********************************************/

		cvInRangeS(p_imgOriginal,
				   CV_RGB(  0,  0,180),
				   CV_RGB(100,100,255),
				   p_imgProcessed);

		p_strStorage = cvCreateMemStorage(0);		// Allocate storage to pass variable into cvHoughCirlceS()

		cvSmooth(p_imgProcessed,		// Input image
				 p_imgProcessed,		// Output image
				 CV_GAUSSIAN,			// Gaussian Filter. Average the value of nearer pixels with closest pixels having higher weightage
				 9,						// Pixels in Row
				 9);					// Pixels in Column

		p_seqCircles = cvHoughCircles(	p_imgProcessed,				// Input Image
										p_strStorage,				// Provide function with Storgae memory
										CV_HOUGH_GRADIENT,			// Algorithm which provide Circle detection
										2,							// size of image / 2 = zccumulator resolution i.e. acc_res = size of image / 2
										p_imgProcessed->height/4,	// Minimun distance in pixels b/w centers of the deected circles
										100,						// High threshold of canny edge detector called by cvHoughCircles()
										50,							// Low threshold of canny edge detector called by cvHoughCircles()
										10,							// Minimum circle radius in pixels
										800);						// Maximum circle radius in pixels
		

		for(i = 0; i < p_seqCircles->total; i++)							//for each element in sequential circles 
		{			
			p_fltXYRadius = (float*)cvGetSeqElem (p_seqCircles, i);			//From the sequintial Circles read the ith value
			printf("Ball Position: X = %f	Y = %f	R = %f \n",	p_fltXYRadius[0],	//X Position of Center of circle
																p_fltXYRadius[1],	//X Position of Center of circle
																p_fltXYRadius[2]);	// Radius of Circle

			// Draw a small green circle at center

			cvCircle(	p_imgOriginal,
						cvPoint(cvRound(p_fltXYRadius[0]),cvRound(p_fltXYRadius[1])),			//Center of circle
						3,												// Radious in Pixels 
						CV_RGB(   0, 255,   0),							// Green Circle in Center
						CV_FILLED);										// Circle is filled

			// Draw Red circle around the detected object

			cvCircle(	p_imgOriginal,
						cvPoint(cvRound(p_fltXYRadius[0]),cvRound(p_fltXYRadius[1])),
						cvRound(p_fltXYRadius[2]),				// Radius of circle = Radius of detecetd circle
						CV_RGB( 255,   0,   0),					// Draw Pure RED
						3);										// Thickness of circle in pixels
			//////////// Beep sound for blue color
			if(i == p_seqCircles->total - 1)
				for(int j=0; j<2;j++)
				{
					Console::Beep(1600,200);				// Frequency = 1600 and Duration = 200 ms
					// Console::Beep(1600 + (int)p_fltXYRadius[0],200+(int)p_fltXYRadius[1]);				// Frequency and Duration is a function of X and Y co-ordinate
				}
			///////////// Beep sound over


		}			//End of the for loop
		
		
		cvShowImage("Original Window",p_imgOriginal);			// Original image with circles overlaping
		cvShowImage("Processed Window", p_imgProcessed);		// Processed Image Black and white as red ball detected

		cvReleaseMemStorage(&p_strStorage);					// Deallocate the memory which we used for passing the variable into cvHoughCircles
		
		charCheckForESC = cvWaitKey(5);					// Delay in milliseconds and get key press, if any

		if(charCheckForESC == 27)
			break;								// If pressed key is 27 then jump out of while loop

		//////////////////////////////////////////////////
		
		/*********************************************
		RED Color DETECTION
		**********************************************/
				
		
		
		cvInRangeS(p_imgOriginal,
				   CV_RGB(180,  0,  0),
				   CV_RGB(255,100,100),
				   p_imgProcessed);

		p_strStorage = cvCreateMemStorage(0);		// Allocate storage to pass variable into cvHoughCirlceS()

		cvSmooth(p_imgProcessed,		// Input image
				 p_imgProcessed,		// Output image
				 CV_GAUSSIAN,			// Gaussian Filter. Average the value of nearer pixels with closest pixels having higher weightage
				 9,						// Pixels in Row
				 9);					// Pixels in Column

		p_seqCircles = cvHoughCircles(	p_imgProcessed,				// Input Image
										p_strStorage,				// Provide function with Storgae memory
										CV_HOUGH_GRADIENT,			// Algorithm which provide Circle detection
										2,							// size of image / 2 = zccumulator resolution i.e. acc_res = size of image / 2
										p_imgProcessed->height/4,	// Minimun distance in pixels b/w centers of the deected circles
										100,						// High threshold of canny edge detector called by cvHoughCircles()
										50,							// Low threshold of canny edge detector called by cvHoughCircles()
										10,							// Minimum circle radius in pixels
										800);						// Maximum circle radius in pixels
		

		for(i = 0; i < p_seqCircles->total; i++)							//for each element in sequential circles 
		{			
			p_fltXYRadius = (float*)cvGetSeqElem (p_seqCircles, i);			//From the sequintial Circles read the ith value
			printf("Ball Position: X = %f	Y = %f	R = %f \n",	p_fltXYRadius[0],	//X Position of Center of circle
																p_fltXYRadius[1],	//X Position of Center of circle
																p_fltXYRadius[2]);	// Radius of Circle

			// Draw a small green circle at center

			cvCircle(	p_imgOriginal,
						cvPoint(cvRound(p_fltXYRadius[0]),cvRound(p_fltXYRadius[1])),			//Center of circle
						3,												// Radious in Pixels 
						CV_RGB(   0, 255,   0),							// Blue Circle in Center
						CV_FILLED);										// Circle is filled

			// Draw Red circle around the detected object

			cvCircle(	p_imgOriginal,
						cvPoint(cvRound(p_fltXYRadius[0]),cvRound(p_fltXYRadius[1])),
						cvRound(p_fltXYRadius[2]),				// Radius of circle = Radius of detecetd circle
						CV_RGB(   0,   0, 255),					// Draw Pure GREEN
						3);										// Thickness of circle in pixels
			//////////// Beep sound for Green color
			if(i == p_seqCircles->total - 1)
				for(int j=0; j<2;j++)
				{
					Console::Beep(250,200);
				}
			///////////// Beep sound over

		}			//End of the for loop

		cvShowImage("Original Window",p_imgOriginal);			// Original image with circles overlaping
		cvShowImage("Processed Window", p_imgProcessed);		// Processed Image Black and white as red ball detected

		cvReleaseMemStorage(&p_strStorage);					// Deallocate the memory which we used for passing the variable into cvHoughCircles
		
		charCheckForESC = cvWaitKey(5);					// Delay in milliseconds and get key press, if any

		if(charCheckForESC == 27)
			break;					
		
		//////////////////////////////////////////////////

		/*********************************************
		GREEN Color DETECTION
		**********************************************/
		
		
		cvInRangeS(p_imgOriginal,
				   CV_RGB(  0,180,  0),
				   CV_RGB(100,255,100),
				   p_imgProcessed);

		p_strStorage = cvCreateMemStorage(0);		// Allocate storage to pass variable into cvHoughCirlceS()

		cvSmooth(p_imgProcessed,		// Input image
				 p_imgProcessed,		// Output image
				 CV_GAUSSIAN,			// Gaussian Filter. Average the value of nearer pixels with closest pixels having higher weightage
				 9,						// Pixels in Row
				 9);					// Pixels in Column

		p_seqCircles = cvHoughCircles(	p_imgProcessed,				// Input Image
										p_strStorage,				// Provide function with Storgae memory
										CV_HOUGH_GRADIENT,			// Algorithm which provide Circle detection
										2,							// size of image / 2 = zccumulator resolution i.e. acc_res = size of image / 2
										p_imgProcessed->height/4,	// Minimun distance in pixels b/w centers of the deected circles
										100,						// High threshold of canny edge detector called by cvHoughCircles()
										50,							// Low threshold of canny edge detector called by cvHoughCircles()
										10,							// Minimum circle radius in pixels
										800);						// Maximum circle radius in pixels
		

		for(i = 0; i < p_seqCircles->total; i++)							//for each element in sequential circles 
		{			
			p_fltXYRadius = (float*)cvGetSeqElem (p_seqCircles, i);			//From the sequintial Circles read the ith value
			printf("Ball Position: X = %f	Y = %f	R = %f \n",	p_fltXYRadius[0],	//X Position of Center of circle
																p_fltXYRadius[1],	//X Position of Center of circle
																p_fltXYRadius[2]);	// Radius of Circle

			// Draw a small green circle at center

			cvCircle(	p_imgOriginal,
						cvPoint(cvRound(p_fltXYRadius[0]),cvRound(p_fltXYRadius[1])),			//Center of circle
						3,												// Radious in Pixels 
						CV_RGB( 255,   0,   0),							// Red Circle in Center
						CV_FILLED);										// Circle is filled

			// Draw Red circle around the detected object

			cvCircle(	p_imgOriginal,
						cvPoint(cvRound(p_fltXYRadius[0]),cvRound(p_fltXYRadius[1])),
						cvRound(p_fltXYRadius[2]),				// Radius of circle = Radius of detecetd circle
						CV_RGB(   0,   0, 255),					// Draw Pure BLUE
						3);										// Thickness of circle in pixels
			//////////// Beep sound for Green color
			if(i == p_seqCircles->total - 1)
				for(int j=0; j<2;j++)
				{
					Console::Beep(900,200);
				}
			///////////// Beep sound over

		}			//End of the for loop

		cvShowImage("Original Window",p_imgOriginal);			// Original image with circles overlaping
		cvShowImage("Processed Window", p_imgProcessed);		// Processed Image Black and white as red ball detected

		cvReleaseMemStorage(&p_strStorage);					// Deallocate the memory which we used for passing the variable into cvHoughCircles
		
		charCheckForESC = cvWaitKey(5);					// Delay in milliseconds and get key press, if any

		if(charCheckForESC == 27)
			break;					
	
		//////////////////////////////////////////////////

	}					// End of the while loop

	cvReleaseCapture(&p_capwebcam);				// Release memory as applicable

	cvDestroyWindow("Original Window");			// Destroy the window for Original image
	cvDestroyWindow("Processed Window");		// Destroy the window for Processed image

	return(0);
}				// End of main function