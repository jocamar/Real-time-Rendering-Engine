#pragma once

// Std. Includes
#include <vector>

#include "Globals.h"
#include "Scene Graph/AttacheableObject.h"
#include "Scene Graph/SceneManager.h"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera : public AttacheableObject
{
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
public:
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	glm::mat4 ViewProjMatrix;
	std::vector<glm::mat4> cubeViewProjectionMatrixes;
	float Near, Far, Ratio;

	bool Ortho;
	float yFactor, xFactor;

	// Constructor with vectors
	Camera(SceneManager *manager, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH, 
		bool ortho = false, float near = 0.1f, float far = 1000.0f, float ratio = 1.0f, float zoom = 90.0f) : AttacheableObject("camera", manager), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(zoom)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->Ortho = ortho;
		this->Near = near;
		this->Far = far;
		this->Ratio = ratio;
		this->updateCameraVectors();

		yFactor = 1.0f / glm::cos(glm::radians(Zoom / 2.0));
		xFactor = 1.0f / glm::cos(glm::atan(glm::tan(glm::radians(Zoom / 2.0))*Ratio));
	}


	Camera(SceneManager *manager, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0,0.0,-1.0),
		bool ortho = false, float near = 0.1f, float far = 1000.0f, float ratio = 1.0f, float zoom = 90.0f) : AttacheableObject("camera", manager), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(zoom)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Front = front;
		auto angle = 0.0;
		if(Front.x != 0 || Front.z != 0)
			angle = glm::acos(glm::dot(glm::vec3(1,0,0),glm::normalize(glm::vec3(Front.x,0,Front.z))));
		if (Front.z < 0)
			angle = -angle;

		this->Pitch = glm::degrees(glm::asin(Front.y / glm::length(Front)));
		this->Yaw = glm::degrees(angle);
		this->Ortho = ortho;
		this->Near = near;
		this->Far = far;
		this->Ratio = ratio;
		this->updateCameraVectors();

		yFactor = 1.0f / glm::cos(glm::radians(Zoom/2.0));
		xFactor = 1.0f / glm::cos(glm::atan(glm::tan(glm::radians(Zoom / 2.0))*Ratio));
	}

	// Constructor with scalar values
	Camera(SceneManager *manager, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch, bool ortho = false, float zoom = 90.0f) : AttacheableObject("camera", manager), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(zoom)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->Ortho = ortho;
		this->Near = 0.1;
		this->Far = 1000.0;
		this->Ratio = 1.0;
		this->updateCameraVectors();

		yFactor = 1.0f / glm::cos(glm::radians(Zoom / 2.0));
		xFactor = 1.0f / glm::cos(glm::atan(glm::tan(glm::radians(Zoom / 2.0))*Ratio));
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->getPosition(), this->getPosition() + this->getFront(), this->getUp());
	}


	glm::mat4 GetProjectionMatrix()
	{
		if(Ortho)
		{
			return glm::ortho(-30.f, 30.f, -30.f, 30.f, Near, Far);
		}
		else
		{
			return  glm::perspective(glm::radians(Zoom), Ratio, Near, Far);
		}
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

	//Inherited from attacheableobject, does nothing since the camera isn't rendered.
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override
	{
		
	};


	bool isLeaf() override
	{
		return true;
	};

	//@TODO maybe move the code to move the camera here.
	void update(float seconds) override
	{
		
	};

	glm::vec3 getPosition()
	{
		if(this->parent)
			return glm::vec3(this->parent->getTransfMatrix() * glm::vec4(this->Position, 1.0));
		else return this->Position;
	}

	glm::vec3 getFront()
	{
		if (this->parent)
		{
			auto front = glm::vec3(this->parent->getTransfMatrix() * glm::vec4(this->Front, 1.0));
			return front - this->getPosition();
		}
		else return this->Front;
	}

	glm::vec3 getUp()
	{
		if (this->parent)
		{
			auto up = glm::vec3(this->parent->getTransfMatrix() * glm::vec4(this->Up, 1.0));
			return up - this->getPosition();
		}
		else return this->Up;
	}

	glm::vec3 getRight()
	{
		if (this->parent)
		{
			auto right = glm::vec3(this->parent->getTransfMatrix() * glm::vec4(this->Right, 1.0));
			return right - this->getPosition();
		}
		else return this->Right;
	}


	RenderOrder getRenderEntities(int material, Camera* camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};

inline RenderOrder Camera::getRenderEntities(int material, Camera* camera, bool shadowMap, Globals::LIGHT_TYPE shadowType)
{
	RenderOrder order;
	return order;
}