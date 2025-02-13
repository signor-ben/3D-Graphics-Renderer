#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "stb_image.h"

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
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO;                                                     // Vertex Array Object, stores VBOs, Vert Att configs, and calls to VA
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);                                                          // generate an obj id with glGenBuffers
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             // bind the VBO to an array buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      // copy data from vertices into the buffer

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);   // define how ogl interprets the vertex data in the VBO
    glEnableVertexAttribArray(0); 
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);                                                           // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                               // Unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);                                       // Unbind EBO

    // textures!!!

    unsigned int texture1, texture2;

    // Texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../src/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);                                                          // free redundant data

    // texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);  
    data = stbi_load("../src/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);                                                          // free redundant data


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);


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
        
        // use our shader program!!!
        shader.use();

        // update shader uniform
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);                                                       // Unbind after drawing

        // check/call events, and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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