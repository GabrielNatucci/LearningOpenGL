#include "../../../include/glad/glad.h"
// #include "../../../../include/glad/glad.h"
// #include <math.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "shader_s.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int verificaCriacaoDoShader(int shader, int* sucess, char* log);

int main()
{
	// inicia o glfw
	glfwInit();

	// essas hints são pra configuração do opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// começa a tela e verifica se deu certo
	GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL", NULL, NULL);
	if (window != NULL) {
		printf("Tela criada com sucesso\n");
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	} else {
		printf("Could not create window");
		glfwTerminate();
		return 1;
	}

	// começa o glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

    Shader ourShader("src/getting_started/shaders/shader.vs", "src/getting_started/shaders/shader.fs");
    
    
    // Shader ourShader("", "");

	// vertices, cordenadas de 0 a 1
	float vertices[] = {
        // posições         // cores
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		// -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	// cria um buffer de vram para os vertices
	printf("Criando buffer vram para os vertices...");
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// glBindVertexArray(0);

    // usa o shader que criamos
	printf("OK\n");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// game loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

        ourShader.use();
		glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

    glfwDestroyWindow(window);
	glfwTerminate();
	printf("Jogo fechando\n");
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, 1);
		printf("Fechando pelo ESC\n");
	}
}

int verificaCriacaoDoShader(int shader, int* sucess, char* log)
{
	glGetShaderiv(shader, GL_COMPILE_STATUS, sucess);

	// verifica se a compilação do shader correu bem
	if (!*sucess) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("Ocorreu um erro a compilar algum Shader: ");
		printf("%s\n", log);
		return 1;
	}

	return 0;
}
