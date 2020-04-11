/**********************************************************
 *   BulletDiceGL:  A class to display an arbitrary Blender
 *   object in OpenGL.  The objects and their locations are 
 *   passed into the program by the setupObjects function.
 *   Blender can be found at www.blender.org and similarly
 *   OpenGL can be found at www.khronos.org.  To understand
 *   the inner workings of OpenGL I used the book:
 *   "OpenGL ES 3.0 Programming Guide Second Edition" 
 *   by Dan Ginsburg and Budirijanto Purnomo published by 
 *   Addison-Wesley 2014. Also useful was:  www.learnopengl.com.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   March 2020 San Diego, California USA
 * ********************************************************/


#include "../include/bulletdicegl.h"


BulletDiceGL::BulletDiceGL()
{
    /** I pass creation and destruction messages
     *  from each class to ensure the class 
     *  is properly handled.
     */
    cout << "\n\n\tCreating BulletDiceGL\n\n";
    rotation[0] = rotation[1] = vec3(0.0f, 0.0f, 0.0f);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logSDLError(cout, "SDL_Init");
        exit(1);
    }
    else
    {
        cout << "\n\n\tInitialized SDL.\n\n";
    }
    image = new CreateImage();
    srand(time(nullptr));
    int i, count = SDL_GetNumAudioDevices(0);

    thread = SDL_CreateThread((SDL_ThreadFunction)BulletDiceGL::sndMaker, "SoundThread", (void *)NULL);
    if (NULL == thread) 
    {
        logSDLError(cout, "SDL_CreateThread failed");
        for (i = 0; i < count; ++i) 
        {
            cout << "\n\n\tFor audio device " << i << " with name " << SDL_GetAudioDeviceName(i, 0) << "\n\n";
        }
    } else 
    {
        cout << "\n\n\tSound thread successfully created.\n\n";
    }
    try
    {
            // Setup the window
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

        window = SDL_CreateWindow("Bullet Dice for OpenGL 1.6", 500, 100, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        if (window == nullptr){
            logSDLError(cout, "CreateWindow");
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated SDL window.\n\n";
        }
        
        context = SDL_GL_CreateContext(window);
        if (context == nullptr){
            logSDLError(cout, "CreateContext");
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(context);
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated SDL context.\n\n";
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr){
            logSDLError(cout, "CreateRenderer");
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(context);
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated renderer.\n";
        }
        SDL_ShowWindow(window);
        SDL_GL_MakeCurrent(window, context);
        glewExperimental=true;
        GLenum err=glewInit();
        if(err!=GLEW_OK)
        {
            //Problem: glewInit failed, something is seriously wrong.
            cout<<"\n\n\tThe function glewInit failed, aborting."<<endl;
            exit(1);
        }
        else
        {
            cout << "\n\n\tInitialized glew.\n";
        }
        cout << "\n\n\tUsing GLEW Version: " << glewGetString(GLEW_VERSION) << "\n\n"; 
        glEnable(GL_DEPTH_TEST);  
        bool enabled = glIsEnabled(GL_DEPTH_TEST);
        if (enabled)
        {
            cout << "\n\n\tDepth Test Enabled\n\n";
        }
        else
        {
            cout << "\n\n\tDepth Test Not Enabled\n\n";
        }
        glDepthFunc(GL_LESS);
        //glFrontFace(GL_CCW);
        glDisable(GL_CULL_FACE);
        enabled = glIsEnabled(GL_CULL_FACE);
        if (enabled)
        {
            cout << "\n\n\tCull Face Enabled\n\n";
        }
        else
        {
            cout << "\n\n\tCull Face Not Enabled\n\n";
        }
        //glCullFace(GL_BACK);
        glDepthRange(0.1f, 10000.0f);
        shader = new Shader();
        shader->initShader("/usr/share/openglresources/shaders/bulletshader.vs", 
        "/usr/share/openglresources/shaders/bulletshader.frag", "bulletshader.bin");
        stageShader = new Shader();
        stageShader->initShader("/usr/share/openglresources/shaders/dicestage.vs", 
        "/usr/share/openglresources/shaders/dicestage.frag", "dicestage.bin");
        camera = new Camera(SCR_WIDTH, SCR_HEIGHT, initPos, vec3(0.0f, 0.0f, 0.0f));
    }
    catch(exception exc)
    {
        cout << "\n\n\tProgram Initialization Error:  " << exc.what() << "\n\n";
    }
    cout << "\n\n\tCreating point lights.\n\n";
    // point light 1
    lightItem.position = pointLightPositions[0];
    lightItem.ambient = vec3(0.05f, 0.05f, 0.05f);
    lightItem.diffuse = vec3(1.0f, 1.0f, 1.0f);
    lightItem.specular = vec3(1.0f, 1.0f, 1.0f);
    lightItem.constant = 1.0f;
    lightItem.linear = 0.09f;
    lightItem.quadratic = 0.032f;
    lights.push_back(lightItem);
    // point light 2
    lightItem.position = pointLightPositions[1];
    lightItem.ambient = vec3(0.05f, 0.05f, 0.05f);
    lightItem.diffuse = vec3(1.0f, 1.0f, 1.0f);
    lightItem.specular = vec3(1.0f, 1.0f, 1.0f);
    lightItem.constant = 1.0f;
    lightItem.linear = 0.09f;
    lightItem.quadratic = 0.032f;
    lights.push_back(lightItem);
    // point light 3
    lightItem.position = pointLightPositions[2];
    lightItem.ambient = vec3(0.05f, 0.05f, 0.05f);
    lightItem.diffuse = vec3(1.0f, 1.0f, 1.0f);
    lightItem.specular = vec3(1.0f, 1.0f, 1.0f);
    lightItem.constant = 1.0f;
    lightItem.linear = 0.09f;
    lightItem.quadratic = 0.032f;
    lights.push_back(lightItem);
    // point light 4
    lightItem.position = pointLightPositions[3];
    lightItem.ambient = vec3(0.05f, 0.05f, 0.05f);
    lightItem.diffuse = vec3(1.0f, 1.0f, 1.0f);
    lightItem.specular = vec3(1.0f, 1.0f, 1.0f);
    lightItem.constant = 1.0f;
    lightItem.linear = 0.09f;
    lightItem.quadratic = 0.032f;    
    lights.push_back(lightItem);
    // point light 5
    lightItem.position = pointLightPositions[4];
    lightItem.ambient = vec3(0.05f, 0.05f, 0.05f);
    lightItem.diffuse = vec3(1.0f, 1.0f, 1.0f);
    lightItem.specular = vec3(1.0f, 1.0f, 1.0f);
    lightItem.constant = 1.0f;
    lightItem.linear = 0.09f;
    lightItem.quadratic = 0.032f;    
    lights.push_back(lightItem);
    // point light 6
    lightItem.position = pointLightPositions[5];
    lightItem.ambient = vec3(0.05f, 0.05f, 0.05f);
    lightItem.diffuse = vec3(1.0f, 1.0f, 1.0f);
    lightItem.specular = vec3(1.0f, 1.0f, 1.0f);
    lightItem.constant = 1.0f;
    lightItem.linear = 0.09f;
    lightItem.quadratic = 0.032f;    
    lights.push_back(lightItem);
    for (int x = 0; x < 3; x++)
    {   
        spotItem.position = spotLightPositions[x];
        spotItem.direction = spotLightDirections[x];
        spotItem.cutOff = 25 * (acos(-1.0f) / 180.0f);
        spotItem.outerCutOff = 30 * (acos(-1.0f) / 180.0f);
        spotItem.ambient = vec3(0.05f, 0.05f, 0.05f);
        spotItem.diffuse = vec3(1.0f, 1.0f, 1.0f);
        spotItem.specular = vec3(1.0f, 1.0f, 1.0f);
        spotItem.constant = 1.0f;
        spotItem.linear = 0.9f;
        spotItem.quadratic = 0.1f; 
        spotLights.push_back(spotItem);
    }
    viewPos = initPos;
    quit = false;
    boxmodel = scale(boxmodel, vec3(5000.0f, 5000.0f, 5000.0f));
    tilemodel = scale(tilemodel, vec3(30.0f, 30.0f, 30.0f));
    width = SCR_WIDTH;
    height = SCR_HEIGHT;
     /* Simply create a thread */
    //! Positions for the floor and wall tiles.
    count = 0;
    //! Define the floor tile positions.
    for (int x = -100; x < 100; x += 4)
    {
        for (int z = -100; z < 100; z += 4)
        {
            float a = (float) x;
            float b = -0.5f;
            float c = (float) z + 25.0f;
            transvec = vec3(a, b, c);
            quadModel = mat4(1.0f);
            // Calculate the model matrix for each object and pass it to shader before drawing
            quadModel = translate(quadModel, transvec);
            floorPos[count++] = quadModel;
        }
    }   
    count = 0;
    //! Define the left wall tile positions.
    for (int x = 0; x < 200; x += 4)
    {
        for (int z = -25; z < 75; z += 2)
        {
            float a = (float) - z - 25;
            float b = (float) x;
            float c = (float) z - 25;
            transvec = vec3(a, b, c);
            quadModel = mat4(1.0f);
            // Calculate the model matrix for each object and pass it to shader before drawing
            quadModel = translate(quadModel, transvec);
            quadModel = rotate(quadModel, pi45, vec3(0.0f, 1.0f, 0.0f));
            rwallPos[count++] = quadModel;
        }
    }   
    count = 0;
    //! Define the right wall tile positions.
    for (int x = 0; x < 200; x += 4)
    {
        for (int z = -25; z < 75; z += 2)
        {
            float a = (float) z + 25;
            float b = (float) x;
            float c = (float) z - 25;
            transvec = vec3(a, b, c);
            quadModel = mat4(1.0f);
            // Calculate the model matrix for each object and pass it to shader before drawing
            quadModel = translate(quadModel, transvec);
            quadModel = rotate(quadModel, -pi45, vec3(0.0f, 1.0f, 0.0f));
            lwallPos[count++] = quadModel;
        }
    }  
    setupObjects();
    createSkyBox();
    execLoop();
    SDL_WaitThread(thread, &threadReturnValue);
    cout << "\n\n\tThread returned value:  " << threadReturnValue << "\n\n";
}

