#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <map>
#include <thread>
#include <chrono>

#include "../core/core.h"
#include "common/shader.hpp"
#include "common/layers.hpp"

using namespace PEPCB::Base;

static double scale_ratio = 10;
static double scale_factor = sqrt(sqrt(2));
static int window_width = 1600;
static int window_height = 900;
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
        {F_CU, {231, 76, 60}},
        {F_SILKSCREEN, {242, 243, 244}},
        {B_CU, {52, 152, 219}},
        {B_SILKSCREEN, {229, 231, 233}},
        {F_COURTYARD, {255, 255, 255}},
        {VIA, {26, 188, 156}},
        {EDGE_CUTS, {255, 255, 128}},
        {HOLE, {0, 0, 0}},
    };

    layer_draw_rank = {
        {F_CU, 50},
        {F_SILKSCREEN, 90},
        {B_CU, 20},
        {B_SILKSCREEN, 10},
        {F_COURTYARD, 99},
        {VIA, 75},
        {EDGE_CUTS, 100},
        {HOLE, 85},
    };
}

TGeometry testGeometry()
{
    TPolygon a;
    a.type = POLYGON;
    a.outer_vertex_list.push_back({0, 0});
    a.outer_vertex_list.push_back({10, 0});
    a.outer_vertex_list.push_back({10, 10});
    a.outer_vertex_list.push_back({0, 10});
    return a;
}

