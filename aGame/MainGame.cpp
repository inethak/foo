#include "MainGame.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "ObjectLoader.h"
#include "Player.h" 

#define PI 3.14159265359


void fatalError(std::string errorString) {
	std::cout << "Fatal Error: ";
	std::cout << errorString << std::endl;
	system("pause");
	SDL_Quit();
	exit(1);
}

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 800;
	_screenHeight = 600;
	_gameState = GameState::PLAY;
	
}


MainGame::~MainGame()
{
}


void MainGame::startGame() {
	initSystems();
	gameLoop();
}
void MainGame::initSystems() {
	SDL_Init(SDL_INIT_EVERYTHING);
	
	_window = SDL_CreateWindow("foo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (!_window)
		fatalError("SDL window couldn't be created.");

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (!glContext)
		fatalError("SDL context couldn't be created.");

	glewExperimental = true;
	GLenum error = glewInit();
	if (error)
		fatalError("Couldn't initialize glew.");

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)_screenWidth/_screenHeight, 1.0, 1500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);


	initSkybox();
	_loader = new TextureLoader();
	texture_id[0] = _loader->LoadTexture("resources\\textures\\ground.bmp", true);
	////////////////////////COLLISION IS IN CPLUSPLUSGUT TUTS!!!!!!!!!!!!!!!!!!!
	for (int i = 0; i < player.object->textures_name.size(); i++)
	{
		player.object->textures.push_back(_loader->LoadTexture(player.object->textures_name[i].c_str(),false));
	}
	drawPlayer();
	
}



void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT)
	{
		processInput();
		render();

		if (player.camera.cameraMove)
		{
			cameraMove(player.camera.direction);
		}
	}
}  

void MainGame::processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
			case SDL_QUIT:
			{
				_gameState = GameState::EXIT;
				break;
			}
			case SDL_MOUSEMOTION:
			{
			    mouseMove(event.motion.x , event.motion.y);
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				 player.camera.x += event.wheel.y;
				 player.camera.z += event.wheel.y;
				break;
			}
	 		case SDL_KEYDOWN:
			{
				if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d)
				{
					player.camera.direction = event.key.keysym.sym;
					player.camera.cameraMove = true;
				}
				break;
			}
			case SDL_KEYUP:
			{
				 player.camera.cameraMove = false;
				 player.camera.direction = '\0';
				break;
			}
		default:
			break;
		}
		
	}

	//SDL_Quit(); //????????????
	//killSkybox(); //???????????
}

