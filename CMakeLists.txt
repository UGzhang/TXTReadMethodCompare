cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 17)
set(Boost_NO_WARN_NEW_VERSIONS 1)

project(TestReadFile)

find_package(Boost REQUIRED COMPONENTS iostreams)

add_executable(
        TestReadFile
        testReadFile.cpp
        )

target_link_libraries(TestReadFile PRIVATE Boost::iostreams)