GLfloat *generateVertexBuffer(std::vector<TGeometry>)
{
    return nullptr;
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

// void drawLogo(GLuint logo)
// {
//     // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//     // glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, logo);
//     glEnable(GL_TEXTURE_2D);
//     glColor4f(1.f, 1.f, 1.f, 1.f);
//     glBegin(GL_QUADS);
//     glTexCoord2f(0.0f, 1.0f);
//     glVertex2f(pxToFloat(10, window_width), pxToFloat(window_height - 10 - 58, window_height));
//     glTexCoord2f(0.0f, 0.0f);
//     glVertex2f(pxToFloat(10, window_width), pxToFloat(window_height - 10, window_height));
//     glTexCoord2f(1.0f, 0.0f);
//     glVertex2f(pxToFloat(10 + 136, window_width), pxToFloat(window_height - 10, window_height));
//     glTexCoord2f(1.0f, 1.0f);
//     glVertex2f(pxToFloat(10 + 136, window_width), pxToFloat(window_height - 10 - 58, window_height));
//     glEnd();
//     glDisable(GL_TEXTURE_2D);
//     glBindTexture(GL_TEXTURE_2D, 0);
// }

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(window_width, window_height, "PEPCB Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "OpenGL shader version: " << major << "." << minor << std::endl;

    GLFWimage window_icons[1];
    window_icons[0].pixels = SOIL_load_image("./res/favicon.png", &window_icons[0].width, &window_icons[0].height, 0, SOIL_LOAD_AUTO);
    glfwSetWindowIcon(window, 1, window_icons);
    SOIL_free_image_data(window_icons[0].pixels);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // GLuint logo_handler = loadLogoTexture();

    TPolygon p1;
    p1.outer_vertex_list.push_back({10, 20});
    p1.outer_vertex_list.push_back({10, 30});
    p1.outer_vertex_list.push_back({20, 30});
    p1.outer_vertex_list.push_back({20, 20});
    p1.inner_vertex_list_list.push_back({});
    p1.inner_vertex_list_list[0].push_back({12, 26});
    p1.inner_vertex_list_list[0].push_back({16, 26});
    p1.inner_vertex_list_list[0].push_back({16, 22});
    p1.inner_vertex_list_list[0].push_back({12, 22});
    TPolygon p2;
    p2.outer_vertex_list.push_back({0, 24});
    p2.outer_vertex_list.push_back({40, 24});
    p2.outer_vertex_list.push_back({40, 25});
    p2.outer_vertex_list.push_back({0, 25});
    PEPCB::UI::LayersRenderer lr1;
    lr1.addPolygon(p1, ELayer::F_CU);
    lr1.addPolygon(p2, ELayer::B_CU);
    lr1.updateBuffer();

    GLfloat g_vertex_buffer_data[] = {
        -10.0f, -10.0f, 1.0f, 0.0f, 0.0f,
        10.0f, -10.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 20.0f, 0.0f, 0.0f, 1.0f,
        0.0f, -20.0f, 0.0f, 0.0f, 1.0f};

    GLuint g_indices_data[] = {
        0, 1, 2,
        0, 1, 3};

    GLfloat logo_vertex_buffer_data[] = {
        pxToFloat(10, window_width), pxToFloat(window_height - 10 - 58, window_height), 0.0f, 1.0f,
        pxToFloat(10, window_width), pxToFloat(window_height - 10, window_height), 0.0f, 0.0f,
        pxToFloat(10 + 136, window_width), pxToFloat(window_height - 10, window_height), 1.0f, 0.0f,
        pxToFloat(10 + 136, window_width), pxToFloat(window_height - 10, window_height), 1.0f, 0.0f,
        pxToFloat(10 + 136, window_width), pxToFloat(window_height - 10 - 58, window_height), 1.0f, 1.0f,
        pxToFloat(10, window_width), pxToFloat(window_height - 10 - 58, window_height), 0.0f, 1.0f};

    // for (int i = 0; i < 6; i++)
    // {
    //     std::cout << logo_vertex_buffer_data[i * 4] << " " << logo_vertex_buffer_data[i * 4 + 1] << " " << logo_vertex_buffer_data[i * 4 + 2] << " " << logo_vertex_buffer_data[i * 4 + 3] << " " << std::endl;
    // }

    GLuint VAO_logo;
    GLuint buffer_logo;
    GLuint texture_id = loadLogoTexture();
    GLuint logoProgramme = PEPCB::UI::LoadShaders("./src/shader/logo.vert", "./src/shader/logo.frag");

    glGenVertexArrays(1, &VAO_logo);
    glBindVertexArray(VAO_logo);
    //glCreateBuffers(1, &buffer_logo);
    glGenBuffers(1, &buffer_logo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_logo);
    //glBufferStorage(GL_ARRAY_BUFFER, sizeof(logo_vertex_buffer_data), logo_vertex_buffer_data, 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(logo_vertex_buffer_data), logo_vertex_buffer_data, GL_STATIC_DRAW);
    glUseProgram(logoProgramme);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint VAO_polygons;
    GLuint EBO_polygons;
    GLuint VBO_polygons;
    GLuint polygonProgramme = PEPCB::UI::LoadShaders("./src/shader/polygon.vert", "./src/shader/polygon.frag");
    glUseProgram(polygonProgramme);

    glGenVertexArrays(1, &VAO_polygons);
    glBindVertexArray(VAO_polygons);
    glGenBuffers(1, &VBO_polygons);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygons);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    // glBufferStorage(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, 0);
    glBufferData(GL_ARRAY_BUFFER, lr1.total_vertex_numbers * 5 * sizeof(GLfloat), lr1.vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO_polygons);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygons);
    // glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices_data), g_indices_data, 0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lr1.total_index_numbers * sizeof(GLuint), lr1.index_buffer_data, GL_STATIC_DRAW);

    // // This will identify our vertex buffer
    // GLuint vertexbuffer;
    // // Generate 1 buffer, put the resulting identifier in vertexbuffer
    // glGenBuffers(1, &vertexbuffer);
    // // The following commands will talk about our 'vertexbuffer' buffer
    // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // // Give our vertices to OpenGL.
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // GLuint polygonProgramme = PEPCB::UI::LoadShaders("./src/shader/polygon.vert", "./src/shader/polygon.frag");
    // GLuint secondProgram = PEPCB::UI::LoadShaders("./src/shader/logo.vert", "./src/shader/logo.frag");

    GLint worg_location = glGetUniformLocation(polygonProgramme, "wOrigin");
    GLint wsize_location = glGetUniformLocation(polygonProgramme, "wSize");
    GLint scale_location = glGetUniformLocation(polygonProgramme, "scale");
    GLint vpos_location = glGetAttribLocation(polygonProgramme, "vPos");

    // static const GLint worg_location = 0;
    // static const GLint wsize_location = 1;
    // static const GLint scale_location = 2;
    // static const GLint vpos_location = 3;
    // static const GLint vcol_location = 4;

    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(vpos_location);

    GLint vcol_location = glGetAttribLocation(polygonProgramme, "vCol");
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(vcol_location);

    // std::cout << "worg loc=" << worg_location << std::endl;
    // std::cout << "wsize loc=" << wsize_location << std::endl;
    // std::cout << "scale loc=" << scale_location << std::endl;
    // std::cout << "vpos loc=" << vpos_location << std::endl;
    // std::cout << "vcol loc=" << vcol_location << std::endl;

    float wOrigin_u[2];
    float wSize_u[2];
    float scale_u;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        wOrigin_u[0] = (float)origin_x;
        wOrigin_u[1] = (float)origin_y;
        wSize_u[0] = (float)window_width;
        wSize_u[1] = (float)window_height;
        scale_u = (float)scale_ratio;

        // drawGeometry(testGeometry());
        glUseProgram(polygonProgramme);
        glBindVertexArray(VAO_polygons);
        glUniform1f(scale_location, scale_u);
        glUniform2f(worg_location, wOrigin_u[0], wOrigin_u[1]);
        glUniform2f(wsize_location, wSize_u[0], wSize_u[1]);
        glDrawElements(GL_TRIANGLES, lr1.total_index_numbers, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(logoProgramme);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glBindVertexArray(VAO_logo);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // drawLogo(logo_handler);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::nanoseconds(16000000));
    }

    glfwTerminate();
    return 0;
}