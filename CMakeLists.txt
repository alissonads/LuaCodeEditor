cmake_minimum_required(VERSION 3.8.1)

project(LuaCodeEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LUA_DIRECTORY /opt/homebrew/Cellar/lua/5.4.6)

add_subdirectory(external/rapidcsv)
add_subdirectory(qtimgui)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Widgets
        QuickWidgets
        Core
        Quick
        Gui
        Qml
        Charts
        Svg
        SvgWidgets
        OpenGLWidgets
        #        Network
        QuickControls2
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
        Widgets
        QuickWidgets
        Core
        Quick
        Gui
        Qml
        Charts
        Svg
        SvgWidgets
        OpenGLWidgets
        #        Network
        QuickControls2
)

set(PROJECT_SOURCES
        UiResources.qrc
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        src/LuaCompilator.cpp
        src/LuaCompilator.h
        src/LuaProjectManager.cpp
        src/LuaProjectManager.h
        src/FileManager.cpp
        src/FileManager.h
        src/Models.cpp
        src/Models.h
        src/IntervalParser.cpp
        src/IntervalParser.h)

set(INCLUDES
    external/rapidcsv/src
    qtimgui/src
    qtimgui/modules/imgui
    qtimgui/modules/implot
    ${LUA_DIRECTORY}/include
    src
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LuaCodeEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LuaCodeEditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LuaCodeEditor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LuaCodeEditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCLUDES}
)

target_link_libraries(LuaCodeEditor PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::SvgWidgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::QuickControls2
    ${LUA_DIRECTORY}/lib/liblua.a
    rapidcsv
    qt_imgui_widgets
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LuaCodeEditor)
endif()
set_target_properties(LuaCodeEditor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS LuaCodeEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LuaCodeEditor)
endif()
