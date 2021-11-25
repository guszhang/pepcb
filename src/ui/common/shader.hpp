#pragma once

#include <GLFW/glfw3.h>

namespace PEPCB{
    namespace UI{
        GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
    }
}
