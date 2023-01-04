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




// ---------------------------------------------------------------------------
// DECLARATIONS
// ---------------------------------------------------------------------------

GLuint compileShader(std::string shaderCode, GLenum shaderType);
GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);



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
	// Earth Initialization
	// -----------------------------------------------------

	float earthAmbient = 0.1;
	float earthDiffuse = 0.5;
	float earthSpecular = 0.8;
	glm::vec3 earthColour = glm::vec3(0.5f, 0.6, 0.8);

	// Shader earthShader = Shader(celestialBodiesVertexShader, celestialBodiesFragmentShader);

	Shader earthShader = Shader(shaders.celestialBodiesVertexShader, shaders.celestialBodiesFragmentShader);

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
	earth.model = glm::translate(earth.model, glm::vec3(0.0, 0.0, -20.0));
	glm::mat4 inverseModel = glm::transpose(glm::inverse(earth.model));


	// -----------------------------------------------------
	// Sun Initialization
	// -----------------------------------------------------

	float sunAmbient = 0.9;
	float sunDiffuse = 0.9;
	float sunSpecular = 0.8;
	glm::vec3 sunColour = glm::vec3(0.9f, 0.5, 0.1);

	Shader sunShader = Shader(shaders.celestialBodiesVertexShader, shaders.celestialBodiesFragmentShader);
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

		// --------------------------------------------
		// Time operations
		// --------------------------------------------

		double now = glfwGetTime();
		auto delta = light_pos + glm::vec3(0.0, 0.0, 2 * std::sin(now));
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
		// Earth Operations
		// --------------------------------------------

		earth.model = glm::translate(earth.model, glm::vec3(0.0, 0.0, 20.0));
		earth.model = glm::rotate(earth.model, glm::radians((float)(1.0)), glm::vec3(0.0, 1.0, 0.0));
		earth.model = glm::translate(earth.model, glm::vec3(0.0, 0.0, -20.0));
		earthShader.use();
		earthShader.setMatrix4("M", earth.model);
		earthShader.setMatrix4("itM", inverseModel);
		earthShader.setMatrix4("V", view);
		earthShader.setMatrix4("P", perspective);
		earthShader.setVector3f("u_view_pos", wm.camera.Position);
		earth.draw();

		// --------------------------------------------
		// Sun Operations
		// --------------------------------------------

		sunShader.use();
		sunShader.setMatrix4("M", sun.model);
		sunShader.setMatrix4("itM", inverseModel);
		sunShader.setMatrix4("V", view);
		sunShader.setMatrix4("P", perspective);
		sunShader.setVector3f("u_view_pos", wm.camera.Position);
		sun.draw();

		// --------------------------------------------
		// CubeMap Operations
		// --------------------------------------------

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		cubeMapShader.setInteger("cubemapTexture", 0);
		glDepthFunc(GL_LEQUAL);
		cubeMapShader.use();
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