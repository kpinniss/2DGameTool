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
#include "../Utils/FileReaderUtil.h"
#include <vector>
#include <fstream>

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

void Game::LoadLevel(int level)
{
    //add systems that need to be processed
    _registry->AddSystem<MovementSystem>();
    _registry->AddSystem<RenderSystem>();
    
    //Add assets to store
    _assetStore->AddTexture(_renderer, "tank-image", "./assets/images/tank-panther-right.png");
    _assetStore->AddTexture(_renderer, "truck-image", "./assets/images/truck-ford-down.png");
    _assetStore->AddTexture(_renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

    //load tilemap
    int mapCols = 25;
    int mapRows = 20;

    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");
    for(int y = 0; y < mapRows; y++)
    {
        for(int x = 0; x < mapCols; x++)
        {
            int srcX, srcY;
            char ch;
            mapFile.get(ch);
            srcY = atoi(&ch) * _baseSize;
            mapFile.get(ch);
            srcX = atoi(&ch) * _baseSize;
            mapFile.ignore();

            Entity tile = _registry->CreateEntity();
            tile.AddComponent<TransformComponent>(
                glm::vec2(x * (_baseScale * _baseSize), y * (_baseScale * _baseSize)), 
                glm::vec2(_baseScale, _baseScale), 
                0.0
            );
            tile.AddComponent<SpriteComponent>("tilemap-image", _baseSize, _baseSize, srcX, srcY);
        }
    }
    mapFile.close();

    // Create an entity
    Entity tank = _registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(_baseScale,_baseScale), 0.0);
    tank.AddComponent<RigidbodyComponent>(glm::vec2(50.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", _baseSize, _baseSize);

    Entity truck = _registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(_baseScale,_baseScale), 0.0);
    truck.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 50.0));
    truck.AddComponent<SpriteComponent>("truck-image", _baseSize, _baseSize);
}

void Game::Setup()
{
    LoadLevel(1);
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