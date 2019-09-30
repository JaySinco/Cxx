#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include "shader.h"
#include "texture.h"
#include "object.h"
#define RESOURCE(relpath) ("D:\\Jaysinco\\Cxx\\common\\opengl\\resources\\" relpath)

using namespace cxx;

void glfw_error_callback(int error, const char* desc) {
    LOG(ERROR) << "glfw failed with code " << error << ", desc=\"" << desc << "\"";
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow *initForWindow(int width, int height, const std::string &title) {
    glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) {
		LOG(ERROR) << "failed to create glfw window";
		glfwTerminate();
		exit(-1);
	}
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // enable vsync
	// initialize OpenGL loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG(ERROR) << "failed to initialize glad";
		exit(-1);
	}
    return window;
}

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    GLFWwindow* window = initForWindow(800, 600, "OpenGL_Functest");
    Shader shader("shader", RESOURCE("shaders\\texture.vs"), RESOURCE("shaders\\texture.fs"));
    std::vector<float> vertices {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    std::vector<unsigned> indices {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    Object triangle("triangle", vertices, indices);
    Texture container("container", RESOURCE("textures\\container.jpg"));
    Texture awesomeface("awesomeface", RESOURCE("textures\\awesomeface.jpg"), true);
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        container.use(0);
        awesomeface.use(1);
        shader.use();
        triangle.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

