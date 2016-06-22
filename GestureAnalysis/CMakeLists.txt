# Define a versão minima do cmake para poder executar esse script
cmake_minimum_required(VERSION 2.8)

# Adiciona o flag ao compilador para usar c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")

# Define o nome do projeto
project(gesture_tracking_project)

#Acha e linka diretorios do openni
FIND_PATH(OPEN_NI_INCLUDE "XnOpenNI.h" "OpenNIConfig.h" HINTS "$ENV{OPEN_NI_INCLUDE}" "/usr/include/ni" "/usr/include/openni" "/opt/ros/groovy/include/openni_camera" "/opt/local/include/ni")
FIND_LIBRARY(OPEN_NI_LIBRARY NAMES OpenNI libOpenNI HINTS $ENV{OPEN_NI_LIB} "/usr/lib" "/opt/local/lib")
LINK_DIRECTORIES($ENV{OPEN_NI_LIB})
INCLUDE_DIRECTORIES(${OPEN_NI_INCLUDE})
LINK_LIBRARIES(${OPEN_NI_LIBRARY})

#Acha diretorios do opengl e glut
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Inclui diretorios necessarios já encontrados
include_directories(${OPENNI_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

# Inclui diretorios do projeto
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/geometry")
include_directories ("${PROJECT_SOURCE_DIR}/libs")
include_directories ("${PROJECT_SOURCE_DIR}/logger")
include_directories ("${PROJECT_SOURCE_DIR}/util")

# Inclui todas libraries nas pastas do projeto
file(GLOB SOURCES "geometry/*.cpp" "libs/*.cpp" "logger/*.cpp" "util/*.cpp" "*.cpp")

# Cria executavel do programa
add_executable(start ${SOURCES})

# Linka as aplicações com as libs
target_link_libraries(start ${OPENNI_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} util)