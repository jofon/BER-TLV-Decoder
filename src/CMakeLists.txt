set(SRC_FILES 
    BerTag.cpp)


add_library(ber_tlv_decoder ${SRC_FILES})

target_include_directories(ber_tlv_decoder PUBLIC ${CMAKE_SOURCE_DIR}/include)
