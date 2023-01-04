#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H


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


class WindowManager{
    public:
        const int width = 1280;
        const int height = 720;

        bool firstMouse = true;

        float lastX = 0.0;
        float lastY = 0.0;

        double prev = 0;
        int deltaFrame = 0;

        Camera camera = Camera(glm::vec3(0.0, 0.0, 10.0));

        GLFWwindow* window;


        WindowManager(){
            std::cout << "Hello from the window manager : " << width << std::endl;
        }


        void init(){
            
            
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
            window = glfwCreateWindow(width, height, "Exercise 10", nullptr, nullptr);
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

        }

        void my_fps(double now){
            double deltaTime = now - prev;
            deltaFrame++;
            if (deltaTime > 0.5) {
                prev = now;
                const double fpsCount = (double)deltaFrame / deltaTime;
                deltaFrame = 0;
                std::cout << "\r FPS: " << fpsCount;
                std::cout.flush();
            }
        }

        void loadCubemapFace(const char* path, const GLenum& targetFace){
            int imWidth, imHeight, imNrChannels;
            unsigned char* data = stbi_load(path, &imWidth, &imHeight, &imNrChannels, 0);
            if (data){
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
            if (firstMouse){
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
            lastX = xpos;
            lastY = ypos;
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

    private:

};

#endif