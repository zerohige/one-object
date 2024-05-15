#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"

const int windowWidth = 1000;
const int windowHeight = 1000;

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "myscene", 0, 0);
    if (window == NULL) {
        std::cout << "window gagal dibuka";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, windowWidth, windowHeight);

    Shader shaderProgram("default.vert", "default.frag");

    // Vertices for a table (top surface and four legs)
    GLfloat vertices[] = {
        // Tabletop (4 vertices)
        -0.5f,  0.0f,  0.5f,  1.0f, 0.0f, 0.0f, // Top left
         0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, // Top right
         0.5f,  0.0f, -0.5f,  0.0f, 0.0f, 1.0f, // Bottom right
        -0.5f,  0.0f, -0.5f,  1.0f, 1.0f, 1.0f, // Bottom left

        // Legs (16 vertices, 4 for each leg)
        // Front left leg
        -0.45f, -0.5f,  0.45f,  1.0f, 0.0f, 0.0f, // Bottom left
        -0.35f, -0.5f,  0.45f,  0.0f, 1.0f, 0.0f, // Bottom right
        -0.35f,  0.0f,  0.45f,  0.0f, 0.0f, 1.0f, // Top right
        -0.45f,  0.0f,  0.45f,  1.0f, 1.0f, 1.0f, // Top left

        // Front right leg
         0.35f, -0.5f,  0.45f,  1.0f, 0.0f, 0.0f, // Bottom left
         0.45f, -0.5f,  0.45f,  0.0f, 1.0f, 0.0f, // Bottom right
         0.45f,  0.0f,  0.45f,  0.0f, 0.0f, 1.0f, // Top right
         0.35f,  0.0f,  0.45f,  1.0f, 1.0f, 1.0f, // Top left

        // Back left leg
        -0.45f, -0.5f, -0.45f,  1.0f, 0.0f, 0.0f, // Bottom left
        -0.35f, -0.5f, -0.45f,  0.0f, 1.0f, 0.0f, // Bottom right
        -0.35f,  0.0f, -0.45f,  0.0f, 0.0f, 1.0f, // Top right
        -0.45f,  0.0f, -0.45f,  1.0f, 1.0f, 1.0f, // Top left

        // Back right leg
         0.35f, -0.5f, -0.45f,  1.0f, 0.0f, 0.0f, // Bottom left
         0.45f, -0.5f, -0.45f,  0.0f, 1.0f, 0.0f, // Bottom right
         0.45f,  0.0f, -0.45f,  0.0f, 0.0f, 1.0f, // Top right
         0.35f,  0.0f, -0.45f,  1.0f, 1.0f, 1.0f  // Top left
    };

    GLuint indices[] = {
        // Tabletop
        0, 1, 2, 2, 3, 0,
        
        // Front left leg
        4, 5, 6, 6, 7, 4,

        // Front right leg
        8, 9, 10, 10, 11, 8,

        // Back left leg
        12, 13, 14, 14, 15, 12,

        // Back right leg
        16, 17, 18, 18, 19, 16
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0875f, 0.076f, 0.45f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        shaderProgram.Activate();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        double currentTime = glfwGetTime();
        if (currentTime - prevTime >= 1 / 60) {
            rotation += 0.01f;
            prevTime = currentTime;
        }

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)(windowWidth / windowHeight), 0.1f, 100.0f);

        int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        int viewLocation = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLocation = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shaderProgram.Delete();
    glfwTerminate();
    return 0;
}
