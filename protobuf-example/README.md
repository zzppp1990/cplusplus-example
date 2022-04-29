# protobuf example
记录protobuf使用示例

./genProto.sh
编译
g++ test_read.cpp addressbook.pb.cc -o test_read  -lprotobuf

g++ test_write.cpp addressbook.pb.cc -o test_write  -lprotobuf