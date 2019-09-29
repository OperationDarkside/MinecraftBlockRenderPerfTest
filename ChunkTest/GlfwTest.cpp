#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Camera.h"

#include <iostream>
#include <array>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <unordered_map>

//#include "ChunkBlockProto.h"
#include "Chunk.h"
#include "ChunkRenderer.h"
#include "ChunkBlockProtoRenderer.h"

std::unique_ptr<Shader> shader;

GLFWwindow *window;
// unsigned int VBO, VAO;

// settings
static const unsigned int SCR_WIDTH = 1200;
static const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
static float lastX;
static float lastY;
static bool firstMouse;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// const unsigned int CHUNKBLOCKSIZE = 300;

// std::array<ChunkBlockProto, CHUNKBLOCKSIZE> chunkBlocks;
Chunk chunk;

//std::array<ChunkBlockProtoRenderer, 16> chunkBlockRenderers;

void initGLFW();
void initShaders();
void processInput(GLFWwindow *window);


int main() {
	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
	firstMouse = true;

	size_t cntr = 0;
	std::array<float, 2000> dTimes;

	std::srand(std::time(nullptr));
	ChunkRenderer chunkRenderer;

	chunk.setWorldPos(glm::vec3(0,0,0));

	ChunkColumn& chunkCol = chunk.getBlocks();

	for (int i = 0; i < chunkCol.size(); ++i) {
		ChunkBlockProto& blockData = chunkCol[i];

		// Random Blocks
		for (int x = 0; x < CHUNK_SIZE; ++x) {
			for (int y = 0; y < CHUNK_SIZE; ++y) {
				for (int z = 0; z < CHUNK_SIZE; ++z) {
					blockData.data[x][y][z] = rand() % 2;
				}
			}
		}
	}

	initGLFW();
	initShaders();

	std::unordered_map<unsigned char, BlockType> blockTypes;
	BlockType typeAir;
	typeAir.id = 0;
	typeAir.opaque = false;
	typeAir.name = "Air";
	blockTypes[0] = typeAir;

	BlockType typeGrass;
	typeGrass.id = 1;
	typeGrass.opaque = true;
	typeGrass.name = "Grass";
	typeGrass.front = {0.1875f, 0.9375f, 0.25f, 0.9375f,0.1875f, 1.0f, 0.25f, 1.0f};
	typeGrass.back = {0.1875f, 0.9375f, 0.25f, 0.9375f,0.1875f, 1.0f, 0.25f, 1.0f};
	typeGrass.right = {0.1875f, 0.9375f, 0.25f, 0.9375f,0.1875f, 1.0f, 0.25f, 1.0f};
	typeGrass.left = {0.1875f, 0.9375f, 0.25f, 0.9375f,0.1875f, 1.0f, 0.25f, 1.0f};
	typeGrass.top = {0.0f, 0.9375f, 0.0625f, 0.9375f,0.0f, 1.0f, 0.0625f, 1.0f};
	typeGrass.bottom = {0.125f, 0.9375f, 0.1875f, 0.9375f,0.125f, 1.0f, 0.1875f, 1.0f};
	blockTypes[1] = typeGrass;

	chunkRenderer.createRenderData(chunk, blockTypes);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("tex/terrain.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader->use();
	shader->setInt("ourTexture", 0);

	float lastFullSec = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		dTimes[cntr++] = deltaTime;
		if (cntr == dTimes.size()) {
			break;
		}

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		shader->use();
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
				(float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		shader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		shader->setMat4("view", view);

		// render boxes
		//glBindVertexArray(VAO);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, chunk.getWorldPos());
		/*float angle = 20.0f * i;
		 model = glm::rotate(model, glm::radians(angle),
		 glm::vec3(1.0f, 0.3f, 0.5f));
		 */
		shader->setMat4("model", model);

		chunkRenderer.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	float avg = std::accumulate(dTimes.begin() + 1, dTimes.end(), 0.0f)
			/ (dTimes.size() - 1);
	float max = *std::max_element(dTimes.begin() + 1, dTimes.end());
	float min = *std::min_element(dTimes.begin() + 1, dTimes.end());

	std::cout << "Average: " << avg << "\nMax: " << max << "\nMin: " << min
			<< "\n\n";

	for (int i = 1; i < dTimes.size(); ++i) {
		std::cout << dTimes[i] << " - ";
	}

	std::cout << std::endl;

	chunkRenderer.deleteStuff();

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL,
	NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
}

void initShaders() {
	shader = std::make_unique<Shader>("shaders/ChunkBlock.vs",
			"shaders/ChunkBlock.fs");
}
