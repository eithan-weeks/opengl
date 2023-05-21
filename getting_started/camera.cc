#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "../shader.hh"
#include "../camera.hh"

namespace getting_started {
	int camera();
}

static GLFWwindow* window;
static float win_w;
static float win_h;

static float deltaTime {0.0f};
static float lastFrame {0.0f};

int getting_started::camera() {
	struct glfw {
		glfw() { glfwInit(); };
		~glfw() { glfwTerminate(); };
	} glfw;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1440, 1080, "Cameras Movement", nullptr, nullptr);
	if (!window)
		return -1;
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		return -1;

	{
		glViewport(0, 0, 1440, 1080);
		win_w = 1440;
		win_h = 1080;
	}
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		win_w = width;
		win_h = height;
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	});

	stbi_set_flip_vertically_on_load(true);

	unsigned int texture1;
	{
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, nrChannels;
		unsigned char * data {stbi_load("getting_started/container.jpg", &width, &height, &nrChannels, 0)};
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int texture2;
	{
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int width, height, nrChannels;
		unsigned char * data {stbi_load("getting_started/awesomeface.png", &width, &height, &nrChannels, 0)};
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	shader ourShader {"getting_started/camera.vert", "getting_started/camera.frag"};
	unsigned int modelLoc;
	unsigned int viewLoc;
	unsigned int projectionLoc;
	{
		glUseProgram(ourShader.id);
		glUniform1i(glGetUniformLocation(ourShader.id, "texture1"), 0);
		glUniform1i(glGetUniformLocation(ourShader.id, "texture2"), 1);
		glUseProgram(0);
		modelLoc = glGetUniformLocation(ourShader.id, "model");
		viewLoc = glGetUniformLocation(ourShader.id, "view");
		projectionLoc = glGetUniformLocation(ourShader.id, "projection");
	}

	unsigned int VBO;
	{
		float vertices[] {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
		};
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	unsigned int EBO;
	{
		unsigned int indices[] {
			 0,  1,  2,  2,  3,  0,
			 4,  5,  6,  6,  7,  4,
			 8,  9, 10, 10, 11,  8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int VAO;
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] {
		glm::vec3( 0.0f,  0.0f,   0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f,  -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,  -7.5f),
		glm::vec3( 1.3f, -2.0f,  -2.5f),
		glm::vec3( 1.5f,  2.0f,  -2.5f),
		glm::vec3( 1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f,  -1.5f)
	};

	struct camera camera {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 3.0f, -90.0f, 0.0f, 45.0f, 5.0f, 50.0f, 50.0f};
	camera.set_move_callback([](bool state[]) {
		state[camera::move::forward    ] = glfwGetKey(window, GLFW_KEY_W         ) == GLFW_PRESS;
		state[camera::move::backward   ] = glfwGetKey(window, GLFW_KEY_S         ) == GLFW_PRESS;
		state[camera::move::right      ] = glfwGetKey(window, GLFW_KEY_D         ) == GLFW_PRESS;
		state[camera::move::left       ] = glfwGetKey(window, GLFW_KEY_A         ) == GLFW_PRESS;
		state[camera::move::up         ] = glfwGetKey(window, GLFW_KEY_SPACE     ) == GLFW_PRESS;
		state[camera::move::down       ] = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
		state[camera::move::rotateright] = glfwGetKey(window, GLFW_KEY_RIGHT     ) == GLFW_PRESS;
		state[camera::move::rotateleft ] = glfwGetKey(window, GLFW_KEY_LEFT      ) == GLFW_PRESS;
		state[camera::move::rotateup   ] = glfwGetKey(window, GLFW_KEY_UP        ) == GLFW_PRESS;
		state[camera::move::rotatedown ] = glfwGetKey(window, GLFW_KEY_DOWN      ) == GLFW_PRESS;
		state[camera::move::zoomin     ] = glfwGetKey(window, GLFW_KEY_Z         ) == GLFW_PRESS;
		state[camera::move::zoomout    ] = glfwGetKey(window, GLFW_KEY_X         ) == GLFW_PRESS;
	});

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		if (win_w == 0 || win_h == 0)
			continue;

		float currentFrame {static_cast<float>(glfwGetTime())};
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glUseProgram(ourShader.id);
		glBindVertexArray(VAO);

		glm::mat4 view {camera.GetViewMatrix()};
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection {glm::perspective(glm::radians(camera.fov), win_w/win_h, 0.1f, 100.0f)};
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (std::size_t i {0}; i < 10; ++i) {
			glm::mat4 model {glm::translate(glm::mat4(1.0f), cubePositions[i])};
			float angle {20.0f * i + static_cast<float>(glfwGetTime()) * 25.0f};
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
		}

		glfwSwapBuffers(window);
	}

	return 0;
}
