#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
	playermove = 5;
	speed = 1;
	T_elapsed = 0.00f;
	Scene = 1;

	FPS = 0;

	camera = new Camera;

	projMatrix = Matrix4::Perspective(0.5f, 15000.0f,
		(float)width / (float)height, 45.0f);

	//Textures
	WaterTex = SOIL_load_OGL_texture(TEXTUREDIR"water2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	SandTex = SOIL_load_OGL_texture(TEXTUREDIR"sand.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	GrassTex = SOIL_load_OGL_texture(TEXTUREDIR"grass2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	MountTex = SOIL_load_OGL_texture(TEXTUREDIR"mount.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	SnowTex = SOIL_load_OGL_texture(TEXTUREDIR"snow.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	TreeText = SOIL_load_OGL_texture(TEXTUREDIR"tree2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	FlowerTex = SOIL_load_OGL_texture(TEXTUREDIR"flowers.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	glActiveTexture(GL_TEXTURE0 + Water);
	glBindTexture(GL_TEXTURE_2D, WaterTex);
	glActiveTexture(GL_TEXTURE0 + Sand);
	glBindTexture(GL_TEXTURE_2D, SandTex);
	glActiveTexture(GL_TEXTURE0 + Grass);
	glBindTexture(GL_TEXTURE_2D, GrassTex);
	glActiveTexture(GL_TEXTURE0 + Mount);
	glBindTexture(GL_TEXTURE_2D, MountTex);
	glActiveTexture(GL_TEXTURE0 + Snow);
	glBindTexture(GL_TEXTURE_2D, SnowTex);
	glActiveTexture(GL_TEXTURE0 + Floawers);
	glBindTexture(GL_TEXTURE_2D, Floawers);
	glActiveTexture(GL_TEXTURE0 + Empty);


	RendererScene1();
	RendererScene2();
	RendererScene3();
	RendererTexture();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	autoplay = true;
}

void  Renderer::UpdateScene(float msec,int fps) {
	FPS = fps;
	set_timer(msec);
	set_speed();
	CameraControl();
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();

	if (Scene == 1) {
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		rootS1->Update(msec);
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
	else if (Scene == 2) {
		InputPlayer();
		
		player->Update(msec, &move);
		Vector3 PP = player->GetWorldTransform().GetPositionVector();
		lightS2->SetPosition(PP + Vector3(100, 500, 100));
		rootS2->Update(msec);
	}
	else if (Scene == 3) {
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		waterRotate += msec / 750.0f;

		rootS3->Update(msec);
		Particles->Update(msec, T_elapsed);
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
	else cout << "Wrong Scene Number...";
}

void Renderer::CameraControl() {
	if (autoplay && Scene == 1) {
		camera->SetPosition(Vector3(1800 + 130 * sin(T_elapsed / 10), 1000, 1800 + 130 * cos(T_elapsed / 10)));
		camera->SetYaw(190 + T_elapsed * 5);
		camera->SetPitch(-35 - 10 * sin(T_elapsed / 5));

		if (T_elapsed > 20 && T_elapsed < 60)
		{
			speed = 2;
			camera->SetPosition(Vector3(1800 + 130 * sin(T_elapsed / 10), 1000, 1800 + 130 * cos(T_elapsed / 10)));
			camera->SetYaw(190 + T_elapsed * 5);
			camera->SetPitch(-35 - 10 * sin(T_elapsed / 5));
		}
		if (T_elapsed > 60 && T_elapsed < 100)
		{
			speed = 3;
			camera->SetPosition(Vector3(1800 + 130 * sin(T_elapsed / 10), 1000, 1800 + 130 * cos(T_elapsed / 10)));
			camera->SetYaw(190 + T_elapsed * 5);
			camera->SetPitch(-35 - 10 * sin(T_elapsed / 5));
		}

		if (T_elapsed > 100)
		{
			speed = 1;
			camera->SetPitch(-80);
			camera->SetYaw(190);
			camera->SetPosition(Vector3(1800, 800 + 300 * (T_elapsed - 100), 1800));
		}

		if (T_elapsed > 105)
		{
			Scene = 2;
			T_elapsed = 0.0;
		}
	}

	if (autoplay && Scene == 2) {
		Vector3 PP = player->GetWorldTransform().GetPositionVector();
		if (T_elapsed < 10) {
			camera->SetPosition(Vector3(0, 2000, 0));
			camera->SetPitch(-30);
			camera->SetYaw(190 + T_elapsed * 30);
		}
		if (T_elapsed > 10)
		{
			playermove = 6;
			camera->SetPitch(-25);
			camera->SetYaw(0);
			Vector3 PP = player->GetWorldTransform().GetPositionVector();
			Vector3 CP = (PP + Vector3(100, 400, 550));
			camera->SetPosition(CP);
		}
		if (T_elapsed > 15 && T_elapsed < 20 && playermove != 5)
		{
			playermove = 8;
			camera->SetPitch(-25);
			camera->SetYaw(0);
			Vector3 PP = player->GetWorldTransform().GetPositionVector();
			Vector3 CP = (PP + Vector3(100, 400, 550));
			camera->SetPosition(CP);
		}
		if (T_elapsed > 20 && T_elapsed < 25 && playermove != 5)
		{
			playermove = 6;
			camera->SetPitch(-25);
			camera->SetYaw(0);
			Vector3 PP = player->GetWorldTransform().GetPositionVector();
			Vector3 CP = (PP + Vector3(100, 400, 550));
			camera->SetPosition(CP);
		}
		if (T_elapsed > 25 && T_elapsed < 30 && playermove != 5)
		{
			playermove = 8;
			camera->SetPitch(-25);
			camera->SetYaw(0);
			Vector3 PP = player->GetWorldTransform().GetPositionVector();
			Vector3 CP = (PP + Vector3(100, 400, 550));
			camera->SetPosition(CP);
		}
		if (T_elapsed > 30 && T_elapsed < 35 && playermove!=5)
		{
			playermove = 6;
			camera->SetPitch(-25);
			camera->SetYaw(0);
			Vector3 PP = player->GetWorldTransform().GetPositionVector();
			Vector3 CP = (PP + Vector3(100, 400, 550));
			camera->SetPosition(CP);
		}
		if (T_elapsed > 35 && playermove != 5)
		{
			Scene = 3;
			T_elapsed = 0.0;
			playermove = 5;
		}
	}

	if (autoplay && Scene == 3) {
		if (T_elapsed < 20) {
			camera->SetPosition(Vector3(1800 + 130 * sin(T_elapsed / 10), 1000, 1800 + 130 * cos(T_elapsed / 10)));
			camera->SetYaw(190 + T_elapsed * 3.6);
			camera->SetPitch(-35 - 10 * sin(T_elapsed / 5));
		}
		if (T_elapsed > 20 && T_elapsed < 60)
		{
			speed = 2;
			camera->SetPosition(Vector3(1800 + 130 * sin(T_elapsed / 10), 1000, 1800 + 130 * cos(T_elapsed / 10)));
			camera->SetYaw(190 + T_elapsed * 3.6);
			camera->SetPitch(-35 - 10 * sin(T_elapsed / 5));
		}
		if (T_elapsed > 60 && T_elapsed < 114)
		{
			speed = 3;
			camera->SetPosition(Vector3(1800 + 130 * sin(T_elapsed / 10), 1000, 1800 + 130 * cos(T_elapsed / 10)));
			camera->SetYaw(190 + T_elapsed * 3.6);
			camera->SetPitch(-35 - 10 * sin(T_elapsed / 5));
		}

		if (T_elapsed > 114 && T_elapsed < 120)
		{
			camera->SetPosition(Vector3(1800 + 170 * (T_elapsed-114), 1000+ (T_elapsed - 114 )*44, 1800+T_elapsed * 4));
			camera->SetYaw(246);
			camera->SetPitch(-35);
		}
		if (T_elapsed > 120 && T_elapsed < 131) {
			speed = 1;
			camera->SetPosition(Vector3(2800, 1250, 1840));
			camera->SetYaw(246);
			camera->SetPitch(-35);
		}
		if (T_elapsed > 131 && T_elapsed < 160) {
			speed = 3;
			camera->SetPosition(Vector3(2800, 1250, 1800));
			camera->SetYaw(246);
			camera->SetPitch(-35);
		}

		if (T_elapsed > 160 && T_elapsed < 168)
		{
			speed = 1;
			camera->SetPitch(-90);
			camera->SetYaw(T_elapsed *3);
			camera->SetPosition(Vector3(1800, 1350 + 430 * (T_elapsed - 160), 1800));
		}

		if (T_elapsed > 166)
		{
			Scene = 1;
			T_elapsed = 0.0;
		}
	}
}

void  Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer


	if (Scene == 1) {

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		RenderScene1();
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
	else if (Scene == 2) {
		RenderScene2();
	}
	else if (Scene == 3) {

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		RenderScene3();
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
	else return;

	RenderTextures();
	 
	glUseProgram(0);
	SwapBuffers();

}

Renderer ::~Renderer(void) {
	delete  camera;
	currentShader = NULL;

	//Text
	delete basicFont;
	delete TextShader;

	//Scene1
	delete  light;
	delete  heightMapS1;
	delete rootS1;
	Tree::DeleteTree();
	delete  TerrainShaderS1;
	delete  TreeShaderS1;


	//Scene2
	delete  player;
	delete lightS2;
	delete rootS2;
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	CubeRobot::DeleteCube();
	delete PlayerShaderS2;
	delete FloorShaderS2;
	delete  shadowShaderS2;


	//Scene3
	delete		heightMapS3;
	delete	    lightShaderS3;
	delete      reflectShaderS3;
	delete      skyboxShaderS3;
	delete		TreeShaderS3;
	delete		ParticleShaderS3;
	delete		Particles;
	delete		rootS3;
	delete		lightS3;

}



// General Scene Managment...
void Renderer::Input() {

	// Start and stop Time
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P)) {
		if (!pause) {
			pause = true;
			speed = 0;
		}
		else {
			pause = false;
			speed = 1;
		}
	}

	// Change speed
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0)) {
		speed = 0;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		speed = 1;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		speed = 2;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
		speed = 3;
	}

	// Change sun movement
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS)) {
		SunSpeed += 0.0005f * msec;
		movesun = false;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS)) {
		SunSpeed -= 0.0005f * msec;
		movesun = false;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0)) {
		if (!movesun) {
			movesun = true;
		}
		else {
			movesun = false;
		}
	}

	// Change between scenes
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT))
	{
		autoplay = false;
		Scene++;
			if (Scene == 4) {
				Scene = 1;
			}
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT))
	{
		autoplay = false;
		Scene--;
		if (Scene == 0) {
			Scene = 3;
		}
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_UP) || Window::GetKeyboard()->KeyTriggered(KEYBOARD_DOWN))
	{
		if (!autoplay) {
			autoplay = true;
		}
		else {
			autoplay = false;
			playermove = 5;
		}
	}
}

