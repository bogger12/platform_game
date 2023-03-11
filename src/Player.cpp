#include "Player.h"
#include "AnimationController.cpp"


#define DO_GRAVITY true

Player::Player(const char *textureLabel, float speed, int x, int y, SDL_Rect srcRect, int controls[NUM_CONTROLS]): 
GameObject(textureLabel, x, y, srcRect) {
    this->scale = scale;
    std::cout << "speed: " << speed << NUM_CONTROLS << std::endl;
    this->speed = speed;
    
    SDL_Rect collisionbox = makeSDL_Rect(position.x,position.y,TILE_SIZE,TILE_SIZE);

    playercollider = new Collider(collisionbox, false, "yellow");

    for (int i = 0; i < NUM_CONTROLS; i++) this->controls[i] = controls[i];
};

// This updates the player object every frame
bool Player::Update() {
    isSupported = checkSupported();
    
    inputCalc(GameSpace::keyboard_state_array);
    setState();
    anim->Update(state, (int)abs(storedvelocity.x)*10);
    doGrabble();
    move();
    playercollider->collider_box.x = round(position.x);
    playercollider->collider_box.y = round(position.y);
    // if (position.y > 640) return false;
    // else return true;
    return true;
}

// Gets the inputs from the keyboard and applies them to Vector2D direction 
void Player::inputCalc(const Uint8 *keyboard_state_array) {
    direction = Vector2D(0,0);
    if (!(keyboard_state_array[controls[UP]] && keyboard_state_array[controls[DOWN]])) {
        //if (keyboard_state_array[controls[UP]]) direction.y = -1;
        if (keyboard_state_array[controls[DOWN]]) direction.y = 1;
    }
    if (!(keyboard_state_array[controls[LEFT]] && keyboard_state_array[controls[RIGHT]])) {
        if (keyboard_state_array[controls[LEFT]]) direction.x = -1;
        if (keyboard_state_array[controls[RIGHT]]) direction.x = 1;
    }

    if ((jump != keyboard_state_array[controls[JUMP]]) && !jump) dojump = true; //only jumps on rising edge of jump input
    else dojump = false;
    jump = keyboard_state_array[controls[JUMP]];
    //std::cout << dojump << " " << jumps << std::endl;
    grabbleactive = keyboard_state_array[controls[GRABBLE]];

    resetpixmove = keyboard_state_array[controls[UP]];
    // stupid pixel by pixel movement for debug lol
    if (!(keyboard_state_array[controls[PIX_UP]] && keyboard_state_array[controls[PIX_DOWN]])) {
        if (keyboard_state_array[controls[PIX_UP]]) pix_direction.y += -1;
        if (keyboard_state_array[controls[PIX_DOWN]]) pix_direction.y += 1;
    }
    if (!(keyboard_state_array[controls[PIX_LEFT]] && keyboard_state_array[controls[PIX_RIGHT]])) {
        if (keyboard_state_array[controls[PIX_LEFT]]) pix_direction.x += -1;
        if (keyboard_state_array[controls[PIX_RIGHT]]) pix_direction.x += 1;
    }
    
}

void Player::doGrabble() {
    if (!grabbleactive) {
        if (grabblehook) {
            delete grabblehook;
            grabblehook = nullptr;
        }
        return;
    };
    
    if (!grabblehook) {
        GameObject *closest = nullptr;
        //scan area around player and find closest grabble point
        for (auto& e: GameSpace::entities) if (e) {
            int x = e->getPos().x - position.x;
            int y = e->getPos().y - position.y;

            if (!closest) closest = e;
            else if (sqrt(closest->getPos().x*closest->getPos().x+closest->getPos().y*closest->getPos().y) > sqrt(x*x+y*y)) {
                closest = e;
            }
        }

        grabblehook = new Spring("../assets/banana.png", this, closest, Vector2D(10,5));
    } 

    grabblehook->Update();
}

// sets the current state of the player - currently only used for sprite animation
void Player::setState() {
    // if (direction.x != 0 || direction.y != 0) state = RUNNING;
    if (!isSupported && storedvelocity.y <= 0) state = JUMPING;
    else if (!isSupported && storedvelocity.y > 0) state = FALLING;
    else if (isSupported && storedvelocity.x != 0) state = RUNNING;
    else state = IDLE;
}

// This function could be saved by having the supported detection in checkBounds() but that makes the isSupported bool a frame late
bool Player::checkSupported() {
    bool supported = false;
    for (auto& c : GameSpace::staticcolliders) {
        if (c) {
            if (c->getSupported(playercollider->collider_box)) supported = true;
        }
    }
    // if (GameSpace::framecount >= supportedUpdate + COYOTETIME || supported) {
    //     isCoyote = supported;
    //     supportedUpdate = GameSpace::framecount;
    // }
    if (supported) supportedUpdate = COYOTETIME+1;
    else if (supportedUpdate > 0) supportedUpdate -= 1;

    return supported;
}

