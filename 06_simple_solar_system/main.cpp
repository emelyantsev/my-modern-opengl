#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <stack>

#include "Utils.h"
#include "player.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

//float cameraX, cameraY, cameraZ;
//float cubeLocX, cubeLocY, cubeLocZ;
//float pyrLocX, pyrLocY, pyrLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// allocate variables used in display() function, so that they won’t need to be allocated during rendering

GLuint mvLoc, projLoc;
int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat;
glm::mat4 tMat, rMat;

stack<glm::mat4> mvStack;

Player player;
GLFWmonitor* monitor;


void setupVertices(void) { // 36 vertices, 12 triangles, makes 2x2x2 cube placed at origin


    float cubePositions[108] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };

    // pyramid with 18 vertices, comprising 6 triangles (four sides, and two on the bottom)
    float pyramidPositions[54] = { 
    -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
    1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
    1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
    -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
    1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
    };

    glGenVertexArrays(numVAOs, vao); // we need at least 1 VAO
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo); // we need at least 2 VBOs
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {

    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

  //  cameraX = 0.0f; cameraY = 0.0f; cameraZ = 18.0f;
  //  cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective
  //  pyrLocX = 1.0f; pyrLocY = 1.0f ; pyrLocZ = 0.0f;

    setupVertices();

    // build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);

    aspect = (float) width / (float) height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

}


void display(GLFWwindow* window, double currentTime) {

    
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    
    
    // get the uniform variables for the MV and projection matrices
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));


    // push view matrix onto the stack
    //vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    vMat = player.GetPlayerLookAt();

    mvStack.push(vMat);

    // ---------------------- pyramid == sun --------------------------------------------

    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // sun position
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float) currentTime, glm::vec3(1.0f, 0.0f, 0.0f));

    // sun rotation
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18); // draw the sun
    mvStack.pop(); // remove the sun’s axial rotation from the stack


    //----------------------- cube == planet ---------------------------------------------
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3( sin((float) currentTime) * 4.0, 0.0f, cos( (float) currentTime) * 4.0 ) );
    
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
    
    // planet rotation

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 36); // draw the planet
    mvStack.pop(); // remove the planet’s axial rotation from the stack


    //----------------------- smaller cube == moon -----------------------------------
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin( (float) currentTime) * 2.0, cos( (float) currentTime) * 2.0));
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));

    // moon rotation
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)); // make the moon smaller

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36); // draw the moon

    // remove moon scale/rotation/position, planet position, sun position, and view matrices from stack
    mvStack.pop(); mvStack.pop(); mvStack.pop(); mvStack.pop();
}


void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {

    aspect = (float )newWidth / (float) newHeight; 
    glViewport(0, 0, newWidth, newHeight); 

    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_W)
        player.MoveForward();

    if (key == GLFW_KEY_S)
        player.MoveBackward();

    if (key == GLFW_KEY_A)
        player.MoveLeft();
    
    if (key == GLFW_KEY_D)
        player.MoveRight();

    if (key == GLFW_KEY_Q)
        player.MoveDown();

    if (key == GLFW_KEY_E)
        player.MoveUp();

    if (key == GLFW_KEY_UP)
        player.RotateDown();

    if (key == GLFW_KEY_DOWN)
        player.RotateUp();

    if (key == GLFW_KEY_LEFT) {

        mods != GLFW_MOD_CONTROL ? player.RotateLeft() : player.RotateZLeft();
    }
        
    if (key == GLFW_KEY_RIGHT) {

        mods != GLFW_MOD_CONTROL ? player.RotateRight() : player.RotateZRight();
    }

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {

        monitor = glfwGetWindowMonitor(window);

        if (monitor == nullptr) {

            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }

        else {

            glfwSetWindowMonitor(window, NULL, 100, 100, 800, 600, 0);
        }

    }

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "framebuffer size callback" << std::endl;
    std::cout << width << " : " << height << std::endl;

    if (height == 0) {
        return;
    }

    aspect = (float) width / (float) height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    glViewport(0,0, width, height);
}



int main(void) { 

    if (!glfwInit()) { 
        
        exit(EXIT_FAILURE); 
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Planar system", NULL, NULL);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) { 
        exit(EXIT_FAILURE); 
    }

    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, window_reshape_callback);
    
    init(window);

    while (!glfwWindowShouldClose(window)) {
        
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}