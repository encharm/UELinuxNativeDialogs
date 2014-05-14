project(UELinuxFileDialogs)

cmake_minimum_required(VERSION 2.8.8)

find_package(Qt4 QUIET COMPONENTS QtGui)
find_package(Qt5Widgets QUIET)
find_package(GTK2 QUIET)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 gtk+-3.0)

if(QT4_FOUND)
message(STATUS "Qt4 found")

add_library(qt4dialog SHARED qtdialog.cpp)

target_link_libraries(qt4dialog Qt4::QtGui)

add_executable(qt4test dialogtest.cpp)
target_link_libraries(qt4test qt4dialog)

endif()

if(Qt5Widgets_FOUND)
message(STATUS "Qt5 found")

add_library(qt5dialog SHARED qtdialog.cpp)

target_link_libraries(qt5dialog Qt5::Widgets)

add_executable(qt5test dialogtest.cpp)
target_link_libraries(qt5test qt5dialog)


endif()

if(GTK2_FOUND)
message(STATUS "GTK2 found")

add_library(gtk2dialog SHARED gtkdialog.c)
set_property( TARGET gtk2dialog PROPERTY INCLUDE_DIRECTORIES ${GTK2_INCLUDE_DIRS})
target_link_libraries(gtk2dialog ${GTK2_LIBRARIES})

add_executable(gtk2test dialogtest.cpp)
target_link_libraries(gtk2test gtk2dialog)

endif()

if(GTK3_FOUND)
message(STATUS "GTK3 found")

add_library(gtk3dialog SHARED gtkdialog.c)
set_property( TARGET gtk3dialog PROPERTY INCLUDE_DIRECTORIES ${GTK3_INCLUDE_DIRS})
target_link_libraries(gtk3dialog ${GTK3_LIBRARIES})

add_executable(gtk3test dialogtest.cpp)
target_link_libraries(gtk3test gtk3dialog)


endif()