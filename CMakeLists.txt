cmake_minimum_required(VERSION 2.8.3)
project(cpp_i2c_test)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare ROS messages and services
##add_message_files(FILES Num.msg)
##add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(subber src/subber.cpp)
target_link_libraries(subber ${catkin_LIBRARIES})
add_dependencies(subber cpp_i2c_test_generate_messages_cpp)

##unit testing for ros usage
add_executable(tester src/tester.cpp)
target_link_libraries(tester ${catkin_LIBRARIES})
add_dependencies(tester cpp_i2c_test_generate_messages_cpp)

##testing conversion and ros cycling
add_executable(hexConverter src/hexConverter.cpp)
target_link_libraries(hexConverter ${catkin_LIBRARIES})
add_dependencies(hexConverter cpp_i2c_test_generate_messages_cpp)

##implementation of ros with i2c commands and conversions -- needs testing still
add_executable(hexConverterv2 src/hexConverterv2.cpp)
target_link_libraries(hexConverterv2 ${catkin_LIBRARIES})
add_dependencies(hexConverterv2 cpp_i2c_test_generate_messages_cpp)
