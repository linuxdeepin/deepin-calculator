# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# 单元测试 CMakeLists.txt - Qt6 兼容版本

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

# Qt6 minimum version
set(QT_MINIMUM_VERSION "6.0.0")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "sw_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif ()

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "mips64")
    message(STATUS "this system is mips64=========")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -march=loongson3a -mhard-float -mno-micromips -mno-mips16 -flax-vector-conversions -mloongson-ext2 -mloongson-mmi -Wl,--as-needed")
endif()

configure_file(${CMAKE_CURRENT_LIST_DIR}/../src/environments.h.in environments.h @ONLY)

# Find the Qt6 library
find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui DBus Xml Svg Test)

# Find Dtk6 via CMake (not pkg-config)
find_package(Dtk6 REQUIRED COMPONENTS Widget Gui Core)

include_directories(${Dtk6Widget_INCLUDE_DIRS})
include_directories(${Dtk6Gui_INCLUDE_DIRS})
include_directories(${Dtk6Core_INCLUDE_DIRS})

# gtest/gmock
pkg_check_modules(GTEST REQUIRED
    gtest
    gmock
)

set (EXE_NAME deepin-calculator)

# Qt6 link libraries
set(LINK_LIBS
    Qt6::Core
    Qt6::DBus
    Qt6::Widgets
    Qt6::Xml
    Qt6::Svg
    Qt6::Test

    Dtk6::Widget
    Dtk6::Gui
    Dtk6::Core
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
target_link_libraries(${PROJECT_NAME_TEST} ${GTEST_LIBRARYS} ${GTEST_MAIN_LIBRARYS} gmock gmock_main gtest gtest_main pthread)

target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${Qt6Widgets_INCLUDE_DIRS}
                                              ${Qt6DBus_INCLUDE_DIRS}
                                              ${Qt6Test_INCLUDE_DIRS}
                                              ${PROJECT_BINARY_DIR}
                                              ${Dtk6Widget_INCLUDE_DIRS}
                                              ${Dtk6Core_INCLUDE_DIRS}
                                              ${Dtk6Gui_INCLUDE_DIRS}
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
