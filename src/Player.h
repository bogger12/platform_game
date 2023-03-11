#pragma once

#include "GameObject.h"
#include "AnimationController.h"
#include "Collider.h"
#include "Spring.h"

#define SPRITESIZE 32
#define SPRITESCALE 2
#define NUMJUMPS 1

enum { // controls
    UP,
    DOWN,
    LEFT,
    RIGHT,
    JUMP,
    PIX_UP,
    PIX_DOWN,
    PIX_LEFT,
    PIX_RIGHT,
    GRABBLE,
    NUM_CONTROLS
};

enum { // states
    IDLE,
    RUNNING,
    JUMPING,
    FALLING,
    NUM_STATES
};

enum { // different types of animations
    CYCLE,
    HALT_AT_END,
    HALT_AT_START,
    NUM_ANIMS
};

class Player: public GameObject {
    public:
        Player(const char *textureLabel, float speed, int x, int y, SDL_Rect srcRect, int controls[NUM_CONTROLS]);
        bool Update();
        void setState();
        void Render();
        void inputCalc(const Uint8 *keyboard_state_array);
        void move();
        Vector2D checkBounds(Vector2D direction);
        bool checkSupported();
        void doGrabble();

        Vector2D direction = Vector2D(0,0);
        bool isSupported = false;
        Vector2D movedisplacement;
        Vector2D storedvelocity; // needed to conserve speed after stopping (currently conserve speed for infinity)

        Vector2D pix_direction = Vector2D(0,0);

        int state_table[NUM_STATES] = {1,7,2,2}; // how many sprites for each state animation
        int type_table[NUM_STATES] = {CYCLE, CYCLE, HALT_AT_END, HALT_AT_START}; // how many sprites for each state animation
        AnimationController* anim = new AnimationController(state_table, type_table);

        int state = IDLE;

        Collider *playercollider;


    private:
        // Movement
        float speed;

        bool dojump = false;
        bool jump = false; // is jump key pressed
        bool resetpixmove = false;
        int airtimeframes = 1;


        int jumps = NUMJUMPS;
        bool isCoyote = false;
        const int COYOTETIME = 2; // (frames)
        Uint32 supportedUpdate = 0;

        bool grabbleactive;
        Spring *grabblehook = nullptr;
        Vector2D grabbleoffset = Vector2D(10,5);

        int controls[NUM_CONTROLS];
        
        SDL_RendererFlip flip = SDL_FLIP_NONE;
};