#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(int* key_pressed, int m_count, int flag);
    void disappear();
    void jump();
    void descent(float init_y);
    float speed;
    float speedx;
    float acclr;
    float g;
    float trap_jump;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

// Floor class
class Floor {
public:
    Floor() {}
    Floor(float x, float y,color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

// trampoline class
class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

// class magnet
class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

//class porcupine
class Porcupine {
public:
    Porcupine() {}
    Porcupine(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
    float speed;
    float init_x;
private:
    VAO *object;
};

//class pond
class Pond {
public:
    Pond() {}
    Pond(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    float radius;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

//class Plank
class Plank {
public:
    Plank() {}
    Plank(float x, float y, float length, float theta, float speed, color_t color);
    glm::vec3 position;
    float speed;
    float rotation;
    float length;
    float angle;
    void tick();
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H


