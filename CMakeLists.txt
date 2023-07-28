cmake_minimum_required(VERSION 3.1)


project(little_fs)

set(SRC
    ./src/lfs.c
    ./src/lfs_util.c
    ./src/lfs_init.c
)

#find_library(flash_driver NAME flash REQUIRED_FILES)


add_library(${PROJECT_NAME} SHARED ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ./inc)
target_link_libraries(${PROJECT_NAME} flash)