#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
  if (!glfwInit())
  {
    std::cerr << "Failed to init GLFW\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to init GLAD\n";
    return -1;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

/*
------------------------------------------------------------
OpenGL Sanity Test (GLFW + GLAD)

This file is a minimal test to verify that OpenGL is working
correctly before integrating rendering into the main project.

Build (Windows, MSVC):
- Open "Developer Command Prompt for Visual Studio"
- cd to the project root
- Compile with:

  cl /std:c++17 /MD /EHsc ^
     src/opengl_test.cpp src/glad.c ^
     /I YOUR_LIBS/glad/include ^
     /I YOUR_LIBS/glfw/include ^
     /link ^
     /LIBPATH:YOUR_LIBS/glfw/lib-vc2022 ^
     glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib

Runtime:
- Ensure glfw3.dll is available (either next to the .exe
  or in the system PATH)
- Run opengl_test.exe

Expected result:
- A window opens with a solid blue background.
------------------------------------------------------------
*/
