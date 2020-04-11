/*********************************************************************
 * *******************************************************************
 * DiceRoll: A class to manage the rolling of a pair of dice.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * November 2019 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/

#ifndef DICEROLL_H
#define DICEROLL_H

#include "commonheader.h"
#include "fallingbody.h"
/** \class DiceRoll The class Dice Roll manages the position of 
 *  the dice during the role, and the animations that occur at 
 *  the end and beginning of a role.  These animations slide the 
 *  camera and dice into their new positions.  This class calls 
 *  the FallingBody class to calculate the locations of the dice.
 */
class DiceRoll
{
public:
    /** \brief The constructor, setting all the dice positions.
     * Set the initial dice and camera positions
     * and call execLoop to roll the dice.
     */
    DiceRoll(vec3 initPos);
    ~DiceRoll();
    /** \brief Dice events moves the camera away to throw the dice, and 
     *  towards the dice, to read the dice at the end of the throw.  In 
     *  between, it monitors the dice which move according to the 
     *  dictates of the FallingBody class.
     */ 
    void diceEvents();
    /** \brief Starts the roll of the dice, causing the FallingBody class 
     * to be reinstantiated and setting the dice in motion.
     */
    void resetDice();
    //! \brief Return the position of die 1.
    vec3 getDie1();
    //! \brief Return the position of die 2.
    vec3 getDie2();
    //! \brief Accessor function returning the end of roll status.
    bool getEndRoll();
    //! \brief Accessor function returning the start of roll status.
    bool getStartAnim();
    //! \brief Report the current camera position.
    vec3 getCameraPos();
    //! \brief Set the current camera position.
    void setCameraPos(vec3 camPos);
    //! \brief Accessor function indicating bumping against a wall.
    bool *getNewAngle();
protected:
    //! \brief Convert the Bullet vector to a glm vector.
    vec3 bullet2Vec3(btVector3 invec);
    //! \brief Compare glm vectors with a minimum of accuracy.
    bool vec3Equal(vec3 vector1, vec3 vector2);
    /** \brief Setup the animation to move the dice and the camera
     * to the center.
     */
    void finalPosition();
    /** \brief If the dice bump each other during the end of 
     * roll animation this function recalculates the animation.
     */
    void recenterEndDice(int count);
    /** \brief If the dice bump each other during the start of 
     * roll animation this function recalculates the animation.
     */
    void recenterStartDice(int count);
    //! \brief Debug function to print vector data.
    void printVec3(vec3 vecVal);
    //! Holds the dice locations.
    vec3 dieposition[2];
    //! Used to determine when the dice are at rest (dieposition = oldposition).
    vec3 oldposition[2];
    /** Wall proximity information.  The count is instituted to 
     * keep the calculation from needlessly repeating itself.
     */
    int deltax[2], deltaCount[2];
    /** The wall bump booleans and the debug flag.
     *  Beware debug will produce copious data.
     */
    bool *newAngle, debug1 = false;
    /** Flag to note that a bump dice is going on during an
     * animation.
     */
    bool moveAdjust = false;
    /** Camera location vector.
     */
    vec3 cameraPos, position;
    //! The velocity vectors for the dice.
    btVector3 velocity[2];
    /** The initial position of the camera passed
     * to the constructor.
     */
    vec3 initPos;
    /** The end of roll position for the camera, so 
     * you can read the dice.
     */
    const vec3 finalPos = vec3(0.0f, 40.0f, 40.0f);
    /** The final position of the dice.
     */
    const vec3 endPos[2] 
    {
        vec3(7.0f, 3.0f, 0.0f),
        vec3(-7.0f, 3.0f, 0.0f)
    };
    /** The start position of the dice.
     */
    const vec3 startPos[2]
    {
        vec3(15.0f, 30.0f, 0.0f),
        vec3(-15.0f, 30.0f, 0.0f)
    };
    //! Used for flagging the camera and dice centering animations.
    bool endroll, startanim;
    //! Bullet transform variables which contain 
    //! A vector position and a quaternion orientation.
    btTransform die1trans, die2trans;
    //! The aforesaid vector positions.
    btVector3 die1pos, die2pos;
    /** Used to determine the location of the camera and dice during
     * the various animations.
     */
    vec3 animdir, startpos, translator[3];
    //! Increments for the animations.
    float animdist, step;
    //! Animation counter.
    int animcount;
    //! The pointer to the Bullet Physics class.
    FallingBody *dicePhys;
    //! Make sure that the wall bump only occurs once.
    int countFrames;
};

#endif //DiceRoll.h
