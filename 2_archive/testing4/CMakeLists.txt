cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}")
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
LINK_LIBRARIES(${PROJECT_NAME} )

 

 
ADD_EXECUTABLE(raspicam_still_test raspicam_still_test.cpp )
INSTALL(TARGETS raspicam_still_test   RUNTIME DESTINATION bin)


