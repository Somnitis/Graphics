#pragma once
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\OBJMesh.h"

class  CubeRobot : public  SceneNode {
public:
	CubeRobot(void);
	~CubeRobot(void) {};
	virtual  void    Update(float  msec);
	virtual  void    Update(float  msec, bool moving);

	static  void  CreateCube() {
		OBJMesh* m = new  OBJMesh();
		m->LoadOBJMesh(MESHDIR"cube.obj");
		cube = m;

	}
	static  void  DeleteCube() { delete  cube; }

protected:
	static  Mesh*   cube;
	SceneNode*		hips;
	SceneNode*      head;
	SceneNode*      leftArm;
	SceneNode*      rightArm;
	SceneNode*      leftLeg;
	SceneNode*      rightLeg;
};