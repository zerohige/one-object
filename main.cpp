
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"shaderClass.h"

const int windowWidth = 1000;
const int windowHeight = 1000;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "myscene", 0, 0);
	if (window == NULL)
	{
		std::cout << "window gagal dibuka";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, windowWidth, windowHeight);

	Shader shaderProgram("default.vert", "default.frag");

	GLfloat vertices[] =
	{
		// Vertex coordinates      // Color             // Texture coordinates
		// Front face
		-0.5f, -0.5f,  0.5f,       1.0f, 0.0f, 0.0f,// Front bottom left 0
		 0.5f, -0.5f,  0.5f,       0.0f, 1.0f, 0.0f,// Front bottom right 1
		 0.5f,  0.5f,  0.5f,       0.0f, 0.0f, 1.0f,// Front top right 2
		-0.5f,  0.5f,  0.5f,       1.0f, 1.0f, 1.0f,// Front top left 3

		// Back face
		-0.5f, -0.5f, -0.5f,       1.0f, 0.0f, 0.0f,// Back bottom left 4
		 0.5f, -0.5f, -0.5f,       0.0f, 1.0f, 0.0f,// Back bottom right 5
		 0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f,// Back top right 6
		-0.5f,  0.5f, -0.5f,       1.0f, 1.0f, 1.0f,// Back top left 7

		// Top face
		-0.5f,  0.5f,  0.5f,       1.0f, 0.0f, 0.0f, // Front top left 8
		 0.5f,  0.5f,  0.5f,       0.0f, 1.0f, 0.0f, // Front top right 9
		 0.5f,  0.5f, -0.5f,       0.0f, 0.0f, 1.0f,// Back top right 10
		-0.5f,  0.5f, -0.5f,       1.0f, 1.0f, 1.0f, // Back top left 11

		// Bottom face
		-0.5f, -0.5f,  0.5f,       1.0f, 0.0f, 0.0f,// Front bottom left 12
		 0.5f, -0.5f,  0.5f,       0.0f, 1.0f, 0.0f, // Front bottom right 13
		 0.5f, -0.5f, -0.5f,       0.0f, 0.0f, 1.0f, // Back bottom right 14
		-0.5f, -0.5f, -0.5f,       1.0f, 1.0f, 1.0f // Back bottom left 15
	};

	GLuint indices[] =
	{
		0, 1, 2, 2, 3, 0,     // Front face
		4, 5, 6, 6, 7, 4,     // Back face
		8, 9, 10, 10, 11, 8,  // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		0, 3, 7, 7, 4, 0,     // Left face
		1, 2, 6, 6, 5, 1  // Left face
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

	//konfigurasi koordinat atribut vertex
	//layout, numComponent, type of component, normalize, stride(Jumlah float pada 1 vertice, offset(index awal float)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//konfigurasi warna atribut vertex
	//layout, numComponent, type of component, normalize, stride(Jumlah float pada 1 vertice, offset(index awal float)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0875f, 0.076f, 0.45f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		shaderProgram.Activate();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		double currentTime = glfwGetTime();
		if (currentTime - prevTime >= 1 / 60)
		{
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