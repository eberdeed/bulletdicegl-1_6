/*********************************************************************
 * *******************************************************************
 * commonheader:  A file to provide common includes.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * April 2020 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/
/**  \class commonheader.h: A common header file for the bulletdicegl dice simulation.
 *  Created by Edward C. Eberle <eberdeed@eberdeed.net>
 *  December 17, 2016, San Diego California USA
 *  Reworked for OpenGL April 11, 2020.
 */

#ifndef COMMONHEADER_H
//! The defines.
#define COMMONHEADER_H
#define NUM_IMAGES 3
#define TILES 2500

//! GLEW The OpenGL library manager
#define GLEW_STATIC
#include <glew.h>

//! GLM The OpenGL math library
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <ext.hpp>
#include <ext/quaternion_trigonometric.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/common.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_interpolation.hpp>
#include <gtc/matrix_access.hpp> 
#include <gtx/euler_angles.hpp>

// SDL The Simple Direct Media Layer
#include <SDL2/SDL.h>

//! Bullet Physics
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <LinearMath/btVector3.h>

//! Std C++
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <cmath>
#include <exception>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

/** \brief  This project is include in the assimpopengl directory and
 * will be compile and installed with the rest of the project.
 */
#include <assimpopengl.h>

//! The namespaces used.
using namespace std;
using namespace glm;

//! Mine, but most of it came from www.learnopengl.com
//! \brief A structure to contain a Texture 
//! for display on the surface of a mesh.
struct Texture {
    GLuint id;
    string type;
    string path;
};
/** \brief A structure to contain a Vertex 
 * for the walls and floor.
 */
struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec3 BiNormal;
    vec2 TexCoord;
};
/** \brief A structure to define a point light.
 */
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
/** \breif A structure to define a spotlight.
 */
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#endif // COMMONHEADER_H