void MainGame::drawPlayer()
{
	int num;
	num = glGenLists(1);
	glNewList(num, GL_COMPILE);

	/*
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player.object->vertex), &player.object->vertex, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);*/
	//glLoadIdentity();
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	//glVertexPointer(3, GL_FLOAT, 0, &player.object->vertex);
	//glDrawArrays(GL_TRIANGLES, 0, player.object->vertex.size()/3);
	


	glEnable(GL_TEXTURE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	GLfloat position[] = { player.coords.x, fabs(player.camera.moveY * 2), player.coords.z , 1.0 };
	GLfloat vector[] = { player.coords.x, fabs(player.camera.moveY), player.coords.z };

	//+glBindTexture(GL_TEXTURE_2D, player.object->texture);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, vector);

	glTranslatef(player.coords.x * player.scale, player.coords.y * player.scale, player.coords.z * player.scale);
	glRotatef(player.coords.angle, 0, 1, 0);

	int index = 0;
	int mats = 0;

	float texcoords[] = { 0., 0., 0., 1., 1., 1. };

	for (int face = 0; face < player.object->vertex_faces.size(); face += 3)
	{
		if (player.object->mat_start.size() > 1 && player.object->mat_start.size() > mats+1)
			if (face > player.object->mat_start[mats+1])
			{
				mats++;
			}
		glBindTexture(GL_TEXTURE_2D, player.object->textures[mats]);

		glBegin(GL_TRIANGLES);

			for (int vertex = 0, c = 0; vertex < 3; vertex++, c+=2)
			{
				//if (player.object->facesHasTex[index])
				//glTexCoord2d(texcoords[c], texcoords[c+1]);
				glTexCoord2f(player.object->UV_vertex[player.object->texture_faces[index] - 1].x , player.object->UV_vertex[player.object->texture_faces[index] - 1].y); //player.object->vertex[player.object->vertex_faces[index] * 3 - 3] * 0.1 * player.scale, player.object->vertex[player.object->vertex_faces[index] * 3 - 1] * 0.1 * player.scale);
			//glNormal3f(player.object->normals[player.object->texture_faces[index] * 3 - 3] * 0.1 * player.scale, player.object->normals[player.object->texture_faces[index] * 3 - 2] * 0.1 * player.scale, player.object->normals[player.object->texture_faces[index] * 3 - 1] * 0.1 * player.scale);
				 glNormal3f(player.object->normals[player.object->normals_faces[index] * 3 - 3] * 0.1 * player.scale, player.object->normals[player.object->normals_faces[index] * 3 - 2] * 0.1 * player.scale, player.object->normals[player.object->normals_faces[index] * 3 - 1] * 0.1 * player.scale);
		//	glVertex3f(player.object->vertex[player.object->texture_faces[index] * 3 - 3] * 0.1 * player.scale, player.object->vertex[player.object->texture_faces[index] * 3 - 2] * 0.1 * player.scale, player.object->vertex[player.object->texture_faces[index] * 3 - 1] * 0.1 * player.scale);
				glVertex3f(player.object->vertex[player.object->vertex_faces[index] * 3 - 3] * 0.1 * player.scale, player.object->vertex[player.object->vertex_faces[index] * 3 - 2] * 0.1 * player.scale, player.object->vertex[player.object->vertex_faces[index] * 3 - 1] * 0.1 * player.scale);
			
				index++;
			
			}
		glEnd();
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	
	//glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEndList();

	player.object->normals.clear();
	player.object->vertex.clear();
	player.object->vertex_faces.clear();
	player.object->normals_faces.clear();
	player.object->UV_vertex.clear();

	lists.push_back(num);
}

#define MAIN_SCALER 1.0
void MainGame::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(player.camera.x, player.camera.y, player.camera.z, player.camera.x + player.camera.lx, player.camera.y + player.camera.ly, player.camera.z + player.camera.lz, 0.f, 1.f, 0.f);
	glTranslatef(player.camera.moveX * MAIN_SCALER, player.camera.moveY * MAIN_SCALER, player.camera.moveZ * MAIN_SCALER);

	drawSkybox(510.0f);
	Render_Floor();
	//drawPlayer();
	glPushMatrix();
	glTranslatef(player.coords.x * player.scale, player.coords.y * player.scale, player.coords.z * player.scale);
	glRotatef(player.coords.angle, 0, 1, 0);
	glCallList(lists[0]);
	glPopMatrix();

	SDL_GL_SwapWindow(_window);
}

 void MainGame::mouseMove(int x, int y)
{

	 player.camera.deltaAngle = (x - player.camera.xOrigin) * 0.01f;
	 player.camera.deltaAngle = (x - player.camera.xOrigin) * 0.01f;
	 player.camera.deltaAngleY = y  * 0.01f;

	 player.camera.lx = sin(player.camera.angle + player.camera.deltaAngle);
	 player.camera.ly = -tan(player.camera.angley + player.camera.deltaAngleY);
	 player.camera.lz = -cos(player.camera.angle + player.camera.deltaAngle);



 }

 float MainGame::collisionPointPlane(float normalX, float normalY, float normalZ, float x1, float z1, float x2, float z2)
 {
	 GLfloat length = sqrt(player.camera.lx * player.camera.lx + player.camera.ly * player.camera.ly + player.camera.lz * player.camera.lz);

	 if (player.camera.moveX >= -x2 && player.camera.moveX <= x1 && player.camera.moveZ >= -z2 && player.camera.moveZ <= z1)
	 {
		 float nX, nY, nZ;
		 bool more;
		 if (sqrt(pow((player.camera.moveX - normalX), 2) + pow((player.camera.moveY - normalY), 2) + pow((player.camera.moveZ - normalZ), 2)) < sqrt(pow((player.camera.moveX - normalX), 2) + pow((player.camera.moveY - (-normalY)), 2) + pow((player.camera.moveZ - normalZ), 2)))
		 {
			// std::cout << "distance to 0;-1;0 is shorter" << std::endl;
			 nX = normalX;
			 nY = -normalY;
			 nZ = normalZ;
			 more = true;
		 }
		 else
		 {
			// std::cout << "distance to 0;1;0 is shorter" << std::endl;
			 nX = normalX;
			 nY = normalY / 100.; //0.01;
			 nZ = normalZ;
			 more = false;
		 }

		 //std::cout << "distance to plane: " << float(fabs(_Camera.moveX * nX + _Camera.moveY * normalY + _Camera.moveZ * nZ) + nY) << std::endl;

		 if (player.camera.isStuck)
		 {
			 float angle = acos((player.camera.lx * nX + player.camera.ly* nY + player.camera.lz * nZ) / (1 * sqrt(player.camera.lx * player.camera.lx + player.camera.ly * player.camera.ly + player.camera.lz * player.camera.lz))) * 180 / PI;
			 //std::cout << "angle is " << angle << " degrees" << std::endl;

			 //float _y = _Camera.ly / length * _Camera.velocity;
			 if (!more)
			 {
				 //std::cout << "angle should be grater than 90 degrees" << std::endl;
				 if (angle < 90)
				 {
					 player.camera.isStuck = false;
					// return false;
					 return  player.camera.ly / length * player.camera.velocity;
				 }
			 }
			 else
			 {
				// std::cout << "angle should be less than 90 degrees" << std::endl;
				 if (angle < 90)
				 {
					
					 player.camera.isStuck = false;
					 return  player.camera.ly / length * player.camera.velocity;
				 }
			 }

		 }
		 else {
			 if (float(fabs(player.camera.moveX * nX + player.camera.moveY *normalY + player.camera.moveZ * nZ) + nY) > 0.1)
			 {
				 player.camera.isStuck = false;
				 return  player.camera.ly / length * player.camera.velocity;
			 }
			 else
			 {
				 player.camera.isStuck = true;
			 }
		 }
	 }
	 else
	 {
		 player.camera.isStuck = false;
		 return  player.camera.ly / length * player.camera.velocity;
	 }

	 return 0.f;
 }

 void MainGame::cameraMove(char dir)
 {
	 GLfloat length = sqrt(player.camera.lx * player.camera.lx + player.camera.ly * player.camera.ly + player.camera.lz * player.camera.lz);
	 switch (dir) {
	 case SDLK_w:
	 {
		player.camera.moveX -= player.camera.lx / length * player.camera.velocity;
		player.coords.x += player.camera.lx / length * player.camera.velocity;
		player.camera.moveZ -= player.camera.lz / length * player.camera.velocity;
		player.coords.z += player.camera.lz / length * player.camera.velocity;

		break;
	 }
	 case SDLK_s:
	 {
		player.camera.moveX += player.camera.lx / length * player.camera.velocity;
		player.coords.x -= player.camera.lx / length * player.camera.velocity;
		player.camera.moveZ += player.camera.lz / length * player.camera.velocity;
		player.coords.z -= player.camera.lz / length * player.camera.velocity;

		break;
	 }
	 case SDLK_d:
	 {

		float tempx = player.coords.x;
		player.coords.x = 0;
		player.coords.angle -= .5;
		player.coords.x = tempx;

		break;
	 }
	 case SDLK_a:
	 {
		float tempx = player.coords.x;
		player.coords.x = 0;
		player.coords.angle += .5;
		player.coords.x = tempx;
		break;
	 }
	 default:
		 break;
	 }

 }

 void MainGame::initSkybox() {
	
	 skybox[SKY_LEFT] = _loader->LoadTexture("resources\\textures\\skybox\\sky_left.bmp", false);
	 skybox[SKY_BACK] = _loader->LoadTexture("resources\\textures\\skybox\\sky_back.bmp", false);
	 skybox[SKY_RIGHT] = _loader->LoadTexture("resources\\textures\\skybox\\sky_right.bmp", false);
	 skybox[SKY_FRONT] = _loader->LoadTexture("resources\\textures\\skybox\\sky_front.bmp", false);
	 skybox[SKY_TOP] = _loader->LoadTexture("resources\\textures\\skybox\\sky_top.bmp", false);
	 skybox[SKY_BOTTOM] = _loader->LoadTexture("resources\\textures\\skybox\\sky_bottom.bmp", false);
 }

 void MainGame::killSkybox() {
	 glDeleteTextures(6, &skybox[0]);
 }

 void MainGame::drawSkybox(float size) {

	 glDisable(GL_LIGHTING);
	 glDisable(GL_DEPTH_TEST);
	 glDisable(GL_LIGHT0);

	// glPushMatrix();
	 glBindTexture(GL_TEXTURE_2D, skybox[SKY_BACK]);
	 glBegin(GL_QUADS);
		 glTexCoord2f(0, 0);
		 glVertex3f(size, size, size);
		 glTexCoord2f(1, 0);
		 glVertex3f(-size, size, size);
		 glTexCoord2f(1, 1);
		 glVertex3f(-size, -size, size);
		 glTexCoord2f(0, 1);
		 glVertex3f(size, -size, size);
	 glEnd();

	 glBindTexture(GL_TEXTURE_2D, skybox[SKY_LEFT]);
	 glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-size, size, size);
		glTexCoord2f(1, 0);
		glVertex3f(-size, size, -size);
		glTexCoord2f(1, 1);
		glVertex3f(-size, -size, -size);
		glTexCoord2f(0, 1);
		glVertex3f(-size, -size, size);
	 glEnd();

	 glBindTexture(GL_TEXTURE_2D, skybox[SKY_RIGHT]);
	 glBegin(GL_QUADS);
		 glTexCoord2f(0, 0);
		 glVertex3f(size, size, -size);
		 glTexCoord2f(1, 0);
		 glVertex3f(size, size, size);
		 glTexCoord2f(1, 1);
		 glVertex3f(size, -size, size);
		 glTexCoord2f(0, 1);
		 glVertex3f(size, -size, -size);
	 glEnd();

	 glBindTexture(GL_TEXTURE_2D, skybox[SKY_FRONT]);
	 glBegin(GL_QUADS);
		 glTexCoord2f(1, 0);
		 glVertex3f(size, size, -size);
		 glTexCoord2f(0, 0);
		 glVertex3f(-size, size, -size);
		 glTexCoord2f(0, 1);
		 glVertex3f(-size, -size, -size);
		 glTexCoord2f(1, 1);
		 glVertex3f(size, -size, -size);
	 glEnd();

	 glBindTexture(GL_TEXTURE_2D, skybox[SKY_TOP]);
	 glBegin(GL_QUADS);
		 glTexCoord2f(1, 0);
		 glVertex3f(size, size, size);
		 glTexCoord2f(0, 0);
		 glVertex3f(-size, size, size);
		 glTexCoord2f(0, 1);
		 glVertex3f(-size, size, -size);
		 glTexCoord2f(1, 1);
		 glVertex3f(size, size, -size);
	 glEnd();

	 glBindTexture(GL_TEXTURE_2D, skybox[SKY_BOTTOM]);
	 glBegin(GL_QUADS);
		 glTexCoord2f(1, 1);
		 glVertex3f(size, -size, size);
		 glTexCoord2f(0, 1);
		 glVertex3f(-size, -size, size);
		 glTexCoord2f(0, 0);
		 glVertex3f(-size, -size, -size);
		 glTexCoord2f(1, 0);
		 glVertex3f(size, -size, -size);
	 glEnd();

	 glBindTexture(GL_TEXTURE_2D, 0);
	  glEnable(GL_DEPTH_TEST);

 }

 void MainGame::Render_Floor()
 {

	 glBindTexture(GL_TEXTURE_2D, texture_id[0]);

	 glBegin(GL_QUADS);
	 glNormal3f(0.0, -1.0, 0.0);

	 glTexCoord2f(0.0f, 0.0f);
	 glVertex3f(0.0f, 0.0f, 0.0f);
	 glTexCoord2f(0.0f, 100.0f);
	 glVertex3f(0.0f, 0.0f, 100.0f);
	 glTexCoord2f(100.0f, 100.0f);
	 glVertex3f(100.0f, 0.0f, 100.0f);
	 glTexCoord2f(100.0f, 0.0f);
	 glVertex3f(100.0f, 0.0f, 0.0f);

	 glEnd();

	 glBindTexture(GL_TEXTURE_2D, 0);
 }