#include "enemy_ball.h"
#include "main.h"

Enemy_Ball::Enemy_Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = ((rand()%5) + 1)/100.0;
    static GLfloat vertex_buffer_data[10000];
    int n = 100;
    float theta = 0;
    int j = 0;
    this->radius = ((rand()%5) + 3)/10.0;
    while(theta < 2*3.14) {
            vertex_buffer_data[j++] = 0.0f;
            vertex_buffer_data[j++] = 0.0f;
            vertex_buffer_data[j++] = 0.0f;

            vertex_buffer_data[j++] = radius*sin(theta);
            vertex_buffer_data[j++] = radius*cos(theta);
            vertex_buffer_data[j++] = 0.0f;

            theta += 2*M_PI/(2*n);

            vertex_buffer_data[j++] = radius*sin(theta);
            vertex_buffer_data[j++] = radius*cos(theta);
            vertex_buffer_data[j++] = 0.0f;

     }

    this->object = create3DObject(GL_TRIANGLES, j, vertex_buffer_data, color, GL_FILL);
}

void Enemy_Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Enemy_Ball::tick() {
    this->position.x += speed;
    if (this -> position.x > 7) {
        this->position.x = -7;
    }
}


void Enemy_Ball::disappear() {
    this->position.x = -0.5;
}


bounding_box_t Enemy_Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    float width = x + this->radius;
    float height = x + this->radius;
    bounding_box_t bbox = { x, y, width, height};
    return bbox;
}
