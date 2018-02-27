cmake_minimum_required(VERSION 2.8.3)
project(motor_control)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  message_runtime
  roscpp
  rospy
  sensor_msgs
  stereo_msgs
  std_msgs
  geometry_msgs
  tf
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

# OpenCV is not a ROS package, so you gotta do this:
#find_package(OpenCV REQUIRED)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS tf geometry_msgs message_generation
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)
include_directories(include)

add_executable(exampleGPIOApp src/exampleGPIOApp.cpp src/jetsonGPIO.cpp
)
target_link_libraries(exampleGPIOApp ${catkin_LIBRARIES})
#target_link_libraries(exampleGPIOApp ${OpenCV_LIBS})
#add_dependencies(exampleGPIOApp ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_generate_messages_cpp)
