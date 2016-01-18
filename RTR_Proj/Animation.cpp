#include "Animation.h"
#include "SceneNode.h"


Animation::Animation()
{
}


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
}



void LinearAnimation::applyRotations(SceneNode *node) {
	node->yaw(curOrientation-prevOrientation);
}



glm::vec3 LinearAnimation::getPos() const
{
	return curPos;
}



BetterAnimation::BetterAnimation(bool looping) : Animation()
{
	this->looping = looping;
	this->started = false;
	this->finished = false;
	glm::vec3 prevPos(0);
	this->prevPitch = 0;
	this->prevYaw = 0;
	this->prevRoll = 0;
	this->totalTime = 0;
}



void BetterAnimation::update(unsigned long milis, bool animationsPaused)
{
	if(this->started && (!this->finished || this->looping))
	{
		auto timeToMove = milis;
		float newPos = currX;
		auto timeToNext = 0;
		while(this->pos_x_Ind < this->pos_xs.size() && timeToMove > 0)
		{
			timeToNext = pos_xs[pos_x_Ind].first - (totalTime+timeToNext);
			if(timeToMove == timeToNext)
			{
				newPos = pos_xs[pos_x_Ind].second;
				pos_x_Ind++;
				break;
			}
			if(timeToMove < timeToNext)
			{
				auto speed = (pos_xs[pos_x_Ind].second - this->currX) / (float)timeToNext;
				auto d_x = speed * milis;

				newPos = currX + d_x;
				break;
			}
			else
			{
				currX = pos_xs[pos_x_Ind].second;
				timeToMove -= timeToNext;
				pos_x_Ind++;
			}
		}

		this->prevPos.x = currX;
		this->currX = newPos;

		timeToMove = milis;
		newPos = currY;
		timeToNext = 0;
		while (this->pos_y_Ind < this->pos_ys.size() && timeToMove > 0)
		{
			timeToNext = pos_ys[pos_y_Ind].first - (totalTime + timeToNext);
			if (timeToMove == timeToNext)
			{
				newPos = pos_ys[pos_y_Ind].second;
				pos_y_Ind++;
				break;
			}
			if (timeToMove < timeToNext)
			{
				auto speed = (pos_ys[pos_y_Ind].second - this->currY) / (float)timeToNext;
				auto d_y = speed * milis;

				newPos = currY + d_y;
				break;
			}
			else
			{
				currY = pos_ys[pos_y_Ind].second;
				timeToMove -= timeToNext;
				pos_y_Ind++;
			}
		}

		this->prevPos.y = currY;
		this->currY = newPos;
		
		timeToMove = milis;
		newPos = currZ;
		timeToNext = 0;
		while (this->pos_z_Ind < this->pos_zs.size() && timeToMove > 0)
		{
			timeToNext = pos_zs[pos_z_Ind].first - (totalTime + timeToNext);
			if (timeToMove == timeToNext)
			{
				newPos = pos_zs[pos_z_Ind].second;
				pos_z_Ind++;
				break;
			}
			if (timeToMove < timeToNext)
			{
				auto speed = (pos_zs[pos_z_Ind].second - this->currZ) / (float)timeToNext;
				auto d_z = speed * milis;

				newPos = currZ + d_z;
				break;
			}
			else
			{
				currZ = pos_zs[pos_z_Ind].second;
				timeToMove -= timeToNext;
				pos_z_Ind++;
			}
		}

		this->prevPos.z = currZ;
		this->currZ = newPos;

		timeToMove = milis;
		newPos = currYaw;
		timeToNext = 0;
		while (this->yaws_Ind < this->yaws.size() && timeToMove > 0)
		{
			timeToNext = yaws[yaws_Ind].first - (totalTime + timeToNext);
			if (timeToMove == timeToNext)
			{
				newPos = yaws[yaws_Ind].second;
				yaws_Ind++;
				break;
			}
			if (timeToMove < timeToNext)
			{
				auto speed = (yaws[yaws_Ind].second - this->currYaw) / (float)timeToNext;
				auto d_yaw = speed * milis;

				newPos = currYaw + d_yaw;
				break;
			}
			else
			{
				currYaw = yaws[yaws_Ind].second;
				timeToMove -= timeToNext;
				yaws_Ind++;
			}
		}

		this->prevYaw = currYaw;
		this->currYaw = newPos;

		timeToMove = milis;
		newPos = currPitch;
		timeToNext = 0;
		while (this->pitches_Ind < this->pitches.size() && timeToMove > 0)
		{
			timeToNext = pitches[pitches_Ind].first - (totalTime + timeToNext);
			if (timeToMove == timeToNext)
			{
				newPos = pitches[pitches_Ind].second;
				pitches_Ind++;
				break;
			}
			if (timeToMove < timeToNext)
			{
				auto speed = (pitches[pitches_Ind].second - this->currPitch) / (float)timeToNext;
				auto d_pitch = speed * milis;

				newPos = currPitch + d_pitch;
				break;
			}
			else
			{
				currPitch = pitches[pitches_Ind].second;
				timeToMove -= timeToNext;
				pitches_Ind++;
			}
		}

		this->prevPitch = currPitch;
		this->currPitch = newPos;

		timeToMove = milis;
		newPos = currRoll;
		timeToNext = 0;
		while (this->rolls_Ind < this->rolls.size() && timeToMove > 0)
		{
			timeToNext = rolls[rolls_Ind].first - (totalTime + timeToNext);
			if (timeToMove == timeToNext)
			{
				newPos = rolls[rolls_Ind].second;
				rolls_Ind++;
				break;
			}
			if (timeToMove < timeToNext)
			{
				auto speed = (rolls[rolls_Ind].second - this->currRoll) / (float)timeToNext;
				auto d_roll = speed * milis;

				newPos = currRoll + d_roll;
				break;
			}
			else
			{
				currRoll = rolls[rolls_Ind].second;
				timeToMove -= timeToNext;
				rolls_Ind++;
			}
		}

		this->prevRoll = currRoll;
		this->currRoll = newPos;

		totalTime += milis;
	}

	if(!finished && this->pos_x_Ind >= this->pos_xs.size() &&
		this->pos_y_Ind >= this->pos_ys.size() &&
		this->pos_z_Ind >= this->pos_zs.size() &&
		this->yaws_Ind >= this->yaws.size() &&
		this->pitches_Ind >= this->pitches.size() &&
		this->rolls_Ind >= this->rolls.size())
	{
		this->finished = true;
	}
}

