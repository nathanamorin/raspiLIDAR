#####################################
cmake_minimum_required (VERSION 2.8) 
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}") 
project (raspicam_test)
find_package(raspicam REQUIRED)
add_executable (simpletest_raspicam simpletest_raspicam.cpp)  
target_link_libraries (simpletest_raspicam ${raspicam_LIBS})
#####################################
