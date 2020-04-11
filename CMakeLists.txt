############################################################
#   The CMake File for the Bullet Dice for OpenGL program.  
#   It provides for the following libraries:  Boost, GLM, 
#   GL, GLEW, pthread, stdc++, freeimageplus, SDL2, assimp
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   March 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 2.6)
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
project(bulletdicegl-1_6)
add_subdirectory(assimpopengl)
add_subdirectory(src)
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN YES)
set(PROJECT_NAME "Bullet Dice OpenGL")
set(PROJECT_BRIEF "A program to emulate a dice throw using the Bullet Physics Library and OpenGL.")
doxygen_add_docs(doc include src assimpopengl/include assimpopengl/src COMMENT "Generating documentation.")
install(DIRECTORY openglresources DESTINATION /usr/share FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY assimpopengl/LGPL DESTINATION /usr/share/doc/bulletdicegl-1_6-doc FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/html DESTINATION /usr/share/doc/bulletdicegl-1_6-doc FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/man DESTINATION /usr/share/doc/bulletdicegl-1_6-doc FILE_PERMISSIONS WORLD_READ)
install(FILES README.txt CHANGELOG.txt KEYMAP.txt bulletdicegl-1_6writeup.odt bulletdicegl-1_6writeup.pdf
DESTINATION /usr/share/doc/bulletdicegl-1_6-doc PERMISSIONS WORLD_READ)
