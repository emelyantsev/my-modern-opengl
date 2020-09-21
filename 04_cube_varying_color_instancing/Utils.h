#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace Utils {


using namespace std;

string readShaderSource(const char *filePath) {

    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";

    while (!fileStream.eof()) {

        getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint createShaderProgram(const char *vp, const char *fp) {


    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);


    string vertShaderStr = readShaderSource(vp);
    string fragShaderStr = readShaderSource(fp);

    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);



    glCompileShader(vShader);
    glCompileShader(fShader);
    
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    
    return vfProgram;

}

}