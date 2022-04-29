# protobuf example
记录protobuf使用示例

生成proto文件：
./genProto.sh

g++编译

g++ test_read.cpp addressbook.pb.cc -o test_read  -lprotobuf

g++ test_write.cpp addressbook.pb.cc -o test_write  -lprotobuf

CMAKE编译：

进入CMakeList.txt的目录，执行cmake ../目录名

make