BulletDiceGL::~BulletDiceGL()
{
    cout << "\n\n\tDestroying BulletDiceGL\n\n";
    /** Deleting the objects produces destroy messages
     *  from the object deleted.
     */
    delete camera;
    delete shader;
    delete skyBoxShader;
    delete model;
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteTextures(1, &skyBox);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &wallTex);
    exit(0);
}

void BulletDiceGL::setupObjects()
{
    /** Here is where the Blender object file name and
     *  position information are provided.  For more
     *  information about the structure ModelInfo
     *  see the file "info.h".
     */
    try
    {
        ModelInfo item;
        item.path = "/usr/share/openglresources/dice/dice.obj";
        dicepos.push_back(vec3(-15.0f, 30.0f, 0.0) + vec3(-10.0f, 0.0f, 40.0f));
        dicepos.push_back(vec3(15.0f, 30.0f, 0.0f) + vec3(10.0f, 0.0f, 40.0f));
        item.idval = 0;
        item.location = dicepos[0];
        vertmodel = mat4(1.0f);
        vertmodel = translate(vertmodel, dicepos[0]);
        vertmodel = scale(vertmodel, vec3(3.0f, 3.0f, 3.0f));
        item.model = vertmodel;
        dicemodel.push_back(vertmodel);
        cout << "\n\n\tStoring item:  " <<  item.path << "  Index:  " << 0 << ".\n\n";
        modelinfo.push_back(item);
        item.idval = 1;
        item.location = dicepos[1];
        vertmodel = mat4(1.0f);
        vertmodel = translate(vertmodel, dicepos[1]);
        vertmodel = rotate(vertmodel, 90.0f * onedegree, vec3(0.0f, 1.0f, 0.0f));
        vertmodel = rotate(vertmodel, 90.0f * onedegree, vec3(0.0f, 0.0f, 1.0f));
        vertmodel = scale(vertmodel, vec3(3.0f, 3.0f, 3.0f));
        dicemodel.push_back(vertmodel);
        item.model = vertmodel;
        cout << "\n\n\tStoring item:  " <<  item.path << "  Index:  " << 1 << ".\n\n";
        modelinfo.push_back(item);
        model = new Model(modelinfo);
        calcQuad();
        for (int x = 0; x < 3; x++)
        {
            tileNames.push_back(feltNames[x]);
        }
        image->create2DTexArray(wallTex, tileNames);
    }
    catch(exception exc)
    {
        cout << "\n\n\tOpenGL Texture Definition Error:  " << exc.what() << "\n";
    }
}   

