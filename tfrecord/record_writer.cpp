/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tfrecord/record_writer.h"

#include "tfrecord/coding.h"
#include "tfrecord/crc32c.h"

namespace tfrecord {

RecordWriter::RecordWriter(std::ofstream* dest)
    : dest_(dest){
}

RecordWriter::~RecordWriter() {
  if (dest_ != nullptr) {
    if (0 != Close()) {
      LOG(ERROR) << "Could not finish writing file";
    }
  }
}

#define RETURN_IF_ERROR(ret) do { \
  auto ret_code = (ret); \
  if (ret_code != 0) { \
      return ret_code; \
  } \
} while(0)

int Append(std::ofstream* os, const char* data, int size) {
    os->write(data, size);
    return os->bad() ? -1 : 0;
}

int RecordWriter::WriteRecord(const char* data, int size) {
  if (dest_ == nullptr) {
    LOG(ERROR) << "Writer not initialized or previously closed";
    return -1;
  }
  // Format of a single record:
  //  uint64    length
  //  uint32    masked crc of length
  //  byte      data[length]
  //  uint32    masked crc of data
  char header[kHeaderSize];
  char footer[kFooterSize];
  PopulateHeader(header, data, size);
  PopulateFooter(footer, data, size);
  RETURN_IF_ERROR(Append(dest_, header, sizeof(header)));
  RETURN_IF_ERROR(Append(dest_, data, size));
  return Append(dest_, footer, sizeof(footer));
}

int RecordWriter::Close() {
  if (dest_ == nullptr) return 0;
  dest_->close();
  bool ret = dest_->bad() ? -1 : 0;
  dest_ = nullptr;
  return ret;
}

int RecordWriter::Flush() {
  if (dest_ == nullptr) {
    return -1;
  }
  return dest_->flush() ? 0 : -1;
}

}  // namespace tfrecord
