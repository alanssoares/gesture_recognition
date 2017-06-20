# Define a versão minima do cmake para poder executar esse script
cmake_minimum_required(VERSION 2.8)

# Adiciona o flag ao compilador para usar c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated -g")

# Define o nome do projeto
project(gesture_tracking_project)

#Acha diretorios do opengl, glut, pcl e vtk
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PCL 1.8 REQUIRED COMPONENTS common io visualization console)

#Acha e linka diretorios do openni
FIND_PATH(OPEN_NI_INCLUDE "XnOpenNI.h" "OpenNIConfig.h" HINTS "$ENV{OPEN_NI_INCLUDE}" "/usr/include/ni" "/usr/include/openni" "/opt/ros/groovy/include/openni_camera" "/opt/local/include/ni")
FIND_LIBRARY(OPEN_NI_LIBRARY NAMES OpenNI libOpenNI HINTS $ENV{OPEN_NI_LIB} "/usr/lib" "/opt/local/lib")
LINK_DIRECTORIES($ENV{OPEN_NI_LIB} ${PCL_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${OPEN_NI_INCLUDE})
LINK_LIBRARIES(${OPEN_NI_LIBRARY})

# Inclui diretorios necessarios já encontrados
include_directories(${PCL_INCLUDE_DIRS} ${OPENNI_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Inclui definições da lib PCL
add_definitions (${PCL_DEFINITIONS})

# Inclui diretorios do projeto
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/geometry")
include_directories ("${PROJECT_SOURCE_DIR}/logger")
include_directories ("${PROJECT_SOURCE_DIR}/util")
include_directories ("${PROJECT_SOURCE_DIR}/classify")
include_directories ("${PROJECT_SOURCE_DIR}/cluster")
include_directories ("${PROJECT_SOURCE_DIR}/test")

# Inclui todas libraries nas pastas do projeto
file(GLOB SOURCES
	"../Commons/geometry/*.cpp"
	"../Commons/logger/*.cpp"
	"../Commons/util/*.cpp"
	"../Commons/classify/*.cpp"
	"../Commons/cluster/*.cpp"
	"test/*.cpp"
	"util/*.cpp"
	"*.cpp")

# Cria executavel do programa
add_executable(start ${SOURCES})

# Linka as aplicações com as libs
target_link_libraries(start ${OPENNI_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PCL_LIBRARIES} util)
