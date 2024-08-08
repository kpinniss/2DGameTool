#include <iostream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/Registry.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"

Game::Game()
{
    Logger::Log("Game contructor called.");
    _gameRunning = false;
    _registry = std::make_unique<Registry>();
    _assetStore = std::make_unique<AssetStore>();
}

Game::~Game()
{
    Logger::Log("Game destructor called.");
}

void Game::Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Error initializing SDL");
        return;
    }
    
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0,&displayMode);
    _windowHeight = displayMode.h;
    _windowWidth = displayMode.w;
    _window = SDL_CreateWindow
    (
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
    //add systems that need to be processed
    _registry->AddSystem<MovementSystem>();
    _registry->AddSystem<RenderSystem>();
    
    //Add assets to store
    _assetStore->AddTexture(_renderer, "tank-image", "./assets/images/tank-panther-right.png");
    _assetStore->AddTexture(_renderer, "truck-image", "./assets/images/truck-ford-down.png");

    // Create an entity
    Entity tank = _registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidbodyComponent>(glm::vec2(50.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);

    Entity truck = _registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 50.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
}

void Game::Update()
{
    //handle timing
    int timeToWait = Mil_Per_Frame - (SDL_GetTicks() - _milLastFrame);
    if(timeToWait > 0 && timeToWait <= Mil_Per_Frame)
    {
        SDL_Delay(timeToWait);
    } 
    //alg: convert difference to seconds
    double deltaTime = (SDL_GetTicks() - _milLastFrame) / 1000.0;

    //cache previous frame
    _milLastFrame = SDL_GetTicks();

    //get systems to update
    _registry->GetSystem<MovementSystem>().Update(deltaTime);
    //TODO update all systems

    //update registry once all systems are updated
    _registry->Update();
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
    
    //invoke all systems that need to render
    _registry->GetSystem<RenderSystem>().Update(_renderer, _assetStore);

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