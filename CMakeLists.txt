cmake_minimum_required(VERSION 3.16.0)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

#最外层一个工程，单项目中不添加工程，只添加执行文件
project(deepin-calculator)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wl,--as-needed -fPIE")
set(QT_MINIMUM_VERSION "6.0.0")
set(CMAKE_EXE_LINKER_FLAGS "-pie")


# Find the Qt library
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
message("Using Qt version: ${QT_VERSION_MAJOR}")

#qt5    
#find_package(Qt5 COMPONENTS Core REQUIRED)
# Select major DTK version.
if(QT_VERSION_MAJOR EQUAL "6")
    set(BUILD_WITH_QT6 ON)
    set(DTK_VERSION_MAJOR 6)
else()
    set(DTK_VERSION_MAJOR "")
endif()
message("Using dtk version: ${DTK_VERSION_MAJOR}")

#add_definitions(-DQT_NO_DEBUG_OUTPUT)

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "sw_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif ()

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "mips64")
    message(STATUS "this system is mips64=========")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -march=loongson3a -mhard-float -mno-micromips -mno-mips16 -flax-vector-conversions -mloongson-ext2 -mloongson-mmi -Wl,--as-needed")
endif()

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

    execute_process(COMMAND bash "${CMAKE_CURRENT_LIST_DIR}/translate_generation.sh"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/)
endif ()

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

configure_file(src/environments.h.in environments.h @ONLY)

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui DBus Xml Svg Test)

#qt5
#find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui DBus Xml Svg Test)

# 替换 pkg_search_module 为 find_package
find_package(Dtk${DTK_VERSION_MAJOR} REQUIRED COMPONENTS Widget Gui Core)

include_directories(${DtkWidget_INCLUDE_DIRS})
include_directories(${DtkGui_INCLUDE_DIRS})
include_directories(${DtkCore_INCLUDE_DIRS})

message(STATUS "DtkGui Include Directories111: ${DtkGui_INCLUDE_DIRS}")
message(STATUS "DtkCore Include Directories11: ${DtkCore_INCLUDE_DIRS}")
message(STATUS "DtkWidget Include Directories111: ${DtkWidget_INCLUDE_DIRS}")

set (EXE_NAME deepin-calculator)

if (Qt6_FOUND)
    # Qt6 environment
    set(LINK_LIBS
    Qt6::Core
    Qt6::DBus
    Qt6::Widgets
    Qt6::Xml
    Qt6::Svg
    Qt6::Test

    Dtk${DTK_VERSION_MAJOR}::Widget
    Dtk${DTK_VERSION_MAJOR}::Gui
    Dtk${DTK_VERSION_MAJOR}::Core
    ${DFrameworkDBus_LIBRARIES}
)
else()
    # Qt5 environment
    set(LINK_LIBS
    Qt5::Core
    Qt5::DBus
    Qt5::Widgets
    Qt5::Xml
    Qt5::Svg
    Qt5::Test

    Dtk${DTK_VERSION_MAJOR}::Widget
    Dtk${DTK_VERSION_MAJOR}::Gui
    Dtk${DTK_VERSION_MAJOR}::Core
    ${DFrameworkDBus_LIBRARIES}
)
endif()


file(GLOB_RECURSE CAL_SRCH ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE CAL_SRCC ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
file(GLOB_RECURSE CAL_SRCCPP ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB_RECURSE CAL_3rdH ${CMAKE_CURRENT_LIST_DIR}/3rdparty/*.h)
file(GLOB_RECURSE CAL_3rdC ${CMAKE_CURRENT_LIST_DIR}/3rdparty/*.c)
file(GLOB_RECURSE CAL_3rdCPP ${CMAKE_CURRENT_LIST_DIR}/3rdparty/*.cpp)

set (DC_QRC_FILES
    src/deepin-calculator.qrc
)

add_executable (${EXE_NAME}
    ${CAL_SRCH}
    ${CAL_SRCC}
    ${CAL_SRCCPP}
    ${CAL_3rdH}
    ${CAL_3rdC}
    ${CAL_3rdCPP}
    ${DC_QRC_FILES}
)

target_include_directories(${EXE_NAME} PUBLIC ${Qt6Widgets_LIBRARIES}
                                              ${Qt6DBus_LIBRARIES}
                                              ${Qt6TestLib_LIBRARIES}
                                              ${PROJECT_BINARY_DIR}
                                              ${DtkWidget_INCLUDE_DIRS}
                                              ${DtkCore_LIBRARIES}
                                              ${DtkGui_INCLUDE_DIRS}
                                              ${DFrameworkDBus_INCLUDE_DIRS})

target_link_libraries (${EXE_NAME} ${LINK_LIBS})

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

install(TARGETS deepin-calculator DESTINATION ${CMAKE_INSTALL_BINDIR})
file(GLOB QM_FILES "${CMAKE_CURRENT_LIST_DIR}/translations/*.qm")
install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/deepin-calculator/translations)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/deepin-calculator.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/src/assets/images/deepin-calculator.svg
            DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps/)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/assets/deepin-calculator
                DESTINATION ${CMAKE_INSTALL_DATADIR}/deepin-manual/manual-assets/application/)

#if (NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "loongarch64")
# 禁用测试
# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
# option (BUILD_TESTING "" ON)
# if (BUILD_TESTING)
# add_subdirectory(tests)  #tests为ut测试文件夹名
# endif ()
# endif ()
#endif ()

#代码覆盖率开关
if(CMAKE_COVERAGE_ARG STREQUAL "CMAKE_COVERAGE_ARG_ON")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -fprofile-arcs -ftest-coverage")
endif()

