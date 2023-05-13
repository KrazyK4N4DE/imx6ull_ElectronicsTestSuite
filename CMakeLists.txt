cmake_minimum_required(VERSION 3.10)

# 交叉编译设置
SET(CMAKE_CROSSCOMPILING TRUE)
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER $ENV{CROSS_COMPILE}gcc)

###########################################################################

PROJECT(main)

INCLUDE_DIRECTORIES(./include)

AUX_SOURCE_DIRECTORY(src/display SRC_display)
AUX_SOURCE_DIRECTORY(src/input SRC_input)
AUX_SOURCE_DIRECTORY(src/font SRC_font)
AUX_SOURCE_DIRECTORY(src/ui SRC_ui)
AUX_SOURCE_DIRECTORY(src/page SRC_page)
AUX_SOURCE_DIRECTORY(src/config SRC_config)
AUX_SOURCE_DIRECTORY(src/business SRC_business)

SET(SRC_LIST ${SRC_display} ${SRC_input} ${SRC_font} ${SRC_ui} ${SRC_page} ${SRC_config} ${SRC_business})

ADD_SUBDIRECTORY(unittest EXCLUDE_FROM_ALL)

MESSAGE(STATUS "src list: ${SRC_LIST}")

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ts pthread freetype m)