#CMakeLists.txt
project(TestProtobuf)
set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.17)

add_executable(test_write test_write.cpp addressbook.pb.cc)
target_link_libraries(test_write protobuf)

add_executable(test_read test_read.cpp addressbook.pb.cc)
target_link_libraries(test_read protobuf)
