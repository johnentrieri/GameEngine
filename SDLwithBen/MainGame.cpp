#include "MainGame.h"
#include "Errors.h"

#include <iostream>
#include <string>

MainGame::MainGame()
{
	_window = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{
}

void MainGame::run() 
{
	initSystems();

	//_sprite.init(-0.75, -0.75, 0.75, 0.75);
	_csprite.init(0.0, 0.0, 0.125);
	gameLoop();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT) 
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << evnt.motion.x << ", " << evnt.motion.y << std::endl;
		default:
			break;
		}
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	
	//_sprite.draw();
	_csprite.draw();

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window);
}

void  MainGame::initSystems() 
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Create SDL Window
	_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr)
	{
		fatalError("SDL Window could not be created.");
	}

	//Create OpenGL Context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr)
	{
		fatalError("GL Context could not be created.");
	}

	//Init GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("Coult not initialize GLEW.");
	}
	
	std::cout << glGetString(GL_VERSION) << "\n";
	std::cout << glGetString(GL_VENDOR) << "\n\n";

	//Enable Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Set Background Color
	glClearColor(0.0, 0.0, 0.0, 1.0);

	initShaders();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.linkShaders();
}
