#include "../../../../../include/glad/glad.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int verificaCriacaoDoShader(int shader, int* sucess, char* log);

const char* vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main() \n"
								   "{\n"
								   "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\0";

int main(int argc, char** argv)
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

	// cria um shader para o vertice
	// que serve para processar os vertices
	printf("VertexShader...");
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// verifica se o shader foi compilado corretamente
	int success;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (verificaCriacaoDoShader(vertexShader, &success, log))
		return 1;
	printf("OK\n");

	// cria o fragment shader para criar cores nos objetos
	printf("fragmentShader...");
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// verifica se a compilação do shader correu bem
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (verificaCriacaoDoShader(fragmentShader, &success, log))
		return 1;
	printf("OK\n");

	// cira um "shader program" que mantem os shaders criados anteriormente
	// para poder usar eles no programa
	printf("shaderProgram...");
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// verifica se houve erro no processo de criação do shader
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		printf("Erro ao linkar os shaders: ");
		printf("%s\n", log);
		return 1;
	}
	printf("OK\n");

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	// vertices, cordenadas de 0 a 1
	float vertices[] = {
		-0.9f, -0.9f, 0.0f,
		0.0f, -0.9f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.9f, 0.9f, 0.0f,
		0.0f, 0.9f, 0.0f,
		0.5f, 0.0f, 0.0f,
	};

	// cria um buffer de vram para os vertices
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// game loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT);

		// usa o shader que criamos
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

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
