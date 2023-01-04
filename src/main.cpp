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
#include "object.h"

#include <math.h>
#include <time.h>





// ---------------------------------------------------------------------------
// GLOBAL VARIABLES
// ---------------------------------------------------------------------------

const int width = 1280;
const int height = 720;

bool firstMouse = true;

float lastX = 0.0;
float lastY = 0.0;






// ---------------------------------------------------------------------------
// SHADERS
// ---------------------------------------------------------------------------

const std::string sourceV =
"#version 330 core\n"
"in vec3 position; \n"
"in vec2 tex_coords; \n"
"in vec3 normal; \n"

"out vec3 v_frag_coord; \n"
"out vec3 v_normal; \n"

"uniform mat4 M; \n"
"uniform mat4 itM; \n"
"uniform mat4 V; \n"
"uniform mat4 P; \n"


" void main(){ \n"
"vec4 frag_coord = M*vec4(position, 1.0); \n"
"gl_Position = P*V*frag_coord; \n"
"v_normal = vec3(itM * vec4(normal, 1.0)); \n"
"v_frag_coord = frag_coord.xyz; \n"
"\n"
"}\n";



const std::string sourceF = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"precision mediump float; \n"

		"in vec3 v_frag_coord; \n"
		"in vec3 v_normal; \n"

		"uniform vec3 u_view_pos; \n"

		//In GLSL you can use structures to better organize your code
		//light
		"struct Light{\n" 
		"vec3 light_pos; \n"
		"float ambient_strength; \n"
		"float diffuse_strength; \n"
		"float specular_strength; \n"
		//attenuation factor
		"float constant;\n"
		"float linear;\n"
		"float quadratic;\n"
		"};\n"
		"uniform Light light;"

		"uniform float shininess; \n"
		"uniform vec3 materialColour; \n"


		"float specularCalculation(vec3 N, vec3 L, vec3 V ){ \n"
			"vec3 R = reflect (-L,N);  \n " //reflect (-L,N) is  equivalent to //max (2 * dot(N,L) * N - L , 0.0) ;
			"float cosTheta = dot(R , V); \n"
			"float spec = pow(max(cosTheta,0.0), 32.0); \n"
			"return light.specular_strength * spec;\n"
		"}\n"

		"void main() { \n"
		"vec3 N = normalize(v_normal);\n"
		"vec3 L = normalize(light.light_pos - v_frag_coord) ; \n"
		"vec3 V = normalize(u_view_pos - v_frag_coord); \n"
		"float specular = specularCalculation( N, L, V); \n"
		"float diffuse = light.diffuse_strength * max(dot(N,L),0.0);\n"
		"float distance = length(light.light_pos - v_frag_coord);"
		"float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);"
		"float light = light.ambient_strength + attenuation * (diffuse + specular); \n"
		"FragColor = vec4(materialColour * vec3(light), 1.0); \n"
		"} \n";


const std::string sourceVCubeMap =
"#version 330 core\n"
"in vec3 position; \n"
"in vec2 tex_coords; \n"
"in vec3 normal; \n"

//only P and V are necessary
"uniform mat4 V; \n"
"uniform mat4 P; \n"

"out vec3 texCoord_v; \n"

" void main(){ \n"
"texCoord_v = position;\n"
//remove translation info from view matrix to only keep rotation
"mat4 V_no_rot = mat4(mat3(V)) ;\n"
"vec4 pos = P * V_no_rot * vec4(position, 1.0); \n"
// the positions xyz are divided by w after the vertex shader
// the z component is equal to the depth value
// we want a z always equal to 1.0 here, so we set z = w!
// Remember: z=1.0 is the MAXIMUM depth value ;)
"gl_Position = pos.xyww;\n"
"\n"
"}\n";



const std::string sourceFCubeMap =
"#version 330 core\n"
"out vec4 FragColor;\n"
"precision mediump float; \n"
"uniform samplerCube cubemapSampler; \n"
"in vec3 texCoord_v; \n"
"void main() { \n"
"FragColor = texture(cubemapSampler,texCoord_v); \n"
"} \n";





// ---------------------------------------------------------------------------
// DECLARATIONS
// ---------------------------------------------------------------------------

GLuint compileShader(std::string shaderCode, GLenum shaderType);
GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
void processInput(GLFWwindow* window);
void loadCubemapFace(const char* file, const GLenum& targetCube);
Camera camera(glm::vec3(0.0, 0.0, 10.0));




// ---------------------------------------------------------------------------
// DEBUG
// ---------------------------------------------------------------------------

