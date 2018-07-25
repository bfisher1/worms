/**
    @file physObj.h
    @author Ben Fisher

    Defines physical object and box structures. Defines functions
    for dealing with the motion of physical objects.
*/
#ifndef PHYS_OBJ_H
#define PHYS_OBJ_H
#include "level.h"

/**
    Structure of a box. A box is used as a frame around the physical
    object for collision detection.
*/
typedef struct {
    int x;
    int y;
    int width;
    int height;
} Box;

/**
    Structure for a physical object. Contains a center at (x,y),
    a frame around it, a velocity, and a direction.
*/
typedef struct {
    float x;
    float y;
    Box *frame;
    float velocity;
    float direction;
    float rotation;
    float yvelocity;
    
} PhysObj;

/**
    Creates a physical object at (x,y) with the given velocity and
    direction. The frame box is also given.

    @param x the x coordinate of the object
    @param y the y coordinate of the object
    @param frame the object's frame
    @param velocity the object's start velocity
    @param direction the object's start direction
*/
PhysObj *createPhysObj(float x, float y, Box *frame, float velocity, float direction);

/**
    Causes the object to fall at the given rate. Stops falling if its
    hit the max velocity.

    @param obj the object that is falling
    @param rate the rate the obj falls at
    @param maxVelocity the max downward velocity the object can have
*/
void fall(PhysObj *obj, float rate, float maxVelocity);

/**
    Accelerate the object at a given direction and rate. It cannot
    exceed maxVelocity in velocity.

    @param obj the object that is accelerating
    @param direction the direction the object is accelerating
    @param maxVelocity the highest the velocity's magnitude can be
*/
void accel(PhysObj *obj, float direction, float rate, float maxVelocity);

/**
    Decelerates the object at the given rate.
    
        @param obj the object that is decelerating
        @param rate the rate the object decelerates at
*/
void decel(PhysObj *obj, float rate);

/**
    Moves the object in the given level. If it hits the wall or
    ground, it is pushed back out in the opposite angle that it
    entered. The amount it bounces back is determined by bounce.
    Bounce should be between 0 and 1.

    @param obj the object that is moving
    @param level the level the object is moving in
    @param bounce the fraction of velocity that the object will
           bounce by when it hits a wall or the ground
*/
void move(PhysObj *obj, Level *level, float bounce);

void ghostMove(PhysObj *obj);

/**
    Frees an object from memory.
    
    @param obj the object being freed
*/
void freePhysObj(PhysObj *obj);

void moveLeft(PhysObj *obj, Level *level, float amount);

void moveRight(PhysObj *obj, Level *level, float amount);

bool onGround(PhysObj *obj, Level *level);

bool wallLeft(PhysObj *obj, Level *level);

bool wallRight(PhysObj *obj, Level *level);

bool wallAbove(PhysObj *obj, Level *level);

void moveDown(PhysObj *obj, Level *level, float amount);

void moveVert(PhysObj *obj, Level *level);

void jump(PhysObj *obj, Level *level, float amount);

bool isColliding(PhysObj *obj, Level *level, bool *leftCol, bool *rightCol, bool *topCol, bool *bottomCol);

void tilt(PhysObj *obj, Level *level, float amount, float max, SDL_Surface *screen);

bool areObjectsColliding(PhysObj *obj1, PhysObj *obj2);

#endif /* PHYS_OBJ_H */