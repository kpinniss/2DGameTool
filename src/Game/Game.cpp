#include <iostream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/Registry.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"

Game::Game()
{
    Logger::Log("Game contructor called.");
    _gameRunning = false;
    _registry = std::make_unique<Registry>();
}

Game::~Game()
{
    Logger::Log("Game destructor called.");
}

void Game::Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        Logger::Err("Error initializing SDL");
        return;
    }
    
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0,&displayMode);
    _windowHeight = displayMode.h;
    _windowWidth = displayMode.w;
    _window = SDL_CreateWindow(
        "Game Tool", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _windowWidth, 
        _windowHeight, 
        SDL_WINDOW_RESIZABLE
    );
    if(!_window){
        Logger::Err("Error creating SDL window");
        return;
    }

    _renderer = SDL_CreateRenderer(_window,-1,0);
    if(!_renderer){
        Logger::Err("Error creating renderer");
        return;
    }
    SDL_SetWindowFullscreen(_window,SDL_WINDOW_FULLSCREEN);
    _gameRunning = true;
}

void Game::Setup()
{
    //TODO
    Entity ent1 = _registry->CreateEntity();

    ent1.AddComponent<TransformComponent>(
        glm::vec2(10.0,10.0),
        glm::vec2(1.0, 1.0),
        0
    );
    ent1.AddComponent<RigidbodyComponent>(
        glm::vec2(20.0,0.0)
    );  
    ent1.RemoveComponent<TransformComponent>();
}

void Game::Update()
{
    
    //handle timing
    int timeToWait = Mil_Per_Frame - (SDL_GetTicks() - _milLastFrame);
    if(timeToWait > 0 && timeToWait <= Mil_Per_Frame){
        SDL_Delay(timeToWait);
    } 
    //alg: convert difference to seconds
    double deltaTime = (SDL_GetTicks() - _milLastFrame) / 1000.0;

    //cache previous frame
    _milLastFrame = SDL_GetTicks();

    //TODO:
    //MovemoentSystem.Update();
    //CollisionSystem.Update();
    //...etc
}

void Game::ProcessInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            _gameRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                _gameRunning = false;
                break;
            
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(_renderer,100,100,100,255); //draw purple with r =100/255 and b = 100/255
    SDL_RenderClear(_renderer);
    //TODO: Render game objects
    SDL_RenderPresent(_renderer); //swap buffers
}

void Game::Run()
{
    Setup();
    while (_gameRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
    
}

void Game::Destroy()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}