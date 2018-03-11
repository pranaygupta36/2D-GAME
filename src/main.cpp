#include "main.h"
#include "timer.h"
#include "ball.h"
#include "enemy_ball.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
Porcupine porc1;
Magnet magnet;
Floor floor1, floor2;
Trampoline tramp;
Ball ball1;
Enemy_Ball eball[100];
Pond pond;
Plank planks[25];
int neball = 40;
int tball = 40;
float score = 0;
int life = 3;
int nplanks = neball/4;
float rat;
int m_count = 0;
int flag = 0;
char title[100] = {};
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;
  //  glm::mat4 VP1 = Matrices.projection *Matrices.view;
    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render

    for (int i = 0;i<neball;i++) {
        eball[i].draw(VP);
    }
    for(int i=0; i<nplanks; i++) {
        planks[i].draw(VP);
    }
    floor1.draw(VP);
    floor2.draw(VP);
    porc1.draw(VP);
    pond.draw(VP);
    ball1.draw(VP);
    tramp.draw(VP);
    if (m_count >= 100 && m_count <= 700) {
        if(flag%2 == 0){
            magnet.position.x = 3.5;
            magnet.rotation = -90;
        }
        else {
            magnet.position.x = -3.5;
            magnet.rotation = 90;
        }
        magnet.draw(VP);
    }
}

void tick_input(GLFWwindow *window , int* key_pressed) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int s = glfwGetKey(window, GLFW_KEY_S);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int d = glfwGetKey(window, GLFW_KEY_D);

    if (left) {
        ball1.position.x -= 0.1;
    }
    if (right) {
        ball1.position.x += 0.1;
    }
    if (up) {

          if (*key_pressed == 0) {
              *key_pressed += 1;
              if (ball1.position.x>2 && ball1.position.x <3.1) ball1.speed += ball1.trap_jump;
              else ball1.speed += ball1.acclr;
          }
    }
    if (down) {
        ball1.position.y -= 0.1;
    }
    if (a) screen_center_x -= 0.1;
    if (d) screen_center_x += 0.1;
    if (w) screen_center_y += 0.1;
    if (s) screen_center_y -= 0.1;

}

void tick_elements(int* key_pressed) {
    for(int i=0;i<neball;i++) {
        eball[i].tick();
    }
    for(int i = 0;i<nplanks;i++) {
        planks[i].tick();
    }
    ball1.tick(key_pressed, m_count , flag);
    porc1.tick();
    if (tball <= 0) {
        cout<<"YOU WON!!!! FINAL SCORE : "<<score<<endl;
        quit(window);
    }
    for(int i=0;i<neball;i++) {
        if (detect_collision(ball1.bounding_box(), eball[i].bounding_box())) {
            eball[i].position.x = -1*((rand()%30) + 7);
            eball[i].speed = 0;
            tball--;
            score += (0.8 - eball[i].radius)*10;
            ball1.speed += (eball[i].radius)*0.05;
            if (i%4 == 0) {
                planks[i/4].position.x = eball[i].position.x;
                planks[i/4].speed = 0;
            }
        }
    }
    if (abs(ball1.position.x - porc1.position.x) <= 1 && ball1.position.y == 0) {
        score -= 50;
        ball1.position.x = 2.5;
        life--;
     }
    for(int i = 0; i<nplanks;i++) {
        if(sqrt(pow((planks[i].position.x - ball1.position.x),2) + pow((planks[i].position.y - ball1.position.y),2) - 0.36) <= planks[i].length) {
            ball1.speed = -1*ball1.speed;
        }
    }
    if (life == 0) {
        quit(window);
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(2, 0, COLOR_BLUE);
    floor1       = Floor(0, -0.5,COLOR_GREEN);
    floor2       = Floor(0,-1 , COLOR_BROWN);
    tramp        = Trampoline(2.5 ,-0.5, COLOR_BLACK);
    for (int i=0;i<neball;i++) {
        color_t COLOR_RAND= {rand()%256, rand()%256, rand()%256};
        eball[i] = Enemy_Ball(-1*((rand()%20) + 5), ((rand()%4) + 1.5), COLOR_RAND);
    }
    for (int i=0; i<nplanks;i++) {
        float theta;
        if (rand()%2 == 0) theta = 90;
        else theta = 270;
        planks[i] = Plank((eball[4*i].radius*cos(theta *(2*M_PI/360.0))+eball[4*i].position.x), (eball[4*i].radius*sin(theta *(2*M_PI/360.0)) + eball[4*i].position.y), eball[4*i].radius, theta, eball[4*i].speed, COLOR_DBROWN);
    }
    magnet      = Magnet(-3.5, 3, COLOR_RED);
    porc1 = Porcupine(-2.5,-0.5, COLOR_RED);
    pond = Pond(0,-0.5, COLOR_POND);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {

    srand(time(0));
    int width  = 1024;
    int height = 768;
    rat = width/(float)height;
    int key_pressed = 0;
    window = initGLFW(width, height);
    initGL (window, width, height);
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw command

            sprintf(title,"score: %f lives: %d enemies_left: %d", score,life,tball);

            glfwSetWindowTitle(window, title);
            if(m_count < 701) m_count++;
            else if(m_count == 701) {
                flag++;
                m_count++;
            }
            else m_count = 0;
            draw();

            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            reshapeWindow (window, width, height);

            tick_elements(&key_pressed);
            tick_input(window,&key_pressed);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (pow(abs(a.x - b.x),2) + pow(abs(a.y - b.y),2) <= pow((abs(a.width - a.x) + abs(b.width - b.x)),2));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left*rat, right*rat, bottom, top, 0.1f, 500.0f);
}
