#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#include "gameobjects.h"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

void draw(VertexObject* vertobj, int fillmode) {
    glPolygonMode(GL_FRONT_AND_BACK, fillmode);
    glBindVertexArray(vertobj->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, vertobj->vert_count, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0); // no need to unbind it every time 
}

void useShader(unsigned int shader_program, vec3 pos) {
    glUseProgram(shader_program);

    unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, pos);
}

void render_begin() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render(Object* gameobject, int fillmode, unsigned int shader_program) {
    VertexObject* vertobj = gameobject->vertobj;
    if (vertobj->texture == 1) {
        glBindTexture(GL_TEXTURE_2D, vertobj->texture);
    }

    mat4 trans = GLM_MAT4_IDENTITY_INIT;
    glm_ortho_default(((float)SCR_WIDTH/SCR_HEIGHT), trans);
    glm_translate(trans, (vec3){gameobject->xpos, gameobject->ypos, 0.0f});
    // glm_translate(trans, (vec3){(float)((int)glfwGetTime())/10.0f,0.0f,0.0f});
    // glm_rotate(trans, glfwGetTime(), GLM_ZUP);
    // glm_scale(trans, (vec3){0.2f, 0.2f, 0.2f});

    useShader(shader_program, trans[0]);
    draw(vertobj, fillmode);
}

void render_end() {

}

void render_cleanup(VertexObject* vertobj) {
    glDeleteVertexArrays(1, &(vertobj->VAO));
    glDeleteBuffers(1, &(vertobj->VBO));
    glDeleteBuffers(1, &(vertobj->EBO));
}

#endif
