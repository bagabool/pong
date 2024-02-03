#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VAO.h"
#include "VBO.h"
#include "VBLayout.h"
#include "Shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// players and ball position
float playerOneYOffset = 0.0f;
float playerTwoYOffset = 0.0f;
float ballXOffset = 0.0f;
float ballYOffset = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float ballVertices[] = {
         0.25f,  0.25f, 0.0f,  
         0.25f, -0.25f, 0.0f,  
        -0.25f, -0.25f, 0.0f,  
        -0.25f,  0.25f, 0.0f   
    };

    float playerVertices[] = {
         0.12f,  1.0f, 0.0f,
         0.12f, -1.0f, 0.0f,
        -0.12f, -1.0f, 0.0f,
        -0.12f,  1.0f, 0.0f
    };

    unsigned int indices[] = {  
        0, 1, 3,  
        1, 2, 3   
    };

    // ball VAO
    VAO ballVAO;
    ballVAO.Bind();
    VBO ballVBO(ballVertices, sizeof(ballVertices));
    EBO ebo(indices, 6);
    VBLayout ballLayout;
    ballLayout.Push<float>(3);
    ballVAO.Append(ballVBO, ballLayout);

    // player one VAO
    VAO playerOneVAO;
    playerOneVAO.Bind();
    VBO playerVBO(playerVertices, sizeof(playerVertices));
    ebo.Bind();
    VBLayout playerLayout;
    playerLayout.Push<float>(3);
    playerOneVAO.Append(playerVBO, playerLayout);

    // player two VAO
    VAO playerTwoVAO;
    playerTwoVAO.Bind();
    ebo.Bind();
    playerTwoVAO.Append(playerVBO, playerLayout);

    Shader ballShader("shader.vert", "shader.frag");
    Shader playerOneShader("shader.vert", "shader.frag");
    Shader playerTwoShader("shader.vert", "shader.frag");

    ballShader.use();
    glm::mat4 ballModel = glm::mat4(1.0f);
    ballModel = glm::scale(ballModel, glm::vec3(0.1f));
    playerTwoShader.setMat4("model", ballModel);
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ballShader.use();
        ballVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        playerOneShader.use();
        glm::mat4 playerOneModel = glm::mat4(1.0f);
        playerOneModel = glm::translate(playerOneModel, glm::vec3(-0.95f, playerOneYOffset, 0.0f));
        playerOneModel = glm::scale(playerOneModel, glm::vec3(0.2f, 0.3f, 0.2f));
        playerOneShader.setMat4("model", playerOneModel);
        playerOneVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        playerTwoShader.use();
        glm::mat4 playerTwoModel = glm::mat4(1.0f);
        playerTwoModel = glm::translate(playerTwoModel, glm::vec3(0.95f, playerTwoYOffset, 0.0f));
        playerTwoModel = glm::scale(playerTwoModel, glm::vec3(0.2f, 0.3f, 0.2f));
        playerTwoShader.setMat4("model", playerTwoModel);
        playerTwoVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}



void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        if (playerOneYOffset <= 0.7f)
            playerOneYOffset += 0.7f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        if (playerOneYOffset >= -0.7f)
            playerOneYOffset -= 0.7f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        if (playerTwoYOffset <= 0.7f)
            playerTwoYOffset += 0.7f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        if (playerTwoYOffset >= -0.7f)
            playerTwoYOffset -= 0.7f * deltaTime;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}