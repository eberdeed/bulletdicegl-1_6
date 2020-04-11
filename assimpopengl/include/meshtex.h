/**********************************************************
 *   MeshTex:  A class that is derived from Mesh. This class
 *   is for textured meshes.  This class isolates tha
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>,
 *   it was originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#ifndef MESHTEX_H
#define MESHTEX_H

#include "commonheader.h"
#include "mesh.h"
#include "shader.h"

//! Forward declarations so it can be used as a library.
struct Vertex;
struct Texture;
struct PointLight;
struct SpotLight;
/** \class MeshTex A class that encapsulates creating and 
 * displaying of a textured mesh.
 */
class MeshTex : public Mesh //Make the base class public so the functions are accessible
{
public:
    //! \brief Empty constructor.
    MeshTex();
    //! \brief Deallocate buffers and arrays.
    ~MeshTex();
    /*  Functions  */
    //! \brief Pass in the data from the Model class to be realized here.
    void setData(Vertex *vertices, GLuint *indices, vector<Texture>textures, int vertSize, int indexSize, int texSize);
    //! \brief Draw the object.
    void Draw(Shader *shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, vector<PointLight> lights, vector<SpotLight>spotLights, vec3 viewPos, int startIndex = 0, bool diffOnly = true);
    //! \brief For debugging.
    void dumpData();
    //! \brief Create the mesh data OpenGL buffer object.
    void setupMesh();
    /*  Mesh Data  */
    //! Class global variables.
    //! The Vertex array.
    Vertex *vertices;
    //! The index array.
    GLuint *indices;
    //! The associated textures.
    vector<Texture>textures;
    /*  Render data  */
    //! The buffer object handles.
    GLuint VAO, VBO, EBO;
    //! The upper bounds of the various buffers.
    int vertSize, indexSize, texSize;
    //! Copious debug data.
    bool debug1 = false;
};


#endif // MESHTEX_H
