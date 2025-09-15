#ifndef BMP_READER_HPP
#define BMP_READER_HPP

#include <cstdio>
#include <GL/glew.h>

// Loads a bmp file adn makes the texture
GLuint LoadBMPTexture(const char* imagepath) {
    // lots of debugging
    printf("Reading image %s\n", imagepath);

    // ppen the file in binary read
    FILE* file = fopen(imagepath, "rb");
    if (!file) {
        printf("Image could not be opened\n");
        return 0;
    }

    // read the BMP header
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file (header too short)\n");
        return 0;
    }

    // Check the file starts with 'BM'
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file (missing BM)\n");
        return 0;
    }

    // Extract header data
    unsigned int imageSize = *(int*)&(header[0x22]);
    unsigned int width = *(int*)&(header[0x12]);
    unsigned int height = *(int*)&(header[0x16]);

    // allocate memory to hold pixel data
    unsigned char* data = new unsigned char[imageSize];

    // read actual pixel data
    fread(data, 1, imageSize, file);

    // close the file
    fclose(file);

    // generate OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);               // unique ID
    glBindTexture(GL_TEXTURE_2D, textureID);    // bind it

    // upload the texture to the GPU
    // target, miplamp lvl, internal format in GPU, dimensions, border, input format, input data type, pointer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // how it is scaled when magnified
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // how it is scaled when minified
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       // next two are wrap behaviour (H and V)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // free the laoded image and return texture ID
    delete[] data;
    return textureID;
}


#endif
