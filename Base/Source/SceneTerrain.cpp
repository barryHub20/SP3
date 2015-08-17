#include "SceneTerrain.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include "SpriteAnimation.h"
#include <sstream>
const float MIN_FOV = 1.f;
const float	MAX_FOV = 100.f;
const float DEFAULT_FOV = 45.f;

SceneTerrain::SceneTerrain()
{
}

SceneTerrain::~SceneTerrain()
{
}

void SceneTerrain::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	/* shadow */
	m_gPassShaderID = LoadShaders( "Shader//GPass.vertexshader", "Shader//GPass.fragmentshader" );
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	
	m_programID = LoadShaders( "Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader" );
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");


	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED_1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 5, 10);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 2;
	
	/* shadow: m_lightDepthFBO */
	m_lightDepthFBO.Init(1024, 1024);

	/*lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;*/
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	/*glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);*/
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_OBJECT] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_OBJECT]->textureID[0] = LoadTGA("Image//brick.tga");
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(1, 0, 0), 36, 1.f, 1.f);
	
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID[0] = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID[0] = LoadTGA("Image//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID[0] = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID[0] = LoadTGA("Image//bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID[0] = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID[0] = LoadTGA("Image//back.tga");
	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 128, 600.0f, 6000.0f, 6.0f, 6.0f);
	meshList[GEO_SKYPLANE]->textureID[0] = LoadTGA("Image//night_sky6.tga"); 
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("GEO_GROUND", Color(1, 1, 1), 1.f, 200.f);
	meshList[GEO_GROUND]->textureID[0] = LoadTGA("Image//brick_road.tga"); 
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("GEO_CROSSHAIR", Color(1, 1, 1), 1.f);
	meshList[GEO_CROSSHAIR]->textureID[0] = LoadTGA("Image//crosshair.tga");
	meshList[GEO_LAKE] = MeshBuilder::GenerateQuad("GEO_LAKE", Color(1, 1, 1), 1.f);
	meshList[GEO_LAKE]->textureID[0] = LoadTGA("Image//sea.tga");

	/* terrain */
	meshList[GEO_TERRAIN] = meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN",  "Image//heightmap.raw", m_heightMap);  
	meshList[GEO_TERRAIN]->textureID[0] = LoadTGA("Image//moss1.tga"); 
	meshList[GEO_TERRAIN]->textureID[1] = LoadTGA("Image//brick.tga"); 
	terrainScale.Set(4000.f, 350.f, 4000.f);

	/* animation */
	meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("hot girl", 3, 7, 1);
	meshList[GEO_SPRITE_ANIMATION]->textureID[0] = LoadTGA("Image//sprite1.tga");

	meshList[GEO_RUNNING_CAT] = MeshBuilder::GenerateSpriteAnimation("hot cat", 1, 5, 1);
	meshList[GEO_RUNNING_CAT]->textureID[0] = LoadTGA("Image//man.tga");
	
		/* police car */
		meshList[GEO_CAR_MAIN_PART] = MeshBuilder::GenerateOBJ("car main part", "OBJ//car_main_part.obj");
		meshList[GEO_CAR_MAIN_PART]->textureID[0] = LoadTGA("Image//car_main_part.tga");
		meshList[GEO_CAR_GLASS] = MeshBuilder::GenerateOBJ("car glass", "OBJ//car_glass.obj");
		meshList[GEO_CAR_GLASS]->textureID[0] = LoadTGA("Image//car_glass.tga");
		meshList[GEO_CAR_SIREN] = MeshBuilder::GenerateOBJ("car siren", "OBJ//siren.obj");
		meshList[GEO_CAR_SIREN]->textureID[0] = LoadTGA("Image//siren.tga");

	/* shadow */
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("Light depth texture", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureID[0] = m_lightDepthFBO.GetTexture();
	
	animation.resize(2);	//2 animations

	animation[0] = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	animation[1] = dynamic_cast<SpriteAnimation*>(meshList[GEO_RUNNING_CAT]);

	animation[0]->init(2.5f, 1, 0, 2, 6, false);
	animation[1]->init(0.5f, 1, 0, 1, 5, false);
	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 unit
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 1000.f, 120.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;

	bLightEnabled = true;

	//lab Prac 2
	playerPos.SetZero();

	//bullet
	bulletSize = 50;
	ammo = new BulletInfo[bulletSize];

	for(int i = 0; i < bulletSize; ++i)
		ammo[i].Init(camera.position, camera.target, 500.f, 10.f);

	//perspective
	fovAngle = DEFAULT_FOV;
	FovRate = 1.f;
	storeFov = fovAngle;
}