void Renderer::set_timer(float time) {
	if (!pause)
		msec = time;
	else
		msec = msec;
}


//General Code used across Scenes
void Renderer::set_speed() {

	T_elapsed += msec * 0.001f * speed;

	T = sin(T_elapsed / 2) / 10;
}
float Renderer::Day_cycle() {

	if (SunSpeed > 10) {
		SunSpeed = 10;
	}
	if (SunSpeed < -10) {
		SunSpeed = -10;
	}
	if (movesun && !pause) {
		t = T_elapsed / 5;
	}

	day = sin(t  * SunSpeed / 5);

	if (day < 0) {
		day = -day;
	}
	if (day >= 0.994f && !night) {
		night = true;
		k = -1;
	}
	if (day <= 0.006 && night) {
		night = false;
		k = 1;
	}
	return day;
}


// Texture Rendering...
void Renderer::RendererTexture() {

	TextShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	if (!TextShader->LinkProgram()) {
		return;
	}
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	init = true;

}
void Renderer::RenderTextures() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	SetCurrentShader(TextShader);

	this->textureMatrix.ToIdentity();


	UpdateShaderMatrices();

	stringstream stream;
	stream << fixed << FPS;
	string s = stream.str();


	glUseProgram(currentShader->GetProgram());	//Enable the shader...
	UpdateShaderMatrices();											

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "textTex"),18);

	glActiveTexture(GL_TEXTURE18);
	////glBindTexture(GL_TEXTURE_2D, WaterTex);
	glBindTexture(GL_TEXTURE_2D, basicFont->TextTexture);
	//Render function to encapsulate our font rendering!
	DrawText(s , Vector3(0,0, 0), 32.0f);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);
	Matrix4 tempModel = modelMatrix;
	Matrix4 Tempview = viewMatrix;
	Matrix4 tempprojMatrix = projMatrix;
	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)

	modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
	viewMatrix.ToIdentity();
	projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	modelMatrix = tempModel;
	viewMatrix = Tempview;
	projMatrix = tempprojMatrix;
	glDisable(GL_BLEND);
	delete mesh; //Once it's drawn, we don't need it anymore!
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


