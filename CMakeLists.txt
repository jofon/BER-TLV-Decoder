cmake_minimum_required(VERSION 3.5)

project(BER_TLV_Decoder LANGUAGES CXX)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


add_subdirectory(src)

# Tests
add_subdirectory(test)

