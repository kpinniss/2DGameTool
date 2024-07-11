#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

const int FPS = 60;
const int Mil_Per_Frame = 1000 / FPS;

class Game
{
    private:
    bool _gameRunning;
    int _windowHeight;
    int _windowWidth;
    int _milLastFrame = 0;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::unique_ptr<Registry> _registry; //uses smar pointer std::unique_ptr for memory alloc and garb collection

    public:
    Game();
    ~Game();
    void Init();
    void ProcessInput();
    void Run();
    void Update();
    void Render();
    void Destroy();
    void Setup();
};

#endif