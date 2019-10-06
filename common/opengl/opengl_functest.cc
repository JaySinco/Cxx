#include <thread>
#include <chrono>
#include <algorithm>
#include "scenery.h"
#include <GLFW/glfw3.h>

#define RESOURCE(relpath) (std::string("D:\\Jaysinco\\Cxx\\common\\opengl\\resources\\")+relpath)

using namespace cxx::gl;

Storage *gStorage;
const int gWidth = 800;
const int gHeight = 600;
float initCameraX = 0.0f;
float initCameraY = 0.0f;
float initCameraZ = 0.0f;
float gCameraZ = 0.0f;
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
        gStorage->get<Object>("OB_item")->spin(0.15 * dz, dy, dx, 0);
    }
    gLastMouseX = xpos;
    gLastMouseY = ypos;
}

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//     if (key == GLFW_KEY_X) {
//         switch (action) {
//         case GLFW_PRESS:
//             break;
//         case GLFW_RELEASE:
//             break;
//         }
//     }
// }

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
            gStorage->get<Object>("OB_item")->reset();
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
    // glfwSetKeyCallback(window, key_callback);
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

struct SceneConfig {
    std::string id;
    std::string model_path;
    std::string texture_path;
    Light::Type light_type;
    glm::mat4 xform_default;
};

void setup(Storage &repo, const std::string &target) {
    SceneConfig options[] = {
        { 
            "porcelain",
            RESOURCE("models\\porcelain.obj"), 
            RESOURCE("textures\\porcelain.jpg"),
            Light::POINT,
            glm::mat4(1.0f),
        },
        { 
            "woman",
            RESOURCE("models\\woman.obj"), 
            RESOURCE("textures\\woman.png"),
            Light::GLOBAL,
            glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f))*
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        },
    };
    auto opt = std::find_if(std::begin(options), std::end(options), [&](const SceneConfig &c) {
        return c.id == target;
    });
    if (opt == std::end(options))
        opt = std::begin(options);
    repo.put<Shader>("SH_universe", RESOURCE("shaders\\universe.vs"), RESOURCE("shaders\\universe.fs"));
    repo.put<Model>("MO_model", opt->model_path);
    repo.put<Texture>("TE_texture", opt->texture_path);
    repo.put<Material>("MA_material", "TE_texture", 0.6f, 25.0f);
    auto object = repo.put<Object>("OB_item", "MO_model", "SH_universe", "MA_material", opt->xform_default);
    // camera
    auto scene = repo.put<Scenery>("SC_functest");
    scene->putObject("OB_item");
    auto rect = scene->getBoundRect(repo);
    float dx = rect.maxX-rect.lowX;
    float dy = rect.maxY-rect.lowY;
    float dz = rect.maxZ-rect.lowZ;
    float dxy = std::hypot(dx, dy);
    initCameraX = (rect.maxX+rect.lowX)/2;
    initCameraY = (rect.maxY+rect.lowY)/2;
    initCameraZ = rect.maxZ + dxy * 0.8f;
    gCameraZ = initCameraZ;
    gCameraMoveSpeed = std::hypot(dz, dxy) * 0.1f;
    repo.put<Camera>("CA_main", 45.0f, (float)gWidth/gHeight,
        gCameraMoveSpeed, gCameraMoveSpeed * 1000);
    scene->putCamera("CA_main");
    // light
    auto ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    auto diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
    auto specular = glm::vec3(1.0f, 1.0f, 1.0f);
    auto direction = glm::vec3(0.0f, 0.0f, -1.0f);
    repo.put<Light>("LI_global", glm::vec3(1.0f, 1.0, 1.0f));
    repo.put<Light>("LI_direct", direction, ambient, diffuse, specular);

    auto constant = 1.0f;
    auto linear = 0.09f;
    auto quadratic = 0.032f;
    auto position = glm::vec3(initCameraX, initCameraY, initCameraZ);
    repo.put<Light>("LI_point", position, ambient, diffuse,
        specular, constant, linear, quadratic);

    auto innerDegree = 1.5f;
    auto outerDegree = 15.6f;
    repo.put<Light>("LI_spot", position, direction, ambient,
        diffuse, specular, innerDegree, outerDegree);

    switch (opt->light_type) {
    case Light::DIRECT: scene->putLight("LI_direct"); break;
    case Light::POINT:  scene->putLight("LI_point"); break;
    case Light::SPOT:   scene->putLight("LI_spot"); break;
    case Light::GLOBAL: scene->putLight("LI_global"); break;
    }
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    GLFWwindow* window = initForWindow(gWidth, gHeight, "OpenGL_Functest");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    gStorage = new Storage("ST_global");
    if (argc < 2) setup(*gStorage, "porcelain");
    else setup(*gStorage, argv[1]);
    auto scene = gStorage->get<Scenery>("SC_functest");
    auto camera = gStorage->get<Camera>("CA_main");
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera->moveTo(initCameraX, initCameraY, gCameraZ);
        scene->render(*gStorage);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete gStorage;
    glfwTerminate();
    return 0;
}

