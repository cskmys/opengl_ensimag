//
// Created by csk on 20/04/2020.
//
#include <learnopengl/shader.h>

#include <GLFW/glfw3.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    Shader ourShader("shader_l02_06_03.vs", "shader_l02_06_03.fs");

    // to which shader program should the data from PC should be sent needs to be describe 1st
    // That's normally how opengl's state machine works, 1st you need to say which one via api and then go on to
    // call other apis. All the manipulations from these apis will be to the object that bound in the 1st place
    ourShader.use();

    // now position n color both in same array
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // after description above, now enable it
    // sending color to layout location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // after description above, now enable it
    // sending texture co-ord to layout location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); // after description above, now enable it

    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    // indexing each point to a specific triangle is done via EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    // Buffer that holds EBO is of type ELEMENT_ARRAY. Since it's different from ARRAY_BUFFER above,
    // we need not worry about unbinding it. We can use all simultaneously.
    // All apis calling ARRAY_BUFFER will manipulate vertices & those calling ELEMENT_ARRAY_BUFFER manipulate indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object, remember the  state machine)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data;
    if( (data = stbi_load("container.jpg", &width, &height, &nrChannels, 0) ) != NULL ){
        // GL_TEXTURE_2D will impact only 2D textures not any other 1D or 3D textures
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // after mipmap generation, image data is not needed anymore
    stbi_image_free(data);
    // texunit is the location of sampler in frag shader to which the texture is assigned to
    // texture data in CPU is linked to GPU sampler via this variable
    ourShader.setInt("texture1", 0);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE0 + 1); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    if( (data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0) ) != NULL ){
        // GL_TEXTURE_2D will impact only 2D textures not any other 1D or 3D textures
        // the png image that we opened contains alpha channel as well, so now we use RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // after mipmap generation, image data is not needed anymore
    stbi_image_free(data);

    ourShader.setInt("texture2", 1);


    while(!glfwWindowShouldClose(window)){ // one iteration of this loop = 1 frame
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function(remember opengl is a state machine)
        // clear color buffer. There exists depth, stencil buffer as well
        glClear(GL_COLOR_BUFFER_BIT); // state using function

        // if we have set of shader programs, then we need to say which shader's vertex array object we are manipulating.
        // so though we have just one here, we are just establishing convention of explicitly calling it
        ourShader.use();
        // before drawing we need to say which texture needs to be applied for a group of vertices.
        // when there are multiple textures we need to activate the texture unit before binding the texture buffer
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // Group of vertices is referred to by VAO. Hence, call this before VAO(remember the state machine)
        // for any drawing related stuff we need to bind VAO 1st but for just sending the data to shader via uniform,
        // we need not do VAO 1st to be safe???
        // there could be large number of VAOs. Hence, before drawing using EBO(if it exists), we need to say which VAO
        // that the EBO should index. To say that 1st we need to bind and then do operations(opengl state machine)
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
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
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