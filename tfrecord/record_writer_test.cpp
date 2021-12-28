#include <string>
#include "tfrecord/record_writer.h"
#include "tfrecord/example.pb.h"
#include "tfrecord/feature.pb.h"
#include "gtest/gtest.h"

TEST(RecordWriterTest, test_all) {
    using tensorflow::Feature;
    using tensorflow::Example;
    std::ofstream f("test.bin", std::fstream::out);
    tfrecord::RecordWriter record_writer(&f);
    tensorflow::Example example;
    tensorflow::Features* features = example.mutable_features();
    auto* feature = features->mutable_feature();
    auto& value = (*feature)["key1"];
    auto* i64_list = value.mutable_int64_list();
    i64_list->add_value(1);
    i64_list->add_value(3);
    i64_list->add_value(4);
    i64_list->add_value(2);
    std::string str;
    example.SerializeToString(&str);
    record_writer.WriteRecord(str.data(), str.size());
    i64_list->add_value(3);
    example.SerializeToString(&str);
    ASSERT_EQ(0, record_writer.WriteRecord(str.data(), str.size()));
    ASSERT_EQ(0, record_writer.Close());
    ASSERT_EQ(0, record_writer.Close());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}