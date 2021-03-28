set(TEST_SRC_FILES 
    main.cpp
    TestBerTlv.cpp)

add_executable(unittests ${TEST_SRC_FILES})
target_link_libraries(unittests PRIVATE ber_tlv_decoder)


include(CTest)
include(Catch)
catch_discover_tests(unittests)