void SceneTerrain::UpdateOpenGL(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneTerrain::Update(double dt)
{
	fps = (float)(1.f / dt);

	/* OpenGL */
	UpdateOpenGL(dt);

	/* fov */
	UpdateFOV(dt);

	/* light */
	UpdateLight(dt);

	/* camera */
	camera.Update(dt, Application::getMyKeys());

	/* player */
	UpdatePlayable(dt);

	/* bullet */
	UpdateBullet(dt);

	/* animation */
	 //animation[0] = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	//animation[1] = dynamic_cast<SpriteAnimation*>(meshList[GEO_RUNNING_CAT]);
	
	for(std::vector<SpriteAnimation*>::iterator it = animation.begin(); it != animation.end(); ++it)
	{
		SpriteAnimation *go = (SpriteAnimation *)*it;
		go->Update(dt);
	}
}

void SceneTerrain::UpdateLight(double dt)
{
	/*if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}*/

	//if(Application::IsKeyPressed('I'))
	//	lights[0].position.z -= (float)(10.f * dt);
	//if(Application::IsKeyPressed('K'))
	//	lights[0].position.z += (float)(10.f * dt);
	//if(Application::IsKeyPressed('O'))
	//	lights[0].position.x -= (float)(10.f * dt);
	//if(Application::IsKeyPressed('P'))
		//lights[0].position.x += (float)(10.f * dt);
	if(Application::IsKeyPressed('O'))
	{
		lights[0].position.y -= (float)(10.f * dt);
	}
	if(Application::IsKeyPressed('P'))
	{
		lights[0].position.y += (float)(10.f * dt);
	}
}

void SceneTerrain::UpdatePlayable(double dt)
{
	//Lab prac 2
	//if(Application::IsKeyPressed(VK_UP))
		
	//if(Application::IsKeyPressed(VK_DOWN))

	//if(Application::IsKeyPressed(VK_LEFT))

	//if(Application::IsKeyPressed(VK_RIGHT))
}

void SceneTerrain::UpdateBullet(double dt)
{
	//update
	static bool pressedDown = false;
	if(Application::getMyKeys()['M'] && !pressedDown)
	{
		pressedDown = true;
		for(int i = 0; i < bulletSize; ++i)
		{
			if(!ammo[i].GetStatus())
			{
				ammo[i].setDir((camera.target - camera.position).Normalized());
				ammo[i].setPos(camera.position);
				ammo[i].SetStatus(true);
				break;
			}
		}
	}

	else if(!Application::IsKeyPressed('M') && pressedDown)
	{
		pressedDown = false;
	}

	for(int i = 0; i < bulletSize; ++i)
	{
		if(ammo[i].GetStatus())
			ammo[i].update(dt);
	}
}

static const float SKYBOXSIZE = 1000.f;

void SceneTerrain::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneTerrain::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID[0] <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneTerrain::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
		if(m_renderPass == RENDER_PASS_PRE)
		{
			Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
			
			glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1,
			GL_FALSE, &lightDepthMVP.a[0]);
			mesh->Render();
			return;
		}
		MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
			modelView = viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE,
			&modelView.a[0]);
			if(enableLight && bLightEnabled)
			{
				glUniform1i(m_parameters[U_LIGHTENABLED], 1);
					modelView_inverse_transpose =
					modelView.GetInverse().GetTranspose();
					glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE],
					1, GL_FALSE, &modelView_inverse_transpose.a[0]);
					
					Mtx44 lightDepthMVP = m_lightDepthProj *
					m_lightDepthView * modelStack.Top();
					
					glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1,
					GL_FALSE, &lightDepthMVP.a[0]);
					
					//load material
					glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1,
					&mesh->material.kAmbient.r);
					glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1,
					&mesh->material.kDiffuse.r);
					glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1,
					&mesh->material.kSpecular.r);
					glUniform1f(m_parameters[U_MATERIAL_SHININESS],
					mesh->material.kShininess);
			}
			else
			{
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
			}
			for(int i = 0; i < 2; ++i)	//2 texture
			{
				if(mesh->textureID[i] > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
					glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
				}
				else
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
			}
		mesh->Render();
}

