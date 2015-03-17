/*
Author: Nathan Morin
Contributers: AVA (see ATTRIBUTIONS for full attribution)
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "raspicam_still.h"
using namespace std;

raspicam::RaspiCam_Still Camera;


int main ( int argc, char *argv[] ) {

    int width = 100;
    int height =75;
    int iso=400;


    cout << "Initializing ..."<<width<<"x"<<height<<endl;
    Camera.setWidth ( width );
    Camera.setHeight ( height );
    Camera.setISO(iso);
    Camera.setEncoding ( raspicam::RASPICAM_ENCODING_BMP );
    Camera.open();
    cout<<"capture"<<endl;
    unsigned int length = Camera.getImageBufferSize(); // Header + Image Data + Padding
    unsigned char * data = new unsigned char[length];
      if ( !Camera.grab_retrieve(data, length) ) {
        cerr<<"Error in grab"<<endl;
        return -1;
    }

    cout<< *data<<endl;
    return 0;
}