#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <math.h>
#include <map>

#include "core.h"

using namespace PEPCB::Base;

static double scale_ratio = 10;
static double scale_factor = sqrt(sqrt(2));
static int window_width = 640;
static int window_height = 480;
static int origin_x = window_width / 2;
static int origin_y = window_height / 2;

static std::map<ELayer, TColor> layer_color;
static std::map<ELayer, int> layer_draw_rank;

GLfloat pxToFloat(int _px, int _dimension)
{
    GLfloat f = (GLfloat)(_px) / (GLfloat)(_dimension)*2.f - 1.f;
    return f;
}

void initUIConstants(void)
{
    layer_color = {
        {TOP_COPPER, {231, 76, 60}},
        {TOP_SILK, {242, 243, 244}},
        {BOTTOM_COPPER, {52, 152, 219}},
        {BOTTOM_SILK, {229, 231, 233}},
        {COLLISION_BOUNDARY, {255, 255, 255}},
        {VIA, {26, 188, 156}},
        {EDGE, {255, 255, 128}},
        {HOLE, {0, 0, 0}},
    };

    layer_draw_rank = {
        {TOP_COPPER, 50},
        {TOP_SILK, 90},
        {BOTTOM_COPPER, 20},
        {BOTTOM_SILK, 10},
        {COLLISION_BOUNDARY, 99},
        {VIA, 75},
        {EDGE, 100},
        {HOLE, 85},
    };
}

TGeometry testGeometry()
{
    TGeometry a;
    a.layer = TOP_COPPER;
    a.type = POLYGON;
    a.vertex_list.push_back({0, 0});
    a.vertex_list.push_back({10, 0});
    a.vertex_list.push_back({10, 10});
    a.vertex_list.push_back({0, 10});
    return a;
}

void drawGeometry(TGeometry a)
{
    glBegin(GL_POLYGON);
    TColor c = layer_color[a.layer];
    float r = c.r / 255.f;
    float g = c.g / 255.f;
    float b = c.b / 255.f;
    glColor3f(r, g, b);
    for (auto it = a.vertex_list.begin(); it < a.vertex_list.end(); it++)
    {
        float xtrans = (origin_x + it->x * scale_ratio) / window_width * 2.f - 1.f;
        float ytrans = (origin_y + it->y * scale_ratio) / window_height * 2.f - 1.f;
        glVertex2f(xtrans, ytrans);
    }
    glEnd();
}

void errorCallback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    /*
        relocate the origin subject to the cursor
        e.g. prior to change, O(320,240), C(400,240), SF=1/4
        after change, O'(380,240)
    */
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (yoffset > 0)
    {
        scale_ratio *= scale_factor;
        origin_x = xpos + (origin_x - xpos) * scale_factor;
        origin_y = (window_height - ypos) + (origin_y - (window_height - ypos)) * scale_factor;
    }
    else
    {
        scale_ratio /= scale_factor;
        origin_x = xpos + (origin_x - xpos) / scale_factor;
        origin_y = (window_height - ypos) + (origin_y - (window_height - ypos)) / scale_factor;
    }
}

GLuint loadLogoTexture(void)
{
    GLuint t;
    uint8_t *data;
    int width, height, ch;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    data = SOIL_load_image("./res/uomlogon.png", &width, &height, &ch, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    SOIL_free_image_data(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return t;
}

void drawLogo(GLuint logo)
{
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, logo);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pxToFloat(10, window_width), pxToFloat(window_height - 10 - 58, window_height));
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pxToFloat(10, window_width), pxToFloat(window_height - 10, window_height));
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pxToFloat(10 + 136, window_width), pxToFloat(window_height - 10, window_height));
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pxToFloat(10 + 136, window_width), pxToFloat(window_height - 10 - 58, window_height));
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(void)
{
    initUIConstants();
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwSetErrorCallback(errorCallback);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(window_width, window_height, "PEPCB Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    GLFWimage window_icons[1];
    window_icons[0].pixels = SOIL_load_image("./res/favicon.png", &window_icons[0].width, &window_icons[0].height, 0, SOIL_LOAD_AUTO);
    glfwSetWindowIcon(window, 1, window_icons);
    SOIL_free_image_data(window_icons[0].pixels);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLuint logo_handler = loadLogoTexture();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        drawGeometry(testGeometry());
        drawLogo(logo_handler);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}