//
// Created by csk on 20/04/2020.
//
#include <learnopengl/shader.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/camera.h>

#include <GLFW/glfw3.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader cubeShader("cube_l03_06_02.vs", "cube_l03_06_02.fs");
    cubeShader.use();
    glm::vec3 plampPos[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    cubeShader.setFloat("material.shininess", 32.0f);
    glm::vec3 lampAmbIntensity = glm::vec3(0.2f);
    glm::vec3 lampDiffIntensity = glm::vec3(0.5f);
    glm::vec3 lampSpecIntensity = glm::vec3(1.0f);
    glm::vec3 lampLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 lampDiffuseColor = lampLightColor   * lampDiffIntensity;
    glm::vec3 lampAmbientColor = lampDiffuseColor * lampAmbIntensity;
    cubeShader.setVec3("dLamp.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    cubeShader.setVec3("dLamp.ambient", lampAmbientColor);
    cubeShader.setVec3("dLamp.diffuse", lampDiffuseColor);
    cubeShader.setVec3("dLamp.specular", lampSpecIntensity);

    cubeShader.setFloat("fLamp.cosInnerCutOff",   glm::cos(glm::radians(12.5f)));
    cubeShader.setFloat("fLamp.cosOuterCutOff",   glm::cos(glm::radians(17.5f)));
    cubeShader.setFloat("fLamp.constant",  1.0f);
    cubeShader.setFloat("fLamp.linear",    0.09f);
    cubeShader.setFloat("fLamp.quadratic", 0.032f);
    cubeShader.setVec3("fLamp.ambient", lampAmbientColor);
    cubeShader.setVec3("fLamp.diffuse", lampDiffuseColor);
    cubeShader.setVec3("fLamp.specular", lampSpecIntensity);

    cubeShader.setVec3("pLamp[0].position", plampPos[0]);
    cubeShader.setFloat("pLamp[0].constant",  1.0f);
    cubeShader.setFloat("pLamp[0].linear",    0.09f);
    cubeShader.setFloat("pLamp[0].quadratic", 0.032f);
    cubeShader.setVec3("pLamp[0].ambient", lampAmbientColor);
    cubeShader.setVec3("pLamp[0].diffuse", lampDiffuseColor);
    cubeShader.setVec3("pLamp[0].specular", lampSpecIntensity);

    cubeShader.setVec3("pLamp[1].position", plampPos[1]);
    cubeShader.setFloat("pLamp[1].constant",  1.0f);
    cubeShader.setFloat("pLamp[1].linear",    0.09f);
    cubeShader.setFloat("pLamp[1].quadratic", 0.032f);
    cubeShader.setVec3("pLamp[1].ambient", lampAmbientColor);
    cubeShader.setVec3("pLamp[1].diffuse", lampDiffuseColor);
    cubeShader.setVec3("pLamp[1].specular", lampSpecIntensity);

    cubeShader.setVec3("pLamp[2].position", plampPos[2]);
    cubeShader.setFloat("pLamp[2].constant",  1.0f);
    cubeShader.setFloat("pLamp[2].linear",    0.09f);
    cubeShader.setFloat("pLamp[2].quadratic", 0.032f);
    cubeShader.setVec3("pLamp[2].ambient", lampAmbientColor);
    cubeShader.setVec3("pLamp[2].diffuse", lampDiffuseColor);
    cubeShader.setVec3("pLamp[2].specular", lampSpecIntensity);

    cubeShader.setVec3("pLamp[3].position", plampPos[3]);
    cubeShader.setFloat("pLamp[3].constant",  1.0f);
    cubeShader.setFloat("pLamp[3].linear",    0.09f);
    cubeShader.setFloat("pLamp[3].quadratic", 0.032f);
    cubeShader.setVec3("pLamp[3].ambient", lampAmbientColor);
    cubeShader.setVec3("pLamp[3].diffuse", lampDiffuseColor);
    cubeShader.setVec3("pLamp[3].specular", lampSpecIntensity);
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int cubeVAO, VBO, cubeDiffMap, cubeSpecMap;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glGenTextures(1, &cubeDiffMap);
    glGenTextures(1, &cubeSpecMap);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeDiffMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data;
    if( (data = stbi_load(FileSystem::getPath("resources/textures/container2.png").c_str(), &width, &height, &nrChannels, 0) ) != NULL ){
        GLenum format;
        switch(nrChannels){
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    cubeShader.setInt("material.diffuseMap", 0);

    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D, cubeSpecMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if( (data = stbi_load(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), &width, &height, &nrChannels, 0) ) != NULL ){
        GLenum format;
        switch(nrChannels){
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    cubeShader.setInt("material.specularMap", 1);

    Shader dlampShader("dlamp_l03_06_02.vs", "dlamp_l03_06_02.fs");
    dlampShader.use();
    glm::vec3 dlampPos(1.2f, 1.0f, 2.0f);// a part of cube will be truncated. But it doesn't matter.
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, dlampPos);
    model = glm::scale(model, glm::vec3(0.2f));
    dlampShader.setMat4("model", model);
    unsigned int dlampVAO;
    glGenVertexArrays(1, &dlampVAO);
    glBindVertexArray(dlampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    Shader plampShader("plamp_l03_06_02.vs", "plamp_l03_06_02.fs");
    plampShader.use();
    unsigned int plampVAO;
    glGenVertexArrays(1, &plampVAO);
    glBindVertexArray(plampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)){
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        plampShader.use();
        plampShader.setMat4("projection", projection);
        plampShader.setMat4("view", view);
        glBindVertexArray(plampVAO);
        for(int i = 0; i < 4; ++i){
            model = glm::mat4(1.0f);
            model = glm::translate(model, plampPos[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            plampShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        dlampShader.use();
        dlampShader.setMat4("projection", projection);
        dlampShader.setMat4("view", view);
        glBindVertexArray(dlampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        cubeShader.use();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);
        cubeShader.setVec3("camPos", camera.Position);
        cubeShader.setVec3("fLamp.position", camera.Position);
        cubeShader.setVec3("fLamp.direction", camera.Front);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeDiffMap);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, cubeSpecMap);

        glBindVertexArray(cubeVAO);
        for(unsigned int i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &dlampVAO);
    glDeleteVertexArrays(1, &plampVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &cubeDiffMap);
    glDeleteTextures(1, &cubeSpecMap);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static float yaw = -90.0f;
    static float pitch = 0.0f;

    static float lastX = SCR_WIDTH / 2;
    static float lastY = SCR_HEIGHT / 2;
    static bool firstMouse = true;

    if(firstMouse == true){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}
