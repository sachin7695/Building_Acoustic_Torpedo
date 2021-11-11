cmake_minimum_required(VERSION 3.0.2)
project(warzone)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	gazebo_ros
)

find_package(gazebo REQUIRED)

catkin_package(
  LIBRARIES warzone
  DEPENDS 
    roscpp 
    gazebo_ros 
)

link_directories(${Boost_LIBRARY_DIRS} ${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED src/${PROJECT_NAME}.cc)
target_link_libraries(${PROJECT_NAME} ${GAZEBO_LIBRARIES})

