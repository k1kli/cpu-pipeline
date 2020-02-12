#include <stdio.h>
#include <glad/glad.h>

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
#include "../Panel.h"
#include "../GUIController.h"
#include "../Label.h"
#include "../Editor.h"
#include "../Input.h"
#include "../Raycast.h"
#include <iostream>
#include "../SphereMeshGenerator.h"
#include "../CuboidMeshGenerator.h"

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

float fov = 60.f;
int current_width = DEFAULT_WIDTH;
int current_height = DEFAULT_HEIGHT;
int old_width = DEFAULT_WIDTH;
int old_height = DEFAULT_HEIGHT;
Input* input = nullptr;

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	input->setWindowDim(width, height);
	old_width = current_width;
	old_height = current_height;
	current_width = width;
	current_height = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	input->setMousePos(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov += (float)yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		input->setKeyPressed(key);
	}
	else if (action == GLFW_RELEASE)
	{
		input->setKeyReleased(key);
	}
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if (codepoint < 256)
	{
		input->characterTyped(codepoint);
	}
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
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
	FrameBuffer fb(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	fb.InitGL();

	GUIController guiController(fb);


	
	Scene scene;
	SceneRenderer sceneRenderer(fb);
	sceneRenderer.SetScene(scene);

	ImageView texture = scene.getImageStorage().addImage("data/lion.jpg");
	ImageView normalImage = scene.getImageStorage().addImage("data/marbleNormalMap.png");
	normalImage.getImage().transform(normalTransformation);

	Mesh sphereMesh = SphereMeshGenerator(1.0f, 10, 15).getMesh();
	Mesh cubeMesh = CuboidMeshGenerator(1.0f, 0.5f, 0.7f).getMesh();
	Material sphereMaterial = Material({ 1.0f, 0.0f, 1.0f }, { 0.2f, 0.3f, 0.2f }, 100.0f,
		std::make_shared<StaticColorSampler>(glm::vec3(0.5f, 0.5f, 0.5f)),
		std::make_shared<ImageSampler>(normalImage));
	Material cubeMaterial = Material({ 0.0f, 0.0f, 0.0f }, { 0.2f, 0.3f, 0.2f }, 5.0f,
		std::make_shared<ImageSampler>(texture),
		std::make_shared<StaticColorSampler>(glm::vec3(0.0f, 0.0f, 1.0f)));
	Transform sphereTransform;
	sphereTransform.SetScale({ 1.0f, 1.0f, 0.5f });
	scene.addSceneObject(new SceneObject(sphereMesh, sphereMaterial, sphereTransform));
	Transform cubeTransform;
	cubeTransform.SetPosition({ 3.0f, 0.0f, 1.0f });
	cubeTransform.SetEulerAngles({ 1.0f, 1.0f, 1.0f });
	scene.addSceneObject(new SceneObject(cubeMesh, cubeMaterial, cubeTransform));

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
	

	input = new Input(window, &fb);

	Editor editor(guiController, sceneRenderer, &scene, *input,fb, window);


	//TODO: initialize camera
	Camera * camera = new Camera({ -0.5f,0.5f,0.5f }, { 1,0,0 }, { 0,1,0 });
	scene.SetMainCamera(*camera);

	double deltaTime = 0.0;
	double currentTime = 0.0;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		input->updateKeyboardInput();
		glfwPollEvents();
		timeMeasurement(window, deltaTime, currentTime);
		input->updateMouseInput();
		if (current_width != old_width || current_height != old_height)
		{
			old_width = current_width;
			old_height = current_height;
			fb.Resize(current_width, current_height);
		}
		camera->SetViewport(0, 0, (float)current_width, (float)current_height);
		camera->SetPerspective(fov, (float)current_height / current_width, 0.1f, 12);


		// Update scene



		fb.ClearColor(0.1f, 0.15f, 0.15f);

		editor.handleInput((float)deltaTime);
		sceneRenderer.RenderScene();
		guiController.Render();

		
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
		
		while (int err = glGetError())
		{
			std::cout <<"end "<< err << std::endl;
		}
	}

	// Cleanup
	delete camera;
	delete input;
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