void SceneTerrain::RenderSkybox()
{
	//left
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

void SceneTerrain::Render()
{
	/****** PRE RENDER PASS *******/
	RenderPassGPass();
	

	/****** MAIN RENDER PASS *******/
	RenderPassMain();

	///** bullet **/
	//RenderBullet();

	///** terrain **/
	//RenderTerrain();

	///** animation **/
	//RenderAnimation();

	/** 2D HUD **/
	RenderHUD();
	
}

void SceneTerrain::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
	&MVP.a[0]);
	if(mesh->textureID[0] > 0)
	{
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
	1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED],
	0);
	}
	mesh->Render();
	if(mesh->textureID[0] > 0)
	{
	glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneTerrain::RenderSkyPlane(Mesh* mesh, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
{
	 modelStack.PushMatrix();
	 modelStack.Translate(500, 5800, -500);
	 RenderMesh(meshList[GEO_SKYPLANE], false);
	 modelStack.PopMatrix();
}

void SceneTerrain::RenderLight()
{
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	/*if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}*/

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();
}

void SceneTerrain::RenderHUD()
{
	/** crosshair **/
	//RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 19);
	
	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	
	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Yaw Velocity " << camera.yawVelocity;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str(std::string());
	ss << camera.position;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5f, 2.f, 11.f);

	ss.str(std::string());
	RenderTextOnScreen(meshList[GEO_TEXT], "Camera pos:", Color(1, 0, 0), 2.5f, 2.f, 15.f);
}

void SceneTerrain::Render3DObjects()
{
	/** object **/
	modelStack.PushMatrix();
	modelStack.Translate(0, 2, 2);
	//modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_CONE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2, 2, 2);
	//modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix();

	////police car
	//modelStack.PushMatrix();
	//	modelStack.Translate(-2, 2, 2);
	//	modelStack.Scale(0.05, 0.05, 0.05);
	//	RenderMesh(meshList[GEO_CAR_MAIN_PART], true);

	//	modelStack.PushMatrix();
	//	RenderMesh(meshList[GEO_CAR_GLASS], true);
	//	modelStack.PopMatrix();


	//	modelStack.PushMatrix();
	//	RenderMesh(meshList[GEO_CAR_SIREN], true);
	//	modelStack.PopMatrix();

	//modelStack.PopMatrix();
}

void SceneTerrain::RenderAnimation()
{
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPRITE_ANIMATION], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_RUNNING_CAT], false);
	modelStack.PopMatrix();
	
}

void SceneTerrain::RenderBullet()
{
	for(int i = 0; i < bulletSize; ++i)
	{
		if(ammo[i].GetStatus())
		{
			modelStack.PushMatrix();
			modelStack.Translate(ammo[i].getPos().x, ammo[i].getPos().y, ammo[i].getPos().z);
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();
		}
	}
}

void SceneTerrain::RenderTerrain()
{
	modelStack.PushMatrix();  
	modelStack.Scale(terrainScale.x, terrainScale.y, terrainScale.z); // values varies.  
	modelStack.Rotate(-45, 0, 0, 1);
	RenderMesh(meshList[GEO_GROUND], false);  
	modelStack.PopMatrix();
}

void SceneTerrain::UpdateFOV(double dt)
{
	//if zoom activated
	//	storeFov = fovAngle;
	static bool zoomActivate = false;
	static double zoomTime = 0.2f;
	static double zoomTimer = zoomTime;

	if(zoomTimer < zoomTime)
		zoomTimer += dt;

	/* Zooming */
	if(Application::getMyKeys()['Z'])	//activate zoom
	{
		if(zoomTimer >= zoomTime)
		{
			if(!zoomActivate)
			{
				storeFov = fovAngle;
				fovAngle = DEFAULT_FOV;	//zoom starts at 45 deg.
				zoomActivate = true;
			}
			else
			{
				fovAngle = storeFov;
				zoomActivate = false;
			}
			zoomTimer = 0.0;
		}
	}

	/* manipulate fov angle */
	if(!zoomActivate)
	{
		if(Application::getMyKeys()['I'] && fovAngle - FovRate >= DEFAULT_FOV) 
			fovAngle -= FovRate;
		if(Application::getMyKeys()['O'] && fovAngle + FovRate <= MAX_FOV)
			fovAngle += FovRate;
	}
	else
	{
		if(Application::getMyKeys()['I'] && fovAngle - FovRate >= MIN_FOV) 
			fovAngle -= FovRate;
		if(Application::getMyKeys()['O'] && fovAngle + FovRate <= DEFAULT_FOV)
			fovAngle += FovRate;
	}
}

//SHADOW
void SceneTerrain::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	//These matrices should change when light position or
	//direction changes
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-15, 15, -15, 15, -20, 20);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);

	m_lightDepthView.SetToLookAt(lights[0].position.x,
	lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	

	RenderWorld();
}

void SceneTerrain::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 800, 600);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	//old stuff
	/* initialize render */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 unit
	perspective.SetToPerspective(fovAngle, Application::getWidth() * (1.f / Application::getHeight()), 0.1f, 1000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	/* axes */
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	/* Render light */
	RenderLight();

	RenderWorld();

	/* render ground */
	RenderGround();
}

void SceneTerrain::RenderGround()
{
	/** ground mesh **/
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -10);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], true);
	modelStack.PopMatrix();

	/** ground mesh **/
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(-50, 1, 0, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], true);
	modelStack.PopMatrix();
}

void SceneTerrain::RenderWorld()
{
	///** 3D objects **/
	Render3DObjects();

	/* Render light */
	//RenderLight();
}

void SceneTerrain::UpdateCameraStatus(const unsigned char key)
{
	camera.UpdateStatus(key);
}

void SceneTerrain::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
