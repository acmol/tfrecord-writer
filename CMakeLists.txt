cmake_minimum_required(VERSION 3.14)
project(tfrecord)

find_package(GLOG REQUIRED)
find_package(GFLAGS REQUIRED)
find_package(GTEST REQUIRED)
find_package(PROTOBUF REQUIRED)

include(tfrecord)