// Scene1 Specific Rendering...
void  Renderer::RendererScene1() {



	//Initialize scene variables
	T_elapsed = 0.00f;
	day = 0.0f;
	movesun = true;
	night = false;
	T = 0;
	t = 0.0f;
	k = 1;
	speed = 1;
	SunSpeed = 1.0f;
	pause = false;
	

	rootS1 = new SceneNode();

	heightMapS1 = new  HeightMap(TEXTUREDIR"terrain.raw");

	light = new  Light(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 1500.0f,
		(RAW_HEIGHT*HEIGHTMAP_Z / 2.0f)),
		Vector4(1, 1, 1, 1),
		(RAW_WIDTH*HEIGHTMAP_X));

	quad = Mesh::GenerateQuad();

	heightMapS1->SetTexture1(WaterTex);
	heightMapS1->SetTexture2(SandTex);
	heightMapS1->SetTexture3(GrassTex);
	heightMapS1->SetTexture4(MountTex);
	heightMapS1->SetTexture5(SnowTex);

	TerrainShaderS1 = new  Shader(SHADERDIR"TerrainVertex3.glsl", SHADERDIR"TerrainFragment3.glsl");
	TreeShaderS1 = new  Shader(SHADERDIR"TreeVert.glsl", SHADERDIR"TreeFragment.glsl");


	if (!TerrainShaderS1->LinkProgram() || !TreeShaderS1->LinkProgram()) {
		return;
	}


	heightMapS1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!cubeMapS1 || !heightMapS1->GetBumpMap()) {
		return;
	}

	SetCurrentShader(TerrainShaderS1);

	HeightMapInitS1();
	SetTextureRepeating(heightMapS1->GetBumpMap(), true);


	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	OBJMesh* m = new OBJMesh(MESHDIR"tree.obj");


	

	m->SetTreeTexture(TreeText);

	srand(rand());
	for (int i = 0; i < 5000; i++)
	{
		float ran = rand() % 5+((rand()%100)/100);
		SceneNode* tree = new SceneNode(m);

		tree->SetModelScale(Vector3(4 + ran, 4 + ran, 4 + ran));

		Vector3 pos = Vector3(0, 0, 0);
		float angle;
		//Pick Random pos (inside heightmap)
		pos = Vector3(
			(float)(rand() % int(HEIGHTMAP_X * RAW_WIDTH - 2)),
			0.0f,
			(float)(rand() % int(HEIGHTMAP_Z * RAW_HEIGHT - 2))
		);

		if (heightMapS1->GetHeightAtPoint(pos.x, pos.z, pos.y, angle))
		{
			tree->SetShader(TreeShaderS1);
			tree->SetTransform(Matrix4::Translation(pos));
			tree->SetTransform(tree->GetTransform()*Matrix4::Rotation(angle * 20, Vector3(1, 0, 1)));
			rootS1->AddChild(tree);
		}
	}
	init = true;
}
void  Renderer::RenderScene1() {

	this->textureMatrix.ToIdentity();
	this->modelMatrix.ToIdentity();


	sun_position = Day_cycle();
	light->SetPosition(Vector3(((1.5*RAW_HEIGHT*HEIGHTMAP_X  * sun_position) - 130.0f),
		(k * 4000 * sun_position), (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f)));
	DrawSkyboxS1();
	DrawHeightmapS1();
	DrawTreeS1(rootS1);

}

