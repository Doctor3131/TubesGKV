#include "textureloader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <glad/glad.h>

GLuint loadTexture(const std::string& texturePath) {
    int width, height, channels;
    
    stbi_set_flip_vertically_on_load(true);
    
    // FORCE 4 channels (RGBA) untuk semua tekstur
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &channels, 4);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return 0;
    }
    
    std::cout << "Loaded texture: " << texturePath << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // SELALU pakai RGBA karena kita force 4 channels di atas
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error after loading texture: " << error << std::endl;
    }
    
    stbi_image_free(data);
    return textureID;
}