/**********************************************************
 *   Model:  A class To extract 3D asset data using the Assimp
 *   library, and pass it along to the mesh classes.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#ifndef MODEL_H
#define MODEL_H

#include "commonheader.h"
#include "meshvert.h"
#include "meshtex.h"
#include "createimage.h"
#include "info.h"
#include "shader.h"

//! Forward declarations so it can be used as a library.
struct PointLight;
struct Texture;
struct SpotLight;

/** \class Model A class to extract 3D asset data from a 
 * resource file and pass it along to the mesh files for 
 * display.
 */
class Model 
{
public:
    /*  Functions   */
    /** \brief Pass a vector containing file names of asset files 
     * and their associated positions and orientations.
     */
    Model(vector<ModelInfo> modelinfo);
    //! \brief Destructor, signals destruction of the class.
    ~Model();
    /** \brief Draw the assets that were obtained.  Pass along
     *  the positions of the point lights and spotlights as well as 
     *  the position and orientation of each object being displayed.
     */
    void Draw(Shader *shader, glm::mat4 view, glm::mat4 projection, vector<ModelInfo>model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, int startIndex = 0, bool diffOnly = true);   
    //! \brief Accessor function to let the calling class know whether there are textures or not.
    bool hasTextures();
private:
    /*  Functions   */
    //! \brief Open the asset for extraction.
    void loadModel(string path);
    //! \brief Process a node and all its subnodes, extracting meshes and textures.
    void processNode(aiNode* node, const aiScene* scene);
    //! \brief Extract a textutre.
    MeshInfo processMesh(aiMesh* mesh, const aiScene* scene);
    //! \brief Compare distances
    static bool cmpdist(const ModelInfo &a, const ModelInfo &b);
    /** \brief Sort the objects by their distance from the
     *  camera.  Furthest first, closest last.
     */
    void sortDists(vec3 viewPos);
    /** \breif Sort the objects by idval.  We use this to reassign
     *  the meshes to each object when we call the draw function.
     */
    //! \brief Compares ID values for sorting.
    static bool cmpid(const ModelInfo &a, const ModelInfo &b);
    /** \brief Sort the objects by their ID values.
     */
    void sortIDs();
    //! \brief Get the texture from the assimp material file.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    /** \brief Get the image data from the graphic file
     *  using Free Image Plus.
     */
    GLint TextureFromFile(string filename);
    //! The model description vector.
    vector<ModelInfo>modelinfo;
    //! A vector of Mesh classes, can be both MeshTex and MeshVert because
    //! they are both implementations of Mesh.
    vector<MeshInfo> meshes;
    //! A vector of Textures acquired for the MeshTex class.
    vector<Texture> textures_loaded;
    //! The textures vector, one for each textured mesh.
    vector<Texture>textures;
    //! \breif The index array.
    GLuint *indices;
    //! The size of the Vertex, Index and Texture arrays respectively.
    int vertSize, indexSize, texSize;
    //! Texture flag.
    bool hasTex;
    //! A variable to hold the directory for image access.
    string directory, filename;
    //! Book keeping variables.
    GLuint texcount = 0, vertcount = 0, count1 = 0, limit = 0;
    //! The alpha value for the shader.
    float opacity;
    //! Copious debug value to be had.
    bool debug1 = false;
};

#endif // MODEL_H
