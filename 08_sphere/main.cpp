#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <SOIL2/soil2.h>

#include "Utils.h"

#include "Sphere.h"

#include "player.h"

using namespace std;

#define numVAOs 1
#define numVBOs 3

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat;

GLuint brickTexture;

Sphere mySphere(48);


Player player;
GLFWmonitor* monitor;


void setupVertices(void) { // 36 vertices, 12 triangles, makes 2x2x2 cube placed at origin

    std::vector<int> ind = mySphere.getIndices();

    std::vector<glm::vec3> vert = mySphere.getVertices();
    std::vector<glm::vec2> tex = mySphere.getTexCoords();
    std::vector<glm::vec3> norm = mySphere.getNormals();

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors

    int numIndices = mySphere.getNumIndices();

    for (int i = 0; i < numIndices; i++) {

        pvalues.push_back((vert[ind[i]]).x);
        pvalues.push_back((vert[ind[i]]).y);
        pvalues.push_back((vert[ind[i]]).z);

        tvalues.push_back((tex[ind[i]]).s);
        tvalues.push_back((tex[ind[i]]).t);

        nvalues.push_back((norm[ind[i]]).x);    
        nvalues.push_back((norm[ind[i]]).y);
        nvalues.push_back((norm[ind[i]]).z);
    }

        glGenVertexArrays(1, vao);
        glBindVertexArray(vao[0]);
        glGenBuffers(3, vbo);

        // put the vertices into buffer #0
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, pvalues.size()*4, &pvalues[0], GL_STATIC_DRAW);
        
        // put the texture coordinates into buffer #1
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, tvalues.size()*4, &tvalues[0], GL_STATIC_DRAW);
        
        // put the normals into buffer #2
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, nvalues.size()*4, &nvalues[0], GL_STATIC_DRAW);

}

void init(GLFWwindow* window) {

    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    cubeLocX = 0.0f; cubeLocY = 0.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective

    setupVertices();

    brickTexture = Utils::loadTexture("2k_earth_daymap.jpg");
}
void display(GLFWwindow* window, double currentTime) {

    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    
    // get the uniform variables for the MV and projection matrices
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    
    // build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);

    aspect = (float) width / (float) height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
    
    // build view matrix, model matrix, and model-view matrix
    //vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    vMat = player.GetPlayerLookAt();

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
    mvMat = vMat * mMat;

    // copy perspective and MV matrices to corresponding uniform variables
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
   
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

  //  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  //  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  //  glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTexture);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glDrawArrays(GL_TRIANGLES, 0, mySphere.getNumIndices());
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


int main(void) { // main() is unchanged from before

    if (!glfwInit()) { 
        
        exit(EXIT_FAILURE); 
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Earth", NULL, NULL);

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_reshape_callback);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) { 
        exit(EXIT_FAILURE); 
    }

    glfwSwapInterval(1);
    
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