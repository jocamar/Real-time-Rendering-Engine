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

#include <irrKlang.h>
using namespace irrklang;

// Properties
GLuint screenWidth = 1280, screenHeight = 720;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement(Viewport *vp);

// Camera
Camera* camera;
//Camera camera2(glm::vec3(3.0f, 3.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool pressingToggleBloom = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

vector<SceneNode*> nodes;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	RenderWindow window_ = RenderWindow(screenWidth, screenHeight, "RTR - Window", false, true);
	SceneManager sceneManager;
	camera = new Camera(&sceneManager, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0, 0, 1), false, 0.1f, 2000.0f, (float)1280 / (float)720, 45.0f);
	auto vp = window_.addViewPort(camera, 0, 0, screenWidth, screenHeight, 0, 0.125, 0.125, 0.25);
	window_.setInputHandlers(key_callback, mouse_callback, scroll_callback);

	ISoundEngine *SoundEngine = createIrrKlangDevice();

	GLfloat ambient[] = { 0.1, 0.1, 0.1 };
	GLfloat diffuse[] = { 0.0, 0.0, 0.0 };
	GLfloat specular[] = { 0.0, 0.0, 0.0 };
	GLfloat specular_brick[] = { 0.3, 0.3, 0.3 };

	GLfloat diffuseL1[] = { 1.0, 0.0, 0.0 };
	GLfloat diffuseL2[] = { 1.0, 1.0, 1.0 };
	sceneManager.addMaterial("box_material", "defaultShader.vs", "defaultShader.frag", "container2.png", "container2_specular.png", nullptr, ambient, nullptr, nullptr, 32, 1, 2, Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("light_material", "lightVertShader.vs", "lightFragShader.frag", nullptr, nullptr, nullptr, nullptr, diffuseL1, nullptr, 0, 1, 1, Material::EMITTER);
	sceneManager.addMaterial("light_material2", "lightVertShader.vs", "lightFragShader.frag", nullptr, nullptr, nullptr, nullptr, diffuseL2, nullptr, 0, 1, 1, Material::EMITTER);
	sceneManager.addMaterial("sky_front", "simpleShader.vs", "simpleShader.frag", "posx.jpg", "black.png", nullptr, nullptr, diffuse, specular, 0, 1, 0, Material::TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_back", "simpleShader.vs", "simpleShader.frag", "negx.jpg", "black.png", nullptr, nullptr, diffuse, specular, 0, 1, 0, Material::TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_left", "simpleShader.vs", "simpleShader.frag", "posz.jpg", "black.png", nullptr, nullptr, diffuse, specular, 0, 1, 0, Material::TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_right", "simpleShader.vs", "simpleShader.frag", "negz.jpg", "black.png", nullptr, nullptr, diffuse, specular, 0, 1, 0, Material::TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_top", "simpleShader.vs", "simpleShader.frag", "posy.jpg", "black.png", nullptr, nullptr, diffuse, specular, 0, 1, 0, Material::TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("sky_bottom", "simpleShader.vs", "simpleShader.frag", "negy.jpg", "black.png", nullptr, nullptr, diffuse, specular, 0, 1, 0, Material::TEXTURED, GL_CLAMP_TO_EDGE);
	sceneManager.addMaterial("road", "defaultShader.vs", "defaultShader.frag", "186.png", nullptr, "186_norm.png", ambient, nullptr, specular, 1, 1, 2, Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("bricks", "defaultShader.vs", "defaultShader.frag", "154.jpg", nullptr, "154_norm.jpg", ambient, nullptr, specular_brick, 5, 1, 2, Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("wall2", "defaultShader.vs", "defaultShader.frag", "152.png", nullptr, "152_norm.png", ambient, nullptr, specular_brick, 5, 1, 1, Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("wall3", "defaultShader.vs", "defaultShader.frag", "196.png", nullptr, "196_norm.png", ambient, nullptr, specular_brick, 5, 1, 1, Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("wall4", "defaultShader.vs", "defaultShader.frag", "178.png", nullptr, "178_norm.png", ambient, nullptr, specular_brick, 5, 1, 1, Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("wall5", "defaultShader.vs", "defaultShader.frag", "180.png", nullptr, "180_norm.png", ambient, nullptr, specular_brick, 5, 1, 1, Material::LIGHTING_TEXTURED);
	sceneManager.addMaterial("wall6", "defaultShader.vs", "defaultShader.frag", "181.png", nullptr, "181_norm.png", ambient, nullptr, specular_brick, 5, 1, 1, Material::LIGHTING_TEXTURED);
	sceneManager.setDefaultMaterial(0);

	sceneManager.addModel("plane", nullptr);
	auto plane = sceneManager.getModel("plane");
	plane->addMesh(new RectangleMesh("top", &sceneManager, 1));

	sceneManager.addModel("planeGround", nullptr);
	auto planeG = sceneManager.getModel("planeGround");
	planeG->addMesh(new RectangleMesh("ground", &sceneManager, 1, 10, 5));

	sceneManager.addModel("planeWall", nullptr);
	auto planeW = sceneManager.getModel("planeWall");
	planeW->addMesh(new RectangleMesh("wall", &sceneManager, 10, 6, 4));

	sceneManager.addModel("planeWallLarge", nullptr);
	auto planeWL = sceneManager.getModel("planeWallLarge");
	planeWL->addMesh(new RectangleMesh("wallL", &sceneManager, 10, 6, 8));

	sceneManager.addModel("planeWallSmall", nullptr);
	auto planeWS = sceneManager.getModel("planeWallSmall");
	planeWS->addMesh(new RectangleMesh("wallS", &sceneManager, 10, 1, 4));

	sceneManager.addModel("cube", nullptr);
	auto cube = sceneManager.getModel("cube");
	cube->addMesh(new Cube("cubinho", &sceneManager));

	sceneManager.addModel("building", "building1/building.3ds");
	sceneManager.addModel("lamp", "Street Lamp/StreetLamp.obj");
	sceneManager.addModel("dumpster", "trash/Dumpster.obj");
	sceneManager.addModel("fence", "fence/fence.fbx");
	sceneManager.addModel("helicopter", "Combine Helicopter/Combine_Helicopter.obj");
	sceneManager.addModel("trash_bin", "Trash_Bin/trashbin.3ds");
	sceneManager.addModel("statue", "droid/attack_droid.obj");

	GLfloat amb_dir[3] = { 0.001f, 0.001f, 0.001f };
	GLfloat dif_dir[3] = { 0.01f, 0.01f, 0.01f };
	GLfloat spec_dir[3] = { 0.01f, 0.01f, 0.01f };
	GLfloat dir[3] = { 0.5f, -0.7f, 1.0f };

	sceneManager.createDirectionalLight("directional", amb_dir, dif_dir, spec_dir, dir);

	auto skyBox = sceneManager.getRoot()->createNewChildNode("skyBox", "sky_top", glm::vec3(0.0f, 0.0f, 0.0f));
	skyBox->yaw(90);

	auto skyEntTop = sceneManager.createEntity("top", "plane", false);
	auto skyNodeTop = skyBox->createNewChildNode("skyNodeTop", "sky_top", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeTop->attach(skyEntTop);
	skyNodeTop->changeScale(glm::vec3(300, 300, 1));
	skyNodeTop->translate(glm::vec3(0, 150, 0));
	skyNodeTop->pitch(-90);
	skyNodeTop->roll(-90);

	auto skyEntFront = sceneManager.createEntity("front", "plane", false);
	auto skyNodeFront = skyBox->createNewChildNode("skyNodeFront", "sky_front", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeFront->attach(skyEntFront);
	skyNodeFront->changeScale(glm::vec3(300, 300, 1));
	skyNodeFront->translate(glm::vec3(0, 0, 150));
	skyNodeFront->roll(180);

	auto skyEntBack = sceneManager.createEntity("back", "plane", false);
	auto skyNodeBack = skyBox->createNewChildNode("skyNodeBack", "sky_back", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeBack->attach(skyEntBack);
	skyNodeBack->changeScale(glm::vec3(300, 300, 1));
	skyNodeBack->translate(glm::vec3(0, 0, -150));
	skyNodeBack->yaw(180);
	skyNodeBack->roll(180);

	auto skyEntLeft = sceneManager.createEntity("left", "plane", false);
	auto skyNodeLeft = skyBox->createNewChildNode("skyNodeLeft", "sky_left", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeLeft->attach(skyEntLeft);
	skyNodeLeft->changeScale(glm::vec3(300, 300, 1));
	skyNodeLeft->translate(glm::vec3(150, 0, 0));
	skyNodeLeft->yaw(90);
	skyNodeLeft->roll(180);

	auto skyEntRight = sceneManager.createEntity("right", "plane", false);
	auto skyNodeRight = skyBox->createNewChildNode("skyNodeRight", "sky_right", glm::vec3(0.0f, 0.0f, 0.0f));
	skyNodeRight->attach(skyEntRight);
	skyNodeRight->changeScale(glm::vec3(300, 300, 1));
	skyNodeRight->translate(glm::vec3(-150, 0, 0));
	skyNodeRight->yaw(-90);
	skyNodeRight->roll(180);

	auto groundEnt = sceneManager.createEntity("groundEnt", "planeGround", false);
	auto ground = sceneManager.getRoot()->createNewChildNode("groundNode" , "road", glm::vec3(0.0f, 0.0f, 0.0f));
	ground->attach(groundEnt);
	ground->changeScale(glm::vec3(20, 10, 1));
	ground->pitch(90);

	auto wallEnt = sceneManager.createEntity("wallEnt", "planeWall");
	auto wall = sceneManager.getRoot()->createNewChildNode("wallNode", "bricks", glm::vec3(5.0f, 2.5f, -5.0f));
	wall->attach(wallEnt);
	wall->changeScale(glm::vec3(10, 5, 1));
    wall->yaw(180);

	auto buildingEnt = sceneManager.createEntity("buildingEnt", "building", false);
	auto building = sceneManager.getRoot()->createNewChildNode("buildingNode", "bricks", glm::vec3(-40.0f, 8.0f, -10.0f));
	building->attach(buildingEnt);
	building->changeScale(glm::vec3(0.003, 0.003, 0.003));
	building->yaw(0);

	auto wallEnt2 = sceneManager.createEntity("wallEnt2", "planeWall");
	auto wall2 = sceneManager.getRoot()->createNewChildNode("wallNode2", "wall2", glm::vec3(10.0f, 2.5f, 0.0f));
	wall2->attach(wallEnt2);
	wall2->changeScale(glm::vec3(10, 5, 1));
	wall2->yaw(90);

	auto wallEnt3 = sceneManager.createEntity("wallEnt3", "planeWall");
	auto wall3 = sceneManager.getRoot()->createNewChildNode("wallNode3", "bricks", glm::vec3(5.0f, 2.5f, 5.0f));
	wall3->attach(wallEnt3);
	wall3->changeScale(glm::vec3(10, 5, 1));
	wall3->yaw(0);

	auto wallEnt3Shadow2 = sceneManager.createEntity("wallEnt3Shadow2", "planeWall");
	auto wall3Shadow2 = sceneManager.getRoot()->createNewChildNode("wallNode3Shadow2", "bricks", glm::vec3(13.0f, 3.5f, 1.0f));
	wall3Shadow2->attach(wallEnt3Shadow2);
	wall3Shadow2->changeScale(glm::vec3(15, 7, 1));
	wall3Shadow2->yaw(90);


	auto wallEnt4 = sceneManager.createEntity("wallEnt4", "planeWall");
	auto wall4 = sceneManager.getRoot()->createNewChildNode("wallNode4", "wall3", glm::vec3(0.0f, 2.5f, 5.0f));
	wall4->attach(wallEnt4);
	wall4->changeScale(glm::vec3(10, 5, 1));
	wall4->yaw(-90);

	auto wallEnt5 = sceneManager.createEntity("wallEnt5", "planeWall");
	auto wall5 = sceneManager.getRoot()->createNewChildNode("wallNode5", "wall3", glm::vec3(-5.0f, 2.5f, 0.0f));
	wall5->attach(wallEnt5);
	wall5->changeScale(glm::vec3(10, 5, 1));
	wall5->yaw(0);

	/*auto wallEnt5Shadow = sceneManager.createEntity("wallEnt5Shadow", "planeWall");
	auto wall5Shadow = sceneManager.getRoot()->createNewChildNode("wallNode5Shadow", "wall3", glm::vec3(-7.0f, 2.5f, 2.0f));
	wall5Shadow->attach(wallEnt5Shadow);
	wall5Shadow->changeScale(glm::vec3(10, 5, 1));
	wall5Shadow->yaw(0);*/

	auto wallEnt6 = sceneManager.createEntity("wallEnt6", "planeWall");
	auto wall6 = sceneManager.getRoot()->createNewChildNode("wallNode6", "wall4", glm::vec3(3.0f, 2.5f, -9.0f));
	wall6->attach(wallEnt6);
	wall6->changeScale(glm::vec3(10, 5, 1));
	wall6->yaw(-90);

	auto wallEnt7 = sceneManager.createEntity("wallEnt7", "planeWallLarge");
	auto wall7 = sceneManager.getRoot()->createNewChildNode("wallNode7", "wall4", glm::vec3(-2.0f, 5.0f, -4.0f));
	wall7->attach(wallEnt7);
	wall7->changeScale(glm::vec3(10, 10, 1));
	wall7->yaw(180);

	/*auto wallEnt8 = sceneManager.createEntity("wallEnt8", "planeWallSmall");
	auto wall8 = sceneManager.getRoot()->createNewChildNode("wallNode8", "wall5", glm::vec3(5.5f, 3.5f, -4.7f));
	wall8->attach(wallEnt8);
	wall8->changeScale(glm::vec3(1, 5, 1));
	wall8->yaw(180);
	wall8->roll(90);

	auto wallEnt9 = sceneManager.createEntity("wallEnt9", "planeWallSmall");
	auto wall9 = sceneManager.getRoot()->createNewChildNode("wallNode9", "wall5", glm::vec3(10.5f, 3.5f, -4.7f));
	wall9->attach(wallEnt9);
	wall9->changeScale(glm::vec3(1, 5, 1));
	wall9->yaw(180);
	wall9->roll(90);

	auto wallEnt10 = sceneManager.createEntity("wallEnt10", "planeWallSmall");
	auto wall10 = sceneManager.getRoot()->createNewChildNode("wallNode10", "wall5", glm::vec3(5.5f, 3.0f, -5.2f));
	wall10->attach(wallEnt10);
	wall10->changeScale(glm::vec3(1, 5, 1));
	wall10->yaw(180);
	wall10->pitch(-90);
	wall10->roll(90);

	auto wallEnt11 = sceneManager.createEntity("wallEnt11", "planeWallSmall");
	auto wall11 = sceneManager.getRoot()->createNewChildNode("wallNode11", "wall5", glm::vec3(10.5f,3.0f, -5.2f));
	wall11->attach(wallEnt11);
	wall11->changeScale(glm::vec3(1, 5, 1));
	wall11->yaw(180);
	wall11->pitch(-90);
	wall11->roll(90);*/

	auto wallEnt12 = sceneManager.createEntity("wallEnt12", "planeWallSmall");
	auto wall12 = sceneManager.getRoot()->createNewChildNode("wallNode12", "wall5", glm::vec3(5.5f, 5.0f, -4.7f));
	wall12->attach(wallEnt12);
	wall12->changeScale(glm::vec3(1, 5, 1));
	wall12->yaw(180);
	wall12->pitch(-90);
	wall12->roll(90);

	auto wallEnt13 = sceneManager.createEntity("wallEnt13", "planeWallSmall");
	auto wall13 = sceneManager.getRoot()->createNewChildNode("wallNode13", "wall5", glm::vec3(10.5f, 5.0f, -4.7f));
	wall13->attach(wallEnt13);
	wall13->changeScale(glm::vec3(1, 5, 1));
	wall13->yaw(180);
	wall13->pitch(-90);
	wall13->roll(90);

	auto wallEnt14 = sceneManager.createEntity("wallEnt14", "planeWall");
	auto wall14 = sceneManager.getRoot()->createNewChildNode("wallNode14", "wall5", glm::vec3(5.5f, 7.5f, -4.2f));
	wall14->attach(wallEnt14);
	wall14->changeScale(glm::vec3(10, 5, 1));
	wall14->yaw(180);

	auto wallEnt15 = sceneManager.createEntity("wallEnt15", "planeWallSmall");
	auto wall15 = sceneManager.getRoot()->createNewChildNode("wallNode15", "wall5", glm::vec3(10.0f, 5.0f, 2.5f));
	wall15->attach(wallEnt15);
	wall15->changeScale(glm::vec3(1, 5, 1));
	wall15->yaw(90);
	wall15->pitch(-90);
	wall15->roll(90);

	auto wallEnt16 = sceneManager.createEntity("wallEnt16", "planeWallSmall");
	auto wall16 = sceneManager.getRoot()->createNewChildNode("wallNode16", "wall5", glm::vec3(10.0f, 5.0f, -1.75f));
	wall16->attach(wallEnt16);
	wall16->changeScale(glm::vec3(1, 5, 1));
	wall16->yaw(90);
	wall16->pitch(-90);
	wall16->roll(90);

	auto wallEnt17 = sceneManager.createEntity("wallEnt17", "planeWallSmall");
	auto wall17 = sceneManager.getRoot()->createNewChildNode("wallNode17", "wall5", glm::vec3(9.5f, 5.5f, -2.5f));
	wall17->attach(wallEnt17);
	wall17->changeScale(glm::vec3(1, 5, 1));
	wall17->yaw(90);
	wall17->roll(90);

	auto wallEnt18 = sceneManager.createEntity("wallEnt18", "planeWallSmall");
	auto wall18 = sceneManager.getRoot()->createNewChildNode("wallNode18", "wall5", glm::vec3(9.5f, 5.5f, 2.5f));
	wall18->attach(wallEnt18);
	wall18->changeScale(glm::vec3(1, 5, 1));
	wall18->yaw(90);
	wall18->roll(90);

	auto wallEnt19 = sceneManager.createEntity("wallEnt19", "planeWall");
	auto wall19 = sceneManager.getRoot()->createNewChildNode("wallNode19", "wall4", glm::vec3(3.0f, 7.5f, -9.0f));
	wall19->attach(wallEnt19);
	wall19->changeScale(glm::vec3(10, 5, 1));
	wall19->yaw(-90);

	auto wallEnt20 = sceneManager.createEntity("wallEnt20", "planeWall");
	auto wall20 = sceneManager.getRoot()->createNewChildNode("wallNode20", "wall4", glm::vec3(-2.0f, 7.5f, -4.0f));
	wall20->attach(wallEnt20);
	wall20->changeScale(glm::vec3(10, 5, 1));
	wall20->yaw(180);

	auto wallEnt21 = sceneManager.createEntity("wallEnt21", "planeWall");
	auto wall21 = sceneManager.getRoot()->createNewChildNode("wallNode21", "bricks", glm::vec3(5.0f, 7.5f, 5.0f));
	wall21->attach(wallEnt21);
	wall21->changeScale(glm::vec3(10, 5, 1));
	wall21->yaw(0);

	auto wallEnt22 = sceneManager.createEntity("wallEnt22", "planeWallSmall");
	auto wall22 = sceneManager.getRoot()->createNewChildNode("wallNode22", "wall6", glm::vec3(0.1f, 5.5f, 2.5f));
	wall22->attach(wallEnt22);
	wall22->changeScale(glm::vec3(1, 5, 1));
	wall22->yaw(-90);
	wall22->roll(90);

	auto wallEnt23 = sceneManager.createEntity("wallEnt23", "planeWallSmall");
	auto wall23 = sceneManager.getRoot()->createNewChildNode("wallNode23", "wall6", glm::vec3(-0.4f, 5.0f, 2.5f));
	wall23->attach(wallEnt23);
	wall23->changeScale(glm::vec3(1, 5, 1));
	wall23->pitch(-90);

	auto wallEnt26 = sceneManager.createEntity("wallEnt26", "planeWallSmall");
	auto wall26 = sceneManager.getRoot()->createNewChildNode("wallNode26", "wall6", glm::vec3(-2.4f, 5.5f, 0.0f));
	wall26->attach(wallEnt26);
	wall26->changeScale(glm::vec3(1, 5, 1));
	wall26->yaw(0);
	wall26->roll(90);

	auto wallEnt27 = sceneManager.createEntity("wallEnt27", "planeWall", false);
	auto wall27 = sceneManager.getRoot()->createNewChildNode("wallNode27", "bricks", glm::vec3(-10.f, 0.5f, -3.0f));
	wall27->attach(wallEnt27);
	wall27->changeScale(glm::vec3(10, 5, 1));
	wall27->yaw(-90);

	auto lampEnt = sceneManager.createEntity("lampEnt", "lamp");
	auto lamp = sceneManager.getRoot()->createNewChildNode("lamp", "bricks", glm::vec3(0.5f, -1.0f, 1.8f));
	lamp->attach(lampEnt);
	lamp->changeScale(glm::vec3(0.3, 0.5, 0.3));
	lamp->yaw(0);

	auto dumpsterEnt = sceneManager.createEntity("dumpsterEnt", "dumpster");
	auto dumpster = sceneManager.getRoot()->createNewChildNode("dumpster", "bricks", glm::vec3(6.5f, 0.0f, 3.0f));
	dumpster->attach(dumpsterEnt);
	dumpster->changeScale(glm::vec3(0.5, 0.5, 0.5));
	dumpster->yaw(-12);

	auto trashbinEnt = sceneManager.createEntity("trashbinEnt", "trash_bin");
	auto trashbin = sceneManager.getRoot()->createNewChildNode("trash_bin", "bricks", glm::vec3(6.5f, 0.0f, -3.0f));
	trashbin->attach(trashbinEnt);
	trashbin->changeScale(glm::vec3(0.01, 0.01, 0.01));

	auto trashbin2Ent = sceneManager.createEntity("trashbin2Ent", "trash_bin");
	auto trashbin2 = sceneManager.getRoot()->createNewChildNode("trash_bin2", "bricks", glm::vec3(-4.5f, 0.0f, -0.6f));
	trashbin2->attach(trashbin2Ent);
	trashbin2->changeScale(glm::vec3(0.01, 0.01, 0.01));

	auto statueEnt = sceneManager.createEntity("statueEnt", "statue");
	auto statue = sceneManager.getRoot()->createNewChildNode("statue", "bricks", glm::vec3(2.5f, 0.0f, -0.8f));
	statue->attach(statueEnt);
	statue->changeScale(glm::vec3(0.06, 0.06, 0.06));
	statue->yaw(135);

	auto fenceEnt = sceneManager.createEntity("fenceEnt", "fence", false, true);
	auto fence = sceneManager.getRoot()->createNewChildNode("fenceNode", "bricks", glm::vec3(-5.0f, 1.0f, -2.0f));
	fence->attach(fenceEnt);
	fence->changeScale(glm::vec3(0.02, 0.03, 0.02));
	fence->pitch(-90);
	fence->yaw(0);

	vector<std::pair<long, float>> points_Y_heli;
	points_Y_heli.push_back(std::pair<long, float>(30000, 3));
	points_Y_heli.push_back(std::pair<long, float>(31500, 6));
	points_Y_heli.push_back(std::pair<long, float>(33000, 9));
	points_Y_heli.push_back(std::pair<long, float>(34500, 12));
	points_Y_heli.push_back(std::pair<long, float>(36000, 15));
	points_Y_heli.push_back(std::pair<long, float>(37500, 18));
	points_Y_heli.push_back(std::pair<long, float>(39000, 20));
	points_Y_heli.push_back(std::pair<long, float>(53000, 20));
	points_Y_heli.push_back(std::pair<long, float>(53500, 19));
	points_Y_heli.push_back(std::pair<long, float>(62000, 10));
	points_Y_heli.push_back(std::pair<long, float>(67000, 11));
	points_Y_heli.push_back(std::pair<long, float>(67500, 12));
	points_Y_heli.push_back(std::pair<long, float>(68000, 14));
	points_Y_heli.push_back(std::pair<long, float>(69000, 18));
	BetterAnimation *anim_heli = new BetterAnimation(false);
	anim_heli->addControlPoints(points_Y_heli, BetterAnimation::Attribute::POS_Y);

	vector<std::pair<long, float>> points_X_heli;
	points_X_heli.push_back(std::pair<long, float>(39000, 0.0));
	points_X_heli.push_back(std::pair<long, float>(39500, -0.5));
	points_X_heli.push_back(std::pair<long, float>(40000, -1.25));
	points_X_heli.push_back(std::pair<long, float>(40500, -2.5));
	points_X_heli.push_back(std::pair<long, float>(41000, -4.0));
	points_X_heli.push_back(std::pair<long, float>(41500, -6.0));
	points_X_heli.push_back(std::pair<long, float>(42000, -9.0));
	points_X_heli.push_back(std::pair<long, float>(42500, -13.0));
	points_X_heli.push_back(std::pair<long, float>(43000, -18.0));
	points_X_heli.push_back(std::pair<long, float>(43500, -23.0));
	points_X_heli.push_back(std::pair<long, float>(44000, -27.0));
	points_X_heli.push_back(std::pair<long, float>(44500, -30.0));
	points_X_heli.push_back(std::pair<long, float>(45000, -32.0));
	points_X_heli.push_back(std::pair<long, float>(45500, -33.5));
	points_X_heli.push_back(std::pair<long, float>(46000, -34.75));
	points_X_heli.push_back(std::pair<long, float>(46500, -35.5));
	points_X_heli.push_back(std::pair<long, float>(47000, -36.0));
	//Finished swooping around
	points_X_heli.push_back(std::pair<long, float>(48000, -36.0));
	points_X_heli.push_back(std::pair<long, float>(48250, -35.0));
	points_X_heli.push_back(std::pair<long, float>(49000, -33.0));
	points_X_heli.push_back(std::pair<long, float>(49500, -29.0));
	points_X_heli.push_back(std::pair<long, float>(50000, -24.0));
	points_X_heli.push_back(std::pair<long, float>(51500, -9.0));
	points_X_heli.push_back(std::pair<long, float>(52000, -5.0));
	points_X_heli.push_back(std::pair<long, float>(53500, -2.0));
	points_X_heli.push_back(std::pair<long, float>(54000, 0.0));
	points_X_heli.push_back(std::pair<long, float>(54500, -0.5));
	points_X_heli.push_back(std::pair<long, float>(55000, -1.25));
	points_X_heli.push_back(std::pair<long, float>(55500, -2.5));
	points_X_heli.push_back(std::pair<long, float>(62000, -8.0));
	points_X_heli.push_back(std::pair<long, float>(67000, -9.0));
	points_X_heli.push_back(std::pair<long, float>(67500, -8.0));
	points_X_heli.push_back(std::pair<long, float>(68000, -6.0));
	points_X_heli.push_back(std::pair<long, float>(69500, 3.0));
	points_X_heli.push_back(std::pair<long, float>(70000, 3.5));
	points_X_heli.push_back(std::pair<long, float>(70500, 3.0));
	points_X_heli.push_back(std::pair<long, float>(71000, 2.0));
	points_X_heli.push_back(std::pair<long, float>(71500, 0.0));
	points_X_heli.push_back(std::pair<long, float>(72000, -4.0));
	points_X_heli.push_back(std::pair<long, float>(80000, -64.0));


	anim_heli->addControlPoints(points_X_heli, BetterAnimation::Attribute::POS_X);

	vector<std::pair<long, float>> points_Z_heli;
	points_Z_heli.push_back(std::pair<long, float>(39000, 0.0));
	points_Z_heli.push_back(std::pair<long, float>(39500, -1.0));
	points_Z_heli.push_back(std::pair<long, float>(40000, -2.0));
	points_Z_heli.push_back(std::pair<long, float>(40500, -4.0));
	points_Z_heli.push_back(std::pair<long, float>(41000, -8.0));
	points_Z_heli.push_back(std::pair<long, float>(41500, -12.0));
	points_Z_heli.push_back(std::pair<long, float>(42000, -15.0));
	points_Z_heli.push_back(std::pair<long, float>(42500, -17.0));
	points_Z_heli.push_back(std::pair<long, float>(43000, -18.0));
	points_Z_heli.push_back(std::pair<long, float>(43500, -17.0));
	points_Z_heli.push_back(std::pair<long, float>(44000, -15.0));
	points_Z_heli.push_back(std::pair<long, float>(44500, -12.0));
	points_Z_heli.push_back(std::pair<long, float>(45000, -8.0));
	points_Z_heli.push_back(std::pair<long, float>(45500, -4.0));
	points_Z_heli.push_back(std::pair<long, float>(46000, -2.0));
	points_Z_heli.push_back(std::pair<long, float>(46500, -1.0));
	points_Z_heli.push_back(std::pair<long, float>(47000, 0.0));
	points_Z_heli.push_back(std::pair<long, float>(47500, -0.5));
	points_Z_heli.push_back(std::pair<long, float>(48000, 0.0));
	points_Z_heli.push_back(std::pair<long, float>(70000, 0.0));
	points_Z_heli.push_back(std::pair<long, float>(80000, 15.0));
	anim_heli->addControlPoints(points_Z_heli, BetterAnimation::Attribute::POS_Z);

	vector<std::pair<long, float>> points_Yaw_heli;
	points_Yaw_heli.push_back(std::pair<long, float>(39000, 0.0));
	points_Yaw_heli.push_back(std::pair<long, float>(40000, 15.0));
	points_Yaw_heli.push_back(std::pair<long, float>(41000, 50.0));
	points_Yaw_heli.push_back(std::pair<long, float>(42000, 90.0));
	points_Yaw_heli.push_back(std::pair<long, float>(45000, 130.0));
	points_Yaw_heli.push_back(std::pair<long, float>(46000, 165.0));
	points_Yaw_heli.push_back(std::pair<long, float>(47000, 180.0));
	points_Yaw_heli.push_back(std::pair<long, float>(51000, 180.0));
	points_Yaw_heli.push_back(std::pair<long, float>(54000, 360.0));
	points_Yaw_heli.push_back(std::pair<long, float>(69500, 360.0));
	points_Yaw_heli.push_back(std::pair<long, float>(70000, 365.0));
	points_Yaw_heli.push_back(std::pair<long, float>(71000, 370.0));
	anim_heli->addControlPoints(points_Yaw_heli, BetterAnimation::Attribute::YAW);

	vector<std::pair<long, float>> points_Pitch_heli;
	points_Pitch_heli.push_back(std::pair<long, float>(48000, 0.0));
	points_Pitch_heli.push_back(std::pair<long, float>(48500, 5.0));
	points_Pitch_heli.push_back(std::pair<long, float>(49000, 7.5));
	points_Pitch_heli.push_back(std::pair<long, float>(49500, 7.5));
	points_Pitch_heli.push_back(std::pair<long, float>(50000, 5.0));
	points_Pitch_heli.push_back(std::pair<long, float>(50500, 0.0));
	points_Pitch_heli.push_back(std::pair<long, float>(51000, -5.0));
	points_Pitch_heli.push_back(std::pair<long, float>(51500, -7.5));
	points_Pitch_heli.push_back(std::pair<long, float>(52500, -7.5));
	points_Pitch_heli.push_back(std::pair<long, float>(53000, -5.0));
	points_Pitch_heli.push_back(std::pair<long, float>(53500, 0.0));
	points_Pitch_heli.push_back(std::pair<long, float>(54000, 2.5));
	points_Pitch_heli.push_back(std::pair<long, float>(54500, 7.5));
	points_Pitch_heli.push_back(std::pair<long, float>(66000, 7.5));
	points_Pitch_heli.push_back(std::pair<long, float>(66500, 5.0));
	points_Pitch_heli.push_back(std::pair<long, float>(67000, 0.0));
	points_Pitch_heli.push_back(std::pair<long, float>(69000, -7.5));
	points_Pitch_heli.push_back(std::pair<long, float>(69500, -5.0));
	points_Pitch_heli.push_back(std::pair<long, float>(70000, 0.0));
	points_Pitch_heli.push_back(std::pair<long, float>(70500, 7.5));
	anim_heli->addControlPoints(points_Pitch_heli, BetterAnimation::Attribute::PITCH);

	vector<std::pair<long, float>> points_Roll_heli;
	points_Roll_heli.push_back(std::pair<long, float>(39000, 0.0));
	points_Roll_heli.push_back(std::pair<long, float>(40000, 5.0));
	points_Roll_heli.push_back(std::pair<long, float>(41000, 15.0));
	points_Roll_heli.push_back(std::pair<long, float>(45000, 15.0));
	points_Roll_heli.push_back(std::pair<long, float>(46000, 10.0));
	points_Roll_heli.push_back(std::pair<long, float>(48000, 0.0));
	points_Roll_heli.push_back(std::pair<long, float>(69500, 0.0));
	points_Roll_heli.push_back(std::pair<long, float>(70500, 2.5));
	points_Roll_heli.push_back(std::pair<long, float>(71500, 10.0));
	anim_heli->addControlPoints(points_Roll_heli, BetterAnimation::Attribute::ROLL);


	auto helicopterEnt = sceneManager.createEntity("helicopterEnt", "helicopter", true);
	auto helicopter = sceneManager.getRoot()->createNewChildNode("helicopterNode", "bricks", glm::vec3(20.0f, 3.0f, 0.0f));
	helicopter->attach(helicopterEnt);
	helicopter->changeScale(glm::vec3(0.02, 0.02, 0.02));
	helicopter->yaw(-90);
	helicopter->pitch(20);

	GLfloat amb_heli[3] = { 0.1f, 0.0f, 0.0f };
	GLfloat dif_heli[3] = { 9.0f, 0.0f, 0.0f };
	GLfloat spec_heli[3] = { 1.0f, 0.0f, 0.0f };

	auto light3 = sceneManager.createLight("light3", amb_heli, dif_heli, spec_heli, 0.0f, 0.0f, 0.2f, nullptr);
	auto heliLightNode = helicopter->createNewChildNode("heliLightNode", "light_material", glm::vec3(0, -100, 130));
	heliLightNode->changeScale(glm::vec3(5, 5, 5));
	heliLightNode->attach(light3);
	helicopter->setAnimation(anim_heli);
	anim_heli->start();


	vector<std::pair<long, float>> points_yaw;
	points_yaw.push_back(std::pair<long, float>(4500, 90));
	points_yaw.push_back(std::pair<long, float>(9000, 80));
	points_yaw.push_back(std::pair<long, float>(13000, 200));
	points_yaw.push_back(std::pair<long, float>(17000, 320));
	points_yaw.push_back(std::pair<long, float>(18000, 270));
	points_yaw.push_back(std::pair<long, float>(25000, 360));
	points_yaw.push_back(std::pair<long, float>(29000, 540));
	points_yaw.push_back(std::pair<long, float>(34000, 540));
	points_yaw.push_back(std::pair<long, float>(35000, 360));
	points_yaw.push_back(std::pair<long, float>(40000, 420));
	points_yaw.push_back(std::pair<long, float>(46000, 520));
	points_yaw.push_back(std::pair<long, float>(49000, 520));
	points_yaw.push_back(std::pair<long, float>(53000, 380));
	points_yaw.push_back(std::pair<long, float>(60000, 360));
	points_yaw.push_back(std::pair<long, float>(72000, 360));
	points_yaw.push_back(std::pair<long, float>(77000, 410));
	BetterAnimation *anim_player = new BetterAnimation(false);
	anim_player->addControlPoints(points_yaw, BetterAnimation::Attribute::YAW);


	vector<std::pair<long, float>> points_pitch;
	points_pitch.push_back(std::pair<long, float>(5000, 30));
	points_pitch.push_back(std::pair<long, float>(9000, -30));
	points_pitch.push_back(std::pair<long, float>(11000, 0));
	points_pitch.push_back(std::pair<long, float>(18000, 45));
	points_pitch.push_back(std::pair<long, float>(25000, -30));
	points_pitch.push_back(std::pair<long, float>(33000, 0));
	points_pitch.push_back(std::pair<long, float>(34000, 0));
	points_pitch.push_back(std::pair<long, float>(38000, -60));
	points_pitch.push_back(std::pair<long, float>(42000, -60));
	points_pitch.push_back(std::pair<long, float>(46000, -30));
	points_pitch.push_back(std::pair<long, float>(49000, -30));
	points_pitch.push_back(std::pair<long, float>(50500, -50));
	points_pitch.push_back(std::pair<long, float>(51000, -60));
	points_pitch.push_back(std::pair<long, float>(52000, -60));
	points_pitch.push_back(std::pair<long, float>(60000, -45));
	points_pitch.push_back(std::pair<long, float>(65000, 5));
	points_pitch.push_back(std::pair<long, float>(74000, -45));
	points_pitch.push_back(std::pair<long, float>(80000, 5));
	anim_player->addControlPoints(points_pitch, BetterAnimation::Attribute::PITCH);

	vector<std::pair<long, float>> points_X;
	points_X.push_back(std::pair<long, float>(4000, 5.0));
	points_X.push_back(std::pair<long, float>(4500, 5.5));
	points_X.push_back(std::pair<long, float>(5000, 5.75));
	points_X.push_back(std::pair<long, float>(7500, 6.75));
	points_X.push_back(std::pair<long, float>(9000, 7.75));
	points_X.push_back(std::pair<long, float>(9500, 7.5));
	points_X.push_back(std::pair<long, float>(10000, 7.15));
	points_X.push_back(std::pair<long, float>(13000, 4.15));
	points_X.push_back(std::pair<long, float>(18000, 2.15));
	points_X.push_back(std::pair<long, float>(18500, 2.0));
	points_X.push_back(std::pair<long, float>(19000, 1.75));
	points_X.push_back(std::pair<long, float>(33000, -4.0));
	points_X.push_back(std::pair<long, float>(35000, -4.0));
	points_X.push_back(std::pair<long, float>(38000, 1.0));
	points_X.push_back(std::pair<long, float>(42000, 5.0));
	points_X.push_back(std::pair<long, float>(52000, 5.0));
	points_X.push_back(std::pair<long, float>(52500, 4.75));
	points_X.push_back(std::pair<long, float>(53000, 4.25));
	points_X.push_back(std::pair<long, float>(53500, 3.5));
	points_X.push_back(std::pair<long, float>(60000, -1.0));
	points_X.push_back(std::pair<long, float>(80000, 1.0));
	anim_player->addControlPoints(points_X, BetterAnimation::Attribute::POS_X);

	vector<std::pair<long, float>> points_Z;
	points_Z.push_back(std::pair<long, float>(500, 1.0));
	points_Z.push_back(std::pair<long, float>(1000, 1.75));
	points_Z.push_back(std::pair<long, float>(1500, 2.25));
	points_Z.push_back(std::pair<long, float>(2000, 2.5));
	points_Z.push_back(std::pair<long, float>(4000, 4.0));
	points_Z.push_back(std::pair<long, float>(4500, 3.85));
	points_Z.push_back(std::pair<long, float>(5000, 3.65));
	points_Z.push_back(std::pair<long, float>(5500, 3.5));
	points_Z.push_back(std::pair<long, float>(10000, -1.0));
	points_Z.push_back(std::pair<long, float>(13000, -4.0));
	points_Z.push_back(std::pair<long, float>(18000, -4.0));
	points_Z.push_back(std::pair<long, float>(33000, -1.0));
	points_Z.push_back(std::pair<long, float>(35000, -1.0));
	points_Z.push_back(std::pair<long, float>(37000, 0.0));
	points_Z.push_back(std::pair<long, float>(37500, 0.35));
	points_Z.push_back(std::pair<long, float>(38000, 0.90));
	points_Z.push_back(std::pair<long, float>(38500, 1.5));
	points_Z.push_back(std::pair<long, float>(40000, 3.5));
	points_Z.push_back(std::pair<long, float>(42000, 4.0));
	points_Z.push_back(std::pair<long, float>(52000, 4.0));
	points_Z.push_back(std::pair<long, float>(52500, 3.9));
	points_Z.push_back(std::pair<long, float>(53000, 3.75));
	points_Z.push_back(std::pair<long, float>(53500, 3.5));
	points_Z.push_back(std::pair<long, float>(60000, -1.5));
	points_Z.push_back(std::pair<long, float>(70000, -1.5));
	points_Z.push_back(std::pair<long, float>(80000, 4.0));
	anim_player->addControlPoints(points_Z, BetterAnimation::Attribute::POS_Z);

	vector<std::pair<long, float>> points_Y;
	points_Y.push_back(std::pair<long, float>(3500, 0.0));
	points_Y.push_back(std::pair<long, float>(5000, -0.5));
	points_Y.push_back(std::pair<long, float>(8500, -0.5));
	points_Y.push_back(std::pair<long, float>(9000, -0.35));
	points_Y.push_back(std::pair<long, float>(9500, -0.1));
	points_Y.push_back(std::pair<long, float>(10500, 1.0));
	points_Y.push_back(std::pair<long, float>(18000, -1.0));
	points_Y.push_back(std::pair<long, float>(20000, -1.75));
	points_Y.push_back(std::pair<long, float>(33000, -2.25));
	points_Y.push_back(std::pair<long, float>(35000, -2.25));
	points_Y.push_back(std::pair<long, float>(40000, 0.0));
	points_Y.push_back(std::pair<long, float>(42000, 1.0));
	points_Y.push_back(std::pair<long, float>(45000, 1.0));
	points_Y.push_back(std::pair<long, float>(45500, 0.9));
	points_Y.push_back(std::pair<long, float>(46000, 0.75));
	points_Y.push_back(std::pair<long, float>(48000, 0.5));
	points_Y.push_back(std::pair<long, float>(52000, -0.5));
	points_Y.push_back(std::pair<long, float>(56000, -2.3));
	points_Y.push_back(std::pair<long, float>(60000, -2.0));
	anim_player->addControlPoints(points_Y, BetterAnimation::Attribute::POS_Y);

	auto cameraNode = sceneManager.getRoot()->createNewChildNode("cameraNode", "box_material", glm::vec3(1.5f, 2.5f, 0.0f));
	cameraNode->attach(camera);
	cameraNode->yaw(90);
	cameraNode->setAnimation(anim_player);
	anim_player->start();


	auto cubeEntity = sceneManager.createEntity("entidade", "cube");
	auto cubeNode = sceneManager.getRoot()->createNewChildNode("cubeNode", "box_material", glm::vec3(3.6f, 0.35f, -4.0f));
	cubeNode->changeScale(glm::vec3(0.7));
	cubeNode->yaw(-10);
	cubeNode->attach(cubeEntity);

	auto cubeEntity2 = sceneManager.createEntity("entidade2", "cube");
	auto cubeNode2 = sceneManager.getRoot()->createNewChildNode("cubeNode2", "box_material", glm::vec3(4.7f, 0.35f, -3.8f));
	cubeNode2->yaw(30);
	cubeNode2->changeScale(glm::vec3(0.7));
	cubeNode2->attach(cubeEntity2);


	GLfloat amb[3] = { 0.0f, 0.0f, 0.01f };
	GLfloat dif[3] = { 0.0f, 0.0f, 1.0f };
	GLfloat spec[3] = { 0.0f, 0.0f, 0.3f };

	GLfloat amb2[3] = { 0.1f, 0.1f, 0.1f };
	GLfloat dif2[3] = { 1.0f, 1.0f, 1.0f };
	GLfloat spec2[3] = { 1.0f, 1.0f, 1.0f };


	auto light2 = sceneManager.createLight("light2", amb2, dif2, spec2, 0.0f, 0.0f, 0.5f, nullptr);
	auto lightCube = sceneManager.createEntity("lightCube", "cube", false);
	auto lightNode2 = sceneManager.getRoot()->createNewChildNode("lightNode", "light_material2", glm::vec3(2.65f, 5.5f, 1.8f));
	auto lightNodeCube = sceneManager.getRoot()->createNewChildNode("lightNodeCube", "light_material2", glm::vec3(2.7f, 5.6f, 1.8f));
	lightNodeCube->changeScale(glm::vec3(0.4, 0.15, 0.3));
	lightNode2->attach(light2);
	lightNodeCube->attach(lightCube);
	lightNodeCube->roll(-20);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	double lastTime = glfwGetTime();
	lastFrame = glfwGetTime();
	int nbFrames = 0;

	double counter = 0;

	// Game loop
	ISound* unreal = SoundEngine->play2D("unreal.mp3", GL_TRUE, false, true);
	ISound* chopper = SoundEngine->play2D("helicopter.mp3", GL_FALSE, true, true);

	while (!window_.close())
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		nbFrames++;

		counter += deltaTime;

		if (counter*1000.0 >= 81000.0)
			return 0;
		if(counter*1000.0 >= 75000.0)
		{
			float dif = 81000.0 - (counter*1000.0);
			float vol = unreal->getVolume();

			float speed = vol / dif;
			unreal->setVolume(vol - (deltaTime*1000*speed));
		}
		if(counter*1000.0 >= 32000.0 && counter*1000.0 <= 35000.0)
		{
			if(chopper->getIsPaused())
			{
				chopper->setIsPaused(false);
				chopper->setVolume(0.0f);
			}
				
			float dif = 35000.0 - (counter*1000.0);
			float vol = chopper->getVolume();

			float speed = (0.5f - vol) / dif;

			chopper->setVolume(vol + (deltaTime * 1000 * speed));
		}
		if (counter*1000.0 >= 53000.0 && counter*1000.0 < 67000.0)
		{
			float dif = 67000.0 - (counter*1000.0);
			float vol = chopper->getVolume();

			float speed = (1.0-vol) / dif;
			chopper->setVolume(vol + (deltaTime * 1000 * speed));
		}
		if(counter*1000.0 >= 67000.0)
		{
			float dif = 81000.0 - (counter*1000.0);
			float vol = chopper->getVolume();

			float speed = vol / dif;
			chopper->setVolume(vol - (deltaTime * 1000 * speed));
		}

		/*if (currentFrame - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago

											 // printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;

		}*/

		// Check and call events
		glfwPollEvents();
		Do_Movement(vp);

		window_.update(sceneManager, deltaTime*1000);

		window_.render(sceneManager);
	}

	return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement(Viewport *vp)
{
	// Camera controls
	/*if (keys[GLFW_KEY_W])
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->ProcessKeyboard(RIGHT, deltaTime);*/
	if (keys[GLFW_KEY_Q])
	{
		pressingToggleBloom = true;
	}
	else if(pressingToggleBloom)
	{
		vp->toggleBloom();
		pressingToggleBloom = false;
	}

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

	//camera->ProcessMouseMovement(xoffset, yoffset);
//	camera2.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera->ProcessMouseScroll(yoffset);
//	camera2.ProcessMouseScroll(yoffset);
}