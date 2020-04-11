/**********************************************************
 * 
 *   MeshTex:  A class that is derived from Mesh. This class
 *   is for textured meshes.  This class isolates tha
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>,
 *   it was originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/
#include "../include/meshtex.h"

MeshTex::MeshTex()
{
    cout << "\n\n\tCreating MeshTex.\n\n";
    return;
}
    
MeshTex::~MeshTex()
{
    cout << "\n\n\tDestroying MeshTex.\n\n";
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
// Pass along data from the model class to be drawn here.
void MeshTex::setData(Vertex *vertices, GLuint *indices, vector<Texture>textures, int vertSize, int indexSize, int texSize)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->vertSize = vertSize;
    this->indexSize = indexSize;
    this->texSize = texSize;
    setupMesh();
    if (debug1)
    {
        dumpData();
     }
}

//! For debugging.
void MeshTex::dumpData()
{
    cout << "\n\n\tVertices, Normals and TexCoords: \n\n";
    for (int x = 0; x < vertSize; x++)
    {
        cout << "\n\tVertex: " << vertices[x].Position[0] << ", " << vertices[x].Position[1] << ", " << vertices[x].Position[2] <<
        " Normal: " << vertices[x].Normal[0] << ", " << vertices[x].Normal[1] << ", " << vertices[x].Normal[2] <<
        " TexCoord: " << vertices[x].TexCoords[0] << ", " << vertices[x].TexCoords[1];
    }
    cout << "\n\n\n\tIndices: \n\n";
    for (int x = 0; x < indexSize; x++)
    {
        cout << " " << indices[x];
        if (((x + 1) % 25) == 0)
        {
            cout << "\n\n";
        }
    }
    if (texSize > 0)
    {
        for (int x = 0; x < texSize; x++)
        {
            cout << "\n\tTexture ID " << textures[x].id 
            << " for texture type " << textures[x].type;
        }
    }
}

//! Allocate the vertex array and index buffer.
void MeshTex::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);  
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);
    
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}  

//! Draw the object.
void MeshTex::Draw(Shader *shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, int startIndex, bool diffOnly) 
{
    glBindVertexArray(VAO);
    bool difftrigger = true;
    bool spectrigger = true;
    bool heighttrigger = true;
    shader->Use();
    shader->setBool("diffOnly", diffOnly);
    //! Bind appropriate textures
    //! Here we allow for the three types of textures: Diffuse, specular and binormal or bumpmap.
    for( int x = 0; x < textures.size(); x++)
    {
        glActiveTexture(GL_TEXTURE0  + startIndex + x); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[x].id);
        //! Texture present.
        if (debug1)
        {
            cout << "\n\t Texture Path:  " << textures[x].path << "  Texture Type:  " << textures[x].type 
            << "  Index:  " << startIndex + x;
        }
        if ((textures[x].type == "texture_diffuse") && (difftrigger))
        {
            shader->setBool("isDiffuse", true);
            shader->setInt("texture_diffuse1", startIndex + x);
            difftrigger = false;
        }
        else if ((textures[x].type == "texture_specular") && (spectrigger))
        {
            shader->setBool("isSpecular", true);
            shader->setInt("texture_specular1", startIndex + x);
            spectrigger = false;
        }
        else if ((textures[x].type == "texture_height") && (heighttrigger))
        {
            shader->setBool("isBinormal", true);
            shader->setInt("texture_binormal1", startIndex + x);
            spectrigger = false;
        }
    }
    if (difftrigger)
    {
       shader->setBool("isDiffuse", false);
    }
    if (spectrigger)
    {
       shader->setBool("isSpecular", false);
    }
    if (heighttrigger)
    {
       shader->setBool("isBinormal", false);
    }
    shader->setFloat("shininess", 10.0f);
    shader->setVec3("viewPos", viewPos);
    shader->setFloat("opacity", opacity);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);
    shader->setVec3("colordiff", vec3(1.0f, 1.0f, 1.0f));
    for (int x = 0; x < lights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("pointLights[" + ss.str() + "].position", lights[x].position);
        shader->setVec3("pointLights[" + ss.str() + "].ambient", lights[x].ambient);
        shader->setVec3("pointLights[" + ss.str() + "].diffuse", lights[x].diffuse);
        shader->setVec3("pointLights[" + ss.str() + "].specular", lights[x].specular);
        shader->setFloat("pointLights[" + ss.str() + "].constant", lights[x].constant);
        shader->setFloat("pointLights[" + ss.str() + "].linear", lights[x].linear);
        shader->setFloat("pointLights[" + ss.str() + "].quadratic", lights[x].quadratic);
    }
    for (int x = 0; x < spotLights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("spotLights[" + ss.str() + "].position", spotLights[x].position);
        shader->setVec3("spotLights[" + ss.str() + "].direction", spotLights[x].direction);
        shader->setFloat("spotLights[" + ss.str() + "].cutOff", spotLights[x].cutOff);
        shader->setFloat("spotLights[" + ss.str() + "].outerCutOff", spotLights[x].outerCutOff);
        shader->setVec3("spotLights[" + ss.str() + "].ambient", spotLights[x].ambient);
        shader->setVec3("spotLights[" + ss.str() + "].diffuse", spotLights[x].diffuse);
        shader->setVec3("spotLights[" + ss.str() + "].specular", spotLights[x].specular);
        shader->setFloat("spotLights[" + ss.str() + "].constant", spotLights[x].constant);
        shader->setFloat("spotLights[" + ss.str() + "].linear", spotLights[x].linear);
        shader->setFloat("spotLights[" + ss.str() + "].quadratic", spotLights[x].quadratic);
    }
    // Draw mesh
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

