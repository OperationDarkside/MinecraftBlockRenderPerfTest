#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

std::unique_ptr<Shader> shader;

GLFWwindow *window;
unsigned int VBO, VAO;

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

const std::array<float, 3> fbl { 0.0f, 0.0f, 0.0f }; // Front Bottom Left
const std::array<float, 3> fbr { 1.0f, 0.0f, 0.0f }; // Front Bottom Right
const std::array<float, 3> ftl { 0.0f, 1.0f, 0.0f }; // Front Top Left
const std::array<float, 3> ftr { 1.0f, 1.0f, 0.0f }; // Front Top Right

const std::array<float, 3> rbl { 0.0f, 0.0f, -1.0f }; // Rear Bottom Left
const std::array<float, 3> rbr { 1.0f, 0.0f, -1.0f }; // Rear Bottom Right
const std::array<float, 3> rtl { 0.0f, 1.0f, -1.0f }; // Rear Top Left
const std::array<float, 3> rtr { 1.0f, 1.0f, -1.0f }; // Rear Top Right

const std::array<float, 3> red { 1.0f, 0.0f, 0.0f };
const std::array<float, 3> green { 0.0f, 1.0f, 0.0f };
const std::array<float, 3> blue { 0.0f, 0.0f, 1.0f };
const std::array<float, 3> white { 1.0f, 1.0f, 1.0f };

std::array<float, 3> data;

std::array<glm::vec3, 50000> cubePositions;

void initGLFW();
void initShaders();
void initVAO();
void processInput(GLFWwindow *window);
template<size_t sz>
void addVertex(std::array<float, sz> &data, size_t offset,
		const std::array<float, 3> &vt);
template<size_t sz>
void addBlock(std::array<float, sz> &data, size_t offset);

int main() {
	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
	firstMouse = true;

	size_t cntr = 0;
	std::array<float, 2000> dTimes;

	// addBlock(data, 0);
	addVertex(data, 0, fbl);
	/*addVertex(data, 3, fbr);
	addVertex(data, 6, ftl);
	addVertex(data, 9, ftr);

	addVertex(data, 12, rbl);
	addVertex(data, 15, rbr);
	addVertex(data, 18, rtl);
	addVertex(data, 21, rtr);
	*/

	std::srand(std::time(nullptr));

	for (int i = 0; i < cubePositions.size(); ++i) {
		//float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X));
		float x = static_cast<float>(rand())
				/ (static_cast<float>(RAND_MAX / 100));
		float y = static_cast<float>(rand())
				/ (static_cast<float>(RAND_MAX / 100));
		float z = static_cast<float>(rand())
				/ (static_cast<float>(RAND_MAX / 100));

		cubePositions[i] = glm::vec3(x, y, z);
	}

	initGLFW();
	initShaders();
	initVAO();

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

		shader->use();
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
				(float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		shader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		shader->setMat4("view", view);

		// render boxes
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < cubePositions.size(); i++) {
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			/*float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle),
					glm::vec3(1.0f, 0.3f, 0.5f));
					*/
			shader->setMat4("model", model);

			glDrawArrays(GL_POINTS, 0, 1);
		}

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

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

template<size_t sz>
void addVertex(std::array<float, sz> &data, size_t offset,
		const std::array<float, 3> &vt) {
	data[offset] = vt[0];
	data[offset + 1] = vt[1];
	data[offset + 2] = vt[2];
}

template<size_t sz>
void addTriangle(std::array<float, sz> &data, size_t offset,
		const std::array<float, 3> &vt1, const std::array<float, 3> &vt2,
		const std::array<float, 3> &vt3) {
	addVertex(data, offset, vt1);
	addVertex(data, offset + 3, vt2);
	addVertex(data, offset + 6, vt3);
}

template<size_t sz>
void addBlock(std::array<float, sz> &data, size_t offset/*,
 std::array<float, 3> &color*/) {
	// Front Quad
	addTriangle(data, 0, fbl, ftr, ftl);
	addTriangle(data, 9, fbl, fbr, ftr);

	// Right Quad
	addTriangle(data, 18, fbr, rtr, ftr);
	addTriangle(data, 27, fbr, rbr, rtr);

	// Back Quad
	addTriangle(data, 36, rbr, rtl, rtr);
	addTriangle(data, 45, rbr, rbl, rtl);

	// Left Quad
	addTriangle(data, 54, rbl, ftl, rtl);
	addTriangle(data, 63, rbl, fbl, ftl);

	// Top Quad
	addTriangle(data, 72, ftl, rtr, rtl);
	addTriangle(data, 81, ftl, ftr, rtr);

	// Bottom Quad
	addTriangle(data, 90, fbl, rbr, rbl);
	addTriangle(data, 99, fbl, fbr, rbr);
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
	shader = std::make_unique<Shader>("shaders/GeoBlock2.vs",
			"shaders/GeoBlock2.fs", "shaders/GeoBlock2.gs");
}

void initVAO() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0],
	GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*) 0);

	// position right attribute
	/*glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
				(void*) (3 * sizeof(float)));
	*/

}

