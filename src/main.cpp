#include <iostream>
#include <string>

#include "setup.h"
#include "shaders_generated.h"
#include "shaders.h"


// setup.cpp
bool setupOpenGL();

int vertices[] = {
      1,  1, 1,  // top right
      1, -1, 1,  // bottom right
    -1, -1, 1,  // bottom left
    -1,  1, 1   // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

int main()
{
  GLFWwindow* window = setupWindow();
  if (window == nullptr) {
    std::cout << "Error setting up window, exiting" << std::endl;
    exit(1);
  }

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

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0); // no need to unbind it every time

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

