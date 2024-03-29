# Minimum CMake version required
cmake_minimum_required(VERSION 3.4)

# Global name of the project
project(Geometric VERSION 0.1.0 LANGUAGES CXX)

# Specify compiler features
set(CMAKE_CXX_STANDARD 14)

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

# Compiler options
if(MSVC)
  message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall ")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message("Compiler: ${CMAKE_CXX_COMPILER_ID}")

  # Default compile flags (read: turn on all checks)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-documentation -pedantic ")

  # Extra compile flags to turn of stuff we don't want to handle at the moment
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-var-template")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-double-promotion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-float-equal")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-qualifiers")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-non-virtual-dtor")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-covered-switch-default")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-implicit-fallthrough")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unreachable-code")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unreachable-code-return")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-macros")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-reinterpret-cast")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-statement-expression")
else()
  message(WARNING "Compiler <${CMAKE_CXX_COMPILER_ID}> is not a supported C++ compiler.")
endif()


# Adjust the CMAKE_MODULE_PATH to include the "local" cmake/Modules directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



################################
# Find the required Qt libraries
find_package(Qt5Core 5.1)
find_package(Qt5Quick 5.1)
find_package(Qt5Gui 5.1)

################################
# Find GMlib
find_package(
  GMlib 0.6.9 EXACT
  PATHS ${GMLIB_DIR}
  NO_DEFAULT_PATH
)
message( "GMlib_INCLUDE_DIRS: ${GMlib_INCLUDE_DIRS}" )
message( "GMlib_LINK_DIRS:    ${GMlib_LINK_DIRS}" )
message( "GMlib_DEFINITIONS:  ${GMlib_DEFINITIONS}" )
message( "GMlib_LIBRARIES:    ${GMlib_LIBRARIES}" )
include_directories( ${GMlib_INCLUDE_DIRS} )
link_directories( ${GMlib_LINK_DIRS} )
add_definitions(${GMlib_DEFINITIONS})

include_directories( ${GLEW_INCLUDE_DIRS} )
if(WIN32)
  add_definitions(-DGLEW_STATIC)
endif(WIN32)




#######
# Files

set( HDRS
  hidmanager/hidaction.h
  hidmanager/hidbinding.h
  hidmanager/hidinput.h
  hidmanager/hidinputevent.h
  hidmanager/hidkbmouseinput.h
  hidmanager/hidmanager.h
  hidmanager/hidmanagertreemodel.h
  hidmanager/standardhidmanager.h
  hidmanager/defaulthidmanager.h

  application/fboinsgrenderer.h
  application/gmlibwrapper.h
  application/guiapplication.h
  application/window.h
  application/utils.h

  testtorus.h
  scenario.h
  lissajous.h
  bspline.h
  erbsplinecurve.h
  curveblending.h
  erbsplinesurf.h
  simplesubsurf.h
  )

set( SRCS
  hidmanager/hidaction.cpp
  hidmanager/hidbinding.cpp
  hidmanager/hidinput.cpp
  hidmanager/hidinputevent.cpp
  hidmanager/hidkbmouseinput.cpp
  hidmanager/hidmanager.cpp
  hidmanager/hidmanagertreemodel.cpp
  hidmanager/standardhidmanager.cpp
  hidmanager/defaulthidmanager.cpp

  application/fboinsgrenderer.cpp
  application/gmlibwrapper.cpp
  application/guiapplication.cpp
  application/window.cpp

  application/main.cpp

  scenario.cpp
  lissajous.c
  bspline.c
  erbsplinecurve.c
  curveblending.c
  erbsplinesurf.c
  )

#########
# Moc'ing
QT5_WRAP_CPP( HDRS_MOC
  hidmanager/hidaction.h
  hidmanager/hidmanager.h
  hidmanager/hidmanagertreemodel.h
  hidmanager/standardhidmanager.h
  hidmanager/defaulthidmanager.h

  application/fboinsgrenderer.h
  application/gmlibwrapper.h
  application/guiapplication.h
  application/window.h

  scenario.h
  )

set( RCCS
 application/application.qrc
)

QT5_ADD_RESOURCES( RCCS_MOC ${RCCS} )




#########
# Compile
add_executable( ${CMAKE_PROJECT_NAME} ${HDRS} ${SRCS} ${HDRS_MOC} ${FORM_HDRS} ${RCCS_MOC} )

######
# Link
target_link_libraries( ${CMAKE_PROJECT_NAME}
  ${GMlib_LIBRARIES}
  Qt5::Core
  Qt5::Quick
  Qt5::Gui
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )


