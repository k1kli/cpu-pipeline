#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "FrameBuffer.h"
#include "Camera.h"
#include "TransformationMatrices.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <TransformationMatrices.h>
#include "../Scene.h"
#include "../SceneRenderer.h"

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

glm::vec3 cameraPos, cameraFront, cameraUp;
float fov = 60.f;
int current_width = DEFAULT_WIDTH;
int current_height = DEFAULT_HEIGHT;
Camera* camera = nullptr;

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//TODO: Write camera input control here
	glm::vec3 right = cross(cameraFront, cameraUp);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += -right * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos += -cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += right * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraUp * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += -cameraUp * deltaTime;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	current_width = width;
	current_height = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//TODO: write mouse camera control
	/*xpos = xpos-current_width/2.0;
	ypos = ypos-current_height/2.0;
	cameraFront = { 0,0,-1 };
	ypos /= current_height / 2.0;
	if (abs(ypos) > 0.001)
	{
		glm::vec3 right = glm::cross(cameraFront, cameraUp);
		cameraFront = TransformationMatrices::getRotationMatrix(ypos * 3 / 2, right)
			* glm::vec4(cameraFront, 1);

	}
	xpos /= current_width / 2.0;
	if (abs(xpos) > 0.001)
	{
		cameraFront = TransformationMatrices::getRotationMatrix(xpos * 4, cameraUp)
			* glm::vec4(cameraFront, 1);
	}*/


}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//TODO: mouse scroll
	fov += yoffset;
}

void timeMeasurement(GLFWwindow* win, double& deltaTime, double& currentTime)
{
	static int nbFrames = 0;
	static double lastTime = 0.0;
	static double lastFrame = 0.0;
	currentTime = glfwGetTime();
	nbFrames++;
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;

	if (currentTime - lastTime >= 1.0) {
		char title[256];
		title[255] = '\0';

		snprintf(title, 255, "CPU Render Pipeline - [FPS: %3.2f][%3.2f ms]", (float)nbFrames, 1000.f / (float)nbFrames);

		glfwSetWindowTitle(win, title);

		nbFrames = 0;
		lastTime += 1.0;
	}
}

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "CPU Render Pipeline", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to create window!\n");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // 0 - Disable vsync

	// Initialize OpenGL loader
	if (gladLoadGL() == 0)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Set callback for resizing window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	FrameBuffer fb(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	fb.InitGL();

	Scene scene;
	SceneRenderer sceneRenderer(fb);
	sceneRenderer.SetScene(scene);
	SceneObject cube;
	scene.AddSceneObject(cube);

	cube.GetMesh().setVertices({
		{0.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f}, {1.0f,1.0f,0.0f}, {1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f}, {0.0f,1.0f,1.0f}, {1.0f,1.0f,1.0f}, {1.0f,0.0f,1.0f}
	});
	//front, back, right, left, up, down
	cube.GetMesh().setNormals({
		{0.0f,0.0f,1.0f},{0.0f,0.0f,-1.0f},
		{1.0f,0.0f,0.0f},{-1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},{0.0f,-1.0f,0.0f},
	});
	//cube:
	//right handed mesh
	//back:
	//1 2
	//0 3

	//front:
	//5 6
	//4 7
	cube.GetMesh().setTriangles({
			{4,7,6},{4,6,5},//front
			{3,0,1},{3,1,2},//back
			{7,3,2},{7,2,6},//right
			{0,4,5},{0,5,1},//left
			{5,6,2},{5,2,1},//up
			{0,7,4},{0,3,7}//down
	});
	cube.GetMesh().setTrianglesNormals({
			{0,0,0},{0,0,0},//front
			{1,1,1},{1,1,1},//back
			{2,2,2},{2,2,2},//right
			{3,3,3},{3,3,3},//left
			{4,4,4},{4,4,4},//up
			{5,5,5},{5,5,5}//down
	});


	cameraPos = { 0,0,1 };
	cameraFront = { 0,0,-1 };
	cameraUp = { 0,1,0 };

	//TODO: initialize camera
	camera = new Camera(cameraPos, cameraFront, cameraUp);
	scene.SetMainCamera(*camera);

	double deltaTime = 0.0;
	double currentTime = 0.0;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		timeMeasurement(window, deltaTime, currentTime);
		processInput(window, deltaTime);

		// Update scene

		glm::mat4 modelBase = TransformationMatrices::getTranslationMatrix({ -0.1,-0.1,-0.1 })
			* TransformationMatrices::getScalingMatrix({ 0.2,0.2,0.2 });


		glm::mat4 rotation = TransformationMatrices::getRotationMatrix(currentTime, cameraUp);
		glm::mat4 translation = TransformationMatrices::getTranslationMatrix({ 0.1,0.1,0 });
		cube.SetWorldMatrix(rotation * modelBase);
		camera->SetViewport(0, 0, current_width, current_height);
		camera->SetPerspective(fov, (float)current_height / current_width, 0.1, 10);
		camera->LookAt(cameraPos, cameraFront, cameraUp);

		//fb.ClearColor(0.5f, 0.5f, 1.0f);
		fb.ClearColor(0.1f, 0.15f, 0.15f);

		//write your render pipeline here



		//TODO: calculate point positions
		sceneRenderer.RenderScene();

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		fb.RenderGL();

		glfwSwapBuffers(window);
	}

	// Cleanup
	delete camera;
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
