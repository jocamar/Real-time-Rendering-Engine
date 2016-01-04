// Std. Includes
#include <string>

#include "Globals.h"

// GL includes
#include "Camera.h"
#include "SceneManager.h"
#include "Cube.h"
#include "RenderWindow.h"
#include "Animation.h"
#include "RectangleMesh.h"

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(1.0f, 1.0f, 3.0f));
//Camera camera2(glm::vec3(3.0f, 3.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

vector<SceneNode*> nodes;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	RenderWindow window_ = RenderWindow(900, 600, "RTR - Window", false, true);
	window_.addViewPort(&camera, 0, 0, 900, 600, 0, 0, 1);
	//window_.addViewPort(&camera2, 450, 0, 450, 300, 1, 1);
	window_.setInputHandlers(key_callback, mouse_callback, scroll_callback);

	SceneManager sceneManager;
	sceneManager.addMaterial("box_material", "container2.png", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag",Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("box_material2", "container.jpg", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag",Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("light_material", nullptr, nullptr, "lightVertShader.vs", "lightFragShader.frag",Material::EMITTER);
	sceneManager.addMaterial("sky_front", "box_front.png", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("sky_back", "box_behind.png", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("sky_left", "box_left.png", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("sky_right", "box_right.png", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("sky_top", "box_ceiling.png", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("sky_bottom", "box_floor.png", "container2_specular.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("road", "road.jpg", "black.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("bricks", "bricks.jpg", "black.png", "boxVertShader.vs", "boxFragShader.frag", Material::LIGHTING_TEXTURED);
	sceneManager.setDefaultMaterial("box_material");

	GLfloat amb_dir[3] = { 0.2f, 0.2f, 0.2f };
	GLfloat dif_dir[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat spec_dir[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat dir[3] = { -0.2f, -1.0f, -0.3f };

	sceneManager.createDirectionalLight("directional", amb_dir, dif_dir, spec_dir, dir);

	auto skyEntTop = sceneManager.createEntity("top", new RectangleMesh("top",10));
	auto skyNodeTop = sceneManager.getRoot()->createNewChildNode("skyNodeTop", "sky_top", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeTop->attach(shared_ptr<AttacheableObject>(skyEntTop));
	skyNodeTop->changeScale(glm::vec3(200, 200, 1));
	skyNodeTop->translate(glm::vec3(0, 100, 0));
	skyNodeTop->pitch(90);

	auto skyNodeFront = sceneManager.getRoot()->createNewChildNode("skyNodeFront", "sky_front", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeFront->attach(shared_ptr<AttacheableObject>(skyEntTop));
	skyNodeFront->changeScale(glm::vec3(200, 200, 1));
	skyNodeFront->translate(glm::vec3(0, 0, 100));
	skyNodeFront->roll(180);
	skyNodeFront->yaw(180);

	auto skyNodeBack = sceneManager.getRoot()->createNewChildNode("skyNodeBack", "sky_back", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeBack->attach(shared_ptr<AttacheableObject>(skyEntTop));
	skyNodeBack->changeScale(glm::vec3(200, 200, 1));
	skyNodeBack->translate(glm::vec3(0, 0, -100));
	skyNodeBack->roll(180);

	auto skyNodeBottom = sceneManager.getRoot()->createNewChildNode("skyNodeBottom", "sky_bottom", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeBottom->attach(shared_ptr<AttacheableObject>(skyEntTop));
	skyNodeBottom->changeScale(glm::vec3(200, 200, 1));
	skyNodeBottom->translate(glm::vec3(0, -100, 0));
	skyNodeBottom->pitch(-90);

	auto skyNodeLeft = sceneManager.getRoot()->createNewChildNode("skyNodeLeft", "sky_left", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeLeft->attach(shared_ptr<AttacheableObject>(skyEntTop));
	skyNodeLeft->changeScale(glm::vec3(200, 200, 1));
	skyNodeLeft->translate(glm::vec3(-100, 0, 0));
	skyNodeLeft->yaw(90);
	skyNodeLeft->roll(180);

	auto skyNodeRight = sceneManager.getRoot()->createNewChildNode("skyNodeRight", "sky_right", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeRight->attach(shared_ptr<AttacheableObject>(skyEntTop));
	skyNodeRight->changeScale(glm::vec3(200, 200, 1));
	skyNodeRight->translate(glm::vec3(100, 0, 0));
	skyNodeRight->yaw(-90);
	skyNodeRight->roll(180);

	auto groundEnt = sceneManager.createEntity("groundEnt", new RectangleMesh("ground", 100));
	auto ground = sceneManager.getRoot()->createNewChildNode("groundNode" , "road", glm::vec3(0.0f, 0.0f, 0.0f));
	ground->attach(shared_ptr<AttacheableObject>(groundEnt));
	ground->changeScale(glm::vec3(10, 10, 1));
	ground->translate(glm::vec3(0, -0.5, 0));
	ground->pitch(90);

	auto wallEnt = sceneManager.createEntity("wallEnt", new RectangleMesh("wall", 100,2.5,5));
	auto wall = sceneManager.getRoot()->createNewChildNode("wallNode", "bricks", glm::vec3(1.0f, 2.5f, 0.0f));
	wall->attach(shared_ptr<AttacheableObject>(wallEnt));
	wall->changeScale(glm::vec3(8, 5, 1));
	wall->translate(glm::vec3(0, -0.5, -1));
    wall->yaw(180);

	auto wall2 = sceneManager.getRoot()->createNewChildNode("wallNode2", "bricks", glm::vec3(0.0f, 2.5f, 1.0f));
	wall2->attach(shared_ptr<AttacheableObject>(wallEnt));
	wall2->changeScale(glm::vec3(8, 5, 1));
	wall2->translate(glm::vec3(-1, -0.5, 0));
	wall2->yaw(-90);

	auto cubeEntity = sceneManager.createEntity("entidade", new Cube("cubinho"));
	auto cubeNode = sceneManager.getRoot()->createNewChildNode("cubeNode", "box_material", glm::vec3(0.0f, 0.0f, 1.0f));
	cubeNode->attach(shared_ptr<AttacheableObject>(cubeEntity));

	auto cubeNode2 = sceneManager.getRoot()->createNewChildNode("cubeNode2", "box_material", glm::vec3(0.0f, 0.0f, 1.0f));
	cubeNode2->translate(glm::vec3(1.2, 0, 0));
	cubeNode2->yaw(30);
	cubeNode2->attach(shared_ptr<AttacheableObject>(cubeEntity));

	auto cubeNode3 = sceneManager.getRoot()->createNewChildNode("cubeNode3", "box_material", glm::vec3(0.5f, 1.0f, 1.0f));
	cubeNode3->yaw(15);
	cubeNode3->attach(shared_ptr<AttacheableObject>(cubeEntity));

	/*auto cubeEntity2 = sceneManager.createEntity("entidade2", new RectangleMesh("cubinho2", 10));
	auto cubeNode2 = cubeNode->createNewChildNode("cubeNode2", "box_material2", glm::vec3(0.0, 1.0, 1.0));*/
	vector<glm::vec3> points;
	points.push_back(glm::vec3(0, 0, 2));
	points.push_back(glm::vec3(-2, 0, 2));
	points.push_back(glm::vec3(-2, 0, 0));
	points.push_back(glm::vec3(0, 0, 0));

	Animation *anim = new LinearAnimation(points, 5, true);
	//cubeNode2->setAnimation(anim);
	//cubeNode2->attach(shared_ptr<AttacheableObject>(cubeEntity2));

	GLfloat amb[3] = { 0.0f,0.0f,0.0f };
	GLfloat dif[3] = { 0.8f, 0.8f, 0.8f };
	GLfloat spec[3] = { 1.0f, 1.0f, 1.0f };

	auto light1 = sceneManager.createLight("light1", amb, dif, spec, 1.0f, 0.5f, 0.1f, new Cube("cubinho3"));
	auto lightNode = sceneManager.getRoot()->createNewChildNode("lightNode", "light_material", glm::vec3(3.0f, 2.0f, 0.0f));
	lightNode->changeScale(glm::vec3(0.2, 0.2, 0.2));
	lightNode->attach(shared_ptr<AttacheableObject>(light1));
	lightNode->setAnimation(anim);

	/*cubeNode->translate(glm::vec3(0, 0, 0));
	cubeNode->changeScale(glm::vec3(1.0, 2.0, 1.0));
	cubeNode->setScaleOrig(glm::vec3(0, -0.5, 0.0));
	cubeNode->setRotOrig(glm::vec3(0, -0.5, 0.0));
	cubeNode->pitch(90);
	cubeNode->roll(45);*/

	// Game loop
	while (!window_.close())
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		window_.Update(sceneManager, deltaTime*1000);

		window_.Render(sceneManager);
	}

	return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	/*if (keys[GLFW_KEY_W])
		camera2.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera2.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera2.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera2.ProcessKeyboard(RIGHT, deltaTime);*/
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
//	camera2.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
//	camera2.ProcessMouseScroll(yoffset);
}