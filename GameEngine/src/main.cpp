#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include "core/Shader.h"
#include "core/VertexArray.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/VertexBufferLayout.h"
#include "core/Texture.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void APIENTRY DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam);

float zoom = 2.0f;
bool isPressedUp = false;
bool isPressedDown = false;

glm::vec2 center = { -0.5f, 0.0f };
bool isPressedW = false;
bool isPressedA = false;
bool isPressedS = false;
bool isPressedD = false;

int maxIterations = 1000;
bool isPressedQ = false;
bool isPressedE = false;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    // Tells OpenGL we want the core-profile (the good one, with just the newer stuff)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GameEngine", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::cin.get();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        std::cin.get();
        return -1;
    }
    std::cout << "\033[1;32m" << glGetString(GL_VERSION) << "\033[0m" << std::endl;
    glDebugMessageCallback(DebugCallBack, nullptr);

    Shader shader("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");

    std::vector<float> vertices = 
    {
        // positions         // colors          // texture coords
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f,  2.0f, 2.0f,
         1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 2.0f
    };

    std::vector<unsigned int> indices = 
    {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<float> texCoords =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f
    };

    VertexArray VAO;
    VAO.Bind();
    VertexBuffer VBO(&vertices[0], static_cast<unsigned int>(vertices.size() * sizeof(float)), GL_STATIC_DRAW);
    VBO.Bind();
    IndexBuffer EBO(&indices[0], static_cast<unsigned int>(indices.size() * sizeof(unsigned int)), GL_STATIC_DRAW);
    EBO.Bind();
    VertexBufferLayout VBL;
    VBL.Push<float>(3); // positions
    VBL.Push<float>(3); // colors
    VBL.Push<float>(2); // texture coords
    VAO.AddBuffer(VBO, VBL);

    VBO.Unbind();
    VAO.Unbind();

    Texture texture0("resources/textures/container.jpg", GL_RGB, TexParam::LINEAR);
    Texture texture1("resources/textures/awesomeface.png", GL_RGBA, TexParam::REPEAT);
    Texture texture2("resources/textures/wall.jpg");

    shader.Bind();
    shader.SetUniform1i("texture0", 0);
    shader.SetUniform1i("texture1", 1);
    shader.SetUniform1i("texture2", 2);


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();

        float timeValue = (float)glfwGetTime();
        float colorValue = (sin(timeValue) / 2.0f) + 0.5f;

        glm::mat4 trans = glm::mat4(1.0f);
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

        shader.SetUniform4f("outColor", 0.0f, colorValue, 0.0f, 1.0f);
        // shader.SetUniform1f("smilePercentage", smilePercentage);
        shader.SetUniformMatrix4fv("transform", trans);

        shader.SetUniform2f("u_resolution", 800.0f, 600.0f);
        shader.SetUniform2f("u_center", center.x, center.y);
        shader.SetUniform1f("u_zoom", zoom);
        shader.SetUniform1i("u_maxIterations", maxIterations);

        texture0.Bind(0);
        texture1.Bind(1);
        texture2.Bind(2);
        VAO.Bind();
        glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    else if (key == GLFW_KEY_UP && action == GLFW_PRESS && !isPressedUp)
        isPressedUp = true;
    else if (key == GLFW_KEY_UP && action == GLFW_RELEASE && isPressedUp)
    {
        isPressedUp = false;
        zoom -= 0.2f * zoom;
        std::cout << "Zoom: " << zoom << "\n";
    }

    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && !isPressedDown)
        isPressedDown = true;
    else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE && isPressedDown)
    {
        isPressedDown = false;
        zoom += 0.2f * zoom;
        std::cout << "Zoom: " << zoom << "\n";
    }

    else if (key == GLFW_KEY_W && action == GLFW_PRESS && !isPressedW)
        isPressedW = true;
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE && isPressedW)
    {
        isPressedW = false;
        center.y += 0.1f * zoom;
        std::cout << "Center: " << center.x << ", " << center.y << "\n";
    }

    else if (key == GLFW_KEY_A && action == GLFW_PRESS && !isPressedA)
        isPressedA = true;
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE && isPressedA)
    {
        isPressedA = false;
        center.x -= 0.1f * zoom;
        std::cout << "Center: " << center.x << ", " << center.y << "\n";
    }

    else if (key == GLFW_KEY_S && action == GLFW_PRESS && !isPressedS)
        isPressedS = true;
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE && isPressedS)
    {
        isPressedS = false;
        center.y -= 0.1f * zoom;
        std::cout << "Center: " << center.x << ", " << center.y << "\n";
    }

    else if (key == GLFW_KEY_D && action == GLFW_PRESS && !isPressedD)
        isPressedD = true;
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE && isPressedD)
    {
        isPressedD = false;
        center.x += 0.1f * zoom;
        std::cout << "Center: " << center.x << ", " << center.y << "\n";
    }

    else if (key == GLFW_KEY_Q && action == GLFW_PRESS && !isPressedQ)
        isPressedQ = true;
    else if (key == GLFW_KEY_Q && action == GLFW_RELEASE && isPressedQ)
    {
        isPressedQ = false;
        maxIterations *= 2;
        std::cout << "Max Iterations: " << maxIterations << "\n";
    }

    else if (key == GLFW_KEY_E && action == GLFW_PRESS && !isPressedE)
        isPressedE = true;
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE && isPressedE)
    {
        isPressedE = false;
        maxIterations /= 2;
        std::cout << "Max Iterations: " << maxIterations << "\n";
    }

}

void APIENTRY
DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam)
{
#ifndef DEBUG || DEBINFO
    if (severity != GL_DEBUG_SEVERITY_MEDIUM && severity != GL_DEBUG_SEVERITY_HIGH) return;
#endif
    std::cout << "\n\033[1;31m ------------ DEBUG ------------ \033[0m\n" << std::endl;
    std::cout << "Message: \033[1;33m" << message << "]\033[0m\n" << std::endl;
    std::cout << "Type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER";
        break;
    default:
        std::cout << type;
        break;
    }
    std::cout << std::endl;

    std::cout << "ID: " << id << std::endl;
    std::cout << "Severity: ";
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Generic Notification";
        break;
    default:
        std::cout << severity;
        break;
    }
    std::cout << std::endl;
    std::cout << "\n\033[1;31m ------------ END OF DEBUG ------------ \033[0m\n" << std::endl;
}