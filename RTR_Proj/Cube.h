#pragma once

#include <iostream>
#include "Mesh.h"

using namespace std;
class Cube : public Mesh
{
private:

public:
	void display() override;
	Cube(string id);
	~Cube();
};

