cmake_minimum_required(VERSION 3.17)
project(Indev-CCR)

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

add_executable(Indev-CCR ${SRC_FILES} ${INC_FILES})
add_subdirectory(ext/SDC Stardust)
add_subdirectory(Core CC-Core)
add_compile_definitions(MATHFU_COMPILE_WITHOUT_SIMD_SUPPORT)
target_include_directories(Indev-CCR PRIVATE src Core/include ext/SDC/ext/mathfu/include)

target_compile_options(Indev-CCR PRIVATE -Wall)
target_link_libraries(Indev-CCR PRIVATE Stardust-Celeste SC-Entry CC-Core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -g3")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

if(PSP)
    create_pbp_file(
            TARGET Indev-CCR
            ICON_PATH NULL
            BACKGROUND_PATH NULL
            PREVIEW_PATH NULL
            TITLE ${PROJECT_NAME}
            BUILD_PRX
    )
endif()