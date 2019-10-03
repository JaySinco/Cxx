#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <thread>
#include <chrono>
#include <glog/logging.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine.h"
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == NULL) {
		LOG(ERROR) << "failed to create glfw window";
		glfwTerminate();
		exit(-1);
	}
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// initialize OpenGL loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG(ERROR) << "failed to initialize glad";
		exit(-1);
	}
    return window;
}

void loadResource(std::shared_ptr<Repository> repo) {
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
    repo->addShader("shader", RESOURCE("shaders\\standard.vs"), RESOURCE("shaders\\standard.fs"));
    const std::vector<Model::VERTEX_ATTR> attr = { Model::POS, Model::COLOR, Model::TXR };
    repo->addModel("two triangle", vertices, indices, attr);
    repo->addTexture("container", RESOURCE("textures\\container.jpg"));
    repo->addTexture("awesomeface", RESOURCE("textures\\awesomeface.jpg"), true);
}

void setupScene(Scene &scene) {
    auto repo = std::make_shared<Repository>();
    loadResource(repo);
    scene.select(repo);
    scene.setCamera(45.0f, 800/ 600, 0.1f, 100.0f);
    scene.moveCameraTo(0.0f, 0.0f, 3.0f);
    auto rect = scene.newItem("rect");
    rect->load(Object::SHADER, "shader");
    rect->load(Object::MODEL, "two triangle");
    rect->load(Object::TEXTURE, "container");
    rect->load(Object::TEXTURE, "awesomeface");
}

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    GLFWwindow* window = initForWindow(800, 600, "OpenGL_Functest");

    Scene scene("test");
    setupScene(scene);

    auto start = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.render();

        glfwSwapBuffers(window);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        (elapsed.count() < 50) ? glfwPollEvents() : glfwWaitEvents();
    }

    glfwTerminate();
    return 0;
}

