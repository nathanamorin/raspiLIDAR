/*
Author: Nathan Morin (nathanamorin@gmail.com, nathanmorin.com, @nathanamorin)

Purpose: provide LIDAR distance measurement using raspberry pi camera & line laser
Created for CNIT 315 Purdue University as part of an honors project

Attributions: Ava group of the University of Cordoba (see ATTRIBUTIONS for fill citation)
*/

#include "raspiLIDAR_xy_main.h"

using namespace std;
using std::vector;
size_t nFramesCaptured = 30;

const double rpc  = 0.0006;
const double ro  = 0.03165;
// const double rpc  = 0.4896;
// const double ro  = 0.1419;
const double h = .15;
const double x_center = 640;
const double y_center = 480;

int main(int argc, char *argv[]){
	if (argc != 2)
	{
		printf("usage : %s filename\n", argv[0]);
		return 1;
	}

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
		executeLIDAR(Camera, argv[1]);
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

int findPoints(int width, int height, unsigned char *data, vector<pixel_point> &points)
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
				
				pixel_point point = {x,y};
				points.push_back(point);
				//x_total += x;
				//numPoints++;
			}
		}
	}

	// if (numPoints <= 0)
	// {
	// 	*x_avg = 0;
	// 	return -1;
	// }

	// *x_avg = (double)(x_total / numPoints);
	return SUCCESS;
}

double calculateDistance_x(double point)
{
	double thea = (point-x_center) * rpc + ro;

	return h / tan(thea);
}

double calculateDistance_y(double point, double dist_x)
{
	double dist_from_center = point-y_center;
	double multiplier = 1;
	if (dist_from_center < 0) 
	{
		dist_from_center *= -1;
		multiplier = -1;
	}
	
	double Di = sqrt( pow(h,2.0) + pow(dist_x,2.0) );

	double thea = dist_from_center * rpc + ro;

	return Di * tan(thea) * multiplier;
}

xy_point calculateDistance(pixel_point point)
{
	double dist_x = calculateDistance_x( (double)(point.x) );
	double dist_y = calculateDistance_y( (double)(point.y) ,dist_x);
	xy_point dist_point = {dist_x,dist_y};
	return dist_point;

}

int executeLIDAR(raspicam::RaspiCam &Camera, char *file)
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
	vector<pixel_point> points;
	t = clock();
	findPoints(Camera.getWidth(), Camera.getHeight(), data, points);
	clock_t imageProcessingTime = clock() - t;

	//Calculate Distance from points
	vector<xy_point> distances;

	t = clock();

	int points_size = points.size();
	for (int i = 0; i < points_size; i++)
	{
		distances.push_back( calculateDistance(points[i]) );
	}
	clock_t distanceCalculationTime = clock() - t;

	if (data)
	{
		std::free(data);
	}


	ofstream outputFile;

	outputFile.open(file);

	if(!outputFile) {return FAILURE;}

	outputFile << "x\ty\n" << endl;

	for (int i = 0; i<distances.size(); i++)
	{
		outputFile << distances[i].x << "\t" << distances[i].y << "\n" << endl;
	}

	outputFile.close();

	cout << "Cature Time \t Image Processing Time \t Calculation Time \n" <<
					((float)captureTime)/CLOCKS_PER_SEC << "\t\t" <<
					((float)imageProcessingTime)/CLOCKS_PER_SEC << "\t\t" <<
					((float)distanceCalculationTime)/CLOCKS_PER_SEC << "\t" << endl;

}



























