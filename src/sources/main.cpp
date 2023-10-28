#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <ppm_io/ppm_io.h>
#include <vector>
#include <string>

#include "ImageProcessingOperation.h"
#include "HistogramEqualization.h"
#include "HistogramMatching.h"
#include "Common.h"
#include "Sobel.h"
#include "GaussianSmoothing.h"
#include "UnsharpMasking.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char* argv[]) {
    std::filesystem::path currPath{ argv[0] };
    currPath.remove_filename();
    std::cout << "Program start at "<<currPath << std::endl;
    std::string openFilePath{};
    std::string saveFilePath{};
    std::mutex mtx;
    std::thread OperationWindow{ [&]()->void {
        std::vector<std::string> menus{"add","sub", "mul", "inv", "log", "pow",
                                       "histeq", "histmatch","sobel","gblur","unshrpmask", "resize","load", "save", "end"};
    while (true) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Menu" << std::endl;
        for (int m = 0; m < menus.size();++m) {
            std::cout <<m+1<< ". "<< menus[m] << std::endl;
        }
        std::cout << "-----------------------------------------" << std::endl;

        std::string command{};
        std::string str{};
        std::getline(std::cin, str);
        std::stringstream sstr{ str };
        sstr >> command;
        std::vector<std::string> details{};
        std::string text{};
        while (sstr >> text) {
            details.push_back(text);
        }
        Operation* operation{};
        bool isSuccess{true};
        if (_strcmpi(command.c_str(), "resize") == false) {
            operation = new ResizeOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "add") == false) {
            operation = new AddOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "sub") == false) {
            operation = new SubOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "mul") == false) {
            operation = new MulOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "inv") == false) {
            operation = new InvOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "log") == false) {
            operation = new LogOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "pow") == false) {
            operation = new PowOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "histmatch") == false) {
            operation = new HistogramMatching{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "histeq") == false) {
            operation = new HistogramEqualization{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "sobel") == false) {
            operation = new SobelOperation{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "gblur") == false) {
            operation = new GaussianSmoothing{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "unshrpmask") == false) {
            operation = new UnsharpMasking{ currPath,details };
            isSuccess = operation->Operate();
        }
        else if (_strcmpi(command.c_str(), "load") == false) {
            operation = new Operation{currPath};
            operation->output = details[0];
        }
        else if (_strcmpi(command.c_str(), "save") == false) {
            mtx.lock();
            saveFilePath = details[0];
            mtx.unlock();
            continue;
        }
        //else if (_strcmpi(command.c_str(), "cd") == false) {
        //    operation = new Operation{currPath};
        //    system(("cd " + details[0]).c_str());
        //}
        else if (_strcmpi(command.c_str(), "end") == false) {
            break;
        }
        else {
            std::cout << "not supported command" << std::endl;
            continue;
        }

        if (isSuccess == false) {
            std::cout << "operation failed" << std::endl;
            continue;
        }

        mtx.lock();
        openFilePath = operation->OutputFile();
        mtx.unlock();

        delete operation;
    }
    } };

    PPM file(openedFile);
    //PPM file("output.ppm");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 800);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    float vertices[] = {
        0.8f,  0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.8f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
       -0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
       -0.8f,  0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[] = {
    0, 3, 1,
    1, 3, 2
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::ifstream vertexSource("./Assets/Shader/BasicTexVert.GLSL");
    char* vertexShaderSource{ new char[1024] {} };
    //vertexSource.seekg(0, std::ios::end);
    //int size = vertexSource.tellg();
    //vertexSource.seekg(0, std::ios::beg);
    vertexSource.read(vertexShaderSource, 1024);
    std::cout << vertexShaderSource << std::endl;
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    vertexSource.close();
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    std::ifstream fragmentSource("./Assets/Shader/BasicTexFrag.GLSL");
    char* fragmentShaderSource{ new char[1024] {} };
    fragmentSource.read(fragmentShaderSource, 1024);
    std::cout << fragmentShaderSource << std::endl;
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    fragmentSource.close();
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //int texWidth, texHeight, texChannels;
    //unsigned char* pixels = stbi_load("./Assets/Test_images/apple-20.ppm", &texWidth,&texHeight,&texChannels,STBI_rgb_alpha);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, file.getW(), file.getH(), 0, GL_RGB, GL_UNSIGNED_BYTE, file.getImageHandler());
    glGenerateMipmap(GL_TEXTURE_2D);



    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    //glBindTextureUnit(GL_TEXTURE0, texture);
    //int location = glGetUniformLocation(shaderProgram, "Texture");
    //if (location == -1)
    //{
    //    std::cout << "error on shader location! - " << "Texture" << std::endl;
    //    return -1;
    //}
    //glUniform1i(location, GL_TEXTURE0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        if (openFilePath.empty() == false) {
            file = PPM(openFilePath);
            file.write(openedFile);

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, file.getW(), file.getH(), 0, GL_RGB, GL_UNSIGNED_BYTE, file.getImageHandler());
            glGenerateMipmap(GL_TEXTURE_2D);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            openFilePath.clear();
        }
        if (saveFilePath.empty() == false) {
            file.write(saveFilePath);
            saveFilePath.clear();
        }
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    
    OperationWindow.join();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}