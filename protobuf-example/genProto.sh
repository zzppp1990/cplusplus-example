#/root/gitproject/cplusplus-example/protobuf-example
SRC_DIR="/root/gitproject/cplusplus-example/protobuf-example"
DST_DIR="/root/gitproject/cplusplus-example/protobuf-example"

protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto
