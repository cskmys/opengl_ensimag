//
// Created by csk on 20/04/2020.
//
#include <learnopengl/shader.h>

#include <GLFW/glfw3.h>
#include <cmath>

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

    Shader ourShader("shader.vs", "shader.fs");

    // to which shader program should the data from PC should be sent needs to be describe 1st
    // That's normally how opengl's state machine works, 1st you need to say which one via api and then go on to
    // call other apis. All the manipulations from these apis will be to the object that bound in the 1st place
    ourShader.use();

    // now position n color both in same array
    float vertices[] = {
            // positions
            0.5f, -0.5f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f,   // top
    };
    float texCoords[] = {
            0.0f, 0.0f,  // lower-left corner
            1.0f, 0.0f,  // lower-right corner
            0.5f, 1.0f   // top-center corner
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

    while(!glfwWindowShouldClose(window)){ // one iteration of this loop = 1 frame
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function(remember opengl is a state machine)
        // clear color buffer. There exists depth, stencil buffer as well
        glClear(GL_COLOR_BUFFER_BIT); // state using function


        // if we have set of shader programs, then we need to say which shader's vertex array object we are manipulating.
        // so though we have just one here, we are just establishing convention of explicitly calling it
        ourShader.use();
        auto timeValue = (float)glfwGetTime(); // due to typecast, type can be deduced by compiler. so auto
        auto greenValue = (float)(sin(timeValue) / 2.0f) + 0.5f;
        // to get layout location we didnt have to do useProgram since the api explicitly asks for the shader
        // but, to set the value, api doesn't ask which shader it is
        // hence we do use program to set desired shader and then use the api below to send data to that
        ourShader.setVec4("ourColor", 0.0, greenValue, 0.0, 1.0);
        // for any drawing related stuff we need to bind VAO 1st but for just sending the data to shader via uniform,
        // we need not do VAO 1st to be safe???
        // there could be large number of VAOs. Hence, before drawing using EBO(if it exists), we need to say which VAO
        // that the EBO should index. To say that 1st we need to bind and then do operations(opengl state machine)
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // after finishing our work with VOA we can(or is it should?) unbind it

        glfwSwapBuffers(window);
        glfwPollEvents(); // checks for events and calls corresponding callbacks
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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