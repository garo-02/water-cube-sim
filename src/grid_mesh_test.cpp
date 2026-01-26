#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// --------------------
// Shader sources
// --------------------
const char *vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

const char *fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.2, 0.7, 0.9, 1.0);
}
)";

int main()
{
  // --------------------
  // Init GLFW
  // --------------------
  if (!glfwInit())
  {
    std::cerr << "Failed to init GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Triangle Test", nullptr, nullptr);

  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // --------------------
  // Init GLAD
  // --------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to init GLAD\n";
    return -1;
  }

  // Set viewport
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  glViewport(0, 0, w, h);

  // --------------------
  // Compile shaders
  // --------------------
  GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vertexShaderSrc, nullptr);
  glCompileShader(vShader);

  GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fragmentShaderSrc, nullptr);
  glCompileShader(fShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vShader);
  glAttachShader(shaderProgram, fShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  // --------------------
  // Triangle data
  // --------------------
  float vertices[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f};

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(
      0, // layout location
      3, // vec3
      GL_FLOAT,
      GL_FALSE,
      3 * sizeof(float),
      (void *)0);
  glEnableVertexAttribArray(0);

  // --------------------
  // RENDER LOOP
  // --------------------
  while (!glfwWindowShouldClose(window))
  {
    // Clear screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers + poll input
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
