#CMakeLists.txt
project(TestProtobuf)
set(CMAKE_CXX_STANDARD 14)
cmake_minimum_required(VERSION 3.17)

include_directories(protobuf)
aux_source_directory(protobuf DIR_SRCS)

add_executable(test_write test_write.cpp ${DIR_SRCS})
target_link_libraries(test_write protobuf)

add_executable(test_read test_read.cpp ${DIR_SRCS})
target_link_libraries(test_read protobuf)