void   Renderer::DrawSkyboxS1() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShaderS3);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}
void   Renderer::DrawHeightmapS1() {
	SetCurrentShader(TerrainShaderS1);
	SetShaderLight(*light);
	sun_position = Day_cycle();
	light->SetPosition(Vector3(((1.5*RAW_HEIGHT*HEIGHTMAP_X  * sun_position) - 130.0f), (k * 4000 * sun_position), (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f)));


	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	SetShaderLight(*light);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "WaterTex"), Water);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "SandTex"), Sand);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "GrassTex"), Grass);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "MountTex"), Mount);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "SnowTex"), Snow);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "T"), T);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "terrainHeight"), heightMapS1->terrainHigh);




	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), bumpTexs1Hei);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	textureMatrix = Matrix4::Scale(Vector3(5.0f, 5.0f, 5.0f)) *
		Matrix4::Rotation(750/15, Vector3(0.5f, 0.0f, 1.0f));
	UpdateShaderMatrices();

	heightMapS1->Draw();
	
	glUseProgram(0);

};

void Renderer::HeightMapInitS1()
{
	float ANISOTROPY = 4;

	glBindTexture(GL_TEXTURE_2D, heightMapS1->GetTexture1());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(heightMapS1->GetTexture1(), true);

	glBindTexture(GL_TEXTURE_2D, heightMapS1->GetTexture2());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(heightMapS1->GetTexture2(), true);

	glBindTexture(GL_TEXTURE_2D, heightMapS1->GetTexture3());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(heightMapS1->GetTexture3(), true);

	glBindTexture(GL_TEXTURE_2D, heightMapS1->GetTexture4());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(heightMapS1->GetTexture4(), true);

	glBindTexture(GL_TEXTURE_2D, heightMapS1->GetTexture5());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(heightMapS1->GetTexture5(), true);
}

