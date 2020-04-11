/*******************************************************************
 * Info:  A header file to allow classes to access the blender 
 * object model structure. It is needed to use the assimpopengl
 * library included in this package.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * March 2020 San Diego, California USA
 ****************************************************************/

#ifndef INFO_H
#define INFO_H
#include "commonheader.h"
#include "mesh.h"
/** \brief A series of structures to encapsulate:
 * The meshes and textures of a blender object
 * (each mesh may have multiple textures). The
 * location of the blender object in the file
 * structure and an affine transform matrix
 * defining position and orientation. Location
 * is provided so a sort by distance to 
 * the camera can be instituted.
 */
//! The mesh and associated textures.
struct MeshInfo {
    Mesh* mesh;
    vector<Texture>textures;
};
//! The blender objects definition in the program.
struct ModelInfo {
    string path;
    vector<MeshInfo> meshes;
    mat4 model;
    vec3 location;
    int dist = 0;
    int idval = 0;
};

#endif // INFO_H
