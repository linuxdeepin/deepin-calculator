cmake_minimum_required(VERSION 3.9.5)

if (NOT DEFINED VERSION)
    set(VERSION 1.2.2)
endif ()

#最外层一个工程，单项目中不添加工程，只添加执行文件
project(deepin-calculator)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wl,--as-needed -fPIE")
set(QT_MINIMUM_VERSION "5.7.1")
set(CMAKE_EXE_LINKER_FLAGS "-pie")

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
file(GLOB QM_FILES "${CMAKE_CURRENT_LIST_DIR}/translations/*.qm")
install(FILES ${QM_FILES} DESTINATION share/deepin-calculator/translations)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/deepin-calculator.desktop
        DESTINATION share/applications/)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/src/assets/images/deepin-calculator.svg
            DESTINATION share/icons/hicolor/scalable/apps/)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/assets/deepin-calculator
                DESTINATION /usr/share/deepin-manual/manual-assets/application/)

add_subdirectory(tests)  #tests为ut测试文件夹名
