add_executable(test
        ${analysis_source}
        MMapTest.cpp
        utils/utils.cpp
        utils/utils.h
        IfstreamTest.cpp
        ReadTest.cpp
        main_test.cpp)
target_link_libraries(test ${GTEST_LIBRARIES} pthread)