void BetterAnimation::applyTranslations(SceneNode* node)
{
	if (this->finished)
		return;
	node->translate(glm::vec3(currX,currY,currZ) - prevPos);
	prevPos = glm::vec3(currX, currY, currZ);
}



void BetterAnimation::applyRotations(SceneNode* node)
{
	if (this->finished)
		return;
	node->yaw(this->currYaw - prevYaw);
	node->pitch(this->currPitch - prevPitch);
	node->roll(this->currRoll - prevRoll);
}



void BetterAnimation::applyActive(SceneNode* node)
{
}



void BetterAnimation::reset()
{
	this->totalTime = 0;
	this->pos_x_Ind = 0;
	this->pos_y_Ind = 0;
	this->pos_z_Ind = 0;

	this->pitches_Ind = 0;
	this->yaws_Ind = 0;
	this->rolls_Ind = 0;
	this->active_Ind = 0;

	glm::vec3 prevPos = glm::vec3(0);
	this->prevPitch = 0;
	this->prevYaw = 0;
	this->prevRoll = 0;

	this->totalTime = 0;
	this->finished = false;
	this->started = true;
}


glm::vec3 BetterAnimation::getPos() const
{
	return glm::vec3(this->currX, this->currY, this->currZ);
}



void BetterAnimation::addControlPoints(vector<std::pair<long, float>> ctrlPoints, Attribute type)
{
	if (type == Attribute::POS_X)
	{
		this->pos_xs = ctrlPoints;
		this->pos_x_Ind = 0;
	}
	else if (type == Attribute::POS_Y)
	{
		this->pos_ys = ctrlPoints;
		this->pos_y_Ind = 0;
	}
	else if (type == Attribute::POS_Z)
	{
		this->pos_zs = ctrlPoints;
		this->pos_z_Ind = 0;
	}
	else if (type == Attribute::PITCH)
	{
		this->pitches = ctrlPoints;
		this->pitches_Ind = 0;
	}
	else if (type == Attribute::YAW)
	{
		this->yaws = ctrlPoints;
		this->yaws_Ind = 0;
	}
	else if (type == Attribute::ROLL)
	{
		this->rolls = ctrlPoints;
		this->rolls_Ind = 0;
	}
}



void BetterAnimation::start()
{
	glm::vec3 prevPos = glm::vec3(0);
	this->prevPitch = 0;
	this->prevYaw = 0;
	this->prevRoll = 0;

	this->totalTime = 0;
	this->finished = false;
	this->started = true;
}