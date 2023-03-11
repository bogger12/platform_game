// TODO:
// add coyote time DONE
// add prejumps
// add some kind of obstacle
// make velocity lock to speedcap if either speed goes over speedcap DONE


#include "GameSpace.h"
#include "GameObject.cpp"
#include "Player.cpp"
#include "Map.cpp"
#include "Text.cpp"
#include "Collider.cpp"
#include "Spring.cpp"
#include "Camera.cpp"
// #include "AnimationController.h"

#define DO_DEBUG 1

// GameSpace Elements
Player *player;
Map *map;
GameObject *grabblepoint;
GameObject *background_img;

// Core Elements
Camera* GameSpace::camera;
SDL_Renderer* GameSpace::renderer = nullptr;
const Uint8* GameSpace::keyboard_state_array = SDL_GetKeyboardState(NULL);
Text* GameSpace::textarray[textcount];
Collider* GameSpace::staticcolliders[MAX_COLLIDERS];
GameObject* GameSpace::entities[MAX_ENTITIES];

// UI Elements
Text* GameSpace::score = nullptr;
Text* GameSpace::highscore = nullptr;

Uint32 GameSpace::framecount = 0;

int GameSpace::screenx;
int GameSpace::screeny;
SDL_Rect GameSpace::objdestRect = makeSDL_Rect(0,0,WIN_WIDTH,WIN_HEIGHT);
SDL_Rect GameSpace::texdestRect;

SDL_Rect GameSpace::GetTextureSize() {
    
    SDL_GetRendererOutputSize(renderer, &screenx, &screeny);
    std::cout << screenx << " " << screeny << std::endl;

    int winx, winy, winw, winh;

    float ratio = (float)WIN_WIDTH/(float)WIN_HEIGHT;

    bool fitheight = WIN_WIDTH<WIN_HEIGHT || ratio < ((float)screenx/(float)screeny);
    //bool fitwidth = WIN_WIDTH>WIN_HEIGHT || ratio > (screenx/screeny);

    if (fitheight) {
        winh = screeny;
        winw = winh * ratio;
        winx = screenx/2 - winw/2;
        winy = 0;
    }
    else {
        winw = screenx;
        winh = winw / ratio;
        winx = 0;
        winy = screeny/2 - winh/2;
    }
    return makeSDL_Rect(winx, winy, winw, winh);
}


void GameSpace::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;

    TTF_Init();

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL Subsystems Initialised" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) std::cout << "Window created!" << std::endl;

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created!" << std::endl;
        }
        isRunning = true;
    }
    else {
        isRunning = false;
    }

    // do stuff now that the window and renderer are made
    texdestRect = GetTextureSize();

    // Initialise all game entities

    player = new Player("../assets/playersheet.png", 0.5, 0, 0, makeSDL_Rect(0,0,32,32), player1controls);

    grabblepoint = new GameObject("../assets/grabblepoint.png", 75, 19, makeSDL_Rect(3,3,1,1));
    entities[0] = grabblepoint;

    map = new Map();

    camera = new Camera();

    // Initialise text (remove)
    int fontsize = 15;
    SDL_Color textcolor = {255,255,255};

    std::cout << "penis" << std::endl;

    textarray[positiontxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "positionplaceholder", textcolor, screenx, fontsize, false);
    textarray[velocitytxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "velocityplaceholder", textcolor, screenx, fontsize*2, false);
    textarray[directiontxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "directionplaceholder", textcolor, screenx, fontsize*3, false);
    textarray[isSupportedtxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "supportedplaceholder", textcolor, screenx, fontsize*4, false);
    textarray[frametimetxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "frametimeplaceholder", textcolor, screenx, fontsize*5, false);
    textarray[ramtrackingtxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "ramplaceholder", textcolor, screenx, fontsize*6, false);
    textarray[collidercounttxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "collidderplaceholder", textcolor, screenx, fontsize*7, false);
    textarray[playerstatetxt] = new Text("../fonts/OpenSans-Regular.ttf", fontsize, "stateplaceholder", textcolor, screenx, fontsize*8, false);

}

void GameSpace::handleEvents() {
    SDL_Event event;

    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            texdestRect = GetTextureSize();

        default:
            break;
    }
}

void GameSpace::updateDebugText(Player* use_player) {
    if (use_player) {
        std::string positiondebugtxt = "position: " + std::string(use_player->getExactPos());
        GameSpace::textarray[positiontxt]->UpdateText(positiondebugtxt.c_str(), false);
        std::string velocitydebugtxt = "finvelocity: " + std::string((use_player->getV()));
        GameSpace::textarray[velocitytxt]->UpdateText(velocitydebugtxt.c_str(), false);
        std::string directiondebugtxt = "direction: " + std::string(use_player->direction);
        GameSpace::textarray[directiontxt]->UpdateText(directiondebugtxt.c_str(), false);
        std::string support = ((use_player->isSupported) ? "true" : "false");
        std::string supporteddebugtxt = "isSupported: " + support;
        GameSpace::textarray[isSupportedtxt]->UpdateText(supporteddebugtxt.c_str(), false);
        std::string statedebugtxt = "state: " + std::to_string(use_player->state);
        GameSpace::textarray[playerstatetxt]->UpdateText(statedebugtxt.c_str(), false);
        
        int numcol = 0;
        for (auto& c : GameSpace::staticcolliders) if (c) numcol++;
        GameSpace::textarray[collidercounttxt]->UpdateText(formatstring("collider_count: %i", numcol), false);

    }
}

void GameSpace::handlePlayerDeath() {
    delete player;
    player = new Player("../assets/playersheet.png", 1, 0,0, makeSDL_Rect(0,0,32,32), player1controls);
}

void GameSpace::update() {
    
    if (!player->Update()) {
        handlePlayerDeath();
    };

    for (auto& e: entities) if (e) e->Update(); 

    if (DO_DEBUG) updateDebugText(player);
}

void GameSpace::render() {

    SDL_Texture *texTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);
    SDL_SetRenderTarget(renderer, texTarget);
    SDL_SetRenderDrawColor(renderer, 183, 205, 232, 255);

    SDL_RenderClear(renderer);
    // this is where we add stuff to render

    //background_img->Render();
    
    // update "camera" offset
    camera->updateCameraOffset(player);

    player->Render();

    map->Draw(false);

    // draw entities
    for (auto& e: entities) if (e) e->Render(); 

    // draws the collider textures
    // for (auto& c: staticcolliders) if (c) c->Render(); 
    player->playercollider->Render();

    

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texTarget, NULL, &texdestRect);
    if (DO_DEBUG) for (auto& t: textarray) t->Draw();


    SDL_RenderPresent(renderer);
    framecount++; // add one to the internal frame counter.
}

void GameSpace::destroyWindow() {
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Quit, window destroyed" << std::endl;
}