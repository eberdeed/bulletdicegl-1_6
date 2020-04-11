/*********************************************************************
 * *******************************************************************
 * DiceRoll: A class to display the rolling of a pair of dice.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * November 2019 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/

#include "../include/diceroll.h"

DiceRoll::DiceRoll(vec3 initPos)
{
    cout << "\n\n\tCreating DiceRoll.\n\n";
    //! Initial position.
    dieposition[0] = startPos[0] + vec3(10.0f, 0.0f, 40.0f);
    dieposition[1] = startPos[1] + vec3(-10.0f, 0.0f, 40.0f);
    oldposition[0] = vec3(0.0f);
    oldposition[1] = vec3(0.0f);
    newAngle = new bool[2];
    deltaCount[0] = deltaCount[1] = 0;
    cameraPos = initPos;
    countFrames = 0;
    this->initPos = initPos;
    endroll = false;
    startanim = false;
    animcount = 0;
    dicePhys = new FallingBody();
}

DiceRoll::~DiceRoll()
{
    cout << "\n\n\tDestroying DiceRoll.\n\n";
    delete dicePhys;
}
//! Report end of roll status.
bool DiceRoll::getEndRoll()
{
    return endroll;
}
//! Report start of roll status.
bool DiceRoll::getStartAnim()
{
    return startanim;
}
//! Position of die 1.
vec3 DiceRoll::getDie1()
{
    return dieposition[0];
}
//! Position of die 2.
vec3 DiceRoll::getDie2()
{
    return dieposition[1];
}
vec3 DiceRoll::getCameraPos()
{
    if (debug1)
    {
        cout <<  "\n\tgetCameraPos() Camera Position:  ";
        printVec3(cameraPos);
    }
    return cameraPos;
}
//! Have the position of the camera provided.
void DiceRoll::setCameraPos(vec3 camPos)
{
    if (debug1)
    {
        cout <<  "\n\tsetCameraPos() 1 Camera Position:  ";
        printVec3(cameraPos);
    }
    cameraPos = camPos;
    if (debug1)
    {
        cout <<  "\n\tsetCameraPos() 2 Camera Position:  ";
        printVec3(cameraPos);
    }
}
bool *DiceRoll::getNewAngle()
{
    return newAngle;
}
//! Manages dice position and beginning and ending
//  of roles.
void DiceRoll::diceEvents()
{
    if (debug1)
    {
        cout << "\n\tDice Position 1 ";
        printVec3(dieposition[0]);
        cout << "  Dice Position 2 ";
        printVec3(dieposition[1]);
        cout << " endroll:  " << endroll
        << " startanim:  " << startanim
        << " animcount:  " << animcount;
    }
    try
    {
        //! End of roll, center dice and camera animation.
        if (endroll)
        {
            if (animcount < 50)
            {
                if (debug1)
                {
                    cout <<  "\n\tEnd Roll 1: Camera Position:  ";
                    printVec3(cameraPos);
                    cout << "  Translator Vector:  ";
                    printVec3(translator[0]);
                }
                cameraPos += translator[0];
                dieposition[0] += translator[1];
                dieposition[1] += translator[2];
                if (distance(dieposition[0], dieposition[1]) < 6.5f)
                {
                    float adjust = (7.0f - abs(dieposition[1].z - dieposition[0].z))/2.0f;
                    dieposition[1].z += adjust;
                    if (((dieposition[0].z < 0) && (dieposition[1].z > 0)) || ((dieposition[0].z > 0) && (dieposition[1].z < 0)))
                    {
                        dieposition[0].z += adjust;
                    }
                    else
                    {
                        dieposition[0].z -= adjust;
                    }
                    moveAdjust = true;
                }
                else if(moveAdjust)
                {
                    moveAdjust = false;
                    recenterEndDice(animcount);
                }
                animcount++;
            }
            if (debug1)
            {
                cout <<  "\n\tEnd Roll 2: Camera Position:  ";
                printVec3(cameraPos);
                cout << "  Translator Vector:  ";
                printVec3(translator[0]);
            }
            return;
        }
        //! Start of roll, move the camera back animation.
        if (startanim)
        {
            if (animcount < 50)
            {
                if (debug1)
                {
                    cout <<  "\n\tStart Animation : Camera Position:  ";
                    printVec3(cameraPos);
                    cout << "  Translator Vector:  ";
                    printVec3(translator[0]);
                }
                cameraPos += translator[0];
                dieposition[0] += translator[1];
                dieposition[1] += translator[2];
                if (distance(dieposition[0], dieposition[1]) < 6.5f)
                {
                    float adjust = (7.0f - abs(dieposition[1].z - dieposition[0].z))/2.0f;
                    dieposition[1].z += adjust;
                    if (((dieposition[0].z < 0) && (dieposition[1].z > 0)) || ((dieposition[0].z > 0) && (dieposition[1].z < 0)))
                    {
                        dieposition[0].z += adjust;
                    }
                    else
                    {
                        dieposition[0].z -= adjust;
                    }
                    moveAdjust = true;
                }
                else if(moveAdjust)
                {
                    moveAdjust = false;
                    recenterStartDice(animcount);
                }
                animcount++;
                return;
            }
            else
            {
                startanim = false;
                animcount = 0;
            }
            return;
        }
        //! Calculate the next postion.
        dicePhys->calcFall();
        //! Get the dice bullet physics transform location information.
        die1trans = dicePhys->retDieTrans1();
        die2trans = dicePhys->retDieTrans2();
        //! Reduce the bullet physics transform to a vector.
        die1pos = die1trans.getOrigin();
        die2pos = die2trans.getOrigin();
        //! Adjust for dice model height and die standing on one corner.
        die1pos.setY(1.75 + die1pos.y());
        die2pos.setY(1.75 + die2pos.y());
        //! Convert the bullet vector to a glm vec3.
        dieposition[0] = bullet2Vec3(die1pos);
        dieposition[1] = bullet2Vec3(die2pos);
        if (((!newAngle[0]) && (deltaCount[0] > 50)) && (((dieposition[0].x - oldposition[0].x) > 0) && (deltax[0] < 0)) ||
        (((dieposition[0].x - oldposition[0].x) < 0) && (deltax[0] > 0)) && (dieposition[0].y > 2.0))
        {
            newAngle[0] = true;
            deltaCount[0] = 0;
        }
        else
        {
            deltaCount[0]++;
            newAngle[0] = false;
        }
        if (((!newAngle[1]) && (deltaCount[1] > 50)) && (((dieposition[1].x - oldposition[1].x) > 0) && (deltax[1] < 0)) ||
        (((dieposition[1].x - oldposition[1].x) < 0) && (deltax[1] > 0)) && (dieposition[1].y > 2.0))
        {
            newAngle[1] = true;
            deltaCount[1] = 0;
        }
        else
        {
            newAngle[1] = false;
            deltaCount[1]++;
        }
        //! Check to see if the dice are at rest.
        countFrames++;
        if (((!newAngle[1]) && (deltaCount[1] > 50)) && (countFrames > 50) && 
        ((!newAngle[1]) && (deltaCount[1] > 50)) && 
        (vec3Equal(dieposition[0], oldposition[0])) && 
        (vec3Equal(dieposition[1], oldposition[1]))) 
        {
            cout << "\n\n\tEnded!\n\n";
            //! Setup the final animation.
            finalPosition();
            countFrames = 0;
            return;
        }
        deltax[0] = dieposition[0].x - oldposition[0].x;
        deltax[1] = dieposition[1].x - oldposition[1].x;
        oldposition[0] = dieposition[0];
        oldposition[1] = dieposition[1];
    }
    catch(exception exc)
    {
        cout << "\n\n\tError in DiceRoll::diceEvents():  " << exc.what() << "\n\n";
        exit(-1);
    }
}


void DiceRoll::resetDice()
{
    //! Center everything.
    endroll = false;
    startanim = true;
    delete dicePhys;
    dicePhys =  new FallingBody();
   for (int x = 0; x < 3; x++)
    {
        translator[x] = vec3(0, 0, 0);
    }
    double diedist;
    animcount = 0;
    //! Center camera.
    if (!vec3Equal(initPos, cameraPos))
    {
        animdir =  initPos - cameraPos;
        animdir = normalize(animdir);
        animdist = distance(initPos, cameraPos);
        step = animdist / 50.0f;
        translator[0] = step * animdir;
    }
    //! Center die 1.
    if (!vec3Equal(dieposition[0], startPos[0]))
    {
        diedist = distance(startPos[0], dieposition[0]);
        step = double(diedist) / 50.0f;
        position = startPos[0] - dieposition[0];
        position = normalize(position);
        translator[1] = step * position;
    }
    //! Center die 2.
    if (!vec3Equal(dieposition[1], startPos[1]))
    {
        diedist = distance(startPos[1], dieposition[1]);
        step = double(diedist) / 50.0f;
        position = startPos[1] - dieposition[1];
        position = normalize(position);
        translator[2] = step * position;
    }
    if (debug1)
    {
        cout <<  "\n\tReset Dice : Camera Position:  ";
        printVec3(cameraPos);
        cout << "  Initial Position:  ";
        printVec3(initPos);
        cout << "  Translator Vector:  ";
        printVec3(translator[0]);
    }
}

void DiceRoll::recenterEndDice(int count)
{
    double diedist;
    //! Center die 1.
    if (!vec3Equal(dieposition[0], endPos[0]))
    {
        diedist = distance(endPos[0], dieposition[0]);
        step = double(diedist) / (50.0f - (float) count);
        position = endPos[0] - dieposition[0];
        position = normalize(position);
        translator[1] = step * position;
    }
    //! Center die 2.
    if (!vec3Equal(dieposition[1], endPos[1]))
    {
        diedist = distance(endPos[1], dieposition[1]);
        step = double(diedist) / (50.0f - (float) count);
        position = endPos[1] - dieposition[1];
        position = normalize(position);
        translator[2] = step * position;
    }
}

void DiceRoll::recenterStartDice(int count)
{
    double diedist;
    //! Center die 1.
    if (!vec3Equal(dieposition[0], startPos[0]))
    {
        diedist = distance(startPos[0], dieposition[0]);
        step = double(diedist) / (50.0f - (float) count);
        position = startPos[0] - dieposition[0];
        position = normalize(position);
        translator[1] = step * position;
    }
    //! Center die 2.
    if (!vec3Equal(dieposition[1], startPos[1]))
    {
        diedist = distance(startPos[1], dieposition[1]);
        step = double(diedist) / (50.0f - (float) count);
        position = startPos[1] - dieposition[1];
        position = normalize(position);
        translator[2] = step * position;
    }
}

//! Vector conversion Bullet to glm.
vec3 DiceRoll::bullet2Vec3(btVector3 invec)
{
    vec3 outvec;
    outvec.x = float(invec.getX());
    outvec.y = float(invec.getY());
    outvec.z = float(invec.getZ());
    return outvec;
}

//! Set up the final animation.
void DiceRoll::finalPosition()
{
    double diedist;
    // Camera position.
    animcount = 0;
    endroll = true;
    countFrames = 0;
    animdir =  finalPos - cameraPos;
    animdir = normalize(animdir);
    animdist = distance(finalPos, cameraPos);
    step = animdist / 50.0f;
    translator[0] = step * animdir;
    // Die 1 position.
    diedist = distance(endPos[0], dieposition[0]);
    step = double(diedist) / 50.0f;
    position = endPos[0] - dieposition[0];
    position = normalize(position);
    translator[1] = step * position;
    // Die 2 position.
    diedist = distance(endPos[1], dieposition[1]);
    step = double(diedist) / 50.0f;
    position = endPos[1] - dieposition[1];
    position = normalize(position);
    translator[2] = step * position;
    cout << "\n\n\tIn finalPos() animcount:  " << animcount << "\n\n";
}
//! Lazily check for equality of two glm vec3s.
bool DiceRoll::vec3Equal(vec3 vector1, vec3 vector2)
{
    bool equals = true;
    equals = (abs(vector1.x - vector2.x) < 0.01);
    if (equals)
    {
         equals = (abs(vector1.y - vector2.y) < 0.01);
         if (equals)
         {
             equals = (abs(vector1.z - vector2.z) < 0.01);
         }
    }
    return equals;
}
void DiceRoll::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}


