#include "GLFWContext.hpp"
Window_Context::Window_Context() {
  // GLFW INIT: ORDER IS IMPORTANT
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) exit(EXIT_FAILURE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Project", NULL,
                            NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSwapInterval(0);
  // GLEW INIT
  GLenum err = glewInit();

  if (err != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));
  }
}

float Window_Context::anglesize = 0.001f;
float Window_Context::rotation = 0.0f;
float Window_Context::stepsize = 0.05f;
GLFWwindow* Window_Context::window;
pipeline Window_Context::world;
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
    } else if (action == GLFW_RELEASE) {
    }
  } else {
    Window_Context::world.update(key, Window_Context::stepsize);
  }
}
static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  double mouse_x = (xpos - WINDOW_WIDTH / 2) * Window_Context::anglesize;
  double mouse_y = (-ypos + WINDOW_HEIGHT / 2) * Window_Context::anglesize;

  Window_Context::world.update(mouse_x, mouse_y);
  glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

