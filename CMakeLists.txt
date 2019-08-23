cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(ssdll 
    src/ssdll/lookupdictionary.cpp
    src/ssdll/dictionarymetadata.cpp
    src/ssdll/logger.cpp
    src/ssdll/utils.cpp
    src/ssdll/indexfile.cpp
    src/ssdll/mapfile.cpp
    src/ssdll/basicdictionary.cpp
    src/ssdll/dictionaryprivate.cpp
    src/ssdll/dictziplib.cpp
    src/ssdll/worddata.cpp)
target_link_libraries(ssdll ${CONAN_LIBS})
