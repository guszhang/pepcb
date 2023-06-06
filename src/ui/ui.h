#pragma once

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

namespace PEPCB
{
    namespace UI
    {
        GLfloat pxToFloat(int _px, int _dimension);
        void errorCallback(int error, const char *description);
        void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

        GLuint loadLogoTexture(void);
        void loadUILayers(void);
        void startUI(int _window_width, int _window_height);
    }
}
