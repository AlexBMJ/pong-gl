#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H
#include "shapes.h"
#include <cglm/cglm.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct Object {
    VertexObject* vertobj;
    float xpos, ypos, rot;
} Object;

typedef struct MovableObject {
    VertexObject* vertobj;
    float xpos, ypos, rot, xvel, yvel, rvel;
} MovableObject;

typedef struct Player {
    VertexObject* vertobj;
    float xpos, ypos, rot, xvel, yvel, rvel;
    int score;
    const float width, height;
} Player;

typedef struct Ball {
    VertexObject* vertobj;
    float xpos, ypos, rot, xvel, yvel, rvel;
    const float size;
} Ball;

typedef struct Scoreboard {
    int player1;
    int player2;
    unsigned long time;
} Scoreboard;

Player* mkPlayer(float x, float y, float width, float height, vec3 color) {
    Player player_init = {.width = width, .height = height};
    Player* player = malloc(sizeof(Player));
    if (player == NULL) abort();
    memcpy(player, &player_init, sizeof *player);

    player->vertobj = colorRect(width, height, color);
    player->xpos = x;
    player->ypos = y;
    player->xvel = 0;
    player->yvel = 0;
    player->score = 0;
    player->rot = 0;
    player->rvel = 0;

    return player;
}

void player_update(Player* p) {
    // movement limit
    if (p->ypos+p->yvel > 1.0f-(p->height)) {
        p->yvel = 0.0f;
        p->ypos = 1.0f-(p->height);
        return;
    }
    if (p->ypos+p->yvel < -1.0f+(p->height)) {
        p->yvel = 0.0f;
        p->ypos = -1.0f+(p->height);
        return;
    }
    p->ypos += p->yvel;
}

Ball* mkBall(float x, float y, float xv, float yv, float size, vec3 color) {
    Ball ball_init = {.size = size};
    Ball* ball = malloc(sizeof(Ball));
    if (ball == NULL) abort();
    memcpy(ball, &ball_init, sizeof *ball);

    ball->vertobj = colorRect(size, size, color);
    ball->xpos = x;
    ball->ypos = y;
    ball->xvel = xv;
    ball->yvel = yv;
    ball->rot = 0;
    ball->rvel = 0;

    return ball;
}

void ball_update(Ball* b, Player* p1, Player* p2) {
    // point to player
    if (b->xpos+b->xvel > p2->xpos) {
        b->xpos = 0.0f;
        b->ypos = 0.0f;
        b->rot = 0.0f;
        p1->score += 1;
        b->yvel = sinf(rand())/100.0f;
        b->xvel = copysignf(0.01f, -b->xvel);
        b->rvel = 0.0f;
        return;
    }
    if (b->xpos+b->xvel < p1->xpos) {
        b->xpos = 0.0f;
        b->ypos = 0.0f;
        b->rot = 0.0f;
        p2->score += 1;
        b->yvel = sinf(rand())/100.0f;
        b->xvel = copysignf(0.01f, -b->xvel);
        b->rvel = 0.0f;
        return;
    }

    // wall bounce
    if (b->ypos+b->yvel > 1.0f-(b->size)) {
        b->yvel *= -1.0f;
        b->ypos = 1.0f-(b->size);
        return;
    }
    if (b->ypos+b->yvel < -1.0f+(b->size)) {
        b->yvel *= -1.0f;
        b->ypos = -1.0f+(b->size);
        return;
    }

    float damp = 5.0f;
    // player bounce
    if (b->xpos+b->xvel > p2->xpos-(b->size)-(p2->width) &&
        b->ypos < p2->ypos+p2->height+b->size &&
        b->ypos > p2->ypos-p2->height-b->size) {
        b->xvel *= -1.0f;
        if (b->yvel+p2->yvel/damp < 0.2f || b->yvel+p2->yvel/damp > -0.2f)
            b->yvel += p2->yvel/damp;
        if (b->rvel+p2->yvel < 0.8f || b->rvel+p2->yvel > -0.8f)
            b->rvel += p2->yvel;
        return;
    }
    if (b->xpos+b->xvel < p1->xpos+(b->size)+(p1->width) &&
        b->ypos < p1->ypos+p1->height+b->size &&
        b->ypos > p1->ypos-p1->height-b->size) {
        b->xvel *= -1.0f;
        if (b->yvel+p1->yvel/damp < 0.2f || b->yvel+p1->yvel/damp > -0.2f)
            b->yvel += p1->yvel/damp;
        if (b->rvel+p1->yvel < 0.8f || b->rvel+p1->yvel > -0.8f)
            b->rvel += -p1->yvel;
        return;
    }

    b->ypos += b->yvel;
    b->xpos += b->xvel;
    b->rot  += b->rvel;
}


#endif