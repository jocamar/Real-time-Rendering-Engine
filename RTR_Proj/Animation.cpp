#include "Animation.h"
#include "SceneNode.h"
#include <string>


Animation::Animation(vector<glm::vec3> ctrlPoints) : ctrlPoints(ctrlPoints)
{
}

LinearAnimation::LinearAnimation(vector<glm::vec3> ctrlPoints, float span, bool looping) : Animation(ctrlPoints)
{
	this->looping = looping;
	finished = false;
	float totalDist = 0;
	curPoint = 0;
	curPos = glm::vec3(0, 0, 0);
	prevPos = glm::vec3(0, 0, 0);
	prevOrientation = 0;

	if (ctrlPoints.size() > 0)
	{
		totalDist += glm::distance(glm::vec3(0, 0, 0), ctrlPoints[0]);

		for (unsigned int i = 0; i < ctrlPoints.size() - 1; i++)
		{
			totalDist += glm::distance(ctrlPoints[i], ctrlPoints[i + 1]);
		}

		speed = totalDist / span;

		curDirection = glm::normalize(ctrlPoints[0]);
		if (!(ctrlPoints[curPoint].x == curPos.x && ctrlPoints[curPoint].z == curPos.z))
			curOrientation = glm::degrees(glm::acos(glm::dot(glm::vec3(0, 0, 1),glm::normalize(glm::vec3(curDirection.x, 0, curDirection.z))))); //angle between vectors
		else
			curOrientation = 0;
	}
}

void LinearAnimation::update(unsigned long milis, bool animationsPaused)
{
	prevPos = curPos;
	prevOrientation = curOrientation;
	if (animationsPaused) {
		return;
	}
	if (curPoint < ctrlPoints.size())
	{
		double elapsedTime = milis;

		float toMoveX = curDirection.x*(speed*elapsedTime / 1000.0);
		float toMoveY = curDirection.y*(speed*elapsedTime / 1000.0);
		float toMoveZ = curDirection.z*(speed*elapsedTime / 1000.0);

		glm::vec3 toNextPosition = glm::vec3(toMoveX, toMoveY, toMoveZ);

		float distToMove = glm::distance(glm::vec3(0, 0, 0), toNextPosition);
		float distToNextCtrlPoint = glm::distance(curPos, ctrlPoints[curPoint]);

		if (distToMove > distToNextCtrlPoint)
		{
			while (distToMove > distToNextCtrlPoint) {
				if (curPoint < ctrlPoints.size() - 1)
				{

					distToMove -= distToNextCtrlPoint;
					curPos = ctrlPoints[curPoint];
					curPoint++;

					curDirection = glm::normalize(glm::vec3(ctrlPoints[curPoint].x - curPos.x, ctrlPoints[curPoint].y - curPos.y, ctrlPoints[curPoint].z - curPos.z));
					if (!(ctrlPoints[curPoint].x == curPos.x && ctrlPoints[curPoint].z == curPos.z))
						curOrientation = glm::degrees(glm::acos(glm::dot(glm::vec3(0, 0, 1), glm::normalize(glm::vec3(curDirection.x, 0, curDirection.z))))); //angle between vectors

					if (curDirection.x < 0)
					{
						curOrientation = 360 - curOrientation;
					}

					distToNextCtrlPoint = glm::distance(curPos, ctrlPoints[curPoint]);
				}
				else
				{
					if (looping && ctrlPoints.size() > 0)
					{
						reset();
						finished = false;
					}
					else
					{
						curPos = ctrlPoints[curPoint];
						finished = true;
					}
					return;
				}
			}
			curPos.x = curPos.x + curDirection.x*distToMove;
			curPos.y = curPos.y + curDirection.y*distToMove;
			curPos.z = curPos.z + curDirection.z*distToMove;
		}
		else
		{
			curPos.x = curPos.x + curDirection.x*distToMove;
			curPos.y = curPos.y + curDirection.y*distToMove;
			curPos.z = curPos.z + curDirection.z*distToMove;
		}
	}
}

void LinearAnimation::reset()
{
	if (ctrlPoints.size() > 0)
	{
		curPoint = 0;
		curPos = glm::vec3(0, 0, 0);
		curDirection = glm::normalize(ctrlPoints[0]);
		if (!(ctrlPoints[curPoint].x == curPos.x && ctrlPoints[curPoint].z == curPos.z))
			curOrientation = glm::degrees(glm::acos(glm::dot(glm::vec3(0, 0, 1), glm::normalize(glm::vec3(curDirection.x, 0, curDirection.z))))); //angle between vectors
		else
			curOrientation = 0;
	}
}

void LinearAnimation::applyTranslations(SceneNode *node) {
	node->translate(curPos-prevPos);
	//std::cout << std::to_string(curPos.x) + " - " + std::to_string(curPos.y) + " - " + std::to_string(curPos.z) + " / " + std::to_string( node->getPosition().x ) + " - " + std::to_string( node->getPosition().y ) + " - " + std::to_string( node->getPosition().z ) << endl;
}

void LinearAnimation::applyRotations(SceneNode *node) {
	node->yaw(curOrientation-prevOrientation);
}

glm::vec3 LinearAnimation::getPos() const
{
	return curPos;
}