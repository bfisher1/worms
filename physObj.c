#include "physObj.h"
#include <math.h>
#include "util.h"
/** Fraction of the box's side that is read to determine a collision. */
#define BORDER_FRACTION 5.0
#define MOVE_TOO_FAR 100
typedef enum {
    noCollision,
    leftCollision,
    rightCollision,
    topCollision,
    bottomCollision
} Collision;

PhysObj *createPhysObj(float x, float y, Box *frame, float velocity, float direction) {
    PhysObj *obj = (PhysObj *) malloc(sizeof(PhysObj));
    obj->x = x;
    obj->y = y;
    obj->velocity = velocity;
    obj->direction = direction;
    obj->frame = frame;
    obj->yvelocity = 0;
    obj->rotation = 0.0;
    return obj;
}

void fall(PhysObj *obj, float rate, float maxVelocity) {
    if(obj->yvelocity < maxVelocity){
        obj->yvelocity += rate;
    }
    //accel(obj, PI / 2.0, rate, maxVelocity);
}

void accel(PhysObj *obj, float direction, float rate, float maxVelocity) {
    float vx, vy;
    vx = obj->velocity * cos(obj->direction) + rate * cos(direction);
    vy = obj->velocity * sin(obj->direction) + rate * sin(direction);
    obj->velocity = dist(0, 0, vx, vy);
    if(obj->velocity > maxVelocity) {
        obj->velocity = maxVelocity;
    }
    obj->direction = atan2(vy, vx);
}

void decel(PhysObj *obj, float rate) {
    float vx, vy;
    vx = obj->velocity * cos(obj->direction);
    vy = obj->velocity * sin(obj->direction);
    if(abs(vx) < rate) {
        vx = 0;
    }
    else if(vx > 0) {
        vx -= rate;
    }
    else if(vx < 0) {
        vx += rate;
    }
    
    obj->velocity = dist(0, 0, vx, vy);
    obj->direction = atan2(vy, vx);
    
}

bool isColliding(PhysObj *obj, Level *level, bool *leftCol, bool *rightCol, bool *topCol, bool *bottomCol) {
    int xstart, ystart, xmax, ymax, x, y;

    int border = obj->frame->width / BORDER_FRACTION;

    *topCol = false;
    *bottomCol = false;
    *leftCol = false;
    *rightCol = false;
    
    xstart = obj->x + obj->frame->x - obj->frame->width / 2.0;
    xmax = obj->x + obj->frame->x + obj->frame->width / 2.0;
    ystart = obj->y + obj->frame->y - obj->frame->height / 2.0;
    ymax = obj->y + obj->frame->y + obj->frame->height / 2.0;
    y = ystart;
    for(x = xstart + border; x < xmax - border; x++ ){
        if(groundAt(level, x, y)) {
            *bottomCol = true;
            break;
        }
    }
    y = ymax;
    for(x = xstart + border; x < xmax - border; x++ ){
        if(groundAt(level, x, y)) {
            *topCol = true;
            break;
        }
    }
    x = xstart;
    for(y = ystart + border; y < ymax - border; y++ ){
        if(groundAt(level, x, y)) {
            *leftCol = true;
            break;
        }
    }
    x = xmax;
    for(y = ystart + border; y < ymax - border; y++ ){
        if(groundAt(level, x, y)) {
            *rightCol = true;
            break;
        }
    }

    /*
    if(xstart < 0) {
        xstart = 0;
    }
    if(xmax > level->width) {
        xstart = 0;
    }
    if(ystart < 0) {
        ystart = 0;
    }
    if(ymax > level->height) {
        ystart = 0;
    }
    */
    
    
    return *topCol || *bottomCol || *leftCol || *rightCol;
}

bool onGround(PhysObj *obj, Level *level) {
    bool leftCol, rightCol,topCol, bottomCol;
    isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
    return bottomCol;
}

bool wallLeft(PhysObj *obj, Level *level) {
    bool leftCol, rightCol,topCol, bottomCol;
    isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
    return leftCol;
}

