#include "ui.h"

using namespace PEPCB::Base;

static double scale_ratio = 1e-5;
static double scale_factor = sqrt(sqrt(2));
static int window_width;  // = 1600;
static int window_height; // = 900;
static int origin_x;      // = window_width / 2;
static int origin_y;      // = window_height / 2;
// initUIConstants();
static GLFWwindow *window;

static GLuint VAO_logo;
static GLuint buffer_logo;
static GLuint texture_id;
static GLuint logoProgramme;
static GLuint VAO_polygons;
static GLuint EBO_polygons;
static GLuint VBO_polygons;
static GLuint polygonProgramme;
static GLint worg_location;
static GLint wsize_location;
static GLint scale_location;
static GLint vpos_location;
static GLint vcol_location;
static float wOrigin_u[2];
static float wSize_u[2];
static float scale_u;

static PEPCB::UI::LayersRenderer LR;
// static std::map<ELayer, TColor> layer_color;
// static std::map<ELayer, int> layer_draw_rank;

GLfloat PEPCB::UI::pxToFloat(int _px, int _dimension)
{
    GLfloat f = (GLfloat)(_px) / (GLfloat)(_dimension)*2.f - 1.f;
    return f;
}

// void initUIConstants(void)
// {
//     layer_color = {
//         {F_CU, {231, 76, 60}},
//         {F_SILKSCREEN, {242, 243, 244}},
//         {B_CU, {52, 152, 219}},
//         {B_SILKSCREEN, {229, 231, 233}},
//         {F_COURTYARD, {255, 255, 255}},
//         {VIA, {26, 188, 156}},
//         {EDGE_CUTS, {255, 255, 128}},
//         {HOLE, {0, 0, 0}},
//     };

//     layer_draw_rank = {
//         {F_CU, 50},
//         {F_SILKSCREEN, 90},
//         {B_CU, 20},
//         {B_SILKSCREEN, 10},
//         {F_COURTYARD, 99},
//         {VIA, 75},
//         {EDGE_CUTS, 100},
//         {HOLE, 85},
//     };
// }

// TGeometry testGeometry()
// {
//     TPolygon a;
//     a.type = POLYGON;
//     a.outer_vertex_list.push_back({0, 0});
//     a.outer_vertex_list.push_back({10, 0});
//     a.outer_vertex_list.push_back({10, 10});
//     a.outer_vertex_list.push_back({0, 10});
//     return a;
// }

// GLfloat *generateVertexBuffer(std::vector<TGeometry>)
// {
//     return nullptr;
// }

void PEPCB::UI::errorCallback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void PEPCB::UI::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

void PEPCB::UI::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
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

void PEPCB::UI::addFootprint(PEPCB::Base::TFootprint footprint)
{
    for (auto it_layer = footprint.layer_list.begin(); it_layer != footprint.layer_list.end(); it_layer++)
    {
        for (auto it_element = it_layer->second.begin(); it_element < it_layer->second.end(); it_element++)
        {
            LR.addGeometry(*it_element, it_layer->first);
            it_element->log("polygon on layer " + std::to_string(it_layer->first));
        }
    }
}

void PEPCB::UI::loadUILayers(void)
{
    // TGeometry p1;
    // p1.outer_vertex_list.push_back({1000000, 2000000});
    // p1.outer_vertex_list.push_back({1000000, 3000000});
    // p1.outer_vertex_list.push_back({2000000, 3000000});
    // p1.outer_vertex_list.push_back({2000000, 2000000});
    // p1.inner_vertex_list_list.push_back({});
    // p1.inner_vertex_list_list[0].push_back({1200000, 2600000});
    // p1.inner_vertex_list_list[0].push_back({1600000, 2600000});
    // p1.inner_vertex_list_list[0].push_back({1600000, 2200000});
    // p1.inner_vertex_list_list[0].push_back({1200000, 2200000});
    // TGeometry p2;
    // p2.outer_vertex_list.push_back({0, 2400000});
    // p2.outer_vertex_list.push_back({4000000, 2400000});
    // p2.outer_vertex_list.push_back({4000000, 2500000});
    // p2.outer_vertex_list.push_back({0, 2500000});

    // TGeometry p2;
    // p2.outer_vertex_list.push_back({1000000, 1000000});
    // p2.outer_vertex_list.push_back({1000000, -1000000});
    // p2.outer_vertex_list.push_back({-1000000, -1000000});
    // p2.outer_vertex_list.push_back({-1000000, 1000000});

    // LR.addGeometry(p1, ELayer::F_CU);
    // LR.addGeometry(p2, ELayer::B_CU);

    LR.updateBuffer();
}

GLuint PEPCB::UI::loadLogoTexture(void)
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

