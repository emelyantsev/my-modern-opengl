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


GLuint loadTexture(const char *texImagePath) {
    GLuint textureID;
    
    textureID = SOIL_load_OGL_texture(texImagePath,
    
    SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    if (textureID == 0) cout << "could not find texture file" << texImagePath << endl;
    
    return textureID;
}

}