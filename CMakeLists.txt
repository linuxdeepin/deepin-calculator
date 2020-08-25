cmake_minimum_required(VERSION 3.9.5)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

project(deepin-calculator)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(QT_MINIMUM_VERSION "5.7.1")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "sw_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif ()

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

    execute_process(COMMAND bash "translate_generation.sh"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif ()

configure_file(src/environments.h.in environments.h @ONLY)

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5DBus REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Test REQUIRED)

find_package(DtkWidget REQUIRED dtkwidget)
find_package(DtkGui REQUIRED dtkgui)
find_package(DtkCore REQUIRED dtkcore)
#find_package(DFrameworkDbus REQUIRED dframeworkdbus)
pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${DtkGui_INCLUDE_DIRS})
include_directories(${DtkCore_INCLUDE_DIRS})

set (EXE_NAME deepin-calculator)

set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg
    Qt5::Test

    ${DtkWidget_LIBRARIES}
    ${DtkCore_LIBRARIES}
    ${DtkGUI_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
)

file(GLOB_RECURSE CAL_H ${CMAKE_CURRENT_LIST_DIR}/src/*.h)

file(GLOB_RECURSE CAL_C ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

file(GLOB_RECURSE CAL_CPP ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
#file(GLOB_RECURSE CAL_TEST ${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp)
include_directories(third_party)                           #一下三行添加包含以便测试用例代码的编写
include_directories(third_party/googlemock/include)
include_directories(third_party/googletest/include)
add_subdirectory(third_party)                              #添加googletest库

set (DC_QRC_FILES
    src/deepin-calculator.qrc
)

add_executable (${EXE_NAME}
    ${CAL_H}
    ${CAL_C}
    ${CAL_CPP}
    ${DC_QRC_FILES}
    "main.cpp"
)
set(PROJECT_NAME_TEST
    ${PROJECT_NAME}_test)
FILE (GLOB allTestSource
    "./tests/*.cpp" "./tests/src/*.cpp" "./tests/src/*.h"
    "./tests/src/control/*.cpp" "./tests/src/control/*.h"
    "./tests/src/widgets/*.cpp" "./tests/src/widgets/*.h"
    "./tests/src/views/*.cpp" "./tests/src/views/*.h")

# 生成测试可执行程序
add_executable(${PROJECT_NAME_TEST} ${CAL_H} ${CAL_C} ${CAL_CPP} ${allTestSource})

# 生成测试可执行程序的依赖库
target_link_libraries(${PROJECT_NAME_TEST} gmock gmock_main gtest gtest_main)

target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${Qt5Widgets_LIBRARIES}
                                              ${Qt5DBus_LIBRARIES}
                                              ${Qt5TestLib_LIBRARIES}
                                              ${PROJECT_BINARY_DIR}
                                              ${DtkWidget_INCLUDE_DIRS}
                                              ${DtkCore_LIBRARIES}
                                              ${DtkGUI_INCLUDE_DIRS}
                                              ${DFrameworkDBus_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME_TEST} ${LINK_LIBS})
target_include_directories(${EXE_NAME} PUBLIC ${Qt5Widgets_LIBRARIES}
                                              ${Qt5DBus_LIBRARIES}
                                              ${Qt5TestLib_LIBRARIES}
                                              ${PROJECT_BINARY_DIR}
                                              ${DtkWidget_INCLUDE_DIRS}
                                              ${DtkCore_LIBRARIES}
                                              ${DtkGUI_INCLUDE_DIRS}
                                              ${DFrameworkDBus_INCLUDE_DIRS})

target_link_libraries (${EXE_NAME} ${LINK_LIBS})

set(CMAKE_INSTALL_PREFIX /usr)

install(TARGETS deepin-calculator DESTINATION bin)
file(GLOB QM_FILES "translations/*.qm")
install(FILES ${QM_FILES} DESTINATION share/deepin-calculator/translations)
install(FILES deepin-calculator.desktop
        DESTINATION share/applications/)
install(FILES src/assets/images/deepin-calculator.svg
            DESTINATION share/icons/hicolor/scalable/apps/)

#------------------------------ 创建'make test'指令---------------------------------------
add_custom_target(test
    COMMAND mkdir -p coverageResult
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_command(TARGET test
    COMMAND echo " =================== CREAT LCOV REPROT BEGIN ==================== "

    #1.清理原先的gcov信息
    COMMAND lcov --directory ./CMakeFiles/${PROJECT_NAME_TEST}.dir --zerocounters
    COMMAND lcov --directory ./CMakeFiles/${PROJECT_NAME}.dir --zerocounters
    COMMAND ./${PROJECT_NAME_TEST}

    #2.收集gcov信息到.info文件中
    COMMAND lcov --directory . --capture --output-file ./coverageResult/${PROJECT_NAME}_Coverage.info

    #3.过滤一些我们不感兴趣的文件的覆盖率信息
    COMMAND echo " =================== do filter begin ==================== "
    COMMAND lcov --remove ./coverageResult/${PROJECT_NAME}_Coverage.info
    '*/${PROJECT_NAME_TEST}_autogen/*' '*/${PROJECT_NAME}_autogen/*' '*/tests/*' '*/googletest/*' '*/usr/include/*' '*/src/test/*' '*/src/core/*' '*/src/math/*'
    -o ./coverageResult/${PROJECT_NAME}_Coverage.info
    COMMAND echo " =================== do filter end ==================== "

    #3.将.info信息生成报告到reprot文件夹中
    COMMAND genhtml -o ./coverageResult/report ./coverageResult/${PROJECT_NAME}_Coverage.info

    COMMAND echo " =================== CREAT LCOV REPROT END ==================== "

    COMMAND echo " Coverage files have been output to ${CMAKE_BINARY_DIR}/coverageResult/report "

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

#'make test'命令依赖与我们的测试程序
add_dependencies(test ${PROJECT_NAME_TEST})

# 设置添加gocv相关信息的输出
set(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage")
