# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# 单元测试 CMakeLists.txt - Qt5/Qt6 兼容版本

cmake_minimum_required(VERSION 3.16.0)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

project(deepin-calculator-test)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

ADD_COMPILE_OPTIONS(-fno-access-control)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wl,--as-needed -fPIE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

# 探测 Qt 版本（与主 CMakeLists.txt 保持一致）
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
message("Using Qt version: ${QT_VERSION_MAJOR}")
set(QT_DESIRED_VERSION ${QT_VERSION_MAJOR})

# 根据 Qt 版本选择 DTK 主版本号
if(QT_VERSION_MAJOR MATCHES 6)
    set(DTK_VERSION_MAJOR 6)
else()
    set(DTK_VERSION_MAJOR "")
endif()
message("Using dtk version: ${DTK_VERSION_MAJOR}")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "sw_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif ()

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "mips64")
    message(STATUS "this system is mips64=========")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -march=loongson3a -mhard-float -mno-micromips -mno-mips16 -flax-vector-conversions -mloongson-ext2 -mloongson-mmi -Wl,--as-needed")
endif()

configure_file(${CMAKE_CURRENT_LIST_DIR}/../src/environments.h.in environments.h @ONLY)

# Find the Qt library (自适应 Qt5/Qt6)
find_package(PkgConfig REQUIRED)
find_package(Qt${QT_DESIRED_VERSION} REQUIRED COMPONENTS Widgets Core Gui DBus Xml Svg Test)

# Find Dtk via CMake (not pkg-config)，自适应 Dtk6/Dtk
find_package(Dtk${DTK_VERSION_MAJOR} REQUIRED COMPONENTS Widget Gui Core)

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${DtkGui_INCLUDE_DIRS})
include_directories(${DtkCore_INCLUDE_DIRS})

# gtest/gmock
# 使用 CMake 内置的 FindGTest 模块查找 gtest；
# gmock 头文件位于标准路径 /usr/include/gmock，库 libgmock.a 位于标准库目录，
# 编译器/链接器可自动定位，下方 target_link_libraries 已显式链接 gmock/gmock_main。
find_package(GTest REQUIRED)

set (EXE_NAME deepin-calculator)

# Qt link libraries (自适应 Qt5/Qt6)
set(LINK_LIBS
    Qt${QT_DESIRED_VERSION}::Core
    Qt${QT_DESIRED_VERSION}::DBus
    Qt${QT_DESIRED_VERSION}::Widgets
    Qt${QT_DESIRED_VERSION}::Xml
    Qt${QT_DESIRED_VERSION}::Svg
    Qt${QT_DESIRED_VERSION}::Test
    ${DtkWidget_LIBRARIES}
    ${DtkGui_LIBRARIES}
    ${DtkCore_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
)

# Head file search path
SET(HEAD ${CMAKE_CURRENT_LIST_DIR}/../src
         ${CMAKE_CURRENT_LIST_DIR}/../src/bak
         ${CMAKE_CURRENT_LIST_DIR}/../src/control
         ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/core
         ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/math
         ${CMAKE_CURRENT_LIST_DIR}/../src/views
         ${CMAKE_CURRENT_LIST_DIR}/../src/widgets
         ${CMAKE_CURRENT_LIST_DIR}/src  # For qt6_compat.h
         )

# Source files from main project
file(GLOB_RECURSE CAL_H ${CMAKE_CURRENT_LIST_DIR}/../src/*.h)
file(GLOB_RECURSE CAL_C ${CMAKE_CURRENT_LIST_DIR}/../src/*.c)
file(GLOB_RECURSE CAL_CPP ${CMAKE_CURRENT_LIST_DIR}/../src/*.cpp)
file(GLOB_RECURSE CAL_3rdH ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/*.h)
file(GLOB_RECURSE CAL_3rdC ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/*.c)
file(GLOB_RECURSE CAL_3rdCPP ${CMAKE_CURRENT_LIST_DIR}/../3rdparty/*.cpp)
list(REMOVE_ITEM CAL_CPP "${CMAKE_CURRENT_LIST_DIR}/../src/main.cpp")

# Test source files
file(GLOB_RECURSE CAL_TEST ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB_RECURSE CAL_TESTH ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

include_directories(${GTEST_INCLUDE_DIRS})

set (DC_QRC_FILES
    ${CMAKE_CURRENT_LIST_DIR}/../src/deepin-calculator.qrc
)

set(PROJECT_NAME_TEST
    ${EXE_NAME}-test)

# Generate test executable
add_executable(${PROJECT_NAME_TEST} ${CAL_H} ${CAL_C} ${CAL_CPP} ${CAL_3rdH} ${CAL_3rdC} ${CAL_3rdCPP} ${CAL_TEST} ${CAL_TESTH})

# Link gtest/gmock libraries
target_link_libraries(${PROJECT_NAME_TEST} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} gmock gmock_main gtest gtest_main pthread)

target_include_directories(${PROJECT_NAME_TEST} PUBLIC
                                              ${PROJECT_BINARY_DIR}
                                              ${DtkWidget_INCLUDE_DIRS}
                                              ${DtkCore_INCLUDE_DIRS}
                                              ${DtkGui_INCLUDE_DIRS}
                                              ${DFrameworkDBus_INCLUDE_DIRS}
                                              ${GTEST_INCLUDE_DIRS}
                                              ${HEAD})
target_link_libraries (${PROJECT_NAME_TEST} ${LINK_LIBS})

if(DEFINED ENV{PREFIX})
    set(CMAKE_INSTALL_PREFIX $ENV{PREFIX})
else()
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

install(TARGETS deepin-calculator DESTINATION bin)
file(GLOB QM_FILES "${CMAKE_CURRENT_LIST_DIR}/../translations/*.qm")
install(FILES ${QM_FILES} DESTINATION share/deepin-calculator/translations)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/../deepin-calculator.desktop
        DESTINATION share/applications/)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/../src/assets/images/deepin-calculator.svg
            DESTINATION share/icons/hicolor/scalable/apps/)


#------------------------------ 创建单元测试执行目标---------------------------------------
# 注意: 由于 enable_testing() 已启用, 'test' 目标由 CTest 保留
# 使用 'ut_run' 作为自定义目标名称

add_custom_target(ut_run)

add_custom_command(TARGET ut_run
    COMMAND echo " =================== RUN UNIT TESTS ==================== "
    COMMAND ${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME_TEST}
    COMMAND echo " =================== TESTS COMPLETED ==================== "
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

#'make ut_run'命令依赖与我们的测试程序
add_dependencies(ut_run ${PROJECT_NAME_TEST})

# 设置添加gocv相关信息的输出
set(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage")
set(CMAKE_SAFETYTEST "${CMAKE_SAFETYTEST_ARG}")
if(CMAKE_SAFETYTEST STREQUAL "")
    set(CMAKE_SAFETYTEST "CMAKE_SAFETYTEST_ARG_OFF")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SAFETYTEST}")
if(CMAKE_SAFETYTEST STREQUAL "CMAKE_SAFETYTEST_ARG_ON")
  #安全测试选项
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=undefined,address -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=undefined,address -O2")
endif()
