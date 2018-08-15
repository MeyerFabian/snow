#ifndef GLFW_CONTEXT
#define GLFW_CONTEXT

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "../utils/defines.hpp"
#include "pipeline/pipeline.hpp"

static GLFWwindow* window;
static bool debug;

static pipeline world;

static float anglesize = 0.001f;
static float stepsize = 0.05f;
static float rotation = 0.0f;

static void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  else if (key == GLFW_KEY_Q) {
    if (action == GLFW_PRESS) {
    }
  } else if (key == GLFW_KEY_E) {
    if (action == GLFW_PRESS) {
    }
  } else if (key == GLFW_KEY_W) {
    if (action == GLFW_PRESS) {
    }

  } else if (key == GLFW_KEY_S) {
    if (action == GLFW_PRESS) {
    }
  } else if (key == GLFW_KEY_D) {
    if (action == GLFW_PRESS) {
      debug = true;
    } else if (action == GLFW_RELEASE) {
      debug = false;
    }
  } else {
    world.update(key, stepsize);
  }
}
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  double mouse_x = (xpos - WINDOW_WIDTH / 2) * anglesize;
  double mouse_y = (-ypos + WINDOW_HEIGHT / 2) * anglesize;

  world.update(mouse_x, mouse_y);
  glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

class Window_Context {
 protected:
  Window_Context() {
    // GLFW INIT: ORDER IS IMPORTANT
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Project",
                              NULL, NULL);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glfwSetCursorPosCallback(window, mouse_callback);

    // GLEW INIT
    GLenum err = glewInit();

    if (err != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));
    }
  }
};

#endif  // GLFW_CONTEXT

