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

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

float fov = 60.f;
int current_width = DEFAULT_WIDTH;
int current_height = DEFAULT_HEIGHT;
Input* input = nullptr;

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	input->setWindowDim(width, height);
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

	/*Image image = Image("data/lion.jpg");
	Image normalImage = Image("data/drop.png");
	normalImage.transform(normalTransformation);*/

	
	Scene scene;
	SceneRenderer sceneRenderer(fb);
	sceneRenderer.SetScene(scene);
	//Mesh cubeMesh = meshGenerator.getCuboidMesh(1.0f, 1.0f, 1.0f);
	//Material cubeMaterial = Material(
	//	0.1f, 0.1f, 0.1f, 1.0f,
	//	std::make_shared<ImageSampler>(ImageSampler(image)),
	//	//StaticColorSampler({ 0.5f,0.2f,0.2f }),
	//	std::make_shared<StaticColorSampler>(StaticColorSampler({ 0.0f,0.0f,1.0f })));
	//	//ImageSampler(normalImage));
	/*SceneObject * cube = new SceneObject(cubeMesh, cubeMaterial);
	SceneObject* cube2 = new SceneObject(
		cubeMesh, cubeMaterial);*/
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
	
	/*scene.AddSceneObject(cube);
	scene.AddSceneObject(cube2);*/

	input = new Input();

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


		// Update scene



		//cube->GetTransform().SetScale({ 0.2,0.2,0.2 });
		//cube->GetTransform().SetPosition({ -0.2,-0.2,0 });
		//cube->GetTransform().SetEulerAngles({ 0,currentTime,0 });
		//cube2->GetTransform().SetScale({ 0.2,0.2,0.2 });
		//cube2->GetTransform().SetPosition({ 2.8,-0.2,0 });
		camera->SetViewport(0, 0, (float)current_width, (float)current_height);
		camera->SetPerspective(fov, (float)current_height / current_width, 0.1f, 12);
		float t = (float)(currentTime) * 0.05f;
		light1.setPosition({ 3.0f * glm::cos(t), 1.1f, -3.0f * glm::sin(t) });
		//fb.ClearColor(0.5f, 0.5f, 1.0f);
		fb.ClearColor(0.1f, 0.15f, 0.15f);

		//write your render pipeline here
		editor.handleInput((float)deltaTime);



		sceneRenderer.RenderScene();
		//textDrawer.DrawTextAt(std::string("abc def"),0 , currentTime * 200);
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
	}

	// Cleanup
	delete camera;
	delete input;
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
