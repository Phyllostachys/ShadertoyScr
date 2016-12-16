#include <cstdio>
#include <iostream>
#include <glad/glad.h>

#include "shader.h"

// src - a file that has been opened already
// dest - a std::string where the contents of src will be put into
static void readFile(std::string& dest, FILE* src)
{
    char* fileBuf;
    size_t fileSize;

    // get size of contents
    fseek(src, 0, SEEK_END);
    fileSize = ftell(src);

    // use size of contents
    dest.reserve(fileSize);
    fileBuf = new char[fileSize+1];

    // get contents
    rewind(src);
    fread(fileBuf, sizeof(char), fileSize, src);
    fileBuf[fileSize] = '\0';

    // assign contents and cleanup
    //dest.assign(fileBuf);
    dest = std::string(fileBuf);
    delete[] fileBuf;
}

Shader::Shader()
{
    anyErrors = false;
    typeFlags = 0;
    compiled = false;
    vertShaderHandle = 0;
    geomShaderHandle = 0;
    tessShaderHandle = 0;
    fragShaderHandle = 0;
}

Shader::Shader(std::string vert_filename, std::string frag_filename)
{
    FILE* inFile;

    inFile = fopen(vert_filename.c_str(), "rb");
    if (inFile == NULL) {
        std::cout << "Vertex shader file doesn't exist.\n";
        anyErrors = true;
        return;
    }
    readFile(vertSource, inFile);

    inFile = fopen(frag_filename.c_str(), "rb");
    if (inFile == NULL) {
        std::cout << "Fragment shader file doesn't exist.\n";
        anyErrors = true;
        return;
    }
    readFile(fragSource, inFile);
    fclose(inFile);

    typeFlags = VERT_SHADER | FRAG_SHADER;
    compiled = false;
    vertShaderHandle = 0;
    geomShaderHandle = 0;
    tessShaderHandle = 0;
    fragShaderHandle = 0;
}

Shader::Shader(std::string vert_src, std::string frag_src, bool heyooo)
{
    vertSource = vert_src;
    fragSource = frag_src;
    if (heyooo) {
        printf("buuurp\n");
    }
}

int32_t Shader::addShader(std::string src_filename, shader_type type)
{
    return -1;
}

GLuint Shader::getShaderProgram()
{
    if (compiled == false) {
        std::cout << "Shader hasn't been compiled yet\n";
        anyErrors = true;
        return 0;
    }

    return shaderProgHandle;
}

static bool isShaderStatusGood(GLint shaderHandle, GLenum statusType, char errorMsg[1024])
{
    GLint status;
    glGetShaderiv(shaderHandle, statusType, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
        std::cout << "Shader compile error is " << length << " chars long.\n";
        glGetShaderInfoLog(shaderHandle, length, NULL, errorMsg);
        std::cout << "glCompileShader failed: \n" << errorMsg << "\n";
        return false;
    } else {
        return true;
    }
}

bool Shader::compile()
{
    char buffer[1024];

    // Vertex shader
    vertShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vs = vertSource.c_str();
    const GLint vsLen = vertSource.length();
    glShaderSource(vertShaderHandle, 1, &vs, &vsLen);
    glCompileShader(vertShaderHandle);

    // check vertex shader compile status
    if (!isShaderStatusGood(vertShaderHandle, GL_COMPILE_STATUS, buffer)) {
        anyErrors = true;
        return false;
    }

    // geometry shader
    // tesselation shader

    // fragment shader
    fragShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fs = fragSource.c_str();
    const GLint fsLen = fragSource.length();
    glShaderSource(fragShaderHandle, 1, &fs, &fsLen);
    glCompileShader(fragShaderHandle);

    // check vertex shader compile status
    if (!isShaderStatusGood(fragShaderHandle, GL_COMPILE_STATUS, buffer)) {
        anyErrors = true;
        return false;
    }

    // create and link full pipeline
    shaderProgHandle = glCreateProgram();
    glAttachShader(shaderProgHandle, vertShaderHandle);
    if (typeFlags & GEOM_SHADER) {
        glAttachShader(shaderProgHandle, geomShaderHandle);
    }
    if (typeFlags & TESS_SHADER) {
        glAttachShader(shaderProgHandle, tessShaderHandle);
    }
    glAttachShader(shaderProgHandle, fragShaderHandle);
    glLinkProgram(shaderProgHandle);
    // remember that a call to glUseProgram will still need to be done

    // check program link status
    GLint status;
    glGetProgramiv(shaderProgHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(shaderProgHandle, GL_INFO_LOG_LENGTH, &length);
        GLchar *info = new GLchar[length];
        glGetProgramInfoLog(shaderProgHandle, length, NULL, info);
        std::cout << "glLinkProgram failed: \n" << info << std::endl;
        delete [] info;
    }
    //glDetachShader(shaderProgHandle, vertShaderHandle);
    //glDetachShader(shaderProgHandle, fragShaderHandle);
    //glDeleteShader(vertShaderHandle);
    //glDeleteShader(fragShaderHandle);

    compiled = true;
    return true;
}
