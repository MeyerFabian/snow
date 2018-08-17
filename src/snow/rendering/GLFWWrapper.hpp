#ifndef GLFWWRAPPER
#define GLFWWRAPPER

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "../defines/defines.hpp"
#include "pipeline/pipeline.hpp"
static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods);

static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
struct GLFWWrapper {
  GLFWWrapper();

  static GLFWwindow* window;
  static float anglesize;
  static float stepsize;
  static float rotation;
  static pipeline world;
};
#endif  // GLFW_WRAPPER