#ifndef NDEBUG
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam){
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source){
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type){
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity){
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
#endif




// ---------------------------------------------------------------------------
// MAIN
// ---------------------------------------------------------------------------






int main(int argc, char* argv[]){


	// -----------------------------------------------------
	// Initialization
	// -----------------------------------------------------

	std::cout << "Welcome to exercice 10: " << std::endl;
	std::cout << "Implement refraction on an object\n" << std::endl;


	//Boilerplate
	//Create the OpenGL context 
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialise GLFW \n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifndef NDEBUG
		//create a debug context to help with Debugging
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	#endif
	

	//Create the window
	GLFWwindow* window = glfwCreateWindow(width, height, "Exercise 10", nullptr, nullptr);
	if (window == NULL){
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window\n");
	}

	glfwMakeContextCurrent(window);

	//load openGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glEnable(GL_DEPTH_TEST);

	#ifndef NDEBUG
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT){
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	#endif


	// -----------------------------------------------------
	// Earth Initialization
	// -----------------------------------------------------

	float earthAmbient = 0.1;
	float earthDiffuse = 0.5;
	float earthSpecular = 0.8;
	glm::vec3 earthColour = glm::vec3(0.5f, 0.6, 0.8);

	Shader earthShader = Shader(sourceV, sourceF);
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

	Shader sunShader = Shader(sourceV, sourceF);
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

	Shader cubeMapShader = Shader(sourceVCubeMap, sourceFCubeMap);
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
		loadCubemapFace(pair.first.c_str(), pair.second);
	}


	// -----------------------------------------------------
	// FPS function
	// -----------------------------------------------------


	double prev = 0;
	int deltaFrame = 0;
	//fps function
	auto fps = [&](double now) {
		double deltaTime = now - prev;
		deltaFrame++;
		if (deltaTime > 0.5) {
			prev = now;
			const double fpsCount = (double)deltaFrame / deltaTime;
			deltaFrame = 0;
			std::cout << "\r FPS: " << fpsCount;
			std::cout.flush();
		}
	};

	glm::vec3 light_pos = glm::vec3(1.0, 2.0, 1.5);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 perspective = camera.GetProjectionMatrix(45.0, 16./9., 0.01, 100.0);//get the perspective in 16/9 ratio 
	

	glfwSwapInterval(1);


	// -----------------------------------------------------
	// MAIN LOOP
	// -----------------------------------------------------


	while (!glfwWindowShouldClose(window)) {

		// --------------------------------------------
		// Time operations
		// --------------------------------------------

		double now = glfwGetTime();
		auto delta = light_pos + glm::vec3(0.0, 0.0, 2 * std::sin(now));
		fps(now);

		// --------------------------------------------
		// ...
		// --------------------------------------------

		processInput(window);
		view = camera.GetViewMatrix();
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
		earthShader.setVector3f("u_view_pos", camera.Position);
		earth.draw();

		// --------------------------------------------
		// Sun Operations
		// --------------------------------------------

		sunShader.use();
		sunShader.setMatrix4("M", sun.model);
		sunShader.setMatrix4("itM", inverseModel);
		sunShader.setMatrix4("V", view);
		sunShader.setMatrix4("P", perspective);
		sunShader.setVector3f("u_view_pos", camera.Position);
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

		glfwSwapBuffers(window);
	}

	// --------------------------------------------
	// clean up ressource and END
	// --------------------------------------------

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}





void loadCubemapFace(const char* path, const GLenum& targetFace){
	int imWidth, imHeight, imNrChannels;
	unsigned char* data = stbi_load(path, &imWidth, &imHeight, &imNrChannels, 0);
	if (data)
	{

		glTexImage2D(targetFace, 0, GL_RGB, imWidth, imHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(targetFace);
	}
	else {
		std::cout << "Failed to Load texture" << std::endl;
		const char* reason = stbi_failure_reason();
		std::cout << reason << std::endl;
	}
	stbi_image_free(data);
}







void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	// std::cout << "test2 : " << xoffset << std::endl;

	camera.ProcessMouseMovement(xoffset, yoffset);
}







void processInput(GLFWwindow* window) {
	//3. Use the cameras class to change the parameters of the camera
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(LEFT, 0.1);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(RIGHT, 0.1);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(FORWARD, 0.1);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(BACKWARD, 0.1);
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(1, 0.0, 1);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(-1, 0.0, 1);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, 1.0, 1);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, -1.0, 1);
	

	double m_xpos, m_ypos;
	glfwGetCursorPos(window, &m_xpos, &m_ypos);
	mouse_callback(window, m_xpos, m_ypos);	

}



