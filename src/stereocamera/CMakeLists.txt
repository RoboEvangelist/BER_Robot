cmake_minimum_required(VERSION 2.4.6)
project(stereocamera)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  message_runtime
  roscpp
  rospy
  sensor_msgs
  std_msgs
  geometry_msgs
  tf
  cv_bridge
  image_transport
  compressed_image_transport
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

# OpenCV is not a ROS package, so you gotta do this:
find_package(OpenCV REQUIRED)

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
#  INCLUDE_DIRS include
  CATKIN_DEPENDS tf geometry_msgs message_generation sensor_msgs
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

add_executable(stereocamera_broadcast src/stereocamera_broadcast.cpp src/libcam.cpp)
target_link_libraries(stereocamera_broadcast ${catkin_LIBRARIES})
#target_link_libraries(exampleGPIOApp ${OpenCV_LIBS})
#add_dependencies(exampleGPIOApp ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_generate_messages_cpp)

#rosbuild_add_executable(stereocam_subscribe src/stereocamera_subscribe.cpp)
