#include <iostream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <algorithm>

#include "setup.h"
#include "shaders_generated.h"
#include "shaders.h"

const int vertices[] = {
      1,  1, 1,  // top right
      1, -1, 1,  // bottom right
    -1, -1, 1,  // bottom left
    -1,  1, 1   // top left
};
const unsigned int indices[] = {
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

// Offsets and zoom level that are sent to the fragment shader
double xOffset, yOffset = 0.0;
double zoom = 1.;

int main()
{
  GLFWwindow* window = setupWindow();
  if (window == nullptr) {
    std::cout << "Error setting up window, exiting" << std::endl;
    exit(1);
  }

  // Create vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  char vertexShaderBuffer[shaders_vertex_glsl_len+1];
  readGeneratedShader(shaders_vertex_glsl, shaders_vertex_glsl_len, vertexShaderBuffer);
  const auto vertexShaderPtr = reinterpret_cast<char*>(vertexShaderBuffer);

  glShaderSource(vertexShader, 1, &vertexShaderPtr, NULL);
  glCompileShader(vertexShader);
  {
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  }

  // Create fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  char fragmentShaderBuffer[shaders_fragment_glsl_len+1];
  readGeneratedShader(shaders_fragment_glsl, shaders_fragment_glsl_len, fragmentShaderBuffer);
  const auto fragmentShaderPtr = reinterpret_cast<char*>(fragmentShaderBuffer);

  glShaderSource(fragmentShader, 1, &fragmentShaderPtr, NULL);
  glCompileShader(fragmentShader);
  {
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  }

  // Create shader program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  {
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  }

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Vertex Array
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Vertex Buffer
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Element Buffer
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(int), (void*)0);
  glEnableVertexAttribArray(0);

  int offsetUniform = glGetUniformLocation(shaderProgram, "uOffset");
  int zoomUniform = glGetUniformLocation(shaderProgram, "uZoom");

  while (!glfwWindowShouldClose(window))
  {

    auto startTime = std::chrono::high_resolution_clock::now();

    processInput(window);

    // Render
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Uniforms
    glUniform2f(offsetUniform, xOffset, yOffset);
    glUniform1f(zoomUniform, zoom);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // Calculate elapsed time since last frame, to lock fps to 60
    auto endTime = std::chrono::high_resolution_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    int wait = std::max(int(1000000./60.) - elapsed, 0);
    usleep(wait);

  }

  glfwTerminate();
  return 0;
}

// Update zoom level, called from setup.cpp
void updateZoom(double newZoom)
{
  static double SENSITIVITY = 50.;

  if (newZoom > 0)
    zoom = zoom / (newZoom/SENSITIVITY+1);

  if (newZoom < 0)
    zoom = zoom * ((newZoom * -1.)/SENSITIVITY+1);

  zoom = std::min(2., zoom);
}

// Update offset coordinates, called from setup.cpp
void updateOffsetCoords(double x, double y)
{
  double SENSITIVITY = zoom * 1/300.;
  xOffset -= x * SENSITIVITY;
  yOffset += y * SENSITIVITY;
}
