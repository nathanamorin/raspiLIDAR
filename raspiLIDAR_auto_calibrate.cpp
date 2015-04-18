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

double rpc  = 0.0006;
double ro  = 0.0299;
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

int calculateVars(double point, double distance)
{
	double b_dist_current = 0;
	double b_dist_best = 0;
	double b_rpc_current = 0;
	double b_rpc_best = 0;
	double b_ro_current = 0;
	double b_ro_best = 0;

	cout << "Given Distance " << distance << endl;

	for (double a=-100; a<100; a++)
	{
		b_rpc_current = a/100000 + rpc;
		for (double b=-100; b<100; b++)
		{
			b_ro_current = b/100000 + ro;
			b_dist_current = h / tan( (point-x_center) * b_rpc_current + b_ro_current);
			double current_dist = fabs (distance - b_dist_current);
			double best_dist = fabs (distance - b_dist_best);
			if (b_dist_current>0 && b_rpc_current > 0 && b_ro_current > 0 && ( current_dist < best_dist ) )
			{
				//	cout << distance << " - " << b_dist_current << " - " << b_dist_best << endl;
				b_rpc_best = b_rpc_current;
				b_ro_best = b_ro_current;
				b_dist_best = b_dist_current;
			}

		}

	}
	cout << "Best Distance" << b_dist_best << endl;
	rpc = b_rpc_best;
	ro = b_ro_best;

	return SUCCESS;
}

int executeLIDAR(raspicam::RaspiCam &Camera)
{


	unsigned char *data;

	cout << "Distance :: " << endl;
	double distance;
	cin >> distance;

	//Capture Image
	if (captureImage(Camera, &data)) return FAILURE;

	//Find Laser Points
	double x_avg = 0;
	findPoints(Camera.getWidth(), Camera.getHeight(), data, &x_avg);

	//Calculate Distance from points
	calculateVars(x_avg, distance);

	cout << "RPC = " << rpc << "RO = " << ro << endl;



	if (data)
	{
		std::free(data);
	}
}



























