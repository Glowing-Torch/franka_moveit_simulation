cmake_minimum_required(VERSION 3.0.2)
project(my_qt)
set(CMAKE_AUTOMOC ON)


## 找到 ROS 和 Qt5 所需的组件
find_package(catkin REQUIRED COMPONENTS
  rviz
  roscpp
  moveit_ros_visualization
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

## 声明 catkin 包
catkin_package()

## 包含头文件目录
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
qt5_wrap_cpp(MOC_SOURCES main_window.hpp)
## 添加可执行文件
add_executable(main_window_node src/main.cpp src/main_window.cpp)
add_executable(call_service src/call_service.cpp)

## 链接 ROS 和 Qt5 库
target_link_libraries(main_window_node
  ${catkin_LIBRARIES}
  Qt5::Widgets
)
target_link_libraries(call_service
  ${catkin_LIBRARIES}
)