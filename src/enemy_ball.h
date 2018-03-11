#include "main.h"

#ifndef ENEMY_BALL_H
#define ENEMY_BALL_H


class Enemy_Ball {
public:
    Enemy_Ball() {}
    Enemy_Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void disappear();
    float speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // ENEMY_BALL_H

