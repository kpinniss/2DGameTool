#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>

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