void BulletDiceGL::createSkyBox()
{
    skyBoxShader = new Shader();
    skyBoxShader->initShader(string("/usr/share/openglresources/shaders/skyboxshader.vs"),
    string("/usr/share/openglresources/shaders/skyboxshader.frag"), string("skyboxshader.bin"));

    glGenTextures(1, &skyBox);
    image->createSkyBoxTex(skyBox, skyBoxNames);

    cout << "\n\n\tCreating skybox vertex buffer.\n\n";
    // Generate the sky box.
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}    


void BulletDiceGL::execLoop()
{
    int count = 0;
    quit = false;
    SDL_Event e;
    diceRoller = new DiceRoll(initPos);
    diceRoller->resetDice();
    bouncecount = 0;
    setAngles();
    rotation[0] = faceUp(rotation[0]);
    rotation[1] = faceUp(rotation[1]);
    randAngles(0);
    randAngles(1);
    diceRoller->setCameraPos(viewPos);
    //! render loop
    //! -----------
    while (!quit)
    {
        if(debug1)
        {
            cout << "\n\t" << count++;
        }
        //! Grab a time to adjust camera speed.
        intbegin  = chrono::system_clock::now();
        //! Find the camera.
        //! render
        // ------
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //! Tell the position class DiceRoller where the camera is.
        if (debug1)
        {
            cout << "\n\tDice Model Size:  " << dicemodel.size();
            cout << "\n\tModel Info Dice 1:  " << dicemodel[0][3][0] << ", " 
            << dicemodel[0][3][1] << ", " << dicemodel[0][3][2];
            cout << "\n\tModel Info Dice 2:  " << dicemodel[1][3][0] << ", " 
            << dicemodel[1][3][1] << ", " << dicemodel[1][3][2];
        }
        modelinfo[0].model = dicemodel[0];
        modelinfo[1].model = dicemodel[1];
        //! Set the viewport and draw the graphic objects. 
        view = camera->getViewMatrix(); //! render
        projection = camera->getPerspective();
        skyBoxShader->Use();
        skyBoxShader->setMat4("view", view);
        skyBoxShader->setMat4("projection", projection);
        skyBoxShader->setMat4("model", boxmodel);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox);
        skyBoxShader->setInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);
        glBindVertexArray(VAO);
        stageShader->Use();
        //! Pass into the shader the light definitions.
        stageShader->setMat4("view", view);
        stageShader->setMat4("projection", projection);
        stageShader->setVec3("viewPos", viewPos);
        for (int x = 0; x < lights.size(); x++)
        {   
            stringstream ss;
            ss << x;
            stageShader->setVec3("pointLights[" + ss.str() + "].position", lights[x].position);
            stageShader->setVec3("pointLights[" + ss.str() + "].ambient", lights[x].ambient);
            stageShader->setVec3("pointLights[" + ss.str() + "].diffuse", lights[x].diffuse);
            stageShader->setVec3("pointLights[" + ss.str() + "].specular", lights[x].specular);
            stageShader->setFloat("pointLights[" + ss.str() + "].constant", lights[x].constant);
            stageShader->setFloat("pointLights[" + ss.str() + "].linear", lights[x].linear);
            stageShader->setFloat("pointLights[" + ss.str() + "].quadratic", lights[x].quadratic);
        }
        for (int x = 0; x < spotLights.size(); x++)
        {   
            stringstream ss;
            ss << x;
            stageShader->setVec3("spotLights[" + ss.str() + "].position", spotLights[x].position);
            stageShader->setVec3("spotLights[" + ss.str() + "].direction", spotLights[x].direction);
            stageShader->setFloat("spotLights[" + ss.str() + "].cutOff", spotLights[x].cutOff);
            stageShader->setFloat("spotLights[" + ss.str() + "].outerCutOff", spotLights[x].outerCutOff);
            stageShader->setVec3("spotLights[" + ss.str() + "].ambient", spotLights[x].ambient);
            stageShader->setVec3("spotLights[" + ss.str() + "].diffuse", spotLights[x].diffuse);
            stageShader->setVec3("spotLights[" + ss.str() + "].specular", spotLights[x].specular);
            stageShader->setFloat("spotLights[" + ss.str() + "].constant", spotLights[x].constant);
            stageShader->setFloat("spotLights[" + ss.str() + "].linear", spotLights[x].linear);
            stageShader->setFloat("spotLights[" + ss.str() + "].quadratic", spotLights[x].quadratic);
        }
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D_ARRAY, wallTex);
        stageShader->setInt("wallTex", 1);
        glDrawArrays(GL_TRIANGLES, 0, 3 * 6 * TILES);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glBindVertexArray(0);
        // Draw the dice.
        shader->Use();
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox);
        skyBoxShader->setInt("SkyBox", 2);
        model->Draw(shader, view, projection, modelinfo, lights, spotLights, viewPos, 3, true);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        intend = chrono::system_clock::now();
        //! Get the DiceRoller adjusted camera position.
        diceRoller->setCameraPos(viewPos);
        if (debug1)
        {
            cout << "\n\tCamera 2:  ";
            printVec3(viewPos);
        }
        if (!pause)
        {
            manageDice();
        }
        SDL_GL_SwapWindow(window);
        camera->setPosition(diceRoller->getCameraPos());
        if (debug1)
        {
            cout << "\n\tCamera 3:  ";
            printVec3(viewPos);
        }
        while (SDL_PollEvent(&e))
        {
            keyDown(e);
            mouseMove(e);
            windowEvent(e);
        };
        viewPos = camera->getPosition();
        if (debug1)
        {
            cout << "\n\tCamera 1:  ";
            printVec3(viewPos);
        }
    };
    //! ------------------------------------------------------------------
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    this->~BulletDiceGL();
}


