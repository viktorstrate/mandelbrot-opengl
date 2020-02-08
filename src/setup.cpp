#include "setup.h"

#include <iostream>

GLFWwindow* setupWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "Mandlebrot", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

  // glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Resize window events
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetScrollCallback(window, scroll_callback);

  return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

bool mouseDown = false;
bool resetMouse = false;

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    if (mouseDown == false)
      resetMouse = true;

    mouseDown = true;
  } else {
    mouseDown = false;
  }
}

double prevCursorX = 0.;
double prevCursorY = 0.;

// From main.cpp
void updateOffsetCoords(double x, double y);
void updateZoom(double zoom);

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (mouseDown) {
    if (resetMouse) {
      prevCursorX = xpos;
      prevCursorY = ypos;
      resetMouse = false;
    }

    double xOffset = xpos - prevCursorX;
    double yOffset = ypos - prevCursorY;
    updateOffsetCoords(xOffset, yOffset);

    prevCursorX = xpos;
    prevCursorY = ypos;
  }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  updateZoom(yoffset);
}

