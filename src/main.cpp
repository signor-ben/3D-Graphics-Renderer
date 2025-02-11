#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)           // define a function for dynamic window resizing
{
    glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window)                                               // function to close window when esc is pressed
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();                                                                     // Init glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                  // set version of opengl to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                  // using opengl core
  
    GLFWwindow* window = glfwCreateWindow(800, 600, "TESTING WINDOW", NULL, NULL);  // create a window object
    if (window == NULL)                                                             // if it failed, print error and terminate
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);                                                 // set main context to the window

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                        // init glad b4 any ogl functions
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glViewport(0, 0, 800, 600);                                                     // define a viewport size for ogl to render in

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);              // register that we call the func on resize

    while(!glfwWindowShouldClose(window))                                           // the render loop
    {
        // inputs
        processInput(window);

        // rendering commands
        glClearColor(0.0f, 1.0f, 0.0f, 0.2f);
        glClear(GL_COLOR_BUFFER_BIT);

        // check/call events, and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}
