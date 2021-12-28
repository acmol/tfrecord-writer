
add_library(
    common_coding
    coding.cpp
)
        
target_link_libraries(common_coding)

add_library(
    common_crc32c
    crc32c.cpp
    crc32c_accelerate.cpp
)
        
target_link_libraries(common_crc32c)

add_library(
    common_tfrecord
    record_writer.cpp
)

target_link_libraries(common_tfrecord common_coding common_crc32c glog gflags)


add_executable(
    record_writer_test
    record_writer_test.cpp
)

target_link_libraries(record_writer_test common_tfrecord common_proto gtest)

install(
       DIRECTORY ${CMAKE_SOURCE_DIR}/
       DESTINATION include
       FILES_MATCHING
       PATTERN "*.h*"
       PATTERN ".git" EXCLUDE
       PATTERN "build*" EXCLUDE
       PATTERN ".vscode" EXCLUDE
       PATTERN "conf" EXCLUDE
       PATTERN "dag" EXCLUDE)


install(TARGETS
        record_writer_test
        common_tfrecord
        common_coding
        common_crc32c
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )