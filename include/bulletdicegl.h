/**********************************************************
 *   BulletDiceGL:  A class to emulate a roll of the dice
 *   in OpenGL.  The dice were created in blender.
 *   Blender can be found at www.blender.org and similarly
 *   OpenGL can be found at www.khronos.org.  To understand
 *   the inner workings of OpenGL I used the book:
 *   "OpenGL ES 3.0 Programming Guide Second Edition" 
 *   by Dan Ginsburg and Budirijanto Purnomo published by 
 *   Addison-Wesley 2014. Also useful was:  www.learnopengl.com.
 *   SDL2 can be found at https://wiki.libsdl.org.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2020 San Diego, California USA
 * ********************************************************/

#ifndef BULLETDICEGL_H
#define BULLETDICEGL_H

#include "commonheader.h"
#include "camera.h"
#include "createimage.h"
#include "diceroll.h"

/**   \class BulletDiceGL A class to emulate the roll of a
 *   pair of dice in OpenGL.  SDL2 is used to provide 
 *   windowing support.
 */
class BulletDiceGL
{
public:
    /** \brief The constructor initializes data and starts up the sound.
     */
    BulletDiceGL();
    /** \brief The destructor deletes pointers and objects.
     */
    ~BulletDiceGL();

protected:
    /** \brief  Pass the blender objects to the program.
     */
    void setupObjects();
    /** \brief Create the surrounding sky box.
     */
    void createSkyBox();
    /** \brief The event loop.
     */
    void execLoop();

    //! SDL2 Display variables.
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GLContext context;

    /** \brief Window resize function.
     */
    void framebufferSize(int width, int height);

    /** \brief Keystroke processing.
     */
    void keyDown(SDL_Event e);

    /** \brief Mouse movement processing.
     */
    void mouseMove(SDL_Event e);

    /** \brief Window resize, close, minimization and maximization.
     */
    void windowEvent(SDL_Event e);

    /** \brief An error output function that takes an
     *  output stream to define where the message goes.
     */
    void logSDLError(ostream &os, const string &msg);

    /** \brief A static sound creation function
     *  that can fit in a thread.
     */
    static void sndMaker();

    //! \brief Manages dice spin and location.
    void manageDice();
    
    //! \brief Provides spin angles for a die.
    vec3 calcAngles(vec3 rotation, float y, vec3 angle, int index);
    
    /** \brief Calculates what is face up for the current
     *  orientation.
     */
    vec3 faceUp(vec3 rotation);
    
    //! \brief Sets the initial angles.
    void setAngles();
    
    //! \brief Sets the increment angles for ongoing rotation.
    void randAngles(int index);
    
    /** \brief Calculates the floor and walls and provides
     *  the supporting OpenGL vertex and array buffers.
     */
    void calcQuad();
    
    //! \brief Functions to print various types of debug data.
    void printMat4(mat4 matVal);
    void printVec3(vec3 vecVal);
    void printVec2(vec2 vecVal);
    void printQuat(quat quatVal);
    
    //! \brief A function to hack off the last variable in a vec4.
    vec3 vec4ToVec3(vec4 data);
    
    //! A thread to hold the sound function.
    SDL_Thread *thread;
    mat4 quadModel;
    //! Initial screen size values.
    const unsigned int SCR_WIDTH = 1000;
    const unsigned int SCR_HEIGHT = 900;
    //! Dynamic screen size values.
    unsigned int width, height;
    //! Sky box variables.
    unsigned int skyboxVAO, skyboxVBO, skyBox, VAO, VBO, wallTex;
    //! Tile textures for the floor and walls.
    vector<string>tileNames;
    //! The array of names to add to tileNames.
    string feltNames[3] =
    {
        "/usr/share/openglresources/dicefaces/greenfelt.png",
        "/usr/share/openglresources/dicefaces/bluefelt.png",
        "/usr/share/openglresources/dicefaces/redfelt.png"
    };
    //! Booleans for program flow.
    bool firstMouse = true, altSet = false;
    bool endroll = false, debug1 = false, startanim = false;
    //! A radian increment value.
    const float onedegree = (float) acos(-1) / 180.0f;
    //! A variable to minimize rotation at the end of a roll.
    int bouncecount = 2;
    //! Mouse pointer location variables.
    float xpos, ypos, lastX, lastY;
    //! The Bullet Physics component.
    DiceRoll *diceRoller;
    //! Pointer to the model class, which is in the libassimpopengl.so.
    Model *model;
    //! Dice positions
    vector<vec3>dicepos;
    //! Mouse pointer positions.
    ivec2 mousePos1, mousePos2;
    //! Rotation increment angle, the rotation variable,
    //! and the location of the wall and floor quad vertices.
    vec3 angle[2], rotation[2], pos[4];
    //! Check for bumping against the wall. Pause the game.
    bool *newAngle, pause = false;
    //! Output strings.
    string value1, value2;
    //! Dice location and orientation in an affine transform.
    vector<mat4>dicemodel;
    //! Thread return value.
    int threadReturnValue;
    //! Affine transformation matrices.
    mat4 vertmodel, view, projection;
    //! Temporary translation vector.
    vec3 transvec;
    /** \brief A structure to contain the bulk of the
     *  configuration and mesh information for a given
     *  blender object.
     */
    vector<ModelInfo>modelinfo;
    //! Camera position.
    vec3 viewPos;
    //! Camera initial position.
    const vec3 initPos = vec3(0.0f, 120.0f, 120.0f);
    //! Angles in radians.
    const float pi45 = acos(-1.0f) / 4.0f;
    const float pi90 = acos(-1.0f) / 2.0f;
    const float pi360 = acos(-1) * 2.0f;
    //! Data for the wall and floor vertices.
    Vertex vertices[TILES * 3 * 6];
    //! Data fo the floor vertices.
    Vertex floorQuad[6];
    //! Data for the wall vertices.
    Vertex wallQuad[6];
    //! Affine transformations defining wall and floor
    //! tile position and orientation.
    mat4 floorPos[TILES];
    mat4 rwallPos[TILES];
    mat4 lwallPos[TILES];
    //! Scalar for the sky box.
    mat4 boxmodel = mat4(1.0f);
    //! Temporary transformation matrix for the walls and floor.
    mat4 tilemodel = mat4(1.0f);
    //! Shader class to create and manage shaders.
    Shader *stageShader, *shader, *skyBoxShader;
    //! Image class to change images to textures.
    CreateImage *image;
    /**! Camera class to manage camera position and 
     *  orientation.
     */
    Camera *camera;
    //! Timing for the camera.
    chrono::system_clock::time_point intbegin, intend;
    //! The six images for the six sides of the sky cube.
    string skyBoxNames[6] = 
    {
        "/usr/share/openglresources/objects/images/skybox/right.jpg", 
        "/usr/share/openglresources/objects/images/skybox/left.jpg", 
        "/usr/share/openglresources/objects/images/skybox/top.jpg", 
        "/usr/share/openglresources/objects/images/skybox/bottom.jpg", 
        "/usr/share/openglresources/objects/images/skybox/front.jpg", 
        "/usr/share/openglresources/objects/images/skybox/back.jpg"
    };
    //! Positions of the point lights.
    vec3 pointLightPositions[6] = 
    {
        vec3( 15.0f,  150.0f,  50.0f),
        vec3( 0.0f,  150.0f,  50.0f),
        vec3( -15.0f,  150.0f,  50.0f),
        vec3( 2.0f,  150.0f, 10.0f),
        vec3( 0.0f,  150.0f, 10.0f),
        vec3( -2.0f,  150.0f, 10.0f)
    };
    //! Positions of the spot lights.
    vec3 spotLightPositions[3] = 
    {
        vec3( 15.0f,  150.0f,  150.0f),
        vec3( 0.0f,  150.0f,  150.0f),
        vec3( -15.0f,  150.0f,  150.0f),
    };
    //! Direction the spot lights are pointing into.
    vec3 spotLightDirections[3] = 
    {
        vec3( -0.1f,  0.5f,  0.5f),
        vec3( 0.0f,  0.5f,  0.5f),
        vec3( 0.1f,  0.5f,  0.5f),
    };
    //! Temporary variable to hold a point light.
    PointLight lightItem;
    //! The point light definition array.
    vector<PointLight> lights;
    //! Temporary variable to hold a spot light.
    SpotLight spotItem;
    //! The spot light definition array.
    vector<SpotLight> spotLights;
    //! Definition of the sky cube.
    float skyboxVertices[108] = 
    {
        // Positions          
       -1.0f,  1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
        
       -1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
       -1.0f, -1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,
        
       -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,
        
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
};
//! The end of program flag.
//! It is outside the class so it can be used
//! in the static sound function with no penalty.
bool quit = false;
#endif // BULLETDICEGL_H

/** \brief The simplest main possible.
 */
int main(int argc, char **argv)
{
    BulletDiceGL rollit;
    return 0;
}
