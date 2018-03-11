#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0;
    this->speedx = 0.1;
    acclr = 0.15;
    g = 0.01;
    trap_jump = 0.20;
    static GLfloat vertex_buffer_data[10000];
    int n = 100;
    float theta = 0;
    int j = 0;
    this->radius = 0.5;
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

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick(int* key_pressed, int m_count, int flag) {
    if (this->position.y > 0 || *key_pressed == 1) {
        speed -= g;
        this->position.y += speed;
   }
    if(m_count >= 100 && m_count <= 700) {
        if (this->position.y > 3) {
            if(flag%2 == 0) this->position.x += 0.15;
            else this->position.x -= 0.15;
        }
    }
    if (this->position.x < -6 || this->position.x > 6) {
        this->position.x = -1*(this->position.x);
    }
    if(this->position.y <= 0) {
        *key_pressed = 0;
        this->speed = 0.1;
        if (this->position.x > 1.25 || this->position.x< -1*(1.25)) {
            this->position.y = 0;
        }
        else {
            if((this->position.y - (-1*sqrt(pow(1.25,2) - pow(this->position.x,2)))) <= 0.000001 ) {
                this->speed = 0;
                this->position.y = -1*sqrt(pow(1.25,2) - pow(this->position.x,2));
            }
            else {
                this->speed -= 0.01;
                this->position.y -= this->speed;
            }
        }
     }
}

void Ball::disappear() {
    this->position.x = 100;
}


bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    float width = x + this->radius;
    float height = x + this->radius;
    bounding_box_t bbox = { x, y, width, height};
    return bbox;
}



// Floor class functions defined here.

Floor::Floor(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    static GLfloat vertex_buffer_data[20] = {
        5, 0, 0, // vertex 1
        5,  -5, 0, //  vertex 2
        -5,  0, 0, // vertex 3

        -5,  0, 0, // vertex 3
        -5, -5, 0, // vertex 4
         5, -5, 0 // vertex 1
    };
    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Floor::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

// trampoline class
Trampoline::Trampoline(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    static GLfloat vertex_buffer_data[10000] = {
        -0.5, 0, 0, // vertex 1
        -0.5, 0.7 , 0, //  vertex 2
        -0.4,  0, 0, // vertex 3

        -0.4,  0, 0, // vertex 3
        -0.4, 0.7, 0, // vertex 4
        -0.5, 0.7, 0, // vertex 1

        0.5, 0, 0, // vertex 1
        0.5, 0.7 , 0, //  vertex 2
        0.6,  0, 0, // vertex 3

        0.6,  0, 0, // vertex 3
        0.6, 0.7, 0, // vertex 4
        0.5, 0.7, 0, // vertex 1

       -0.5, 0.7, 0,
        0.6, 0.7, 0,
        0.05, 0.35, 0
    };

    this->object = create3DObject(GL_TRIANGLES, 15, vertex_buffer_data, color, GL_FILL);
}
void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

// magnet functions
Magnet::Magnet(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 90;
    static GLfloat vertex_buffer_data[5000] = {
        -0.5, 0, 0, // vertex 1
        -0.5, 0.7 , 0, //  vertex 2
        -0.4,  0, 0, // vertex 3

        -0.4,  0, 0, // vertex 3
        -0.4, 0.7, 0, // vertex 4
        -0.5, 0.7, 0, // vertex 1

        0.5, 0, 0, // vertex 1
        0.5, 0.7 , 0, //  vertex 2
        0.6,  0, 0, // vertex 3

        0.6,  0, 0, // vertex 3
        0.6, 0.7, 0, // vertex 4
        0.5, 0.7, 0, // vertex 1

        -0.5, 0.7, 0,
        0.6, 0.7, 0,
        0.05, 1.05, 0


    };
    this->object = create3DObject(GL_TRIANGLES, 15, vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

// porcupine functions
Porcupine::Porcupine(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed = 0.05;
    this->init_x = x;
    static GLfloat vertex_buffer_data[5000] = {
        -0.5, 0, 0, // vertex 1
        -0.4, 0.5 , 0, //  vertex 2
        -0.3,  0, 0, // vertex 3

        -0.3, 0, 0, // vertex 1
        -0.2, 0.5 , 0, //  vertex 2
        -0.1,  0, 0, // vertex 3

        -0.1, 0, 0, // vertex 1
        0, 0.5 , 0, //  vertex 2
        0.1,  0, 0, // vertex 3

        0.1, 0, 0, // vertex 1
        0.2, 0.5 , 0, //  vertex 2
        0.3,  0, 0, // vertex 3

        0.3, 0, 0, // vertex 1
        0.4, 0.5 , 0, //  vertex 2
        0.5,  0, 0, // vertex 3

    };
    this->object = create3DObject(GL_TRIANGLES, 15, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

// -4 -2.5 -1  1 2.5 4
void Porcupine::tick() {
    if (this->position.x == 4 || this->position.x >= -1.7) this->speed = -1*(this->speed);
    else if (this->position.x <= -4 || this->position.x == 1) this->speed = -1*(this->speed);
    this->position.x += this->speed;

}

// pond class
Pond::Pond(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    static GLfloat vertex_buffer_data[10000];
    int n = 100;
    float theta = M_PI/2;
    int j = 0;
    this->radius = 1.25;
    while(theta < 3*M_PI/2) {
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

void Pond::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);

    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

//functions of the plank class
Plank::Plank(float x, float y, float len, float theta, float speed, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->length = len;
    this->angle = theta *(2*M_PI/360.0);
    this->speed = speed;
    GLfloat vertex_buffer_data[20] = {
        this->length*cos(M_PI/2.0 - this->angle), this->length*sin(M_PI/2.0 - this->angle), 0, //1
        this->length*cos(M_PI/2.0 - this->angle) + 0.1*sin(M_PI/2.0 - this->angle), 0.1*cos(M_PI/2.0 - this->angle)+this->length*sin(M_PI/2.0 - this->angle), 0, //2
        -1*(this->length)*cos(M_PI/2.0 - this->angle) + 0.1*sin(M_PI/2.0 - this->angle), 0.1*cos(M_PI/2.0 - this->angle) + -1*(this->length)*sin(M_PI/2.0 - this->angle), 0,

        -1*(this->length)*cos(M_PI/2.0 - this->angle), -1*(this->length)*sin(M_PI/2.0 - this->angle), 0,
        -1*(this->length)*cos(M_PI/2.0 - this->angle) + 0.1*sin(M_PI/2.0 - this->angle), 0.1*cos(M_PI/2.0 - this->angle) + -1*(this->length)*sin(M_PI/2.0 - this->angle), 0,
        this->length*cos(M_PI/2.0 - this->angle), this->length*sin(M_PI/2.0 - this->angle), 0, //1
    };
    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Plank::tick() {
    this->position.x += speed;
    if (this -> position.x > 7) {
        this->position.x = -7;
    }
}

void Plank::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
