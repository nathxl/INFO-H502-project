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
int animationIndex = 0;
int animationDirection = 1;




// ---------------------------------------------------------------------------
// DECLARATIONS
// ---------------------------------------------------------------------------

// GLuint compileShader(std::string shaderCode, GLenum shaderType);
// GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);

void loadTexture(GLuint &texture, const char* path);

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
	// Loading textures
	// -----------------------------------------------------


	GLuint earthTexture1;
	loadTexture(earthTexture1, "../../src/textures/planets/earth1.jpg");
	GLuint earthTexture2;
	loadTexture(earthTexture2, "../../src/textures/planets/earth2.jpg");

	GLuint sunTextures[20];
	loadTexture(sunTextures[0], "../../src/textures/planets/ezgif-frame-001.jpg");
	loadTexture(sunTextures[1], "../../src/textures/planets/ezgif-frame-002.jpg");
	loadTexture(sunTextures[2], "../../src/textures/planets/ezgif-frame-003.jpg");
	loadTexture(sunTextures[3], "../../src/textures/planets/ezgif-frame-004.jpg");
	loadTexture(sunTextures[4], "../../src/textures/planets/ezgif-frame-005.jpg");
	loadTexture(sunTextures[5], "../../src/textures/planets/ezgif-frame-006.jpg");
	loadTexture(sunTextures[6], "../../src/textures/planets/ezgif-frame-007.jpg");
	loadTexture(sunTextures[7], "../../src/textures/planets/ezgif-frame-008.jpg");
	loadTexture(sunTextures[8], "../../src/textures/planets/ezgif-frame-009.jpg");
	loadTexture(sunTextures[9], "../../src/textures/planets/ezgif-frame-010.jpg");
	loadTexture(sunTextures[10], "../../src/textures/planets/ezgif-frame-011.jpg");
	loadTexture(sunTextures[11], "../../src/textures/planets/ezgif-frame-012.jpg");
	loadTexture(sunTextures[12], "../../src/textures/planets/ezgif-frame-013.jpg");
	loadTexture(sunTextures[13], "../../src/textures/planets/ezgif-frame-014.jpg");
	loadTexture(sunTextures[14], "../../src/textures/planets/ezgif-frame-015.jpg");
	loadTexture(sunTextures[15], "../../src/textures/planets/ezgif-frame-016.jpg");
	loadTexture(sunTextures[16], "../../src/textures/planets/ezgif-frame-017.jpg");
	loadTexture(sunTextures[17], "../../src/textures/planets/ezgif-frame-018.jpg");
	loadTexture(sunTextures[18], "../../src/textures/planets/ezgif-frame-019.jpg");
	loadTexture(sunTextures[19], "../../src/textures/planets/ezgif-frame-020.jpg");

	// -----------------------------------------------------
	// Earth Initialization
	// -----------------------------------------------------

   
	float earthAmbient = 0.0;
	float earthDiffuse = 0.9;
	float earthSpecular = 0.1;
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
	earth.inverseModel = inverseModel;


	// -----------------------------------------------------
	// Moon Initialization
	// -----------------------------------------------------

	float moonAmbient = 0.2;
	float moonDiffuse = 0.5;
	float moonSpecular = 1.;
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
	moon.inverseModel = glm::transpose(glm::inverse(moon.model));
	
	
	


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
	sun.inverseModel = glm::transpose(glm::inverse(sun.model));


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


	glm::vec3 light_pos = glm::vec3(.0, .0, .5);
	glm::mat4 view = wm.camera.GetViewMatrix();
	glm::mat4 perspective = wm.camera.GetProjectionMatrix(45.0, 16./9., 0.01, 100.0);//get the perspective in 16/9 ratio 
	

	glfwSwapInterval(1);


	// -----------------------------------------------------
	// MAIN LOOP
	// -----------------------------------------------------


	while (!glfwWindowShouldClose(wm.window)) {
		animationFrame++;
		// animationFrame++;
		
		if (animationFrame > 5){
			animationFrame = 0;
			animationIndex += animationDirection;
			if (animationIndex > 18 || animationIndex < 1){
				animationDirection = - animationDirection;				
			}
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
		earth.inverseModel = glm::transpose( glm::inverse(earth.model));

		earthShader.use();
		earthShader.setMatrix4("M", earth.model);
		earthShader.setMatrix4("itM", earth.inverseModel);
		earthShader.setMatrix4("V", view);
		earthShader.setMatrix4("P", perspective);
		earthShader.setVector3f("u_view_pos", wm.camera.Position);
		earthShader.setVector3f("u_light_pos", light_pos);

		glBindTexture(GL_TEXTURE_2D, earthTexture2);
		
		earth.draw();

		earth.model = glm::rotate(earth.model, glm::radians((float)(- 200.0 * now)), glm::vec3(0.0, 1.0, 0.0));
		earth.model = glm::translate(earth.model, glm::vec3(x1, 0.0, -y1));

		// --------------------------------------------
		// Moon Operations
		// --------------------------------------------
		

		moon.model = glm::translate(moon.model, glm::vec3(-xr, 0.0, yr));
		moon.model = glm::scale(moon.model, glm::vec3(0.5, 0.5, 0.5));
		moon.inverseModel = glm::transpose(glm::inverse(moon.model));
		moonShader.use();
		moonShader.setMatrix4("M", moon.model);
		moonShader.setMatrix4("itM", moon.inverseModel);
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
		sunShader.setMatrix4("itM", sun.inverseModel);
		sunShader.setMatrix4("V", view);
		sunShader.setMatrix4("P", perspective);
		sunShader.setVector3f("u_view_pos", wm.camera.Position);

		glBindTexture(GL_TEXTURE_2D, sunTextures[animationIndex]);
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


void loadTexture(GLuint &texture, const char* path){
    glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture loaded" << std::endl;
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

}