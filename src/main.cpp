#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);          // define a function for dynamic window resizing
void processInput(GLFWwindow *window);                                              // function to close window when esc is pressed

int main()
{
    glfwInit();                                                                     // Init glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                  // set version of opengl to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                  // using opengl core
  
    GLFWwindow* window = glfwCreateWindow(800, 600, "TESTING WINDOW", NULL, NULL);  // create a window object
    if (window == NULL)                                                             /* window creation error checking */
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);                                                 // set main context to the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);              // register that we call the func on resize

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                        // glad init before using ogl functions
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    Shader shader("../src/shaders/vshader.txt", "../src/shaders/fshader.txt");
    
    // vertex data and input 
    float vertices[] = {
        // first triangle    //colors
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // top right 
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left
         0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
    }; 

    unsigned int VAO, VBO;                                                          // Vertex Array Object, stores VBOs, Vert Att configs, and calls to VA
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);                                                          // generate an obj id with glGenBuffers
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             // bind the VBO to an array buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      // copy data from vertices into the buffer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);   // define how ogl interprets the vertex data in the VBO
    glEnableVertexAttribArray(0); 
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);                                               // Unbind VBO
    glBindVertexArray(0);                                                           // Unbind VAO



    
    // the render loop
    while(!glfwWindowShouldClose(window))                                           
    {
        // inputs
        processInput(window);

        // rendering 
        float timeValue = glfwGetTime();
        float red = (sin(timeValue * 2.0f) + 1.0f) / 2.0f;  // Oscillates between 0 and 1
        float green = (cos(timeValue * 2.0f) + 1.0f) / 2.0f;
        float blue = (sin(timeValue * 1.5f) + 1.0f) / 2.0f;

        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

                                                                     // set ogl to use this SPO for shading and rendering
        
        shader.use();

        // update shader uniform
        float xoffset = sin(timeValue*2.0f) * 0.5f;  // Moves back and forth
        float yoffset = cos(timeValue*2.0f) * 0.5f;
        float coloroffset = sin(timeValue*5.0f) * 0.5f;
        shader.setFloat("xOffset", xoffset);
        shader.setFloat("yOffset",yoffset);
        shader.setFloat("colOff", coloroffset);
        
        glBindVertexArray(VAO);                                                     // binds this VAO for current rendering
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);                                                       // Unbind after drawing

        // check/call events, and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)           // give functionality
{
    glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window)                                               // give functionality
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}