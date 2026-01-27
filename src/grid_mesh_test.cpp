#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

// --------------------
// Grid resolution
// --------------------
const int N = 64; // same N as your simulation

// --------------------
// Shaders
// --------------------
const char *vertexShaderSrc = R"(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  
  uniform mat4 MVP;
  
  void main()
  {
      gl_Position = MVP * vec4(aPos, 1.0);
  }
  )";

const char *fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.1, 0.6, 0.9, 1.0);
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
      glfwCreateWindow(800, 800, "Grid Mesh Test", nullptr, nullptr);

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

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
  glUseProgram(shaderProgram);
  int mvpLoc = glGetUniformLocation(shaderProgram, "MVP");

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  // --------------------
  // Build grid vertices
  // --------------------
  std::vector<float> vertices;
  vertices.reserve(N * N * 3);

  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      float x = -1.0f + 2.0f * j / (N - 1);
      float y = -1.0f + 2.0f * i / (N - 1);
      float z = 0.0f; // flat for now

      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
    }
  }

  // --------------------
  // Build triangle indices
  // --------------------
  std::vector<unsigned int> indices;
  indices.reserve((N - 1) * (N - 1) * 6);

  for (int i = 0; i < N - 1; ++i)
  {
    for (int j = 0; j < N - 1; ++j)
    {
      int idx = i * N + j;

      // Triangle 1
      indices.push_back(idx);
      indices.push_back(idx + N);
      indices.push_back(idx + 1);

      // Triangle 2
      indices.push_back(idx + 1);
      indices.push_back(idx + N);
      indices.push_back(idx + N + 1);
    }
  }

  // --------------------
  // Upload to GPU
  // --------------------
  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
      GL_ARRAY_BUFFER,
      vertices.size() * sizeof(float),
      vertices.data(),
      GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      indices.size() * sizeof(unsigned int),
      indices.data(),
      GL_STATIC_DRAW);

  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE,
      3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  float t = 0.00f;

  // Simple tilted view matrix (column-major)
  // Orthographic projection + slight tilt (column-major)
  float MVP[16] = {
      // column 0
      1.0f, 0.0f, 0.0f, 0.0f,
      // column 1
      0.0f, 0.7f, -0.3f, 0.0f,
      // column 2
      0.0f, 0.3f, 0.7f, 0.0f,
      // column 3
      0.0f, 0.0f, 0.0f, 1.0f};

  // --------------------
  // Render loop
  // --------------------
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // --- UPDATE HEIGHTS ---
    t += 0.03f;

    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        int idx = (i * N + j) * 3;

        float x = vertices[idx + 0];
        float y = vertices[idx + 1];

        vertices[idx + 2] = 0.05f * sinf(5.0f * (x + y) + t);   // Change amplitude
      }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        vertices.size() * sizeof(float),
        vertices.data());

    // --- DRAW ---
    glLinkProgram(shaderProgram);
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, MVP);
    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(indices.size()),
        GL_UNSIGNED_INT,
        0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
