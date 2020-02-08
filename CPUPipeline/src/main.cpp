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
#include "../StaticColorSampler.h"
#include "../Image.h"
#include "../ImageSampler.h"
#include "../MeshGenerator.h"

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

glm::vec3 cameraPos, cameraFront, cameraUp;
float fov = 60.f;
int current_width = DEFAULT_WIDTH;
int current_height = DEFAULT_HEIGHT;
double oldXpos = 0;
double oldYpos = 0;
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
	xpos = xpos-current_width/2.0;
	ypos = ypos-current_height/2.0;
	double xDiff = xpos - oldXpos;
	double yDiff = ypos - oldYpos;
	oldXpos = xpos;
	oldYpos = ypos;
	yDiff /= current_height / 2.0;
	if (abs(yDiff) > 0.001)
	{
		glm::vec3 right = glm::cross(cameraFront, cameraUp);
		cameraFront = TransformationMatrices::getRotationMatrix(-(float)yDiff * 0.6f, right)
			* glm::vec4(cameraFront, 1);

	}
	xDiff /= current_width / 2.0;
	if (abs(xDiff) > 0.001)
	{
		cameraFront = TransformationMatrices::getRotationMatrix(-(float)xDiff * 1.6f, cameraUp)
			* glm::vec4(cameraFront, 1);
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//TODO: mouse scroll
	fov += (float)yoffset;
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
int floatToIntColor(const glm::vec3& floatColor)
{
	uint8_t r = (uint8_t)(glm::clamp<float>(floatColor.r, 0.f, 1.f) * 255);
	uint8_t g = (uint8_t)(glm::clamp<float>(floatColor.g, 0.f, 1.f) * 255);
	uint8_t b = (uint8_t)(glm::clamp<float>(floatColor.b, 0.f, 1.f) * 255);
	uint8_t a = (uint8_t)(255);
	return RGBA(r, g, b, a);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	FrameBuffer fb(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	fb.InitGL();

	Image image = Image("data/uvtexture.png");
	Image normalImage = Image("data/brick_normalmap.png");
	normalImage.transform(normalTransformation);

	
	MeshGenerator meshGenerator;
	Scene scene;
	SceneRenderer sceneRenderer(fb);
	sceneRenderer.SetScene(scene);
	Mesh cubeMesh = meshGenerator.getCylinderMesh(2.0f, 0.7f, 10);
	Material cubeMaterial = Material(
		0.9f, 0.1f, 0.1f, 190.0f,
		ImageSampler(image),
		//StaticColorSampler({ 0.0f,0.2f,0.2f }),
		StaticColorSampler({ 0.0f,0.0f,1.0f }));
		//ImageSampler(normalImage));
	SceneObject cube = SceneObject(cubeMesh, glm::identity<glm::mat4>(), cubeMaterial);
	Light light1 = Light({ 2.0f,0.0f,0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
		1.0f, 0.09f, 0.032f);
	Light light2 = Light({ -2.0f,0.0f,0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
		1.0f, 0.09f, 0.032f);
	Light light3 = Light({ 0.0f,0.0f,2.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
		1.0f, 0.09f, 0.032f);
	Light light4 = Light({ 0.0f,-2.0f,0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
		1.0f, 0.09f, 0.032f);
	scene.AddLight(light1);
	scene.AddLight(light2);
	scene.AddLight(light3);
	scene.AddLight(light4);
	
	scene.AddSceneObject(cube);

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
		processInput(window, (float)deltaTime);

		// Update scene

		glm::mat4 modelBase = TransformationMatrices::getScalingMatrix({ 0.2,0.2,0.2 });


		//glm::mat4 rotation = TransformationMatrices::getRotationMatrix(
		//	(float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 translation = TransformationMatrices::getTranslationMatrix({ 0.1,0.1,0 });
		cube.SetWorldMatrix(modelBase);
		camera->SetViewport(0, 0, (float)current_width, (float)current_height);
		camera->SetPerspective(fov, (float)current_height / current_width, 0.1f, 12);
		camera->LookAt(cameraPos, cameraFront, cameraUp);
		float t = (float)(currentTime) * 0.05f;
		light1.setPosition({ 3.0f * glm::cos(t), 1.1f, -3.0f * glm::sin(t) });
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
