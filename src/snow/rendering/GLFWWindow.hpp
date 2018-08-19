#ifndef GLFW_CONTEXT
#define GLFW_CONTEXT

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "../utils/defines.hpp"
#include "particleRenderer.hpp"
#include "pipeline/pipeline.hpp"

class ParticleRenderer;

static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods);

static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
class GLFWWindow {
 public:
  GLFWWindow();
  static bool shouldClose();
  static void stop();
  static float anglesize;
  static float stepsize;
  static float rotation;
  static void clear();
  static void swapBuffers();

 private:
  static GLFWwindow* window;
};
#endif  // GLFW_CONTEXT

