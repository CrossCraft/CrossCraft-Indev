cmake_minimum_required(VERSION 3.17)
project(CC-Classic-R)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    add_compile_definitions(BUILD_PLAT=0)
elseif(PSP)
    add_compile_definitions(BUILD_PLAT=2)
elseif(3DS)
    add_compile_definitions(BUILD_PLAT=7)
elseif(VITA)
    add_compile_definitions(BUILD_PLAT=3)
else()
    add_compile_definitions(BUILD_PLAT=1)
endif()

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE INC_FILES src/*.hpp)

add_executable(CC-Classic ${SRC_FILES} ${INC_FILES})
add_subdirectory(ext/SDC Stardust)
add_subdirectory(Core CC-Core)
target_include_directories(CC-Classic PRIVATE src Core/include)
target_compile_options(CC-Classic PRIVATE -Wall)
target_link_libraries(CC-Classic PRIVATE Stardust-Celeste SC-Entry CC-Core)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")