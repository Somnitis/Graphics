#pragma once
#include <algorithm >
#include "..\..\nclgl\SceneNode.h"
#include "..\..\nclgl\OBJMesh.h"

class  Tree : public  SceneNode {
public:
	Tree();
	~Tree(void) {};
	virtual  void    Update(float  msec);
	void SetRotetion(float r,Vector3 x);

	static  void  CreateTree() {
		OBJMesh* m = new  OBJMesh();
		m->LoadOBJMesh(MESHDIR"tree.obj");
		branch = m;
	}
	static  void  DeleteTree() { delete  branch; }
	Vector3 Posistion;
	float rotation;
protected:
	static OBJMesh*   branch;
	
};
