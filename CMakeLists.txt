project(NyanCatEvolution)
cmake_minimum_required(VERSION 2.8.11.2)

if (MSVC)
  # Microsoft Visual Studio
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")

  find_package(OpenCV REQUIRED)
  if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found")
  endif()

  # Set include directories and additional dependencies for MSVC.
  # Rename OpenCV_INCLUDE_DIRS to be compatible with non-MSVC environment.
  set(OPENCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
  set(OpenCV_ADDITIONAL_DEPENDENCIES
    ${OpenCV_DIR}/x86/vc12/lib/opencv_ts300d.lib
    ${OpenCV_DIR}/x86/vc12/lib/opencv_world300d.lib
    )
  link_directories(${OpenCV_DIR}/x86/vc12/lib)

else(MSVC)
  # non-MSVC environment.
  include(FindPkgConfig)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(OPENCV REQUIRED opencv)

  if (NOT OPENCV_FOUND)
    message(FATAL_ERROR "OpenCV not found")
  endif( )
endif(MSVC)

set(nyan_cat_SOURCE_FILES
  DnaBrush.cpp
  DnaCanvas.cpp
  DnaPoint.cpp
  DnaPolygon.cpp
  fitness.cpp
  main.cpp
  renderer.cpp
  tools.cpp
  )

include_directories(${OPENCV_INCLUDE_DIRS})
add_definitions(-std=c++11)
add_executable(nyan_cat ${nyan_cat_SOURCE_FILES})

if (MSVC)
  target_link_libraries(nyan_cat ${OpenCV_ADDITIONAL_DEPENDENCIES})
ELSE ()
  target_link_libraries(nyan_cat ${OPENCV_LIBRARIES})
endif(MSVC)