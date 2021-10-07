#include <GLFW/glfw3.h>
#include <stdio.h>

void draw_rectangle()
{
    glBegin(GL_LINE_LOOP);
    glColor3f(255, 0, 0);
    glVertex2f(.5f, .5f);
    glVertex2f(-.5f, .5f);
    glVertex2f(-.5f, -.5f);
    glVertex2f(.5f, -.5f);
    glEnd();
}

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (yoffset > 0)
    {
        printf("wheel up@%.2f, %.2f\n", xpos, ypos);
    }
    else
    {
        printf("wheel down@%.2f, %.2f\n", xpos, ypos);
    }
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwSetErrorCallback(error_callback);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        draw_rectangle();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}