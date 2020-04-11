/**  \class assimpopengl.h: A common header file for the assimpopengl
 *  library.  It provides a standard interface for all the classes
 *  involved in creating the blender object as an OpenGL buffer object.
 *  Created by Edward C. Eberle <eberdeed@eberdeed.net>
 *  December 17, 2016, San Diego California USA
 *  Reworked for OpenGL April 11, 2020.
 */
#ifndef ASSIMPOPENGL_H
#define ASSIMPOPENGL_H

//! Assimp the Asset Importation Library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// Free Image Plus
#include <FreeImagePlus.h>

//! This library
#include "commonheader.h"
#include "model.h"
#include "mesh.h"
#include "meshvert.h"
#include "meshtex.h"
#include "info.h"
#include "createimage.h"
#include "shader.h"

#endif // ASSIMPOPENGL_H
