#pragma once
#include <map>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VBO.h"
#include "Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H 

struct Character {
    unsigned int id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class Text
{
public:
    Text(VAO& vao, VBO& vbo) : vao(vao), vbo(vbo) {};
    void Load(FT_Face& face);
    void Append(VAO& vao, VBO& vbo);
    void Render(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);
private:
    std::map<char, Character> Characters;
    VAO vao;
    VBO vbo;
};

