/*
Author: Nathan Morin (nathanamorin@gmail.com, nathanmorin.com, @nathanamorin)

Purpose: provide LIDAR distance measurement using raspberry pi camera & line laser
Created for CNIT 315 Purdue University as part of an honors project
*/
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <sys/timeb.h>
#include "raspicam.h"
using namespace std;


#ifndef SUCCESS
#define SUCCESS 0
#endif

#ifndef FAILURE
#define FAILURE 1
#endif
/*
Define data structure for RGB pixel 
*/
struct RGB_pixel{
	int R;
	int G;
	int B;
	// int x = NULL;
	// int y = NULL;
};
struct pixel_point{
	int x;
	int y;
};


int calculateDistance(double point, double *dist);

int findPoints(int width, int height, unsigned char *data, double *x_avg);

int captureImage(raspicam::RaspiCam &Camera, unsigned char **data);

/*
Sets up camera settings before image is taken
@input Camera - opened Camera if type RaspiCam
@return - true success - false failure
*/
bool setCamera(raspicam::RaspiCam &Camera);

/*
Saves image taken from camera to file specified in .ppm format
overriting any existing file
@input file path - string where image will be written
@input data - unsigned char of image data
@input Camera - camera where data has been taken
@return - true success - false failure
*/
bool saveImage(string filepath, unsigned char *data, raspicam::RaspiCam &Camera);

/*
Replaces all occurrences of a string in another string

Contributing Sources::
http://stackoverflow.com/questions/20406744/how-to-find-and-replace-all-occurrences-of-a-substring-in-a-string
*/

bool strReplace(string *main_string, string re_old, string re_new){

	string::size_type n = 0;

	while ( (n = (*main_string).find(re_old,n)) != string::npos)
	{
		(*main_string).replace(n, re_old.size(), re_new);
		n += re_new.size();
	}

	return true;
}

int executeLIDAR(raspicam::RaspiCam &Camera);