void PEPCB::UI::startUI(int _window_width, int _window_height)
{
    window_width = _window_width;
    window_height = _window_height;
    origin_x = window_width / 2;
    origin_y = window_height / 2;

    /* Initialize the library */
    if (!glfwInit())
        PEPCB::UI::errorCallback(-1, "glfwInit() Error.");
    glfwSetErrorCallback(PEPCB::UI::errorCallback);

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
        PEPCB::UI::errorCallback(-1, "GL window not created.");
        return;
    }
    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "OpenGL shader version: " << major << "." << minor << std::endl;

    GLFWimage window_icons[1];
    window_icons[0].pixels = SOIL_load_image("./res/favicon.png", &window_icons[0].width, &window_icons[0].height, 0, SOIL_LOAD_AUTO);
    glfwSetWindowIcon(window, 1, window_icons);
    SOIL_free_image_data(window_icons[0].pixels);

    glfwSetKeyCallback(window, PEPCB::UI::keyCallback);
    glfwSetScrollCallback(window, PEPCB::UI::scrollCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // GLuint logo_handler = loadLogoTexture();

    GLfloat g_vertex_buffer_data[] = {
        -10.0f, -10.0f, 1.0f, 0.0f, 0.0f,
        10.0f, -10.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 20.0f, 0.0f, 0.0f, 1.0f,
        0.0f, -20.0f, 0.0f, 0.0f, 1.0f};

    GLuint g_indices_data[] = {
        0, 1, 2,
        0, 1, 3};

    GLfloat logo_vertex_buffer_data[] = {
        PEPCB::UI::pxToFloat(10, window_width), PEPCB::UI::pxToFloat(window_height - 10 - 58, window_height), 0.0f, 1.0f,
        PEPCB::UI::pxToFloat(10, window_width), PEPCB::UI::pxToFloat(window_height - 10, window_height), 0.0f, 0.0f,
        PEPCB::UI::pxToFloat(10 + 136, window_width), PEPCB::UI::pxToFloat(window_height - 10, window_height), 1.0f, 0.0f,
        PEPCB::UI::pxToFloat(10 + 136, window_width), PEPCB::UI::pxToFloat(window_height - 10, window_height), 1.0f, 0.0f,
        PEPCB::UI::pxToFloat(10 + 136, window_width), PEPCB::UI::pxToFloat(window_height - 10 - 58, window_height), 1.0f, 1.0f,
        PEPCB::UI::pxToFloat(10, window_width), PEPCB::UI::pxToFloat(window_height - 10 - 58, window_height), 0.0f, 1.0f};

    // for (int i = 0; i < 6; i++)
    // {
    //     std::cout << logo_vertex_buffer_data[i * 4] << " " << logo_vertex_buffer_data[i * 4 + 1] << " " << logo_vertex_buffer_data[i * 4 + 2] << " " << logo_vertex_buffer_data[i * 4 + 3] << " " << std::endl;
    // }

    texture_id = PEPCB::UI::loadLogoTexture();
    logoProgramme = PEPCB::UI::LoadShaders("./src/shader/logo.vert", "./src/shader/logo.frag");

    glGenVertexArrays(1, &VAO_logo);
    glBindVertexArray(VAO_logo);
    // glCreateBuffers(1, &buffer_logo);
    glGenBuffers(1, &buffer_logo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_logo);
    // glBufferStorage(GL_ARRAY_BUFFER, sizeof(logo_vertex_buffer_data), logo_vertex_buffer_data, 0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(logo_vertex_buffer_data), logo_vertex_buffer_data, GL_STATIC_DRAW);
    glUseProgram(logoProgramme);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    polygonProgramme = PEPCB::UI::LoadShaders("./src/shader/polygon.vert", "./src/shader/polygon.frag");
    glUseProgram(polygonProgramme);

    glGenVertexArrays(1, &VAO_polygons);
    glBindVertexArray(VAO_polygons);
    glGenBuffers(1, &VBO_polygons);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygons);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    // glBufferStorage(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, 0);
    glBufferData(GL_ARRAY_BUFFER, LR.total_vertex_numbers * 5 * sizeof(GLfloat), LR.vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO_polygons);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygons);
    // glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices_data), g_indices_data, 0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, LR.total_index_numbers * sizeof(GLuint), LR.index_buffer_data, GL_STATIC_DRAW);

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

    worg_location = glGetUniformLocation(polygonProgramme, "wOrigin");
    wsize_location = glGetUniformLocation(polygonProgramme, "wSize");
    scale_location = glGetUniformLocation(polygonProgramme, "scale");
    vpos_location = glGetAttribLocation(polygonProgramme, "vPos");

    // static const GLint worg_location = 0;
    // static const GLint wsize_location = 1;
    // static const GLint scale_location = 2;
    // static const GLint vpos_location = 3;
    // static const GLint vcol_location = 4;

    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(vpos_location);

    vcol_location = glGetAttribLocation(polygonProgramme, "vCol");
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(vcol_location);

    // std::cout << "worg loc=" << worg_location << std::endl;
    // std::cout << "wsize loc=" << wsize_location << std::endl;
    // std::cout << "scale loc=" << scale_location << std::endl;
    // std::cout << "vpos loc=" << vpos_location << std::endl;
    // std::cout << "vcol loc=" << vcol_location << std::endl;

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
        glDrawElements(GL_TRIANGLES, LR.total_index_numbers, GL_UNSIGNED_INT, 0);
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
    return;
}