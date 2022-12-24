#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#include <cglm/cglm.h>
#include <cglm/call.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "shapes.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

int FILLMODE = GL_FILL;
int prevkey = GLFW_RELEASE;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pong", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }

    unsigned int shader_program = loadShaders("./resources/shaders/");
    struct VertexObject* vertobj = colorRect(0.2f, 2.5f, (vec3){1.0f,1.0f,1.0f});


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glPolygonMode(GL_FRONT_AND_BACK, FILLMODE);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (vertobj->texture == 1) {
            glBindTexture(GL_TEXTURE_2D, vertobj->texture);
        }

        glUseProgram(shader_program);

        mat4 trans = GLM_MAT4_IDENTITY_INIT;
        glm_ortho_default(((float)SCR_WIDTH/SCR_HEIGHT), trans);
        glm_translate(trans, (vec3){glfwGetTime()/10,sin((float)glfwGetTime()),0.0f});
        glm_rotate(trans, sin((float)glfwGetTime()), GLM_ZUP);
        glm_scale(trans, (vec3){0.2f, 0.2f, 0.2f});

        unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans[0]);

        glBindVertexArray(vertobj->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 
 
        glfwSwapBuffers(window);
        glfwPollEvents(); // poll inputs mouse/keyboard
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vertobj->VAO);
    glDeleteBuffers(1, &vertobj->VBO);
    glDeleteBuffers(1, &vertobj->EBO);
    glDeleteProgram(shader_program);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    int debugkey = glfwGetKey(window, GLFW_KEY_F2);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (debugkey == GLFW_PRESS && prevkey == GLFW_RELEASE)
        FILLMODE ^= (GL_LINE ^ GL_FILL);
    prevkey = debugkey;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0,0,width,height);
}
