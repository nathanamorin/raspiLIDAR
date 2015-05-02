/*
Author: Nathan Morin (nathanamorin@gmail.com, nathanmorin.com, @nathanamorin)

Purpose: provide LIDAR distance measurement using raspberry pi camera & line laser
Created for CNIT 315 Purdue University as part of an honors project

Attributions: Ava group of the University of Cordoba (see ATTRIBUTIONS for fill citation)
*/

#include "raspiLIDAR.h"
#include <vector>
#include <math.h>

using namespace std;
using std::vector;
size_t nFramesCaptured = 30;

const double rpc  = 0.0006;
const double ro  = 0.03165;
// const double rpc  = 0.4896;
// const double ro  = 0.1419;
const double h = .15;
const double x_center = 640;

int main(){

	raspicam::RaspiCam Camera;
	if (!setCamera(Camera))
	{
		cout << "There was an error setting up the Camera" <<endl;
		return 1;
	}


	if (!Camera.open())
	{
		cout << "There was an error opeing the Camera" <<endl;
		return 1;
	}
	
	while(true)
	{
		executeLIDAR(Camera);
	}


	Camera.release();

}


bool setCamera(raspicam::RaspiCam &Camera){


	Camera.setWidth(1280);
	Camera.setHeight(960);
	Camera.setBrightness(50);
	Camera.setSharpness(0);
	Camera.setContrast(0);
	Camera.setShutterSpeed(0);
	Camera.setISO(100);
	Camera.setExposureCompensation(5);
	Camera.setExposure(raspicam::RASPICAM_EXPOSURE_AUTO);
	Camera.setAWB(raspicam::RASPICAM_AWB_AUTO);
	Camera.setFormat(raspicam::RASPICAM_FORMAT_RGB);
	Camera.setVerticalFlip(true);
	//Camera.setAWB_RB(1,1);
	return true;
}

bool saveImage(string filepath, unsigned char *data, raspicam::RaspiCam &Camera){
	std::ofstream outFile (filepath.c_str(), std::ios::binary);
	outFile << "P6\n";
	outFile << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";
	outFile.write( (char*) data, Camera.getImageBufferSize() );

	return true;
}





int captureImage(raspicam::RaspiCam &Camera, unsigned char **data)
{

	*data = new unsigned char [ Camera.getImageBufferSize()];

	Camera.grab();

	Camera.retrieve(*data);
	size_t i = 0;
	while (++i < nFramesCaptured)
	{
		Camera.grab();
		Camera.retrieve(*data);
	}

	return SUCCESS;
}

int findPoints(int width, int height, unsigned char *data, double *x_avg)
{

	int x_total = 0;
	int numPoints = 0;

	int x_left_head = 0;
	int x_right_head = width;

	for (int y=0; y<height; y++)
	{	

		for (int x=x_left_head; x<x_right_head; x++)
		{
			
			if (data[(y*width + x)*3] > 100)
			{
				x_left_head = x - 100;
				if (x_left_head < 0) x_left_head = 0;
				
				x_right_head = x + 100;
				if (x_right_head > width) x_right_head = width;
				

				x_total += x;
				numPoints++;
			}
		}
	}

	if (numPoints <= 0)
	{
		*x_avg = 0;
		return -1;
	}

	*x_avg = (double)(x_total / numPoints);
	return SUCCESS;
}

int calculateDistance(double point, double *dist)
{
	double thea = (point-x_center) * rpc + ro;

	*dist = h / tan(thea);

	return SUCCESS;
}

int executeLIDAR(raspicam::RaspiCam &Camera)
{
		//Set Filename for future file output
	time_t rawtime;
	time(&rawtime);
	string imageFilename("images/image_test_");
	imageFilename += ctime(&rawtime);
	imageFilename += ".ppm";
	strReplace(&imageFilename," ","_");


	unsigned char *data;

	//Capture Image
	clock_t t = clock();
	if (captureImage(Camera, &data)) return FAILURE;
	clock_t captureTime = clock() - t;

	//Find Laser Points
	double x_avg = 0;
	t = clock();
	findPoints(Camera.getWidth(), Camera.getHeight(), data, &x_avg);
	clock_t imageProcessingTime = clock() - t;

	//Calculate Distance from points
	double dist = 0;
	t = clock();
	calculateDistance(x_avg, &dist);
	clock_t distanceCalculationTime = clock() - t;

	cout << "Distance to point -- " << dist << endl;

	cout << "Cature Time \t Image Processing Time \t Calculation Time \n" <<
					((float)captureTime)/CLOCKS_PER_SEC << "\t\t" <<
					((float)imageProcessingTime)/CLOCKS_PER_SEC << "\t\t" <<
					((float)distanceCalculationTime)/CLOCKS_PER_SEC << "\t" << endl;

	if (data)
	{
		std::free(data);
	}
}




