void  Renderer::DrawTreeS1(SceneNode*n) {

	if (n->GetMesh()) {
		UpdateShaderMatrices();

		currentShader = n->GetShader();
		glUseProgram(currentShader->GetProgram());

		Matrix4  transform = n->GetTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(),
			"modelMatrix"), 1, false, (float *)& transform);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
			"nodeColour"), 1, (float *)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTreeTexture());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "Treetex"), 10);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TreeText);

		n->Draw(*this);
	}
	for (vector <SceneNode *>::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawTreeS1(*i);
	}
	glUseProgram(0);
}


 //Scene2 Specific Rendering...
void  Renderer::RendererScene2() 
{

	lightdirection = 0;

	CubeRobot::CreateCube();

	rootS2 = new SceneNode();

	Floor = Mesh::GenerateFloor();

	
	camera->SetPosition(Vector3(0, 100, 750.0f));

	PlayerShaderS2 = new  Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");
	FloorShaderS2 = new  Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");
	shadowShaderS2 = new  Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");


	if (!PlayerShaderS2->LinkProgram() || !FloorShaderS2->LinkProgram() || !shadowShaderS2->LinkProgram()) {
		return;
	}

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
		GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, ShadowTex2);


	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SetCurrentShader(FloorShaderS2);

	Floor->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"brickDOT3.tga" , SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	Floor->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"grass2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));


	FloorInit();

	if (!Floor->GetFTexture()) {
		return;
	}
	SetCurrentShader(PlayerShaderS2);

	player = new  SceneNode();
	SceneNode* robot = new CubeRobot();
	player->AddChild(robot);
	player->SetTransform(Matrix4::Translation(Vector3(-4000, 0, 4000)));
	player->SetModelScale(Vector3(PlayerScale, PlayerScale, PlayerScale));

	lightS2 = new Light(Vector3(0, 20, 0), Vector4(1, 1, 1, 1), 1.0f);

	SetCurrentShader(TreeShaderS1);

	OBJMesh* m = new OBJMesh(MESHDIR"tree.obj");

	m->SetTreeTexture(TreeText);

	srand(rand());
	for (int i = 0; i < 200; i++)
	{
		float ran = rand() % 20 + ((rand() % 100) / 100);
		SceneNode* tree = new SceneNode(m);

		tree->SetModelScale(Vector3(12 + ran, 12 + ran, 12 + ran));

		Vector3 pos = Vector3(0, 0, 0);
		pos = Vector3((float)((rand() % 10000)-5000), 0.0f, (float)((rand() % 10000) - 5000));
		tree->SetShader(TreeShaderS1);
		tree->SetTransform(Matrix4::Translation(pos));
		rootS2->AddChild(tree);
	}


	init = true;
}
void  Renderer::RenderScene2() {


	DrawShadowSceneS2();
	DrawCombinedSceneS2();

	DrawFloor();	
	DrawPlayer(player);
	DrawTreeS2(rootS2);

	glUseProgram(0);
};

void Renderer::InputPlayer() {
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Y) || playermove == 8) {
		player->SetTransform(Matrix4::Translation(player->GetTransform()*Vector3(0, 0, -runspeed)));
		lightdirection = 8;
		move = true;
	}
	else if (Window::GetKeyboard()->KeyDown(KEYBOARD_H) || playermove == 2) {
		player->SetTransform(Matrix4::Translation(player->GetTransform()*Vector3(0, 0, runspeed)));
		lightdirection = 2;
		move = true;
	}
	else if (Window::GetKeyboard()->KeyDown(KEYBOARD_G) || playermove == 4) {
		player->SetTransform(player->GetTransform()*Matrix4::Rotation(-90.0f, Vector3(0, 1, 0)));
		player->SetTransform(Matrix4::Translation(player->GetTransform()*Vector3(0, 0, runspeed)));
		lightdirection = 4;
		move = true;
	}
	else if (Window::GetKeyboard()->KeyDown(KEYBOARD_J) || playermove == 6) {
		player->SetTransform(player->GetTransform()*Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
		player->SetTransform(Matrix4::Translation(player->GetTransform()*Vector3(0, 0, runspeed)));
		lightdirection = 6;
		move = true;
	}
}

