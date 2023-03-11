#include "AnimationController.h"

AnimationController::AnimationController(int state_table[4], int type_table[4]) {
    this->cycle_frame = 0;
    this->state_table = state_table;
    this->type_table = type_table;

    lastUpdate = SDL_GetTicks();
    
}

void AnimationController::Update(int newstate, int velocity) {
    const int num_sprites = state_table[state];
    const int state_type = type_table[state];
    int last_state_type;
    int framediv;
    
    if (state == IDLE) framediv = 60;
    else if (state == RUNNING) framediv = ceil((60/(velocity+0.01)));
    else if (state == JUMPING) framediv = 10;
    else if (state == FALLING) framediv = 2;
    
    if (state != newstate) {
         
        if (transition) NULL;
        else if (type_table[state]==HALT_AT_START) {
            transition = true;
            cycle_frame = 0;
        }
        else {
            state = newstate;
            cycle_frame = 0;
        }
    }
    currentTime = GameSpace::framecount;

    if (transition) {
        if (currentTime > (lastUpdate + framediv) || cycle_frame == 0) {
            if (cycle_frame < state_table[state]-1) {
                cycle_frame++;
            }
            else {
                transition = false;
                state = newstate;
                cycle_frame = 0;
            }
            lastUpdate = GameSpace::framecount;
        }
    }


    if (currentTime > (lastUpdate + framediv)) {

        if (state_type == CYCLE) {
            if (cycle_frame < (num_sprites-1)) cycle_frame++;
            else cycle_frame = 0;
        }

        else if (state_type == HALT_AT_END) {
            if (cycle_frame < num_sprites-1) cycle_frame++;
        }

        else if (state_type == HALT_AT_START) {
            cycle_frame = 0;
        }

        lastUpdate = GameSpace::framecount;
    }
}

SDL_Rect AnimationController::GetCurrentFrame(int newstate, const int spritesize) {
    srcRect = makeSDL_Rect(cycle_frame * spritesize, state * spritesize, spritesize, spritesize);

    return srcRect;
    
}