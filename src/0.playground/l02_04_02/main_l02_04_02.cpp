//
// Created by csk on 20/04/2020.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // register window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // callback functions should be registered after we've created the window and before the render loop is initiated.

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // opengl render screen location(0,0) and max dimensions (800, 600) where opengl should map it's (-1,1) in x & y
    glViewport(0, 0, 800, 600);

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    const char *fragmentShaderSource =  "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main()\n"
                                        "{\n"
                                        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                        "}\n\0";
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // linking causes op of vertex shader to go to next stage which eventually brings it to fragment shader
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // since we have setup a shader program, individual objects are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // to which shader program should the data from PC should be sent needs to be describe 1st
    // That's normally how opengl's state machine works, 1st you need to say which one via api and then go on to
    // call other apis. All the manipulations from these apis will be to the object that bound in the 1st place
    glUseProgram(shaderProgram);

    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    // There's quite a chain to describe how a data from an array in PC's RAM goes into which vertex and
    // which attribute in memory of Graphic card. This whole chain once created, is referenced and
    // managed(enable/disable loading data to attribute) using a vertex array object
    // since opengl is a state machine, in which we need to bind something 1st and then do operations to
    // manipulate the bound object, we 1st create and bind a VAO and then go on to describe the chain
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // all the chain from RAM to GPU described from on on will be now will be bound to this VAO
    glBindVertexArray(VAO);
    unsigned int VBO; // vertex buffer object
    // create buffer to load the vertex data
    glGenBuffers(1, &VBO);
    // OpenGL has many types of buffer objects Ex: GL_ARRAY_BUFFER.
    // bind the above buffer to ARRAY_BUFFER of opengl, which is one amongst the several types of buffers that it has
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Note: You can bind several buffers at once if they are different type.
    // From now onwards, any calls on type GL_ARRAY_BUFFER, will manipulate data in buffer referenced via id in VBO var.
    // vertices copied to array buffer i,e, the one whose id is in VBO variable
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Each vertex attribute is referenced by a layout no in vertex shader & it takes data from memory managed by a VBO.
    // Out of multiple VBOs that may exist which VBO is the data taken from, is determined by the VBO currently
    // bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer. To which attribute in the vertex shader does this
    // VBO's data go to is specified in the 1st argument of glVertexAttribPointer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // after description above, now enable it

    // indexing each point to a specific triangle is done via EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    // Buffer that holds EBO is of type ELEMENT_ARRAY. Since it's different from ARRAY_BUFFER above,
    // we need not worry about unbinding it. We can use all simultaneously.
    // All apis calling ARRAY_BUFFER will manipulate vertices & those calling ELEMENT_ARRAY_BUFFER manipulate indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // NOTE: it's not an attribute, hence, no need of attribpointer api to specify layout number on gpu.
    // now to draw we should use EBO, before calling draw api, we need to bind EBO 1st.
    // this would have been needed to done always in while loop where we draw
    // luckily VAO stores the last EBO that was bound. Hence, referring to VAO will implicitly also indicate EBO.
    // now since we have already called the BindBuffer for EBO in code above, VAO is already configured to use EBO.
    // unless we unbind EBO, VAO will always refer to this VBO. But, still in the code below we explicitly
    // mention to have a safe and std convention

    while(!glfwWindowShouldClose(window)){ // one iteration of this loop = 1 frame
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function(remember opengl is a state machine)
        // clear color buffer. There exists depth, stencil buffer as well
        glClear(GL_COLOR_BUFFER_BIT); // state using function


        // if we have set of shader programs, then we need to say which shader's vertex array object we are manipulating.
        // so though we have just one here, we are just establishing convention of explicitly calling it
        glUseProgram(shaderProgram);
        // using the same set of steps above, there could be large number of VAOs.
        // Hence before drawing using EBO, we need to say which VAO that the EBO should index
        // To say that 1st we need to bind and then do operations(opengl state machine)
        glBindVertexArray(VAO);
        // we draw using EBO, hence before calling the draw api, we need to bind relevant EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // we need to draw using the elements
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // after finishing our work with VOA we can(or is it should?) unbind it

        glfwSwapBuffers(window);
        glfwPollEvents(); // checks for events and calls corresponding callbacks
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// callback when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}