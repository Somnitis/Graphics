#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/camera.h"
#include "../../nclgl/HeightMap.h"
#include "Tree.h"
#include "TextMesh.h"
#include "CubeRobot.h"
#include "ParticleEmitter.h"
#include "Snowing.h"

#define  SHADOWSIZE  2048   

enum Scenes{	Scene1,
				Scene2,
				Scene3,
MaxScenes };
enum Textures { Water= 10, Sand, Grass, Mount, Snow, Floawers, Tree, bumpTexs1Hei, 
	ShadowTex2, diffuseTex2, bumpTex2, cameraPos2 , 
	cameraPoss3, ParticlText, cubetext3, 
	Nothing, Empty
};

class  Renderer : public  OGLRenderer {
public:
	Renderer(Window &parent);

	virtual ~Renderer(void);
	virtual  void  UpdateScene(float msec, int fps);
	virtual void  RenderScene();
	
	void Input();
	void set_timer(float msec);
	float msec;
	void CameraControl();

	void RenderTextures();
	void RendererTexture();

	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

	void RendererScene1();
	void RendererScene2();
	void RendererScene3();

	void RenderScene1();
	void RenderScene2();
	void RenderScene3();

protected:
	//General Variables
	void set_speed();


	Camera*      camera;
	Light*       light;



	bool pause;
	bool autoplay;

	float T_elapsed;
	float timer;
	int speed;

	int Scene;

	// FPS Counter
	int FPS;

	// Text Variables
	Font*	basicFont;
	Mesh*		Text;
	Shader*   TextShader;


	// Scene 1 Variables
	float	Day_cycle();
	void	HeightMapInitS1();
	void	DrawHeightmapS1();
	void	DrawSkyboxS1();
	void DrawTreeS1(SceneNode*n);
	

	HeightMap*   heightMapS1;


	Shader*   TerrainShaderS1;
	Shader*   TreeShaderS1;
	Shader*	  SnowShaderS1;

	SceneNode*		rootS1;


	GLuint cubeMapS1;
	GLuint WaterTex;
	GLuint SandTex;
	GLuint GrassTex;
	GLuint MountTex;
	GLuint SnowTex;
	GLuint TreeText;
	GLfloat T;

	float day;
	float SunSpeed;
	float sun_position;
	float t;
	float x;

	int k;

	bool night;
	bool movesun;


	// Scene 2 Variables
	void DrawShadowSceneS2();
	void DrawCombinedSceneS2();
	void FloorInit();
	void InputPlayer();
	void DrawFloor();
	void DrawPlayer(SceneNode*n);
	void DrawTreeS2(SceneNode*n);

	SceneNode*	player;
	SceneNode*		rootS2;
	CubeRobot	robot;
	Mesh*		Floor;
	Vector3 PlayerPosition;
	Light*       lightS2;

	Shader*		PlayerShaderS2;
	Shader*		FloorShaderS2;
	Shader*		shadowShaderS2;
	Snowing*  snow;

	GLuint    shadowTex;
	GLuint    shadowFBO;
	GLuint cubeMapS2;
	GLuint blendMap;
	GLuint GrasTex;
	GLuint FlowerTex;
	GLuint DirtTex;
	GLuint RoadTex;

	float runspeed = 5;
	float turn = 160;
	float camoffset = 4000;
	int P1movespeed;
	int P1turnspeed;
	float PlayerScale = 0.5f;
	int lightdirection;
	int playermove;
	bool move;
	bool snowing;


	// Scene 3 Variables
	void	DrawSkyboxS3();
	void	DrawHeightmapS3();
	void	DrawNode(SceneNode*n);
	void	DrawParticlesS3();
	void	SetShaderParticleSizeS3(float f);
	void	DrawWater();
	void	DrawTreeS3(SceneNode*n);

	HeightMap*  heightMapS3;

	Shader*      lightShaderS3;
	Shader*      reflectShaderS3;
	Shader*      skyboxShaderS3;
	Shader*		 TreeShaderS3;
	Shader*		 ParticleShaderS3;
	ParticleEmitter*	Particles;
	SceneNode*		rootS3;
	SceneNode*		playerS3;
	Light*			lightS3;

	GLuint cubeMapS3;
	float particleSize;
	float         waterRotate;


	////////////////////////////////////
	Mesh*         quad;




};

//float mixerlimit(float x);