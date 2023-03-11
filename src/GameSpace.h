#pragma once

#include "Dependencies.h"
#include "Player.h"
#include "Text.h"
#include "Collider.h"
#include "Camera.h"


#define MAX_COLLIDERS 512
#define MAX_ENTITIES 256

#define WIN_WIDTH 240
#define WIN_HEIGHT 180

#define TILE_SIZE 8


enum {
    positiontxt,
    velocitytxt,
    directiontxt,
    isSupportedtxt,
    frametimetxt,
    ramtrackingtxt,
    collidercounttxt,
    playerstatetxt,
    textcount
};

class GameSpace {
    public:
        void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);
        void update();
        void render();
        void destroyWindow();
        void handleEvents();
        void inputCalc(SDL_Keycode key);
        void updateDebugText(Player* use_player);
        void handlePlayerDeath();
        SDL_Rect GetTextureSize();

        bool running() {return isRunning;}

        static SDL_Renderer *renderer;
        static const Uint8 *keyboard_state_array;

        static Camera *camera;

        static Text *textarray[textcount];

        static Collider *staticcolliders[MAX_COLLIDERS];
        static GameObject *entities[MAX_ENTITIES];

        static Text *score;
        static Text *highscore;

        static Uint32 framecount;

        static int screenx, screeny;

        static SDL_Rect texdestRect;
        static SDL_Rect objdestRect; // texture holding all objects

    
    private:
        bool isRunning;
        SDL_Window *window;
        int player1controls[10] = {
            SDL_SCANCODE_W,
            SDL_SCANCODE_S,
            SDL_SCANCODE_A,
            SDL_SCANCODE_D,
            SDL_SCANCODE_COMMA,
            SDL_SCANCODE_I,
            SDL_SCANCODE_K,
            SDL_SCANCODE_J,
            SDL_SCANCODE_L,
            SDL_SCANCODE_SLASH
        };

};