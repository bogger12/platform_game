#pragma once


// this class switches through frames depending on what the player is doing
class AnimationController {
    public:
        AnimationController(int state_table[4], int type_table[4]);

        SDL_Rect GetCurrentFrame(int state, const int spritesize);
        void Update(int state, int fps);

        int cycle_frame;
        int* state_table;
        int* type_table;

    private:

        bool transition = false;

        int state = 0;
        int laststate = 0;
        SDL_Rect srcRect;

        Uint32 lastUpdate;
        Uint32 currentTime;
};