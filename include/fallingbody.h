/*********************************************************************
 * *******************************************************************
 * Dice:  A dice simulation class using the Bullet Physics library.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * April 2020 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/
#ifndef FALLINGBODY_H
#define FALLINGBODY_H

#include "commonheader.h"

/** \class FallingBody Sets the initial conditions of a floor, left wall and 
 * right wall. Calculates die position by iterating through the 
 * dice paths.
 !*/
class FallingBody
{
public:
    /** \brief Initialize the bullet physics library.
     * define the right and left wall and 
     * initial dice position, shape and mass.
     */
    FallingBody();
    /** \brief Echos the destruction of this class.
     */
    ~FallingBody();
    //! \brief Take one step in the action.
    void calcFall();
    //! \brief Accessor functions returning the transform of the step.
    btTransform retDieTrans1();
    btTransform retDieTrans2();
protected:
    //! Class global variables.
    //! The physical world parameters object.
    btDiscreteDynamicsWorld* dynamicsWorld;
    //! The phase coordinator object.
    btBroadphaseInterface* broadphase;
    //! The collision configuration object.
    btDefaultCollisionConfiguration* collisionConfiguration;
    //! The event dispatcher object.
    btCollisionDispatcher* dispatcher;
    //! The event constraint solver object.
    btSequentialImpulseConstraintSolver* solver;
    //! The object shape object for the ground.
    btCollisionShape* groundShape;
    //! The dice shape object.
    btCollisionShape* fallShape;
    //! The definition of the dice as rigid bodies.
    btRigidBody *fallRigidBody1, *fallRigidBody2;
    /** The transform objects for the dice 
     * holding position and orientation information.
     */
    btTransform trans1, trans2;
};

#endif // FALLINGBODY_H