//! Whenever the window size changed (by OS or user resize) this callback function executes.  This is not fully implemented.
// ---------------------------------------------------------------------------------------------
void BulletDiceGL::framebufferSize(int width, int height)
{
    /** Make sure the viewport matches the new window 
     *  dimensions; note that width and height will be
     *  significantly larger than specified on retina 
     *  displays.
     */
    glViewport(0, 0, width, height);
    //! Set a videomode - 1280x1024
}

//! ---------------------------------------------------------------------------------------------------------
// process all input: query SDL2 whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void BulletDiceGL::keyDown(SDL_Event e)
{
    int delta = (int)
    //! Use timing to create a cameraSpeed variable.
    chrono::duration_cast<chrono::milliseconds>(intend -
    intbegin).count();
    float cameraSpeed = (float) delta / 45.0f;
    //! Motion keys.
    //! Forward motion.
    if (e.type == SDL_KEYDOWN) 
    {
        cout << "\n\n\tIn keyDown:  " << SDL_GetKeyName(e.key.keysym.sym) << "\n\n";
        switch (e.key.keysym.sym)
        {
            case SDLK_SPACE:
                diceRoller->resetDice();
                bouncecount = 2;
                randAngles(0);
                randAngles(1);
                break;
            case SDLK_w:
                camera->processKeyboard(Camera::Camera_Movement::FORWARD, cameraSpeed);
                break;
            //! Backwards motion.
            case SDLK_s:
                camera->processKeyboard(Camera::Camera_Movement::BACKWARD, cameraSpeed);
                break;
            //! Move left.
            case SDLK_a:
                camera->processKeyboard(Camera::Camera_Movement::LEFT, cameraSpeed);
                break;
            //! Move right.
            case SDLK_d:
                camera->processKeyboard(Camera::Camera_Movement::RIGHT, cameraSpeed);
                break;
            //! Move up.
            case SDLK_r:
                camera->processKeyboard(Camera::Camera_Movement::UP, cameraSpeed);
                break;
            //! Move down.
            case SDLK_f:
                camera->processKeyboard(Camera::Camera_Movement::DOWN, cameraSpeed);
                break;
            //! Reset the camera.
            case SDLK_z:
                camera->resetCamera();
                break;
            //! Reverse the camera.
            case SDLK_x:
                camera->reverseDirection();
                break;
            //! Pause the game.
            case SDLK_p:
                pause = !pause;
                break;
            //! Zoom keys.
            //! Zoom in.
            case SDLK_UP:
                camera->processMouseScroll(Camera::Camera_Movement::CLOSER);
                break;
            //! Zoom out.
            case SDLK_DOWN:
                camera->processMouseScroll(Camera::Camera_Movement::AWAY);
                break;
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_RALT:
                altSet = true;
                break;
            case SDLK_LALT:
                altSet = true;
                break;
            case SDLK_RETURN:
                if (altSet)
                {
                    cout << "\n\n\tSet window fullscreen.\n\n";
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    altSet = false;
                }
                break;
        }
    }
}    

