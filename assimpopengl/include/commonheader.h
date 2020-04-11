/*********************************************************************
 * *******************************************************************
 * commonheader:  A file to provide common includes.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * November 2019 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/
/**  \class commonheader.h: A common header file for the bulletdicegl dice simulation.
 *  Created by Edward C. Eberle <eberdeed@eberdeed.net>
 *  December 17, 2016, San Diego California USA
 *  Reworked for OpenGL April 11, 2020
 */

/** \brief These are some files that we need to include to use OpenGL. 
 * Also included are GLEW, the OpenGL Math Library (GLM), the SimpleDirect 
 * Media Layer (SDL), the Boost library, the Assimp library, c++ system files, 
 * Free Image Plus and the Bullet Physics Library.
 */
#ifndef COMMONHEADER_H
#define COMMONHEADER_H
// GLEW The OpenGL library manager
#define GLEW_STATIC
#include <glew.h>
// GLM The OpenGL Math Library
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_interpolation.hpp>
#include <gtc/matrix_access.hpp> 
#include <gtx/euler_angles.hpp>
// Assimp the Asset Importation Library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// Free Image Plus
#include <FreeImagePlus.h>
// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
// Boost The standard template library clone ++.
#include <boost/chrono/include.hpp>
#include <filesystem.hpp>


//! The namespaces used.
using namespace std;
using namespace glm;
using namespace boost::filesystem;

//! Structures.
/** \brief A structure to contain a Vertex 
 * for textured meshes.
 */
struct Vertex {
    float Position[3];
    float Normal[3];
    float BiNormal[3];
    float TexCoords[2];
};
/** \brief A structure to contain a Vertex 
 * for untextured meshes.
 */
struct Vertex1 {
    float Position[3];
    float Normal[3];
    float BiNormal[3];
};
//! \brief A structure to contain a Texture 
//! for display on the surface of a mesh.
struct Texture {
    GLuint id;
    string type;
    string path;
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

/** \brief A structure to define a spotlight.
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
