#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <array>

#include "Renderer.h"
#include "tools.h"
#include "VertexBufferLayout.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define RESOLUTION_WIDTH 1920
#define RESOLUTION_HEIGHT 1080

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, "Application", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::clog << "GlEW ERROR!\n";

    std::clog << glGetString(GL_VERSION) << std::endl << std::flush;



    std::array<float, 8> positions = {
        -1, -1, // 0
         1, -1, // 1
         1,  1, // 2
        -1,  1  // 3
    };

    std::array<uint32_t, 6> indices = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray* vertex_array = new VertexArray();
    VertexBuffer* vertex_buffer =  new VertexBuffer(positions.data(), sizeof(positions));

    VertexBufferLayout layout;
    layout.Push<float>(2U);
    vertex_array->AddBuffer(*vertex_buffer, layout);

    IndexBuffer* index_buffer = new IndexBuffer(indices.data(), indices.size());
    Shader* shader = new Shader(RESOURCE_DIR "shaders\\VertexShader.vert", RESOURCE_DIR "shaders\\FragmentShader.frag");



    // Unbind all
    GLCALL(glBindVertexArray(0));
    GLCALL(glUseProgram(0));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    Renderer renderer;

    auto base_time = std::chrono::system_clock::now();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        shader->Bind();
        shader->SetUniform2f("u_Resolution", RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
        float time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - base_time).count();
        shader->SetUniform1f("u_Time", time_ms / 1000.f);

        renderer.Draw(*vertex_array, *index_buffer, *shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete vertex_array;
    delete vertex_buffer;
    delete index_buffer;
    delete shader;

    // ! Ensure all OpenGL objects are deleted before this point
    //      Cannot call glDeleteBuffer without valid GL context
    glfwTerminate();
    return 0;
}