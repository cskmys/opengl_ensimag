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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // opengl render screen location(0,0) and max dimensions (800, 600) where opengl should map it's (-1,1) in x & y
    glViewport(0, 0, 800, 600);
    // register window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // callback functions should be registered after we've created the window and before the render loop is initiated.


    while(!glfwWindowShouldClose(window)){ // one iteration of this loop = 1 frame
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function(remember opengl is a state machine)
        // clear color buffer. There exists depth, stencil buffer as well
        glClear(GL_COLOR_BUFFER_BIT); // state using function

        glfwSwapBuffers(window);
        glfwPollEvents(); // checks for events and calls corresponding callbacks
    }
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