void Renderer::FloorInit()
{
	/*
	float ANISOTROPY = 4;

	glBindTexture(GL_TEXTURE_2D, Floor->GetFTexture());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(Floor->GetFTexture(), true);

	glBindTexture(GL_TEXTURE_2D, Floor->GetFTexture1());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(Floor->GetFTexture1(), true);

	glBindTexture(GL_TEXTURE_2D, Floor->GetFTexture2());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(Floor->GetFTexture2(), true);

	glBindTexture(GL_TEXTURE_2D, Floor->GetFTexture3());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(Floor->GetFTexture3(), true);

	glBindTexture(GL_TEXTURE_2D, Floor->GetFTexture4());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &ANISOTROPY);
	glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, ANISOTROPY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetTextureRepeating(Floor->GetFTexture4(), false);
	*/
}

void  Renderer::DrawPlayer(SceneNode*n){

	SetCurrentShader(PlayerShaderS2);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	if (n->GetMesh()) {
		Matrix4  transform = n->GetWorldTransform() *
			Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(
			glGetUniformLocation(currentShader->GetProgram(),
				"modelMatrix"), 1, false, (float *)& transform);

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
			"nodeColour"), 1, (float *)&n->GetColour());

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);


		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTexture());
		n->Draw(*this);
	}
	for (vector <SceneNode *>::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawPlayer(*i);
	}
}
void Renderer::DrawFloor() {
	SetCurrentShader(FloorShaderS2);
	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();


	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "GrassTex"), Grass);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "FlowerTex"), Nothing);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "DirtTex"), Nothing);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "RoadTex"), Nothing);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "blendMap"), Nothing);



	glActiveTexture(GL_TEXTURE0+Grass);
	glBindTexture(GL_TEXTURE_2D, GrasTex);
	glActiveTexture(GL_TEXTURE0+ Nothing);
	glBindTexture(GL_TEXTURE_2D, FlowerTex);
	glActiveTexture(GL_TEXTURE0 + Nothing);
	glBindTexture(GL_TEXTURE_2D, DirtTex);
	glActiveTexture(GL_TEXTURE0 + Nothing);
	glBindTexture(GL_TEXTURE_2D, RoadTex);
	glActiveTexture(GL_TEXTURE0 + Nothing);
	glBindTexture(GL_TEXTURE_2D, blendMap);

	modelMatrix = Matrix4::Rotation(0, Vector3(1, 0, 0)) *
		Matrix4::Scale(Vector3(1, 1, 1));
	Matrix4  tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "textureMatrix"), 1, false, *& tempMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram()
		, "modelMatrix"), 1, false, *& modelMatrix.values);

	Floor->Draw();

}
void  Renderer::DrawTreeS2(SceneNode*n) {

	if (n->GetMesh()) {
		UpdateShaderMatrices();

		SetCurrentShader(TreeShaderS3);
		glUseProgram(currentShader->GetProgram());

		Matrix4  transform = n->GetTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(),
			"modelMatrix"), 1, false, (float *)& transform);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
			"nodeColour"), 1, (float *)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTreeTexture());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "Treetex"), Tree);
		glActiveTexture(GL_TEXTURE0+ Tree);
		glBindTexture(GL_TEXTURE_2D, TreeText);

		n->Draw(*this);
	}
	for (vector <SceneNode *>::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawTreeS3(*i);
	}

	
}

