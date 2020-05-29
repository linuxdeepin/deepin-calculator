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

find_package(DtkWidget REQUIRED dtkwidget)
find_package(DtkGui REQUIRED dtkgui)
find_package(DtkCore REQUIRED dtkcore)

include_directories("src/core")
include_directories("src/math")
include_directories("src/modules")
include_directories("src/widgets")

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

    ${DtkWidget_LIBRARIES}
    ${DtkCore_LIBRARIES}
    ${DtkGUI_LIBRARIES}
)

set (DC_CPP_FILES
    src/core/book.cpp
        src/core/constants.cpp
        src/core/evaluator.cpp
        src/core/functions.cpp
        src/core/numberformatter.cpp
        src/core/opcode.cpp
        src/core/pageserver.cpp
        src/core/session.cpp
        src/core/sessionhistory.cpp
        src/core/settings.cpp
        src/core/userfunction.cpp
        src/core/variable.cpp
        src/math/cmath.cpp
        src/math/cnumberparser.cpp
        src/math/hmath.cpp
        src/math/quantity.cpp
        src/math/rational.cpp
        src/math/units.cpp
        src/math/floatcommon.c
        src/math/floatconst.c
        src/math/floatconvert.c
        src/math/floaterf.c
        src/math/floatexp.c
        src/math/floatgamma.c
        src/math/floathmath.c
        src/math/floatincgamma.c
        src/math/floatio.c
        src/math/floatipower.c
        src/math/floatlog.c
        src/math/floatlogic.c
        src/math/floatlong.c
        src/math/floatnum.c
        src/math/floatpower.c
        src/math/floatseries.c
        src/math/floattrig.c
        src/math/number.c
        src/modules/basicmodule.cpp
        src/modules/expressionbar.cpp
        src/modules/memoryitemdelegate.cpp
        src/modules/memoryitemwidget.cpp
        src/modules/memorylistwidget.cpp
        src/modules/memorywidget.cpp
        src/modules/scientificmodule.cpp
        src/modules/sciexpressionbar.cpp
        src/modules/scihistorywidget.cpp
        src/modules/simplelistdelegate.cpp
        src/modules/simplelistmodel.cpp
        src/modules/simplelistview.cpp
        src/widgets/basickeypad.cpp
        src/widgets/equalbutton.cpp
        src/widgets/funcombobox.cpp
        src/widgets/iconbutton.cpp
        src/widgets/inputedit.cpp
        src/widgets/memorybutton.cpp
        src/widgets/memorykeypad.cpp
        src/widgets/scibasickeypad.cpp
        src/widgets/scientifickeypad.cpp
        src/widgets/shortcut.cpp
        src/widgets/textbutton.cpp
        src/widgets/tricombobox.cpp
        src/dsettings.cpp
        src/main.cpp
        src/mainwindow.cpp
        src/utils.cpp
)

set (DC_HEADER_FILES
    src/core/book.h
        src/core/constants.h
        src/core/errors.h
        src/core/evaluator.h
        src/core/functions.h
        src/core/numberformatter.h
        src/core/opcode.h
        src/core/pageserver.h
        src/core/session.h
        src/core/sessionhistory.h
        src/core/settings.h
        src/core/userfunction.h
        src/core/variable.h
        src/math/cmath.h
        src/math/cnumberparser.h
        src/math/floatcommon.h
        src/math/floatconfig.h
        src/math/floatconst.h
        src/math/floatconvert.h
        src/math/floaterf.h
        src/math/floatexp.h
        src/math/floatgamma.h
        src/math/floathmath.h
        src/math/floatincgamma.h
        src/math/floatio.h
        src/math/floatipower.h
        src/math/floatlog.h
        src/math/floatlogic.h
        src/math/floatlong.h
        src/math/floatnum.h
        src/math/floatpower.h
        src/math/floatseries.h
        src/math/floattrig.h
        src/math/hmath.h
        src/math/number.h
        src/math/quantity.h
        src/math/rational.h
        src/math/units.h
        src/modules/basicmodule.h
        src/modules/expressionbar.h
        src/modules/memoryitemdelegate.h
        src/modules/memoryitemwidget.h
        src/modules/memorylistwidget.h
        src/modules/memorywidget.h
        src/modules/scientificmodule.h
        src/modules/sciexpressionbar.h
        src/modules/scihistorywidget.h
        src/modules/simplelistdelegate.h
        src/modules/simplelistmodel.h
        src/modules/simplelistview.h
        src/widgets/basickeypad.h
        src/widgets/equalbutton.h
        src/widgets/funcombobox.h
        src/widgets/iconbutton.h
        src/widgets/inputedit.h
        src/widgets/memorybutton.h
        src/widgets/memorykeypad.h
        src/widgets/scibasickeypad.h
        src/widgets/scientifickeypad.h
        src/widgets/shortcut.h
        src/widgets/textbutton.h
        src/widgets/tricombobox.h
        src/dsettings.h
        src/mainwindow.h
        src/utils.h
)

set (DC_QRC_FILES
    src/deepin-calculator.qrc
)

add_executable (${EXE_NAME}
    ${DC_HEADER_FILES}
    ${DC_CPP_FILES}
    ${DC_QRC_FILES}
)

target_include_directories(${EXE_NAME} PUBLIC ${Qt5Widgets_LIBRARIES}
                                              ${Qt5DBus_LIBRARIES}
                                              ${PROJECT_BINARY_DIR}
                                              ${DtkWidget_INCLUDE_DIRS}
                                              ${DtkCore_LIBRARIES}
                                              ${DtkGUI_INCLUDE_DIRS})

target_link_libraries (${EXE_NAME} ${LINK_LIBS})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/deepin-calculator
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
file(GLOB QM_FILES "translations/*.qm")
install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-calculator/translations)
install(FILES deepin-calculator.desktop
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)
install(FILES src/assets/images/deepin-calculator.svg
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps/)