// This checks if player is colliding and changes the displacement accordingly - this only accounts for player colliding with one square object at once
Vector2D Player::checkBounds(Vector2D displacement) {
    for (auto& c : GameSpace::staticcolliders) {
        if (c) {
            bool collided = c->checkCollision(displacement, playercollider->collider_box);
            if (collided) {
                displacement = c->getDisplacement(displacement, playercollider->collider_box);
                if (!displacement.x) velocity.x = movedisplacement.x = 0;
                if (!displacement.y) velocity.y = movedisplacement.y = 0;
            }
        }
    }
    return displacement;
}

// This applies all inputs/external effects to player velocity - legibility can probs be improved
void Player::move() {

    const float gravityconst = 5;
    Vector2D speedcap = Vector2D(1,3.75);
    airtimeframes = isSupported ? 1 : airtimeframes+1;

    const float JUMPINIT = 1.5; 
    const float JUMPMULTP = 1;
    
    const float JUMPENDFRICTION = 0.2;

    
    // apply forces to object (add to velocity)
    if (DO_GRAVITY && pix_direction == Vector2D(0,0)) {

        Vector2D jumpforce;
        if ((dojump) && (jumps > 0)) {
            velocity.y = 0; // velocity is changed here - not good practice
            jumpforce = Vector2D(0,-JUMPINIT) * (supportedUpdate > 0);
            jumps -= 1;
        } else if (isSupported) jumps = NUMJUMPS;

        // unrealistic jump logic:
        Vector2D jumpassist;
        if (velocity.y < 0 && jump) jumpassist = Vector2D(0,-(gravityconst/60) * JUMPMULTP/(0.1*(airtimeframes+9)));
        else if (velocity.y < 0 && airtimeframes > 5 && !grabbleactive) jumpassist = Vector2D(0,-velocity.y*JUMPENDFRICTION);

        // slows down if no direction pressed
        Vector2D currentmovement = direction * speed * (direction.isnotzero()); //apply simple movement force

        Vector2D moveAllow = ((movedisplacement+currentmovement).absv() <= speedcap); 
        movedisplacement += currentmovement * moveAllow + moveAllow.invbin() * movedisplacement.bin()*(speedcap-movedisplacement.absv());
        // slows down:
        movedisplacement.x *= (direction.x == 0) ? 0.8: 1;
        movedisplacement.y *= (direction.y == 0) ? 0.8: 1;
        if (abs(movedisplacement.x) < 0.01 && !direction.x) movedisplacement.x = 0; // makes x velocity 0 if it is below a certain point


        // Applying all forces
        Vector2D addtovelocity = Vector2D(0,0); 
        addtovelocity += Vector2D(0, (gravityconst/60)); //apply gravity
        addtovelocity += jumpforce; //apply init force of jump
        addtovelocity += jumpassist; //apply force given by holding jump

        velocity += addtovelocity;

        velocity.x *= (!grabbleactive && direction.x != velocity.bin().x) ? 0.8 : 1;
        //if (abs(velocity.x) < 0.01 && !direction.x) velocity.x = 0; // makes x velocity 0 if it is below a certain point
        if (grabbleactive) velocity -= velocity*0.02; // air friction;

        storedvelocity = (velocity+movedisplacement);
        
        position += checkBounds(storedvelocity);
    }
    else if (pix_direction != Vector2D(0,0)) {
        if (resetpixmove) pix_direction = Vector2D(0,0);
        if (pix_direction.y <=1 && pix_direction.y >=-1 && pix_direction.x <=1 && pix_direction.x >=-1) position += pix_direction;
    }
    else position += (direction * speed);

}

// Gets the animation sprite for this frame and renders it to the screen.
void Player::Render() {

    if (grabblehook) grabblehook->Render();

    destRect = makeSDL_Rect(
        round(position.x), 
        round(position.y), 
        TILE_SIZE, 
        TILE_SIZE
    );
    srcRect = anim->GetCurrentFrame(state, SPRITESIZE);
    if (direction.x < 0) flip = SDL_FLIP_HORIZONTAL;
    else if (direction.x > 0) flip = SDL_FLIP_NONE;

    SDL_RenderCopyEx(GameSpace::renderer, objTexture, &srcRect, GameSpace::camera->transformRect(&destRect), 0, NULL, flip);
}
