// ---------------------------------------------------------------------------
// IMPORTS
// ---------------------------------------------------------------------------


#include <iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <map>

#include "camera.h"
#include "shader.h"
#include "shaders.h"
#include "object.h"
#include "windowManager.h"


#include <math.h>
#include <time.h>


// ---------------------------------------------------------------------------
// GLOBAL VARIABLES
// ---------------------------------------------------------------------------


Shaders shaders;

int animationFrame = 0;




// ---------------------------------------------------------------------------
// DECLARATIONS
// ---------------------------------------------------------------------------

// GLuint compileShader(std::string shaderCode, GLenum shaderType);
// GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);



// ---------------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------------


int main(int argc, char* argv[]){

	// -----------------------------------------------------
	// Initialization
	// -----------------------------------------------------


	std::cout << "Welcome to our project : " << std::endl;
	WindowManager wm;
	wm.init();


	// -----------------------------------------------------
	// Earth texture 1
    // -----------------------------------------------------

	GLuint earthTexture1;

   
    glGenTextures(1, &earthTexture1);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthTexture1);
	

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    unsigned char *data = stbi_load("../../src/textures/planets/earth1.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// Earth texture 2
    // -----------------------------------------------------

	GLuint earthTexture2;

   
    glGenTextures(1, &earthTexture2);
	glBindTexture(GL_TEXTURE_2D, earthTexture2);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/earth2.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 1
    // -----------------------------------------------------

	GLuint sunTexture1;

    glGenTextures(1, &sunTexture1);
	glBindTexture(GL_TEXTURE_2D, sunTexture1);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-001.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 2
    // -----------------------------------------------------

	GLuint sunTexture2;

    glGenTextures(1, &sunTexture2);
	glBindTexture(GL_TEXTURE_2D, sunTexture2);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-002.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 3
    // -----------------------------------------------------

	GLuint sunTexture3;

    glGenTextures(1, &sunTexture3);
	glBindTexture(GL_TEXTURE_2D, sunTexture3);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-003.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 4
    // -----------------------------------------------------

	GLuint sunTexture4;

    glGenTextures(1, &sunTexture4);
	glBindTexture(GL_TEXTURE_2D, sunTexture4);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-004.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

	// -----------------------------------------------------
	// sun texture 5
    // -----------------------------------------------------

	GLuint sunTexture5;

    glGenTextures(1, &sunTexture5);
	glBindTexture(GL_TEXTURE_2D, sunTexture5);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-005.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 6
    // -----------------------------------------------------

	GLuint sunTexture6;

    glGenTextures(1, &sunTexture6);
	glBindTexture(GL_TEXTURE_2D, sunTexture6);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-006.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 7
    // -----------------------------------------------------

	GLuint sunTexture7;

    glGenTextures(1, &sunTexture7);
	glBindTexture(GL_TEXTURE_2D, sunTexture7);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-007.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 8
    // -----------------------------------------------------

	GLuint sunTexture8;

    glGenTextures(1, &sunTexture8);
	glBindTexture(GL_TEXTURE_2D, sunTexture8);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-008.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 9
    // -----------------------------------------------------

	GLuint sunTexture9;

    glGenTextures(1, &sunTexture9);
	glBindTexture(GL_TEXTURE_2D, sunTexture9);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-009.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 10
    // -----------------------------------------------------

	GLuint sunTexture10;

    glGenTextures(1, &sunTexture10);
	glBindTexture(GL_TEXTURE_2D, sunTexture10);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-010.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



	// -----------------------------------------------------
	// sun texture 11
    // -----------------------------------------------------

	GLuint sunTexture11;

    glGenTextures(1, &sunTexture11);
	glBindTexture(GL_TEXTURE_2D, sunTexture11);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-011.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 12
    // -----------------------------------------------------

	GLuint sunTexture12;

    glGenTextures(1, &sunTexture12);
	glBindTexture(GL_TEXTURE_2D, sunTexture12);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-012.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 13
    // -----------------------------------------------------

	GLuint sunTexture13;

    glGenTextures(1, &sunTexture13);
	glBindTexture(GL_TEXTURE_2D, sunTexture13);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-013.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



	// -----------------------------------------------------
	// sun texture 14
    // -----------------------------------------------------

	GLuint sunTexture14;

    glGenTextures(1, &sunTexture14);
	glBindTexture(GL_TEXTURE_2D, sunTexture14);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-014.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



	// -----------------------------------------------------
	// sun texture 15
    // -----------------------------------------------------

	GLuint sunTexture15;

    glGenTextures(1, &sunTexture15);
	glBindTexture(GL_TEXTURE_2D, sunTexture15);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-015.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 16
    // -----------------------------------------------------

	GLuint sunTexture16;

    glGenTextures(1, &sunTexture16);
	glBindTexture(GL_TEXTURE_2D, sunTexture16);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-016.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 17
    // -----------------------------------------------------

	GLuint sunTexture17;

    glGenTextures(1, &sunTexture17);
	glBindTexture(GL_TEXTURE_2D, sunTexture17);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-017.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



	// -----------------------------------------------------
	// sun texture 18
    // -----------------------------------------------------

	GLuint sunTexture18;

    glGenTextures(1, &sunTexture18);
	glBindTexture(GL_TEXTURE_2D, sunTexture18);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-018.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 19
    // -----------------------------------------------------

	GLuint sunTexture19;

    glGenTextures(1, &sunTexture19);
	glBindTexture(GL_TEXTURE_2D, sunTexture19);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-019.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// sun texture 20
    // -----------------------------------------------------

	GLuint sunTexture20;

    glGenTextures(1, &sunTexture20);
	glBindTexture(GL_TEXTURE_2D, sunTexture20);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("../../src/textures/planets/ezgif-frame-020.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


	// -----------------------------------------------------
	// Earth Initialization
	// -----------------------------------------------------

   
	float earthAmbient = 0.2;
	float earthDiffuse = 0.5;
	float earthSpecular = 0.8;
	glm::vec3 earthColour = glm::vec3(0.5f, 0.6, 0.8);

	Shader earthShader = Shader(shaders.TextureVertexShader, shaders.TextureFragmentShader);

	earthShader.use();
	
	earthShader.setFloat("shininess", 32.0f);
	earthShader.setVector3f("materialColour", earthColour);
	earthShader.setFloat("light.ambient_strength", earthAmbient);
	earthShader.setFloat("light.diffuse_strength", earthDiffuse);
	earthShader.setFloat("light.specular_strength", earthSpecular);
	earthShader.setFloat("light.constant", 1.0);
	earthShader.setFloat("light.linear", 0.14);
	earthShader.setFloat("light.quadratic", 0.07);
	earthShader.setFloat("refractionIndice", 1.52);
	
	char path_to_sphere_obj[] = "../../src/objects/sphere_smooth.obj";
	Object earth(path_to_sphere_obj);
	earth.makeObject(earthShader);
	glm::mat4 inverseModel = glm::transpose(glm::inverse(earth.model));


	// -----------------------------------------------------
	// Moon Initialization
	// -----------------------------------------------------

	float moonAmbient = 0.2;
	float moonDiffuse = 0.5;
	float moonSpecular = 0.8;
	glm::vec3 moonColour = glm::vec3(0.6f, 0.6, 0.6);

	Shader moonShader = Shader(shaders.celestialBodiesVertexShader, shaders.celestialBodiesFragmentShader);

	moonShader.use();
	moonShader.setFloat("shininess", 32.0f);
	moonShader.setVector3f("materialColour", moonColour);
	moonShader.setFloat("light.ambient_strength", moonAmbient);
	moonShader.setFloat("light.diffuse_strength", moonDiffuse);
	moonShader.setFloat("light.specular_strength", moonSpecular);
	moonShader.setFloat("light.constant", 1.0);
	moonShader.setFloat("light.linear", 0.14);
	moonShader.setFloat("light.quadratic", 0.07);
	moonShader.setFloat("refractionIndice", 1.52);
	
	Object moon(path_to_sphere_obj);
	moon.makeObject(moonShader);
	inverseModel = glm::transpose(glm::inverse(moon.model));
	


	// -----------------------------------------------------
	// Sun Initialization
	// -----------------------------------------------------

	float sunAmbient = 0.5;
	float sunDiffuse = 0.9;
	float sunSpecular = 0.8;
	glm::vec3 sunColour = glm::vec3(0.9f, 0.5, 0.1);

	// Shader sunShader = Shader(shaders.celestialBodiesVertexShader, shaders.celestialBodiesFragmentShader);
	Shader sunShader = Shader(shaders.TextureVertexShader, shaders.TextureFragmentShader);


	sunShader.use();
	sunShader.setFloat("shininess", 32.0f);
	sunShader.setVector3f("materialColour", sunColour);
	sunShader.setFloat("light.ambient_strength", sunAmbient);
	sunShader.setFloat("light.diffuse_strength", sunDiffuse);
	sunShader.setFloat("light.specular_strength", sunSpecular);
	sunShader.setFloat("light.constant", 1.0);
	sunShader.setFloat("light.linear", 0.14);
	sunShader.setFloat("light.quadratic", 0.07);
	sunShader.setFloat("refractionIndice", 1.52);

	Object sun(path_to_sphere_obj);
	sun.makeObject(sunShader);
	sun.model = glm::scale(sun.model, glm::vec3(3.0, 3.0, 3.0));


	// -----------------------------------------------------
	// CubeMap Initialization
	// -----------------------------------------------------

	Shader cubeMapShader = Shader(shaders.cubeMapVertexShader, shaders.cubeMapFragmentShader);
	char pathCube[] = "../../src/objects/cube.obj";
	Object cubeMap(pathCube);
	cubeMap.makeObject(cubeMapShader);

	GLuint cubeMapTexture;
	glGenTextures(1, &cubeMapTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

	// texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(true);

	std::string pathToCubeMap = "../../src/textures/cubemaps/space/";

	
	std::map<std::string, GLenum> facesToLoad = {
		{pathToCubeMap + "posx.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_X},
		{pathToCubeMap + "posy.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
		{pathToCubeMap + "posz.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
		{pathToCubeMap + "negx.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
		{pathToCubeMap + "negy.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
		{pathToCubeMap + "negz.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
	};

	//load the six faces
	for (std::pair<std::string, GLenum> pair : facesToLoad) {
		wm.loadCubemapFace(pair.first.c_str(), pair.second);
	}


	glm::vec3 light_pos = glm::vec3(1.0, 2.0, 1.5);
	glm::mat4 view = wm.camera.GetViewMatrix();
	glm::mat4 perspective = wm.camera.GetProjectionMatrix(45.0, 16./9., 0.01, 100.0);//get the perspective in 16/9 ratio 
	

	glfwSwapInterval(1);


	// -----------------------------------------------------
	// MAIN LOOP
	// -----------------------------------------------------


	while (!glfwWindowShouldClose(wm.window)) {
		animationFrame++;
		animationFrame++;
		if (animationFrame > 100){
			animationFrame = 0;
		}

		// --------------------------------------------
		// Time operations
		// --------------------------------------------

		double now = glfwGetTime();
		// auto delta = light_pos + glm::vec3(0.0, 0.0, 2 * std::sin(now));
		wm.my_fps(now);

		// --------------------------------------------
		// ...
		// --------------------------------------------

		wm.processInput();

		view = wm.camera.GetViewMatrix();
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// --------------------------------------------
		// Vector Operations
		// --------------------------------------------

		// v1 : sun-earth verctor
		// v2 : earth-moon vector

		// spherical to cartesian coordinates of v1 and v2
		double r1 = 10.0;
		double phi1 = now;
		double x1 = r1 * std::cos(phi1);
		double y1 = r1 * std::sin(phi1);

		double r2 = 3.0;
		double phi2 = 3*now;
		double x2 = r2 * std::cos(phi2);
		double y2 = r2 * std::sin(phi2);

		// vr = v1 + v2
		double xr = x1 + x2;
		double yr = y1 + y2;

		// cartesian to spherical coordinates of vr

		double phir;
		if (std::signbit(yr)){
			phir = - std::acos(xr / std::sqrt(xr*xr + yr*yr));
		} else {
			phir = + std::acos(xr / std::sqrt(xr*xr + yr*yr));
		}

		double rr = std::sqrt(xr*xr + yr*yr);

		// --------------------------------------------
		// Earth Operations
		// --------------------------------------------

		
		
		earth.model = glm::translate(earth.model, glm::vec3(-x1, 0.0, y1));
		earth.model = glm::rotate(earth.model, glm::radians((float)(200.0 * now)), glm::vec3(0.0, 1.0, 0.0));

		earthShader.use();
		earthShader.setMatrix4("M", earth.model);
		earthShader.setMatrix4("itM", inverseModel);
		earthShader.setMatrix4("V", view);
		earthShader.setMatrix4("P", perspective);
		earthShader.setVector3f("u_view_pos", wm.camera.Position);

		glBindTexture(GL_TEXTURE_2D, earthTexture2);
		
		earth.draw();

		earth.model = glm::rotate(earth.model, glm::radians((float)(- 200.0 * now)), glm::vec3(0.0, 1.0, 0.0));
		earth.model = glm::translate(earth.model, glm::vec3(x1, 0.0, -y1));

		// --------------------------------------------
		// Moon Operations
		// --------------------------------------------
		

		moon.model = glm::translate(moon.model, glm::vec3(-xr, 0.0, yr));
		moon.model = glm::scale(moon.model, glm::vec3(0.5, 0.5, 0.5));
		moonShader.use();
		moonShader.setMatrix4("M", moon.model);
		moonShader.setMatrix4("itM", inverseModel);
		moonShader.setMatrix4("V", view);
		moonShader.setMatrix4("P", perspective);
		moonShader.setVector3f("u_view_pos", wm.camera.Position);
		moon.draw();
		moon.model = glm::scale(moon.model, glm::vec3(2.0, 2.0, 2.0));
		moon.model = glm::translate(moon.model, glm::vec3(xr, 0.0, -yr));


		// --------------------------------------------
		// Sun Operations
		// --------------------------------------------

		sunShader.use();
		sunShader.setMatrix4("M", sun.model);
		sunShader.setMatrix4("itM", inverseModel);
		sunShader.setMatrix4("V", view);
		sunShader.setMatrix4("P", perspective);
		sunShader.setVector3f("u_view_pos", wm.camera.Position);
		glBindTexture(GL_TEXTURE_2D, sunTexture1);
		if (animationFrame <= 5){
			glBindTexture(GL_TEXTURE_2D, sunTexture1);
		} else if(animationFrame > 5 && animationFrame <= 10) {
			glBindTexture(GL_TEXTURE_2D, sunTexture2);
		} else if(animationFrame > 10 && animationFrame <= 15) {
			glBindTexture(GL_TEXTURE_2D, sunTexture3);
		} else if(animationFrame > 15 && animationFrame <= 20) {
			glBindTexture(GL_TEXTURE_2D, sunTexture4);
		} else if(animationFrame > 20 && animationFrame <= 25) {
			glBindTexture(GL_TEXTURE_2D, sunTexture5);
		} else if(animationFrame > 25 && animationFrame <= 30) {
			glBindTexture(GL_TEXTURE_2D, sunTexture6);
		} else if(animationFrame > 30 && animationFrame <= 35) {
			glBindTexture(GL_TEXTURE_2D, sunTexture7);
		} else if(animationFrame > 35 && animationFrame <= 40) {
			glBindTexture(GL_TEXTURE_2D, sunTexture8);
		} else if(animationFrame > 40 && animationFrame <= 45) {
			glBindTexture(GL_TEXTURE_2D, sunTexture9);
		} else if(animationFrame > 45 && animationFrame <= 50) {
			glBindTexture(GL_TEXTURE_2D, sunTexture10);
		} else if(animationFrame > 50 && animationFrame <= 55) {
			glBindTexture(GL_TEXTURE_2D, sunTexture11);
		} else if(animationFrame > 55 && animationFrame <= 60) {
			glBindTexture(GL_TEXTURE_2D, sunTexture12);
		} else if(animationFrame > 60 && animationFrame <= 65) {
			glBindTexture(GL_TEXTURE_2D, sunTexture13);
		} else if(animationFrame > 65 && animationFrame <= 70) {
			glBindTexture(GL_TEXTURE_2D, sunTexture14);
		} else if(animationFrame > 70 && animationFrame <= 75) {
			glBindTexture(GL_TEXTURE_2D, sunTexture15);
		} else if(animationFrame > 75 && animationFrame <= 80) {
			glBindTexture(GL_TEXTURE_2D, sunTexture16);
		} else if(animationFrame > 80 && animationFrame <= 85) {
			glBindTexture(GL_TEXTURE_2D, sunTexture17);
		} else if(animationFrame > 85 && animationFrame <= 90) {
			glBindTexture(GL_TEXTURE_2D, sunTexture18);
		} else if(animationFrame > 90 && animationFrame <= 95) {
			glBindTexture(GL_TEXTURE_2D, sunTexture19);
		} else {
			glBindTexture(GL_TEXTURE_2D, sunTexture20);
		}

		sun.draw();

		// --------------------------------------------
		// CubeMap Operations
		// --------------------------------------------

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		cubeMapShader.use();
		cubeMapShader.setInteger("cubemapTexture", 0);
		glDepthFunc(GL_LEQUAL);		
		cubeMapShader.setMatrix4("V", view);
		cubeMapShader.setMatrix4("P", perspective);
		cubeMapShader.setInteger("cubemapTexture", 0);
		cubeMap.draw();
		glDepthFunc(GL_LESS);

		// --------------------------------------------
		// Swap buffers
		// --------------------------------------------

		glfwSwapBuffers(wm.window);
	}

	// --------------------------------------------
	// clean up ressource and END
	// --------------------------------------------

	glfwDestroyWindow(wm.window);
	glfwTerminate();

	return 0;
}