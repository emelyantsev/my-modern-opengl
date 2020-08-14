
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLfloat p_size = 1.0f;
GLfloat p_inc = 1.0f;

float x = 0.0f;
float inc = 0.01f;


GLuint createShaderProgram() {

    const char *vshaderSource =
    "#version 430 \n"
    "uniform float offset; \n"
    "void main(void) \n"
    "{ gl_Position = vec4(0.0 + offset, 0.0, 0.0, 1.0); }";

    const char *fshaderSource =
    "#version 430 \n"
    "out vec4 color; \n"
    "void main(void) \n"
    "{ if (gl_FragCoord.x < 400) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(0.0, 0.0, 1.0, 1.0); }";

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);
    
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    
    return vfProgram;
}

void init(GLFWwindow* window) { 

    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {

    glClear(GL_DEPTH_BUFFER_BIT);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);


    if (p_size > 100.0f) {
        p_inc = -1.0f;
    }
    else if (p_size <= 1.0f) {
        p_inc = 1.0f;
    }

    p_size += p_inc;

    glPointSize(p_size);


    glUseProgram(renderingProgram);

    x += inc;

    if (x > 1.0f) inc = -0.01f;
    if (x < -1.0f) inc = 0.01f;

    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");

    glProgramUniform1f(renderingProgram, offsetLoc, x);

    glDrawArrays(GL_POINTS, 0, 1);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int main(void) {
    
    if ( !glfwInit() ) {
         exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Program 01", NULL, NULL);

    glfwSetKeyCallback(window, key_callback);


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