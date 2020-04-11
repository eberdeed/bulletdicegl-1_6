/*********************************************************************
 * *******************************************************************
 * Dice:  A dice simulation class using the Bullet Physics library.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * November 2019 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/

#include "../include/fallingbody.h"

//! Set the wall and floor positions and define the dice themselves.
FallingBody::FallingBody()
{
    cout << "\n\n\tCreating FallingBody.\n\n";
    try 
    {
        //! Set random velocity for each  die.
        btVector3 velocity[2];
        double ex = double(rand() % 2000)/ 1000.0;
        double zee =  -(double(rand() % 2000)/ 1000.0) - 4;
        velocity[0] = btVector3(ex, 0, zee);
        ex = -double(rand() % 2000)/ 1000.0;
        zee =  -(double(rand() % 2000)/ 1000.0) - 4;
        velocity[1] = btVector3(ex, 0, zee);
        //! Initialize the bullet library.
        broadphase = new btDbvtBroadphase();
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));
        dynamicsWorld->synchronizeMotionStates();
        
        //! Setup the floor.
        groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
        btDefaultMotionState* groundMotionState = new btDefaultMotionState();
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
        groundRigidBody->setRestitution(0.85);
        dynamicsWorld->addRigidBody(groundRigidBody);
        
        //! Setup the left wall.
        btVector3 normal(1, 0, 1);
        normal = normal.normalize();
        groundShape = new btStaticPlaneShape(normal, 1);
        groundMotionState = new btDefaultMotionState();
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI1(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        groundRigidBody = new btRigidBody(groundRigidBodyCI1);
        groundRigidBody->setRestitution(0.85);
        dynamicsWorld->addRigidBody(groundRigidBody);
    
        //! Setup the right wall.
        normal = btVector3(-1, 0, 1);
        normal = normal.normalize();
        groundShape = new btStaticPlaneShape(normal, 1);
        groundMotionState = new btDefaultMotionState();
        btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI2(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        groundRigidBody = new btRigidBody(groundRigidBodyCI2);
        groundRigidBody->setRestitution(0.85);
        dynamicsWorld->addRigidBody(groundRigidBody);

        //! Initial die information.
        fallShape = new btBoxShape(btVector3(3.0f, 3.0f, 3.0f));
        btScalar mass = 1.5f;
        btVector3 fallInertia(0, 0, 0);
        fallShape->calculateLocalInertia(mass, fallInertia);
        
        //! Setup die one.
        btDefaultMotionState* fallMotionState1 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-15, 30, 0)));
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState1, fallShape, fallInertia);
        fallRigidBody1 = new btRigidBody(fallRigidBodyCI);
        fallRigidBody1->setRestitution(0.85);
        fallRigidBody1->applyImpulse(velocity[0], btVector3(4, 0, -4));
        dynamicsWorld->addRigidBody(fallRigidBody1);
        
        //! Setup die two.
        btDefaultMotionState* fallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(15, 30, 0)));
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI1(mass, fallMotionState2, fallShape, fallInertia);
        fallRigidBody2 = new btRigidBody(fallRigidBodyCI1);
        fallRigidBody2->setRestitution(0.85);
        fallRigidBody2->applyImpulse(velocity[1], btVector3(-4, 0, -4));
        dynamicsWorld->addRigidBody(fallRigidBody2);
    }
    catch (exception exc)
    {
        cout << "\n\n\tBullet Library Error:  " << exc.what() << "\n";
    }

}

    
//! Delete the objects upon close.
FallingBody::~FallingBody()
{
    cout << "\n\n\tDestroying FallingBody.\n\n";
    delete fallShape;
    delete groundShape;
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

//! Calculate the tranjectory and orientation for each die.
void FallingBody::calcFall()
{
    try
    {
        dynamicsWorld->stepSimulation(0.042, 5, 0.1);
        fallRigidBody1->getMotionState()->getWorldTransform(trans1);
        fallRigidBody2->getMotionState()->getWorldTransform(trans2);
    }
    catch(exception exc)
    {
        cout << "\n\n\tError in FallingBody::calcFall():  " << exc.what() << "\n\n";
        exit(-1);
    }
}

//! Return the transform for die one.
btTransform FallingBody::retDieTrans1()
{
    return trans1;
}

//! Return the transform for die two.
btTransform FallingBody::retDieTrans2()
{
    return trans2;
}