bool wallRight(PhysObj *obj, Level *level) {
    bool leftCol, rightCol,topCol, bottomCol;
    isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
    return rightCol;
}

bool wallAbove(PhysObj *obj, Level *level) {
    bool leftCol, rightCol,topCol, bottomCol;
    isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
    return topCol;
}

void moveToNearestOpenLocation(PhysObj *obj, Level *level) {
    float startx, starty, x, y, dx, dy, px, py, nx, ny, angle, newx, newy, r;
    bool leftCol, rightCol, topCol, bottomCol;
    startx = obj->x;
    starty = obj->y;
    px = 0;
    py = 0;
    nx = 0;
    ny = 0;
    x = 0;
    y = 0;
    dx = 0;
    dy = 1;
    r = 2;
    while(x < 2 * obj->frame->width) {
        
        if(!isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol)){
            newx = startx + x;
            newy = starty + y;
            angle = atan2(newy - starty, newx - startx) + PI;
            obj->x = newx + r * cos(angle);
            obj->y = newy + r * sin(angle);
            return;
        }
        y -= dy;
        x += dx;
        obj->x = x + startx;
        obj->y = y + starty;
        if(x > nx) {
            dx = 0;
            dy = 1;
            nx++;
        }
        if(x < px) {
            dx = 0;
            dy = -1;
            px--;
        }
        if(y > ny) {
            dy = 0;
            dx = 1;
            ny++;
        }
        if(y < py) {
            dy = 0;
            dx = -1;
            py--;
        }
    }
}
void ghostMove(PhysObj *obj) {
    float vx, vy;
    vx = obj->velocity * cos(obj->direction);
    vy = obj->velocity * sin(obj->direction);
    obj->x += vx;
    obj->y += vy;
}

