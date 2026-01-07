#include "Glimpse 4D.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Camera.h"
#include "MeshData.h"

#include <iostream>
#include <vector>

using namespace std;

/**
 * Project a 4D point into 3D using W-perspective
 */
Vec3 project4Dto3D(const Vec4& p, float fov)
{
    // perspective along W (symmetric)
    float w = p.w + fov;

    if (w <= 0.001f)
        return { 0.f, 0.f, 0.f };

    float s = fov / w;

    return {
        p.x * s,
        p.y * s,
        p.z * s
    };
}

void updateCameraControls(GLFWwindow* window, Camera4D& cam, float dt)
{
    float s = 1.5f * dt;

    if (glfwGetKey(window, GLFW_KEY_A)) cam.rotation = Rot4::Plane(0, 1, s) * cam.rotation;
    if (glfwGetKey(window, GLFW_KEY_D)) cam.rotation = Rot4::Plane(0, 1, -s) * cam.rotation;

    if (glfwGetKey(window, GLFW_KEY_W)) cam.rotation = Rot4::Plane(0, 2, s) * cam.rotation;
    if (glfwGetKey(window, GLFW_KEY_S)) cam.rotation = Rot4::Plane(0, 2, -s) * cam.rotation;

    // ⭐ 4D rotations (THIS is what you were missing)
    if (glfwGetKey(window, GLFW_KEY_Q)) cam.rotation = Rot4::Plane(0, 3, s) * cam.rotation;
    if (glfwGetKey(window, GLFW_KEY_E)) cam.rotation = Rot4::Plane(0, 3, -s) * cam.rotation;

    if (glfwGetKey(window, GLFW_KEY_R)) cam.rotation = Rot4::Plane(1, 3, s) * cam.rotation;
    if (glfwGetKey(window, GLFW_KEY_F)) cam.rotation = Rot4::Plane(1, 3, -s) * cam.rotation;

    if (glfwGetKey(window, GLFW_KEY_Z)) cam.rotation = Rot4::Plane(2, 3, s) * cam.rotation;
    if (glfwGetKey(window, GLFW_KEY_C)) cam.rotation = Rot4::Plane(2, 3, -s) * cam.rotation;
}

int main()
{
    // Load 4D meshes
    vector<MeshData> MESHES = MeshData::load();

    // 4D Camera (placed in -W looking toward origin)
    Camera4D camera(
        Vec4{ 0.f, 0.f, 0.f, -4.f },
        Rot4::Identity(),
        1.f,
        0.1f
    );
    CameraController cameraController;


    // GLFW init
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    int width = 1920; int height = 1080;


    GLFWwindow* window = glfwCreateWindow(width, height, "Glimpse 4D", nullptr, nullptr);
    glfwGetFramebufferSize(window, &width, &height);
    //glfwMaximizeWindow(window);

    if (!window) {
        cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSwapInterval(1);

    // ---------------- OPENGL SETUP ----------------
    glViewport(0, 0, width, height);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    float aspect = (float)width / (float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
        -aspect, aspect,
        -1.0f, 1.0f,
        -5.0f, 5.0f
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.f, 1.f, 1.f);
    // ------------------------------------------------
    float last = (float)glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float now = glfwGetTime();
        float dt = now - last;
        last = now;

        updateCameraControls(window, camera, dt);

        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_LINES);

        for (const MeshData& mesh : MESHES) {
            for (const Edge& e : mesh.edges) {

                Vec4 v0 = camera.apply(mesh.vertices[e.a]);
                Vec4 v1 = camera.apply(mesh.vertices[e.b]);

                Vec3 p0 = project4Dto3D(v0, camera.fov);
                Vec3 p1 = project4Dto3D(v1, camera.fov);

                glVertex3f(p0.x, p0.y, p0.z);
                glVertex3f(p1.x, p1.y, p1.z);
            }
        }

        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}