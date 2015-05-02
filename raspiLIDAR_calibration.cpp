/*
Author: Nathan Morin (nathanamorin@gmail.com, nathanmorin.com, @nathanamorin)

Purpose: provide LIDAR distance measurement using raspberry pi camera & line laser
Created for CNIT 315 Purdue University as part of an honors project

Attributions: Ava group of the University of Cordoba (see ATTRIBUTIONS for fill citation)
*/

#include "raspiLIDAR_calibration.h"
#include <vector>
#include <math.h>

using namespace std;
using std::vector;
size_t nFramesCaptured = 30;

const float rpc  = 0.0006;
const float ro  = 0.0299;
const float h = .15;
const int x_center = 640;

int main(){

	raspicam::RaspiCam Camera;
	
	//Set Filename for future file output
	time_t rawtime;
	time(&rawtime);
	string imageFilename("images/image_test_");
	imageFilename += ctime(&rawtime);
	imageFilename += ".ppm";
	//string::replace(imageFilename.begin(),imageFilename.end(),' ','_');
	strReplace(&imageFilename," ","_");
	
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

	unsigned char *data = new unsigned char [ Camera.getImageBufferSize()];

	Camera.grab();

	Camera.retrieve(data);
	size_t i = 0;
	while (++i < nFramesCaptured)
	{
		Camera.grab();
		Camera.retrieve(data);
	}
	int width = Camera.getWidth();
	int height = Camera.getHeight();
	int currentPos;
	cout << "test" << endl;
	//RGB_pixel image = new RGB_pixel [height][width];
	vector<vector<RGB_pixel> > image;
	cout << "starting analysis" << endl;
	

	for (int y=0; y<height; y++)
	{
		//if (y % 10 == 0) cout << y << endl;
		image.resize(height);
		for (int x=0; x<width; x++)
		{
			//if (x % 10 == 0) cout << x << endl;
			image[y].resize(width);
			currentPos = (y*width + x)*3;
			image[y][x].R = data[currentPos];
			image[y][x].G = data[currentPos+1];
			image[y][x].B = data[currentPos+2];
		}
	}
	cout << "done rendering image"<<endl;
	int x_avg = 0, numPoints = 0;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			if (image[y][x].R > 100 && image[y][x].G > 100)
			{
				//cout << x<<endl;
				x_avg += x;
				numPoints++;
			}
			
		}
	}

	x_avg /= numPoints;

	cout << "Average x :: " << x_avg << "Height :: " << height << "Width :: " << width << endl;

	float thea = (float) (x_avg-x_center) * rpc + ro;

	cout << "Thea :: " << thea << endl;

	float dist = h / tan(thea);

	cout << "Distance :: " << dist << endl;


	saveImage(imageFilename, data, Camera);

	cout << Camera.getImageBufferSize() <<endl;
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


