void move(PhysObj *obj, Level *level, float bounce) {
    int startx, starty, startv;
    float vx, vy;
    bool leftCol, rightCol, topCol, bottomCol;
    //xprev = obj->x;
    //yprev = obj->y;
    startx = obj->x;
    starty = obj->y;
    vx = obj->velocity * cos(obj->direction);
    vy = obj->velocity * sin(obj->direction);
    obj->x += vx;
    obj->y += vy;
    startv = obj->velocity;
    /*
    if(isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol)) {
        moveToNearestOpenLocation(obj, level);
    }
    
    return;
    */
    if(isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol)) {
        //obj->x = xprev;
        //obj->y = yprev;
        //move until free
        
        obj->velocity = bounce * dist(0, 0, vx, vy);
        if(obj->y < 0 || obj->x < 0 || obj->y >= level->height || obj->x >= level->width) {
            return;
        }
        int de = 0;
        int moved = 0;
        if(topCol && !bottomCol) {
            obj->velocity = dist(0, 0, vx, bounce * vy);
            obj->direction = atan2(abs(vy), vx);
            
            while(topCol) {
                obj->y-= 1;
                isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
                moved++;
            } 
            if(moved){
                obj->y++;
            }
            //obj->velocity = 0;
            obj->y += de;
        }
        else if(bottomCol) {
            obj->velocity = dist(0, 0, vx, bounce * vy);
            obj->direction = atan2(-abs(vy), vx);
            while(bottomCol) {
                obj->y+= 1;
                isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
                moved++;
            } 
        }
        if(leftCol) {
            obj->velocity = dist(0, 0, bounce * vx, vy);
            obj->direction = atan2(vy, abs(vx));
            while(leftCol) {
                //printf("MOVING RIGHT>>> x %d y %d \n", (int) obj->x, (int) obj->y);
                obj->x += 1;
                isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
                moved++;
            } 
            if(moved){
                obj->x--;
            }
        }
        if(rightCol) {
            obj->velocity = dist(0, 0, bounce * vx, vy);
            obj->direction = atan2(vy, -abs(vx));
            bool moved = false;
            while(rightCol) {
                obj->x-= 1;
                isColliding(obj, level, &leftCol, &rightCol, &topCol, &bottomCol);
                moved++;
            } 
            if(moved){
                obj->x++;
            }
        }
        if(moved >= MOVE_TOO_FAR) {
            obj->x = startx;
            obj->y = starty;
            obj->velocity = startv / 2;
        } else {
            obj->direction += PI;
            obj->velocity *= bounce;
        }
        
        
    }
}
#include <SDL/SDL.h>
#include "graphics.h"
void tilt(PhysObj *obj, Level *level, float amount, float max, SDL_Surface *screen) {
    //left and right corners, polar coordinates, and center coordinates
    float lcx, lcy, rcx, rcy, rad, dir, cx, cy;
    bool leftCorner, rightCorner;
    cx = obj->x + obj->frame->x;
    cy = obj->y + obj->frame->y;
    
    lcx = cx - obj->frame->width / 2.0;
    lcy = cy + obj->frame->height / 2.0;
    rcx = cx + obj->frame->width / 2.0;
    rcy = lcy;
    rad = dist(cx, cy, lcx, lcy);
    dir = atan2(lcy - cy, lcx - cx) + obj->rotation;
    lcx = obj->x + obj->frame->x + rad * cos(dir);
    lcy = obj->y + obj->frame->y + rad * sin(dir);
    rad = dist(cx, cy, rcx, rcy);
    dir = atan2(rcy - cy, rcx - cx) + obj->rotation;
    rcx = obj->x + obj->frame->x + rad * cos(dir);
    rcy = obj->y + obj->frame->y + rad * sin(dir);
    leftCorner = groundAt(level, lcx, lcy);
    rightCorner = groundAt(level, rcx, rcy);
    /*
    Color green = {0, 255, 0};
    Color blue = {0, 0, 255};
    for(int i = -2; i < 3; i++){
        for(int j = -2; j < 3; j++){
            plotpix(screen, lcx + i, lcy + j, green );
            plotpix(screen, rcx + i, rcy + j, green );
            plotpix(screen, 100 + i, 100 + j, green );
            plotpix(screen, cx + i, cy + j, blue );
        }
    }
    */
    

    if(leftCorner) {
        if(!rightCorner && obj->rotation < max) {
            obj->rotation += amount;
            return;
        }
    } else if(rightCorner) {
        if(!leftCorner  && obj->rotation > -max) {
            obj->rotation -= amount;
            return;
        }
    } else {
        /*
        if(abs(obj->rotation) <= amount) {
            obj->rotation = 0;
        }
        else if(obj->rotation > 0) {
            obj->rotation -= amount;
        }
        else if(obj->rotation < 0) {
            obj->rotation += amount;
        }
        */
        
    }

}

void freePhysObj(PhysObj *obj) {
    free(obj->frame);
    free(obj);
}

void moveDown(PhysObj *obj, Level *level, float amount) {
    obj->y += amount;
    bool moved = false;
    while(onGround(obj, level)) {
        obj->y--;
        moved = true;
    }
    if(moved) {
        obj->y++;
    }
}

void moveLeft(PhysObj *obj, Level *level, float amount) {
    obj->x -= amount;
    bool moved = false;
    while(wallLeft(obj, level)) {
        obj->x++;
        moved = true;
    }
    if(moved) {
        obj->x--;
        printf("a\n");
    }
}

void moveRight(PhysObj *obj, Level *level, float amount) {
    obj->x += amount;
    while(wallRight(obj, level)) {
        obj->x--;
    }
}
void moveVert(PhysObj *obj, Level *level) {
    moveDown(obj, level, obj->yvelocity);
}
void jump(PhysObj *obj, Level *level, float amount) {
    if(onGround(obj, level)){
        obj->yvelocity = -amount;
    }
}

bool areObjectsColliding(PhysObj *obj1, PhysObj *obj2) {
    int dx = abs(obj1->x - obj2->x);
    int dy = abs(obj1->y - obj2->y);
    int widthSum = obj1->frame->width / 2 + obj2->frame->width / 2;
    int heightSum = obj1->frame->height / 2 + obj2->frame->height / 2;
    return dx <= widthSum && dy <= heightSum;
}