void BulletDiceGL::windowEvent(SDL_Event e)
{
    if (e.type == SDL_WINDOWEVENT) {
        switch (e.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            cout << "\n\n\tWindow " << e.window.windowID << " shown.\n\n";
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            cout << "\n\n\tWindow " << e.window.windowID << " hidden.\n\n";
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            cout << "\n\n\tWindow " << e.window.windowID << " exposed.\n\n";
            break;
        case SDL_WINDOWEVENT_MOVED:
            cout << "\n\n\tWindow " << e.window.windowID << " moved to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            break;
        case SDL_WINDOWEVENT_RESIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " resized to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            width = (unsigned  int)e.window.data1;
            height = (unsigned  int)e.window.data2;
            glViewport(0, 0, e.window.data1, e.window.data2);
            camera->resizeView(e.window.data1, e.window.data2);
            //defineFrameObjects();
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            cout << "\n\n\tWindow " << e.window.windowID << " size changed to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            width = (unsigned  int)e.window.data1;
            height = (unsigned  int)e.window.data2;
            glViewport(0, 0, e.window.data1, e.window.data2);
            camera->resizeView(e.window.data1, e.window.data2);
            //defineFrameObjects();
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " minimized.\n\n";
            width = (unsigned  int)0;
            height = (unsigned  int)0;
            //glViewport(0, 0, 0, 0);
            //camera->resizeView(0, 0);
            //defineFrameObjects();
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " maximized.\n\n";
            width = (unsigned int)e.window.data1;
            height = (unsigned int)e.window.data2;
            SDL_SetWindowSize(window, width, height);
            glViewport(0, 0, width, height);
            camera->resizeView(width, height);
            //defineFrameObjects();
            break;
        case SDL_WINDOWEVENT_RESTORED:
            cout << "\n\n\tWindow " << e.window.windowID << " restored.\n\n";
            break;
        case SDL_WINDOWEVENT_ENTER:
            cout << "\n\n\tMouse entered window " << e.window.windowID << ".\n\n";
            break;
        case SDL_WINDOWEVENT_LEAVE:
            cout << "\n\n\tMouse left window " << e.window.windowID << ".\n\n";
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            cout << "\n\n\tWindow " << e.window.windowID << " gained keyboard focus.\n\n";
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            cout << "\n\n\tWindow " << e.window.windowID << " lost keyboard focus.\n\n";
            break;
        case SDL_WINDOWEVENT_CLOSE:
            cout << "\n\n\tWindow " << e.window.windowID << " closed.\n\n";
            quit = true;
            break;
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            cout << "\n\n\tWindow " << e.window.windowID << " is offered focus.\n\n";
            break;
        case SDL_WINDOWEVENT_HIT_TEST:
            cout << "\n\n\tWindow " << e.window.windowID << " has a special hit test.\n\n";
            break;
        default:
            cout << "\n\n\tWindow " << e.window.windowID << " received an unknown event.\n\n";
            break;
        }
    }
}

