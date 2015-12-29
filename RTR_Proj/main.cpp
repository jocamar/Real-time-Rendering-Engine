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

	GLfloat ambient[] = { 0.2, 0.2, 0.2 };
	GLfloat diffuse[] = { 0.0, 0.0, 0.0 };
	SceneManager sceneManager;
	sceneManager.addMaterial("box_material", "boxVertShader.vs", "boxFragShader.frag", ambient, nullptr, "container2.png", "container2_specular.png", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("box_material2", "boxVertShader.vs", "boxFragShader.frag", ambient, nullptr, "container.jpg", "container2_specular.png", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("light_material", "lightVertShader.vs", "lightFragShader.frag", nullptr, nullptr, nullptr, nullptr,Material::EMITTER);
	sceneManager.addMaterial("sky_front", "boxVertShader.vs", "boxFragShader.frag", nullptr, diffuse, "box_front.png", "black.png", Material::LIGHTING_TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_back", "boxVertShader.vs", "boxFragShader.frag", nullptr, diffuse, "box_behind.png", "black.png", Material::LIGHTING_TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_left", "boxVertShader.vs", "boxFragShader.frag",  nullptr, diffuse, "box_left.png", "black.png", Material::LIGHTING_TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_right", "boxVertShader.vs", "boxFragShader.frag", nullptr, diffuse, "box_right.png", "black.png", Material::LIGHTING_TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_top", "boxVertShader.vs", "boxFragShader.frag", nullptr, diffuse, "box_ceiling.png", "black.png", Material::LIGHTING_TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_bottom", "boxVertShader.vs", "boxFragShader.frag", nullptr, diffuse, "box_floor.png", "black.png", Material::LIGHTING_TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("road", "boxVertShader.vs", "boxFragShader.frag", ambient, nullptr, "road.jpg", "black.png", Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("bricks", "boxVertShader.vs", "boxFragShader.frag", ambient, nullptr, "bricks.jpg", "black.png", Material::LIGHTING_TEXTURED);
	sceneManager.setDefaultMaterial(0);

	sceneManager.addModel("plane", nullptr);
	auto plane = sceneManager.getModel("plane");
	plane->addMesh(new RectangleMesh("top", &sceneManager, 10));

	sceneManager.addModel("planeGround", nullptr);
	auto planeG = sceneManager.getModel("planeGround");
	planeG->addMesh(new RectangleMesh("ground", &sceneManager, 100));

	sceneManager.addModel("planeWall", nullptr);
	auto planeW = sceneManager.getModel("planeWall");
	planeW->addMesh(new RectangleMesh("wall", &sceneManager, 100, 2.5, 5));

	sceneManager.addModel("cube", nullptr);
	auto cube = sceneManager.getModel("cube");
	cube->addMesh(new Cube("cubinho", &sceneManager));

	//sceneManager.addModel("nanosuit", "cs_havana/cs_havana.obj");
	//sceneManager.addModel("nanosuit", "colony sector/colony sector.obj");
	//sceneManager.addModel("nanosuit", "CODMapShipment/Files/CODMapShipment.obj");
	//sceneManager.addModel("nanosuit", "castle/castle.obj");
	//sceneManager.addModel("nanosuit", "Small Tropical Island/Small Tropical Island.obj");
	//sceneManager.addModel("nanosuit", "nanosuit/nanosuit.obj");
	//sceneManager.addModel("nanosuit", "city/Center City Sci-Fi.obj");
	sceneManager.addModel("nanosuit", "Damaged Downtown/Downtown_Damage_0.obj");

	GLfloat amb_dir[3] = { 1.0f, 1.0f, 1.0f };
	GLfloat dif_dir[3] = { 1.0f, 1.0f, 1.0f };
	GLfloat spec_dir[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat dir[3] = { -0.2f, -1.0f, -0.3f };

	sceneManager.createDirectionalLight("directional", amb_dir, dif_dir, spec_dir, dir);

	auto skyEntTop = sceneManager.createEntity("top", "plane");
	auto skyNodeTop = sceneManager.getRoot()->createNewChildNode("skyNodeTop", "sky_top", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeTop->attach(skyEntTop);
	skyNodeTop->changeScale(glm::vec3(500, 500, 1));
	skyNodeTop->translate(glm::vec3(0, 250, 0));
	skyNodeTop->pitch(-90);
	skyNodeTop->roll(180);

	auto skyNodeFront = sceneManager.getRoot()->createNewChildNode("skyNodeFront", "sky_front", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeFront->attach(skyEntTop);
	skyNodeFront->changeScale(glm::vec3(500, 500, 1));
	skyNodeFront->translate(glm::vec3(0, 0, 250));
	skyNodeFront->roll(180);

	auto skyNodeBack = sceneManager.getRoot()->createNewChildNode("skyNodeBack", "sky_back", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeBack->attach(skyEntTop);
	skyNodeBack->changeScale(glm::vec3(500, 500, 1));
	skyNodeBack->translate(glm::vec3(0, 0, -250));
	skyNodeBack->yaw(180);
	skyNodeBack->roll(180);

	auto skyNodeBottom = sceneManager.getRoot()->createNewChildNode("skyNodeBottom", "sky_bottom", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeBottom->attach(skyEntTop);
	skyNodeBottom->changeScale(glm::vec3(500, 500, 1));
	skyNodeBottom->translate(glm::vec3(0, -250, 0));
	skyNodeBottom->pitch(90);
	skyNodeBottom->roll(180);

	auto skyNodeLeft = sceneManager.getRoot()->createNewChildNode("skyNodeLeft", "sky_left", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeLeft->attach(skyEntTop);
	skyNodeLeft->changeScale(glm::vec3(500, 500, 1));
	skyNodeLeft->translate(glm::vec3(250, 0, 0));
	skyNodeLeft->yaw(90);
	skyNodeLeft->roll(180);

	auto skyNodeRight = sceneManager.getRoot()->createNewChildNode("skyNodeRight", "sky_right", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeRight->attach(skyEntTop);
	skyNodeRight->changeScale(glm::vec3(500, 500, 1));
	skyNodeRight->translate(glm::vec3(-250, 0, 0));
	skyNodeRight->yaw(-90);
	skyNodeRight->roll(180);

	/*auto groundEnt = sceneManager.createEntity("groundEnt", "planeGround");
	auto ground = sceneManager.getRoot()->createNewChildNode("groundNode" , "road", glm::vec3(0.0f, 0.0f, 0.0f));
	ground->attach(groundEnt);
	ground->changeScale(glm::vec3(10, 10, 1));
	ground->translate(glm::vec3(0, -0.5, 0));
	ground->pitch(90);

	auto wallEnt = sceneManager.createEntity("wallEnt", "planeWall");
	auto wall = sceneManager.getRoot()->createNewChildNode("wallNode", "bricks", glm::vec3(1.0f, 2.5f, 0.0f));
	wall->attach(wallEnt);
	wall->changeScale(glm::vec3(8, 5, 1));
	wall->translate(glm::vec3(0, -0.5, -1));
    wall->yaw(180);

	auto wall2 = sceneManager.getRoot()->createNewChildNode("wallNode2", "bricks", glm::vec3(0.0f, 2.5f, 1.0f));
	wall2->attach(wallEnt);
	wall2->changeScale(glm::vec3(8, 5, 1));
	wall2->translate(glm::vec3(-1, -0.5, 0));
	wall2->yaw(-90);

	auto cubeEntity = sceneManager.createEntity("entidade", "cube");
	auto cubeNode = sceneManager.getRoot()->createNewChildNode("cubeNode", "box_material", glm::vec3(0.0f, 0.0f, 1.0f));
	cubeNode->attach(cubeEntity);

	auto cubeNode2 = sceneManager.getRoot()->createNewChildNode("cubeNode2", "box_material", glm::vec3(0.0f, 0.0f, 1.0f));
	cubeNode2->translate(glm::vec3(1.2, 0, 0));
	cubeNode2->yaw(30);
	cubeNode2->attach(cubeEntity);*/

	auto nanosuitEntity = sceneManager.createEntity("entidade2", "nanosuit");
	auto cubeNode3 = sceneManager.getRoot()->createNewChildNode("cubeNode3", "box_material", glm::vec3(0.5f, 1.0f, 1.0f));
	cubeNode3->yaw(15);
	//cubeNode3->changeScale(glm::vec3(0.1, 0.1, 0.1));
	cubeNode3->attach(nanosuitEntity);

	vector<glm::vec3> points;
	points.push_back(glm::vec3(0, 0, 2));
	points.push_back(glm::vec3(-2, 0, 2));
	points.push_back(glm::vec3(-2, 0, 0));
	points.push_back(glm::vec3(0, 0, 0));

	Animation *anim = new LinearAnimation(points, 5, true);

	GLfloat amb[3] = { 0.0f,0.0f,0.0f };
	GLfloat dif[3] = { 0.5f, 0.5f, 0.5f };
	GLfloat spec[3] = { 1.0f, 1.0f, 1.0f };

	auto light1 = sceneManager.createLight("light1", amb, dif, spec, 1.0f, 0.5f, 0.1f, "cube");
	auto lightNode = sceneManager.getRoot()->createNewChildNode("lightNode", "light_material", glm::vec3(3.0f, 2.0f, 0.0f));
	lightNode->changeScale(glm::vec3(0.2, 0.2, 0.2));
	lightNode->attach(light1);
	lightNode->setAnimation(anim);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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