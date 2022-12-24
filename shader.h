#ifndef SHADER_H
#define SHADER_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <glad/glad.h>
#include <glib.h>

#include <stdio.h>

const char* fileExt(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void* readFile(const char* filepath) {
    int fd = open(filepath, O_RDONLY);
    int len = lseek(fd, 0, SEEK_END);
    return mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
}

unsigned int compileShader(const char* shader_source, unsigned int shader_type) {
    // build and compile our shader program
    unsigned int shader = glCreateShader(shader_type); // Pointer to shader. Given shader_type: GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
    return shader;
}

unsigned int linkShaders(int argc, unsigned int* shaders) {
    unsigned int shader_program = glCreateProgram();
    for (int i=0;i<argc;i++) {
        glAttachShader(shader_program, shaders[i]);
    }
    glLinkProgram(shader_program);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    for (int i=0;i<argc;i++) {
        glDeleteShader(shaders[i]);
    }

    return shader_program;
}

// Returns a shader program or 0
unsigned int loadShaders(const char* shader_path) {
    GError *err = NULL;
    GDir* shadir = g_dir_open(shader_path, 0, &err);
    unsigned int shaders[64]; // Hardcoded shader max count
    shaders[0] = 0;
    int shader_count = 0;
    unsigned int shader_program;

    if (err != NULL) {
        fprintf(stderr, "Unable to list files in dir \"%s\": %s\n", shader_path, err->message);
        g_error_free(err);
        g_dir_close(shadir);
        exit(1);
    }

    // Read filenames in dir and check extension
    while (true) {
        const char* filename = g_dir_read_name(shadir);
        if (filename==NULL) { break; }
        unsigned int shader_type;
        const char* ext = fileExt(filename);
        if (strcmp(ext,"vert") == 0) {
            shader_type = GL_VERTEX_SHADER;
        } else if (strcmp(ext,"frag") == 0) {
            shader_type = GL_FRAGMENT_SHADER;
        } else { continue; }

        // If correct extension compile and link shader
        char* full_path = malloc(strlen(shader_path)+strlen(filename)+2);
        strcpy(full_path, shader_path);
        if (shader_path[strlen(shader_path)-1] != '/') { 
            full_path = strcat(full_path, "/");
        }
        full_path = strcat(full_path, filename);
        char* shader_source = readFile(full_path);
        unsigned int shader = compileShader(shader_source, shader_type); // Pointer to compiled shader
        
        shaders[shader_count] = shader;
        shader_count++;
    }
    g_dir_close(shadir);
    if (shaders[0] == 0) { return 0; }
    shader_program = linkShaders(shader_count, shaders);
    return shader_program;
}

#endif