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
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../AssetStore/AssetStore.h"
#include "../Utils/FileReaderUtil.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderGizmosSystem.h"
#include <vector>
#include <fstream>

enum LayerType
{
    TILEMAP_LAYER,
    TILE_LAYER,
    ENEMY_LAYER,
    PLAYER_LAYER,
    PROJECTILE_LAYER,
    UI_LAYER
};

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
    _registry->AddSystem<AnimationSystem>();
    _registry->AddSystem<CollisionSystem>();
    _registry->AddSystem<RenderGizmosSystem>();
    
    //Add assets to store
    _assetStore->AddTexture(_renderer, "tank-image", "./assets/images/tank-panther-right.png");
    _assetStore->AddTexture(_renderer, "chopper-image", "./assets/images/chopper.png");
    _assetStore->AddTexture(_renderer, "radar-image", "./assets/images/radar.png");
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
            tile.AddComponent<SpriteComponent>("tilemap-image", _baseSize, _baseSize, TILEMAP_LAYER, srcX, srcY);
        }
    }
    mapFile.close();

    //Create game entities
    Entity tank = _registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(_baseScale,_baseScale), 0.0);
    tank.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", _baseSize, _baseSize, ENEMY_LAYER);
    tank.AddComponent<BoxColliderComponent>(_baseSize*_baseScale, _baseSize*_baseScale);

    Entity truck = _registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(110.0, 10.0), glm::vec2(_baseScale,_baseScale), 0.0);
    truck.AddComponent<RigidbodyComponent>(glm::vec2(-20.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", _baseSize, _baseSize, ENEMY_LAYER);
    truck.AddComponent<BoxColliderComponent>(_baseSize*_baseScale, _baseSize*_baseScale);

    Entity chopper = _registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2(100.0, 200.0), glm::vec2(_baseScale,_baseScale), 0.0);
    chopper.AddComponent<RigidbodyComponent>(glm::vec2(80.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", _baseSize, _baseSize, PLAYER_LAYER);
    chopper.AddComponent<AnimationComponent>(2, 15, true);

    Entity radar = _registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2((_windowWidth - (_baseSize * 2)) / 2, 0.0), glm::vec2(_baseScale,_baseScale), 0.0);
    radar.AddComponent<RigidbodyComponent>(glm::vec2(0.0, 0.0));
    radar.AddComponent<SpriteComponent>("radar-image", _baseSize * 2, _baseSize * 2, UI_LAYER);
    radar.AddComponent<AnimationComponent>(8, 10, true);
    radar.Destroy();
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
    //update registry once all systems are updated
    _registry->Update();

    //get systems to update
    _registry->GetSystem<MovementSystem>().Update(deltaTime);
    _registry->GetSystem<AnimationSystem>().Update();
    _registry->GetSystem<CollisionSystem>().Update();
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
            case SDLK_0:
                _viewGizmos = !_viewGizmos;
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
    if(_viewGizmos)
    {
        _registry->GetSystem<RenderGizmosSystem>().Update(_renderer);
    }
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