void  Renderer::DrawShadowSceneS2() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShaderS2);

	if (lightdirection == 8) {
		viewMatrix = Matrix4::BuildViewMatrix(lightS2->GetPosition(), Vector3(0, 0, -9000));
	}
	else if (lightdirection == 2) {
		viewMatrix = Matrix4::BuildViewMatrix(lightS2->GetPosition(), Vector3(0, 0, 9000));
	}
	else if (lightdirection == 4) {
		viewMatrix = Matrix4::BuildViewMatrix(lightS2->GetPosition(), Vector3(-9000, 0, 0));
	}
	else if (lightdirection == 6) {
		viewMatrix = Matrix4::BuildViewMatrix(lightS2->GetPosition(), Vector3(9000, 0, 0));
	}
	else {
		viewMatrix = Matrix4::BuildViewMatrix(lightS2->GetPosition(), Vector3(0, 0, -9000));
	}
	textureMatrix = biasMatrix *(projMatrix*viewMatrix);
	projMatrix = Matrix4::Perspective(0.5f, 10000.0f,
		(float)width / (float)height, 45.0f);
	UpdateShaderMatrices();

	DrawFloor();
	DrawTreeS2(rootS2);

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	projMatrix = Matrix4::Perspective(0.5f, 20000.0f,
		(float)width / (float)height, 45.0f);
}
void  Renderer::DrawCombinedSceneS2() {
	SetCurrentShader(FloorShaderS2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), Grass);
	glActiveTexture(GL_TEXTURE0+ Grass);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), bumpTex2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"shadowTex"), ShadowTex2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), cameraPos2, (float *)& camera->GetPosition());

	SetShaderLight(*lightS2);

	glActiveTexture(GL_TEXTURE0+ ShadowTex2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glActiveTexture(GL_TEXTURE0+ Empty);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();


}

// Scene3 Specific Rendering...
void  Renderer::RendererScene3() {

	this->textureMatrix.ToIdentity();
	this->modelMatrix.ToIdentity();

	rootS3 = new SceneNode();

	
	heightMapS3 = new  HeightMap(TEXTUREDIR"terrain2.raw");
	quad = Mesh::GenerateQuad();

	camera->SetPosition(Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f + 30,
		800.0f, RAW_WIDTH*HEIGHTMAP_X));

	lightS3 = new  Light(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 1500.0f,
		(RAW_HEIGHT*HEIGHTMAP_Z / 2.0f)),
		Vector4(1, 1, 1, 1),
		(RAW_WIDTH*HEIGHTMAP_X) / 1.0f);

	reflectShaderS3 = new  Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShaderS3 = new  Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShaderS3 = new  Shader(SHADERDIR"TerrainVertex4.glsl", SHADERDIR"TerrainFragment4.glsl");
	TreeShaderS3 = new Shader(SHADERDIR"TreeVert.glsl", SHADERDIR"TreeFragment.glsl");
	ParticleShaderS3 = new Shader(SHADERDIR"Particlevertex.glsl", SHADERDIR"Particlefragment.glsl", SHADERDIR"Particlegeometry.glsl");

	if (!reflectShaderS3->LinkProgram() || !lightShaderS3->LinkProgram() ||
		!skyboxShaderS3->LinkProgram() || !TreeShaderS3->LinkProgram() || !ParticleShaderS3->LinkProgram()) {
		return;
	}
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water2.jpg",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	

	heightMapS3->SetTexture1(SandTex);
	heightMapS3->SetTexture2(GrassTex);
	heightMapS3->SetTexture3(MountTex);
	/////////////////////////////////////////////////////////////////////////////////////
	heightMapS3->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMapS3 = SOIL_load_OGL_cubemap(TEXTUREDIR"cottoncandy_ft.tga", TEXTUREDIR"cottoncandy_bk.tga",
		TEXTUREDIR"cottoncandy_up.tga", TEXTUREDIR"cottoncandy_dn.tga",   TEXTUREDIR"cottoncandy_rt.tga",
		TEXTUREDIR"cottoncandy_lf.tga", SOIL_LOAD_RGB, 25 , SOIL_FLAG_MIPMAPS);

	if (!cubeMapS3 || !quad->GetTexture() || !heightMapS3->GetTexture1() ||
		!heightMapS3->GetBumpMap()) {
		return;
	}
	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(heightMapS3->GetBumpMap(), true);
	init = true;
	waterRotate = 0.0f;

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	OBJMesh* m = new OBJMesh(MESHDIR"tree.obj");


	Particles = new ParticleEmitter(Vector3(3600, 300, 2150));

	m->SetTreeTexture(TreeText);

	srand(rand());
	for (int i = 0; i < 100; i++)
	{
		float ran = rand() % 5 + ((rand() % 100) / 100);
		SceneNode* tree = new SceneNode(m);

		tree->SetModelScale(Vector3(4 + ran, 4 + ran, 4 + ran));

		Vector3 pos = Vector3(0, 0, 0);
		float angle;
		//Pick Random pos (inside heightmap)
		pos = Vector3(
			(float)(rand() % int(HEIGHTMAP_X * RAW_WIDTH - 2)),
			0.0f,
			(float)(rand() % int(HEIGHTMAP_Z * RAW_HEIGHT - 2))
		);

		if (heightMapS3->GetHeightAtPoint(pos.x, pos.z, pos.y, angle))
		{
			tree->SetShader(TreeShaderS3);
			tree->SetTransform(Matrix4::Translation(pos));
			rootS3->AddChild(tree);
		}
	}
	init = true;
}
void  Renderer::RenderScene3() {

	this->textureMatrix.ToIdentity();
	this->modelMatrix.ToIdentity();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkyboxS3();
	sun_position = Day_cycle();
	lightS3->SetPosition(Vector3(2000 * sin(T_elapsed )+2000, 1000, 2000 * cos(T_elapsed ) + 2000));
	DrawHeightmapS3();
	DrawWater();
	DrawTreeS3(rootS3);
	DrawParticlesS3();

}

