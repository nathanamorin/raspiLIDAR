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

/*
Sets up camera settings before image is taken
@input Camera - opened Camera if type RaspiCam
@return - true success - false failure
*/
bool setCamera(raspicam::RaspiCam &Camera);

/*
Saves image taken from camera to file specified in .ppm format
overriting any existing file
@input filepath - string where image will be written
@input data - unsigned char of image data
@input Camera - camera where data has been taken
@return - true success - false failure
*/
bool saveImage(string filepath, unsigned char *data, raspicam::RaspiCam &Camera);

/*
Replaces all occurences of a string in another string

Contributing Sources::
http://stackoverflow.com/questions/20406744/how-to-find-and-replace-all-occurrences-of-a-substring-in-a-string
*/

bool strReplace(string *main_string, string re_old, string re_new){

	string::size_type n = 0;

	while ( (n = (*main_string).find(re_old,n)) != string::npos)
	{
		cout << "found" << endl;
		(*main_string).replace(n, re_old.size(), re_new);
		n += re_new.size();
	}

	return true;
}