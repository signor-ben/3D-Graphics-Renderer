#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);          // define a function for dynamic window resizing
void processInput(GLFWwindow *window);                                              // function to close window when esc is pressed

const char *vertexShaderSource =                                                    // source code for the vertex shader stored in a string
    "#version 330 core\n"        
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =                                                  // source code for the fragment shader stored in a string
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.510f, 0.051f, 1.0f);\n"
    "}\n\0";

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

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);                   // vertex shader object, generate id and create the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);                     // attach shader source to the new VSO
    glCompileShader(vertexShader);                                                  // compile the VSO

    int success;                                                                   /* compilation error checking */
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);               // frag shader object, same process as VSO but with Fragment Shader instead
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);                     /* compilation error checking */
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders with shader program
    unsigned int shaderProgram = glCreateProgram();                                 // shader Program object
    glAttachShader(shaderProgram, vertexShader);                                    // attach the shaders to the SPO and then link them together
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);                                                   // delete now obsolete shader objects
    glDeleteShader(fragmentShader);  

    // vertex data and input 
    float vertices[] = {
        // first triangle
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left 
        // second triangle
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    }; 

    unsigned int VAO;                                                               // Vertex Array Object, stores VBOs, Vert Att configs, and calls to VA
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);

    unsigned int VBO;                                                               // vertex buffer object, stores a bunch of vertices
    glGenBuffers(1, &VBO);                                                          // generate an obj id with glGenBuffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             // bind the VBO to an array buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      // copy data from vertices into the buffer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   // define how ogl interprets the vertex data in the VBO
    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);                                               // Unbind VBO
    glBindVertexArray(0);                                                           // Unbind VAO

    // the render loop
    while(!glfwWindowShouldClose(window))                                           
    {
        // inputs
        processInput(window);

        // rendering 
        glClearColor(0.094f, 0.090f, 0.086f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);                                                // set ogl to use this SPO for shading and rendering
        glBindVertexArray(VAO);                                                     // binds this VAO for current rendering
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);                                                       // Unbind after drawing

        // check/call events, and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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