void   Renderer::DrawSkyboxS3() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShaderS3);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}
void   Renderer::DrawHeightmapS3() {
	SetCurrentShader(lightShaderS3);
	SetShaderLight(*lightS3);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), cameraPoss3, (float *)& camera->GetPosition());
	////////////////////////////////////////////////////////////////////////////////////////
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "SandTex"), Sand);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "GrassTex"), Grass);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "MountTex"), Mount);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "terrainHeight"), heightMapS3->terrainHigh);



	glActiveTexture(GL_TEXTURE0+ Sand);
	glBindTexture(GL_TEXTURE_2D, SandTex);
	glActiveTexture(GL_TEXTURE0+ Grass);
	glBindTexture(GL_TEXTURE_2D, GrassTex);
	glActiveTexture(GL_TEXTURE0+ Mount);
	glBindTexture(GL_TEXTURE_2D, MountTex);


	////////////////////////////////////////////////////////////////////////////////////////
	     glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		                    "cameraPos"), cameraPoss3, (float *)& camera->GetPosition());
	

	     glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		                   "waterTex"), Water);



	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	textureMatrix = Matrix4::Scale(Vector3(5.0f, 5.0f, 5.0f)) *
		Matrix4::Rotation(waterRotate / 15, Vector3(0.5f, 0.0f, 1.0f));
	UpdateShaderMatrices();

	heightMapS3->Draw();

	glUseProgram(0);
}
void Renderer::DrawParticlesS3() {

	currentShader = ParticleShaderS3;

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "ParticlText"), ParticlText);

	Matrix4 tempmodel = modelMatrix;

	modelMatrix.ToIdentity();

	viewMatrix = camera->BuildViewMatrix();

	SetShaderParticleSizeS3(Particles->GetParticleSize());
	Particles->SetParticleSize(30.0f);
	Particles->SetParticleVariance(5.0f);
	Particles->SetLaunchParticles(5.0f);
	Particles->SetParticleLifetime(8000.0f);
	Particles->SetParticleSpeed(0.1f*cos(T_elapsed / 300));
	UpdateShaderMatrices();


	Particles->Draw(T_elapsed);
	modelMatrix = tempmodel;
	glUseProgram(0);
}
void Renderer::SetShaderParticleSizeS3(float f) {
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}
void Renderer::DrawWater() {
	SetCurrentShader(reflectShaderS3);
	SetShaderLight(*lightS3);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), cameraPoss3, (float *)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"waterTex"), Water);
	glActiveTexture(GL_TEXTURE0+ Water);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), cubetext3);
	glActiveTexture(GL_TEXTURE0+ cubetext3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapS3);

	float  heightX = (RAW_WIDTH*HEIGHTMAP_X / 2.0f);

	float  heightY = 115 * HEIGHTMAP_Y / 3.0f;

	float  heightZ = (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));
	UpdateShaderMatrices();
	glActiveTexture(GL_TEXTURE0+ Empty);
	quad->Draw();
	glActiveTexture(GL_TEXTURE0+ Empty);
	glUseProgram(0);
}
void  Renderer::DrawTreeS3(SceneNode*n) {


	if (n->GetMesh()) {
		UpdateShaderMatrices();

		SetCurrentShader(TreeShaderS3);
		glUseProgram(currentShader->GetProgram());

		Matrix4  transform = n->GetTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(),
			"modelMatrix"), 1, false, (float *)& transform);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(),
			"nodeColour"), 1, (float *)&n->GetColour());

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTreeTexture());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "Treetex"), 23);
		glActiveTexture(GL_TEXTURE23);
		glBindTexture(GL_TEXTURE_2D, TreeText);

		n->Draw(*this);
	}
	for (vector <SceneNode *>::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawTreeS3(*i);
	}
	glUseProgram(0);
}
