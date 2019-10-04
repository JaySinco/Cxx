#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <thread>
#include <chrono>
#include <glog/logging.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine.h"
#define RESOURCE(relpath) (std::string("D:\\Jaysinco\\Cxx\\common\\opengl\\resources\\")+relpath)

using namespace cxx;

Scene *gScene;
const int gWidth = 800;
const int gHeight = 600;
const float initCameraZ = 1.0f;
float gCameraZ = initCameraZ;
float gCameraMoveSpeed = 1.0f;
bool gLMousePressed = false;
double gLastMouseX = 0.0;
double gLastMouseY = 0.0;

void glfw_error_callback(int error, const char* desc) {
    LOG(ERROR) << "glfw failed with code " << error << ", desc=\"" << desc << "\"";
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    gCameraZ += -1 * gCameraMoveSpeed * yoffset;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (gLMousePressed) {
        float dx = xpos - gLastMouseX;
        float dy = ypos - gLastMouseY;
        float dz = std::hypot(dx, dy);
        gScene->rotateCamera(-0.15 * dz, dy, dx, 0);
    }
    gLastMouseX = xpos;
    gLastMouseY = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_X) {
        switch (action) {
        case GLFW_PRESS:
            gCameraMoveSpeed = 0.1f;
            break;
        case GLFW_RELEASE:
            gCameraMoveSpeed = 1.0f;
            break;
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
        switch (action) {
        case GLFW_PRESS:
            gLMousePressed = true;
            break;
        case GLFW_RELEASE:
            gLMousePressed = false;
            break;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        switch (action) {
        case GLFW_PRESS:
            gScene->resetCamera();
            gCameraZ = initCameraZ;
            break;
        }
    }
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
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
	// initialize OpenGL loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG(ERROR) << "failed to initialize glad";
		exit(-1);
	}
    return window;
}

void load(std::shared_ptr<Repository> repo, const std::string &target) {
    repo->addShader("shader", RESOURCE("shaders\\standard.vs"), RESOURCE("shaders\\standard.fs"));
    repo->addModel("model", RESOURCE("models\\"+target+".obj"));
    repo->addTexture("texture", RESOURCE("textures\\"+target+".jpg"));
}

void setup(Scene &scene, const std::string &target) {
    auto repo = std::make_shared<Repository>();
    load(repo, target);
    scene.select(repo);
    scene.setCamera(45.0f, (float)gWidth/gHeight, 0.1f, 100.0f);
    auto item = scene.newItem("item");
    item->load(Object::SHADER, "shader");
    item->load(Object::MODEL, "model");
    item->load(Object::TEXTURE, "texture");
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    GLFWwindow* window = initForWindow(gWidth, gHeight, "OpenGL_Functest");
    glEnable(GL_DEPTH_TEST);
    gScene = new Scene("OpenGL_Functest");
    if (argc < 2) setup(*gScene, "porcelain");
    else setup(*gScene, argv[1]);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gScene->moveCameraTo(0.0f, 0.0f, gCameraZ);
        gScene->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete gScene;
    glfwTerminate();
    return 0;
}

