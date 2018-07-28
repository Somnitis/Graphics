#include "Tree.h"

OBJMesh* Tree::branch = NULL;

Tree::Tree() {
	//Optional , uncomment  if you  want a local  origin  marker!
	// SetMesh(cube);

	SceneNode*body = new  SceneNode(branch, Vector4(0, 1, 0, 1));    //Green!
	body->SetModelScale(Vector3(10, 10, 10));
	body->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	body->SetTransform(Matrix4::Rotation(rotation, Posistion));

	body->SetBoundingRadius(10.0f);
}

void Tree::SetRotetion(float r,Vector3 x) {
	transform = transform *
		Matrix4::Rotation(r, x);
}

void  Tree::Update(float  msec) {
//	transform = transform *
//		Matrix4::Rotation(angle, Vector3(1, 1, 1));

	SceneNode::Update(msec);
}