void BulletDiceGL::mouseMove(SDL_Event e)
{
    if (e.type == SDL_MOUSEWHEEL)
    {
            if(e.wheel.y > 0)
            {
                camera->processMouseScroll(Camera::Camera_Movement::CLOSER);
            }
            else if (e.wheel.y < 0)
            {
                camera->processMouseScroll(Camera::Camera_Movement::AWAY);
            }
    }
    else if( e.type == SDL_MOUSEBUTTONDOWN)
    {
        cout << "\n\n\tIn mouseMove() Mouse Down X, Y:  " << e.motion.x << ", " << e.motion.y << "\n\n";
        mousePos1.x = e.motion.x;
        mousePos1.y = e.motion.y;
    }
    else if( e.type == SDL_MOUSEBUTTONUP)
    {
        cout << "\n\n\tIn mouseMove() Mouse Up X, Y:  " << e.motion.x << ", " << e.motion.y << "\n\n";
        mousePos2.x = e.motion.x;
        mousePos2.y = e.motion.y;
        camera->processMouseMovement(mousePos2.x - mousePos1.x, mousePos2.y - mousePos1.y);
    }
    
}    
void BulletDiceGL::logSDLError(ostream &os, const string &msg)
{
    os << "\n\n\t" << msg << " error: " << SDL_GetError() << "\n\n";
}
void BulletDiceGL::sndMaker()
{
    Uint32 wave_len; // length of our sample
    Uint8 *wave_buf; // buffer containing our audio file
    SDL_AudioSpec wave_spec, ret_spec; // the specs of our piece of music
    cout << "\n\n\tStarting sound production.";
    cout << "\n\tLoading WAV file.\n\n";
    
    if (SDL_LoadWAV("/usr/share/openglresources/sound/celticfive.wav", &wave_spec, &wave_buf, &wave_len) ==  NULL)
    {
        cout << "\n\n\tUnable to load WAV file:  " << SDL_GetError() << "\n\n";
        exit(-1);
    }
    else
    {
        cout << "\n\n\tWAV file successfully loaded.\n\n";
    }
    string name  = SDL_GetAudioDeviceName(0, 0);
    cout << "\n\n\tAudio device:  " << name << "\n\n";
    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(name.c_str(), 0, &wave_spec, &ret_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (dev < 0)
    {
        cout << "\n\n\tUnable to open audio device:  " << SDL_GetError() << "\n\n";
        exit(-1);
    }
    else
    {
        cout << "\n\n\tAudio device successfully opened for WAV file with length:  "  << wave_len  << ".\n\n";
        int result = SDL_QueueAudio(dev, wave_buf, wave_len);
        if (result != 0)
        {
            cout <<  "\n\n\tUnable to queue audio data.\n\n";
            exit(-1);
        }
        else
        {
            cout << "\n\n\tAudio data successfully queued.\n\n";
        }
    }
    while (!quit)
    {
        SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
        cout << "\n\n\tPlayed the audio.\n\n";
        SDL_Delay(383000); /* let the audio callback play some sound for 5 seconds. */
        int result = SDL_QueueAudio(dev, wave_buf, wave_len);
        if (result != 0)
        {
            cout <<  "\n\n\tUnable to queue audio data.\n\n";
            exit(-1);
        }
        else
        {
            cout << "\n\n\tAudio data successfully queued.\n\n";
        }
    }
    SDL_CloseAudioDevice(dev);
    cout << "\n\n\tClosed audio device.\n\n";
	SDL_FreeWAV(wave_buf);
}

//! Dice location and orientation are mananged from here.
void BulletDiceGL::manageDice()
{
    try
    {
        diceRoller->diceEvents();
        //! Obtain the dice location.
        dicepos[0] = diceRoller->getDie1();
        dicepos[1] = diceRoller->getDie2();
        if (debug1)
        {
            if(endroll)
            {
                value1 = "endroll";
            }
            else
            {
                value1 = "not endroll";
            }
            if(startanim)
            {
                value2 = "startanim";
            }
            else
            {
                value2 = "not startanim";
            }
            cout << "\n\tBooleans:  " << value1 << " : " << value2;
            cout << "\n\tDice  Model 1";
            printMat4(dicemodel[0]);
            cout << "\n\tDice Model 2";
            printMat4(dicemodel[1]);
            cout << "\n\tAngle 1";
            printVec3(angle[0]);
            cout << "\n\tAngle 2";
            printVec3(angle[1]);
            cout << "\n\tRotation 1";
            printVec3(rotation[0]);
            cout << "\n\tRotation 2";
            printVec3(rotation[1]);
            cout << "\n\tPosition 1";
            printVec3(dicepos[0]);
            cout << "\n\tPosition 2";
            printVec3(dicepos[1]);
            cout << "\n\n";
        }
        vertmodel = mat4(1.0f);
        //! Set that location.
        vertmodel = translate(vertmodel, dicepos[0]);
        vertmodel = scale(vertmodel, vec3(3.0f, 3.0f, 3.0f));
        dicemodel[0] = vertmodel;
        vertmodel = mat4(1.0f);
        //! Set that location.
        vertmodel = translate(vertmodel, dicepos[1]);
        vertmodel = scale(vertmodel, vec3(3.0f, 3.0f, 3.0f));
        dicemodel[1] = vertmodel;
        //! Get animation status.
        endroll = diceRoller->getEndRoll();
        startanim = diceRoller->getStartAnim();
        newAngle = diceRoller->getNewAngle();
        //! Roll is ongoing, calculate orientation using euler angles.
        if ((!endroll) && (!startanim))
        {
            rotation[0] = calcAngles(rotation[0], dicepos[0].y, angle[0], 0);
            dicemodel[0] = rotate(dicemodel[0], rotation[0].x, vec3(1.0f, 0.0f, 0.0f));
            dicemodel[0] = rotate(dicemodel[0], rotation[0].y, vec3(0.0f, 1.0f, 0.0f));
            dicemodel[0] = rotate(dicemodel[0], rotation[0].z, vec3(0.0f, 0.0f, 1.0f));

            rotation[1] = calcAngles(rotation[1], dicepos[1].y, angle[1], 1);
            dicemodel[1] = rotate(dicemodel[1], rotation[1].x, vec3(1.0f, 0.0f, 0.0f));
            dicemodel[1] = rotate(dicemodel[1], rotation[1].y, vec3(0.0f, 1.0f, 0.0f));
            dicemodel[1] = rotate(dicemodel[1], rotation[1].z, vec3(0.0f, 0.0f, 1.0f));
        }
        else
        {
            rotation[0] = faceUp(rotation[0]);
            dicemodel[0] = rotate(dicemodel[0], rotation[0].x, vec3(1.0f, 0.0f, 0.0f));
            dicemodel[0] = rotate(dicemodel[0], rotation[0].y, vec3(0.0f, 1.0f, 0.0f));
            dicemodel[0] = rotate(dicemodel[0], rotation[0].z, vec3(0.0f, 0.0f, 1.0f));
            rotation[1] = faceUp(rotation[1]);
            dicemodel[1] = rotate(dicemodel[1], rotation[1].x, vec3(1.0f, 0.0f, 0.0f));
            dicemodel[1] = rotate(dicemodel[1], rotation[1].y, vec3(0.0f, 1.0f, 0.0f));
            dicemodel[1] = rotate(dicemodel[1], rotation[1].z, vec3(0.0f, 0.0f, 1.0f));
        }
    }
    catch(exception exc)
    {
        cout << "\n\n\tError in BulletDiceGL::manageDice():  " << exc.what() << "\n\n";
        exit(-1);
    }
            
}

//! Calculate which face is up.    
vec3 BulletDiceGL::faceUp(vec3 rotation)
{
    float pi2 = (3.14159f / 2.0f); 
    rotation.x = (float) ((int) round(rotation.x / pi2) % 4) * pi2;
    rotation.y = (float) ((int) round(rotation.y / pi2) % 4) * pi2;
    rotation.z = (float) ((int) round(rotation.z / pi2) % 4) * pi2;
    return rotation;
}

void BulletDiceGL::setAngles()
//! Set the initial angle increment values.
{
    angle[0].x = (float)(rand() % 314159) / 10000.0f;
    angle[0].y = (float)(rand() % 314159) / 10000.0f;
    angle[0].z = (float)(rand() % 314159) / 10000.0f;
    angle[1].x = (float)(rand() % 314159) / 10000.0f;
    angle[1].y = (float)(rand() % 314159) / 10000.0f;
    angle[1].z = (float)(rand() % 314159) / 10000.0f;
    rotation[0] += angle[0];
    rotation[1] += angle[1];
}
void BulletDiceGL::randAngles(int index)
//! Set the initial angle increment values.
{
    angle[index].x = (float)(rand() % 314159) / 10000000.0f;
    angle[index].y = (float)(rand() % 314159) / 10000000.0f;
    angle[index].z = (float)(rand() % 314159) / 10000000.0f;
}


/* For each die, calculate spin angles. If we are close to the floor,
 * randomly choose new angles in the opposite direction.
 */
vec3 BulletDiceGL::calcAngles(vec3 rotation, float y, vec3 angle, int index)
{
    try
    {
        if (y < 7.0f)
        {
            angle.x *= (1.0f)/(float) bouncecount;
            angle.y *= (1.0f)/(float) bouncecount;
            angle.z *= (1.0f)/(float) bouncecount;
            bouncecount  += 1;
            if (bouncecount > 10)
            {
                rotation = faceUp(rotation);
            }
        }
        else
        {
            bouncecount = 0;
        }
        if (newAngle[index])
        {
            angle = -angle;
        }

        rotation += angle;

        rotation.x = remainder(rotation.x, pi360);
        rotation.y = remainder(rotation.y, pi360);
        rotation.z = remainder(rotation.z, pi360);
        if (isnan(rotation.x))
        {
            rotation.x = 0.0f;
        }
        if (isnan(rotation.y))
        {
            rotation.y = 0.0f;
        }
        if (isnan(rotation.z))
        {
            rotation.z = 0.0f;
        }
    }
    catch(exception exc)
    {
        cout << "\n\n\tProgram error in calcAngles():  " << exc.what() << "\n\n";
        exit(-1);
    }
    return rotation;    
}    
void BulletDiceGL::calcQuad()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    vec3 pos[2][4];
    int count = 0, vertCount = 0;
    float cornerVerts[TILES * 3 * 6 * 11];
    Vertex point;
    //! Floorquad
    pos[0][0] = vec3(0.0f, 0.0f, 4.0f);
    pos[0][1] = vec3(0.0f, 0.0f, 0.0f);
    pos[0][2] = vec3(4.0f, 0.0f, 0.0f);
    pos[0][3] = vec3(4.0f, 0.0f, 4.0f);
    //! Walls
    pos[1][0] = vec3(0.0f,  4.0f, 0.0f);
    pos[1][1] = vec3(0.0f,  0.0f, 0.0f);
    pos[1][2] = vec3(4.0f,  0.0f, 0.0f);
    pos[1][3] = vec3(4.0f,  4.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);  
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm = vec3(0.0f, 1.0f, 0.0f);
        

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1;
    glm::vec3 tangent2;
    // Walls
    // ----------
    glm::vec3 edge1 = pos[0][1] - pos[0][0];
    glm::vec3 edge2 = pos[0][2] - pos[0][0];
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    // triangle 2
    // ----------
    edge1 = pos[0][2] - pos[0][0];
    edge2 = pos[0][3] - pos[0][0];
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);



    float fQuad[66] {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos[0][0].x, pos[0][0].y, pos[0][0].z, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, uv1.x, uv1.y,
        pos[0][1].x, pos[0][1].y, pos[0][1].z, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, uv2.x, uv2.y,
        pos[0][2].x, pos[0][2].y, pos[0][2].z, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, uv3.x, uv3.y, 

        pos[0][0].x, pos[0][0].y, pos[0][0].z, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, uv1.x, uv1.y, 
        pos[0][2].x, pos[0][2].y, pos[0][2].z, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, uv3.x, uv3.y, 
        pos[0][3].x, pos[0][3].y, pos[0][3].z, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, uv4.x, uv4.y
    };
    for (int x = 0; x < 6; x++)
    {
        floorQuad[x].Position = vec3(fQuad[(x * 11)], fQuad[(x * 11) + 1], fQuad[(x * 11) + 2]); 
        floorQuad[x].Normal = vec3(fQuad[(x * 11) + 3], fQuad[(x * 11) + 4], fQuad[(x * 11) + 5]); 
        floorQuad[x].BiNormal = vec3(fQuad[(x * 11) + 6], fQuad[(x * 11) + 7], fQuad[(x * 11) + 8]); 
        floorQuad[x].TexCoord = vec2(fQuad[(x * 11) + 9], fQuad[(x * 11) + 10]);
    }
    for (int x = 0; x < (TILES * 6); x += 6)
    {
        for (int y = 0; y < 6; y++)
        {
            vertices[ x + y ].Position = vec4ToVec3(floorPos[count] * vec4(floorQuad[y].Position, 1.0));
            vertices[ x + y ].Normal = vec4ToVec3(floorPos[count] * vec4(floorQuad[y].Normal, 1.0f));
            vertices[ x + y ].BiNormal = vec4ToVec3(floorPos[count] * vec4(floorQuad[y].BiNormal, 1.0f));
            vertices[ x + y ].TexCoord = floorQuad[y].TexCoord;
        }
        count++;
    }
    // Walls
    // ----------
    count = 0;
    edge1 = pos[1][1] - pos[1][0];
    edge2 = pos[1][2] - pos[1][0];
    deltaUV1 = uv2 - uv1;
    deltaUV2 = uv3 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    // triangle 2
    // ----------
    edge1 = pos[1][2] - pos[1][0];
    edge2 = pos[1][3] - pos[1][0];
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);


    
    float wQuad[66] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos[1][0].x, pos[1][0].y, pos[1][0].z, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, uv1.x, uv1.y,
        pos[1][1].x, pos[1][1].y, pos[1][1].z, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, uv2.x, uv2.y,
        pos[1][2].x, pos[1][2].y, pos[1][2].z, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, uv3.x, uv3.y, 

        pos[1][0].x, pos[1][0].y, pos[1][0].z, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, uv1.x, uv1.y, 
        pos[1][2].x, pos[1][2].y, pos[1][2].z, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, uv3.x, uv3.y, 
        pos[1][3].x, pos[1][3].y, pos[1][3].z, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, uv4.x, uv4.y
    };
    for (int x = 0; x < 6; x++)
    {
        wallQuad[x].Position = vec3(wQuad[(x * 11)], wQuad[(x * 11) + 1], wQuad[(x * 11) + 2]); 
        wallQuad[x].Normal = vec3(wQuad[(x * 11) + 3], wQuad[(x * 11) + 4], wQuad[(x * 11) + 5]); 
        wallQuad[x].BiNormal = vec3(wQuad[(x * 11) + 6], wQuad[(x * 11) + 7], wQuad[(x * 11) + 8]); 
        wallQuad[x].TexCoord = vec2(wQuad[(x * 11) + 9], wQuad[(x * 11) + 10]);
    }
    for (int x = 0; x < (TILES * 6); x += 6)
    {
        for (int y = 0; y < 6; y++)
        {
            vertices[(TILES * 6) + x + y].Position = vec4ToVec3(rwallPos[count] * vec4(wallQuad[y].Position, 1.0));
            vertices[(TILES * 6) + x + y].Normal = vec4ToVec3(rwallPos[count] * vec4(wallQuad[y].Normal, 1.0f));
            vertices[(TILES * 6) + x + y].BiNormal = vec4ToVec3(rwallPos[count] * vec4(wallQuad[y].BiNormal, 1.0f));
            vertices[(TILES * 6) + x + y].TexCoord = wallQuad[y].TexCoord;
        }
        count++;
    }
    count = 0;
    for (int x = 0; x < (TILES * 6); x += 6)
    {
        for (int y = 0; y < 6; y++)
        {
            vertices[(TILES * 12) + x + y].Position = vec4ToVec3(lwallPos[count] * vec4(wallQuad[y].Position, 1.0));
            vertices[(TILES * 12) + x + y].Normal = vec4ToVec3(lwallPos[count] * vec4(wallQuad[y].Normal, 1.0f));
            vertices[(TILES * 12) + x + y].BiNormal = vec4ToVec3(lwallPos[count] * vec4(wallQuad[y].BiNormal, 1.0f));
            vertices[(TILES * 12) + x + y].TexCoord = wallQuad[y].TexCoord;
        }
        count++;
    }
    for (int x = 0; x < (TILES * 3 * 6); x++)
    {
        for (int y = 0; y < 3; y++)
        {
            cornerVerts[vertCount++] = vertices[x].Position[y];
        }
        for (int y = 0; y < 3; y++)
        {
            cornerVerts[vertCount++] = vertices[x].Normal[y];
        }
        for (int y = 0; y < 3; y++)
        {
            cornerVerts[vertCount++] = vertices[x].BiNormal[y];
        }
        for (int y = 0; y < 2; y++)
        {
            cornerVerts[vertCount++] = vertices[x].TexCoord[y];
        }
        if(debug1)
        {
            
            cout << "\n\tIndex:  " << x << "\tPosition:  ";
            printVec3(vertices[x].Position);
            cout << "\tNormal:  ";
            printVec3(vertices[x].Normal);
            cout << "\tBiNormal:  ";
            printVec3(vertices[x].BiNormal);
            cout << "\tTexCoord:  ";
            printVec2(vertices[x].TexCoord);
        }
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cornerVerts), cornerVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    if (debug1)
    {
        cout << "\n\n\tFloor Quad\n";
        for (int x = 0; x < 66; x += 11)
        {
            cout << "\n\tPosition:  " << fQuad[x] << ", " << fQuad[x + 1] << ", " << fQuad[x + 2]
            << "  Normal:  " << fQuad[x + 3] << ", " << fQuad[x + 4] << ", " << fQuad[x + 5]
            << "  Tangent:  " << fQuad[x + 6] << ", " << fQuad[x + 7] << ", " << fQuad[x + 8]
            << "  Texture:  " << fQuad[x + 9] << ", " << fQuad[x + 10];
        }
    }
    if (debug1)
    {
        cout << "\n\n\tWall Quad\n";
        for (int x = 0; x < 66; x += 11)
        {
            cout << "\n\tPosition:  " << wQuad[x] << ", " << wQuad[x + 1] << ", " << wQuad[x + 2]
            << "  Normal:  " << wQuad[x + 3] << ", " << wQuad[x + 4] << ", " << wQuad[x + 5]
            << "  Tangent:  " << wQuad[x + 6] << ", " << wQuad[x + 7] << ", " << wQuad[x + 8]
            << "  Texture:  " << wQuad[x + 9] << ", " << wQuad[x + 10];
        }
    }
}

void BulletDiceGL::printMat4(mat4 matVal)
{
    cout << "  4x4 Matrix\n\t";
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            cout << matVal[y][x] << ", ";
        }
        cout << "\n\t";
    }
}

void BulletDiceGL::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void BulletDiceGL::printVec2(vec2 vecVal)
{
    cout << "  2 Float Vector:  ";
    for  (int x = 0; x < 2; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void BulletDiceGL::printQuat(quat quatVal)
{
    cout << "  Quaternion  Angle:  "
    << quatVal.w << "  Axis:  "
    << quatVal.x  << ", " << quatVal.y
    << quatVal.z;
}
    
vec3 BulletDiceGL::vec4ToVec3(vec4 data)
{
    return vec3(data.x, data.y, data.z);
}
