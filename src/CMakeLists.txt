set(SRC_FILES 
    BerTlv.cpp)


add_library(ber_tlv_decoder ${SRC_FILES})

target_include_directories(ber_tlv_decoder PUBLIC ${CMAKE_SOURCE_DIR}/include)

install(TARGETS ber_tlv_decoder DESTINATION ${CMAKE_SOURCE_DIR}/lib)