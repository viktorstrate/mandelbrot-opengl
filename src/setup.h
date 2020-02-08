#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* setupWindow();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
