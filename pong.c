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

#include "gameobjects.h"

#include "render.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void update();

// settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

int FILLMODE = GL_FILL;
int prevkey = GLFW_RELEASE;

Player* player1;
Player* player2;
Ball* ball;
Object* game_border;

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pong", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }

    unsigned int shader_program = loadShaders("./resources/shaders/");
    Object game_border = {colorRectOutline(1.2f, 1.0f, 0.01f, (vec3){1.0f, 1.0f, 1.0f}), 0.0f, 0.0f};
    player1 = mkPlayer(-0.95f, 0.0f, 0.02f, 0.25f, (vec3){1.0f,1.0f,1.0f});
    player2 = mkPlayer(0.95f, 0.0f, 0.02f, 0.25f, (vec3){1.0f,1.0f,1.0f});
    ball = mkBall(0.0f, 0.0f, 0.01f, 0.03f, 0.02f, (vec3){1.0f,1.0f,1.0f});

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        render_begin();
        render((Object*)player1, FILLMODE, shader_program);
        render((Object*)player2, FILLMODE, shader_program);
        render((Object*)ball, FILLMODE, shader_program);
        render(&game_border, FILLMODE, shader_program);
        update();

        glfwSwapBuffers(window);
        glfwPollEvents(); // poll inputs mouse/keyboard
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    render_cleanup(player1->vertobj);
    render_cleanup(player2->vertobj);
    glDeleteProgram(shader_program);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void update() {
    player_update(player1);
    player_update(player2);
    ball_update(ball, player1, player2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Player 1
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player1->yvel = 0.03f;
    } else 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player1->yvel = -0.03f;
    } else {
        player1->yvel *= 0.9f;
    }

    // Player 2
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        player2->yvel = 0.03f;
    } else 
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player2->yvel = -0.03f;
    } else {
        player2->yvel *= 0.9f;
    }

    int debugkey = glfwGetKey(window, GLFW_KEY_F2);
    if (debugkey == GLFW_PRESS && prevkey == GLFW_RELEASE)
        FILLMODE ^= (GL_LINE ^ GL_FILL);
    prevkey = debugkey;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0,0,width,height);
}
