############################################################
#   The CMake File for the Bullet Dice for OpenGL program.  
#   It provides for the following libraries:  Boost, GLM, 
#   GL, GLEW, pthread, stdc++, freeimageplus, SDL2, assimp
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   March 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 3.12)
if(POLICY CMP0015)
  cmake_policy(SET CMP0015 NEW)
endif()
project(bulletdicegl-1_6)
add_executable(bulletdicegl-1_6 bulletdicegl.cpp
camera.cpp diceroll.cpp fallingbody.cpp)
add_definitions(-g -fPIC -std=c++17 -pthread)
include_directories(/usr/local/include /usr/include/glm /usr/include/GL 
/usr/include/bullet /usr/local/include/assimp /usr/include/boost 
"../assimpopengl/include")
link_directories(/usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib "../assimpopengl/src")
target_link_libraries(bulletdicegl-1_6 stdc++ pthread GL GLEW SDL2-2.0 
freeimage freeimageplus boost_filesystem boost_system assimp 
assimpopengl BulletCollision BulletDynamics LinearMath )
install(TARGETS bulletdicegl-1_6 DESTINATION /usr/bin)
