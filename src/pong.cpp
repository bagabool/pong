#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VAO.h"
#include "VBO.h"
#include "VBLayout.h"
#include "Shader.h"
#include "Text.h"

#include <iostream>
#include <random>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void processBallMovement();
void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color, VAO& vao, VBO& vbo);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// players and ball offset positions
float playerOneYOffset = 0.0f;
float playerTwoYOffset = 0.0f;
float ballXOffset = 0.0f;
float ballYOffset = 0.0f;

// ball launching angle
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(0, 1);
static float randomAngle = glm::radians(0.0f);


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

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

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

    // ball vertices
    VAO ballVAO;
    ballVAO.Bind();
    VBO ballVBO(ballVertices, sizeof(ballVertices));
    EBO ebo(indices, 6);
    VBLayout ballLayout;
    ballLayout.Push<float>(3);
    ballVAO.Append(ballVBO, ballLayout);

    // player one vertices
    VAO playerOneVAO;
    playerOneVAO.Bind();
    VBO playerVBO(playerVertices, sizeof(playerVertices));
    ebo.Bind();
    VBLayout playerLayout;
    playerLayout.Push<float>(3);
    playerOneVAO.Append(playerVBO, playerLayout);

    // player two vertices
    VAO playerTwoVAO;
    playerTwoVAO.Bind();
    ebo.Bind();
    playerTwoVAO.Append(playerVBO, playerLayout);

    // glyphs
    VAO glyphVAO;
    glyphVAO.Bind();
    VBO glyphVBO(NULL, sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW);
    VBLayout glyphLayout;
    glyphLayout.Push<float>(4);
    glyphVAO.Append(glyphVBO, glyphLayout);

    Text text(glyphVAO, glyphVBO);
    text.Load(face);
    FT_Done_FreeType(ft);

    // shaders
    Shader ballShader("res/shaders/shader.vert", "res/shaders/shader.frag");
    Shader playerOneShader("res/shaders/shader.vert", "res/shaders/shader.frag");
    Shader playerTwoShader("res/shaders/shader.vert", "res/shaders/shader.frag");
    Shader glyphShader("res/shaders/glyph.vert", "res/shaders/glyph.frag");

    // ball launching side
    bool rightSide = distrib(gen) == 0;

    // ball launching angle
    if (rightSide) {
        std::uniform_real_distribution<> dis(-90.0, 90.0);
        randomAngle = glm::radians(dis(gen));
    }
    else {
        std::uniform_real_distribution<> dis(135.0, 225.0);
        randomAngle = glm::radians(dis(gen));
    }

    randomAngle = glm::radians(-70.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        processBallMovement();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ballShader.Use();
        glm::mat4 ballModel = glm::mat4(1.0f);
        ballModel = glm::translate(ballModel, glm::vec3(ballXOffset, ballYOffset, 0.0f));
        ballModel = glm::scale(ballModel, glm::vec3(0.1f));
        playerTwoShader.setMat4("model", ballModel);
        ballVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        playerOneShader.Use();
        glm::mat4 playerOneModel = glm::mat4(1.0f);
        playerOneModel = glm::translate(playerOneModel, glm::vec3(-0.95f, playerOneYOffset, 0.0f));
        playerOneModel = glm::scale(playerOneModel, glm::vec3(0.2f, 0.3f, 0.2f));
        playerOneShader.setMat4("model", playerOneModel);
        playerOneVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        playerTwoShader.Use();
        glm::mat4 playerTwoModel = glm::mat4(1.0f);
        playerTwoModel = glm::translate(playerTwoModel, glm::vec3(0.95f, playerTwoYOffset, 0.0f));
        playerTwoModel = glm::scale(playerTwoModel, glm::vec3(0.2f, 0.3f, 0.2f));
        playerTwoShader.setMat4("model", playerTwoModel);
        playerTwoVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        text.Render(glyphShader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        text.Render(glyphShader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        
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

void processBallMovement() {
    ballXOffset += cos(randomAngle) * deltaTime;
    ballYOffset += sin(randomAngle) * deltaTime;

    glm::vec3 direction = glm::normalize(glm::vec3(ballXOffset, ballYOffset, 0.0f));
    float playerOneMinYOffset = playerOneYOffset - 0.35f;
    float playerOneMaxYOffset = playerOneYOffset + 0.35f;
    float playerTwoMinYOffset = playerTwoYOffset - 0.35f;
    float playerTwoMaxYOffset = playerTwoYOffset + 0.35f;

    if (ballYOffset > 0.95f) {
        glm::vec3 yUpNormal = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 reflect = glm::reflect(direction, yUpNormal);
        randomAngle = atan2(reflect.y, reflect.x);
    }

    if (ballYOffset < -0.95f) {
        glm::vec3 yDownNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 reflect = glm::reflect(direction, yDownNormal);
        randomAngle = atan2(reflect.y, reflect.x);
    }

    if (ballXOffset > 0.9f) {
        if (ballYOffset >= playerTwoMinYOffset && ballYOffset <= playerTwoMaxYOffset) {
            glm::vec3 xRightNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
            glm::vec3 reflect = glm::reflect(direction, xRightNormal);
            randomAngle = atan2(reflect.y, reflect.x);
        }
        else {
            exit(0);
        }
    }

    if (ballXOffset < -0.9f) {
        if (ballYOffset >= playerOneMinYOffset && ballYOffset <= playerOneMaxYOffset) {
            glm::vec3 xLeftNormal = glm::vec3(1.0f, 0.0f, 0.0f);
            glm::vec3 reflect = glm::reflect(direction, xLeftNormal);
            randomAngle = atan2(reflect.y, reflect.x);
        }
        else {
            exit(0);
        }
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}