#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <gl/GLU.h>
#include "TextureLoader.h"
#include "Player.h"


enum class GameState {PLAY, EXIT};

class MainGame
{
	struct camera {
		float lx, ly, lz;
		float x, y, z;
		float deltaAngle, deltaAngleY;
		float angle, angley;
		int xOrigin;
		float moveX, moveY, moveZ;
		bool cameraMove;
		char direction;
		float velocity;
		bool isStuck;
	};

public:
	MainGame();
	~MainGame();
	void startGame();

private:
	void initSystems();
	void gameLoop();
	void processInput();
	void mouseMove(int x, int y);
	void cameraMove(char dir);
	void render();
	//
	float collisionPointPlane(float normalX, float normalY, float normalZ, float x1, float z1, float x2, float z2);
	//
	enum { SKY_LEFT = 0, SKY_BACK, SKY_RIGHT, SKY_FRONT, SKY_TOP, SKY_BOTTOM };
	unsigned int skybox[6];
	void initSkybox();
	void killSkybox();
	void drawSkybox(float size);
	//
	void BindTexture(void *data, int width, int height, int format, unsigned int number);
	unsigned int texture_id[100];
	//
	void initGlew();
	//
	void drawFloor();
	void drawPlayer();

	//
	float rotate2DAboutPoint(float x, float z, float x0, float z0, float angle);
	float rotate2D(float x, float z, float angle);
	//

	Player *player;
	SDL_Window *_window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	camera _Camera;

	vector<unsigned int> lists;

	unsigned int vbo;

	TextureLoader *_loader;

	
};

