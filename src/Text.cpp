#include "Text.h"
#include "GameSpace.h"


Text::Text(const char* font_string, int ptsize, const char* in_string, SDL_Color in_color, int x, int y, bool alignleft) {

    font = TTF_OpenFont(font_string, ptsize);
    string = in_string;
    color = in_color;

    textTexture = nullptr;

    this->x = x;
    this->y = y;

    UpdateText(string, alignleft);
    
}

Text::~Text() {
    if (textTexture) SDL_DestroyTexture(textTexture);

}

void Text::UpdateText(const char* new_text, bool alignleft) {
    string = new_text;
    int w=0,h=0;

    TTF_SizeText(font, string, &w, &h);

    if (!alignleft) text_rect.x = x-w;
    else text_rect.x = x;
    text_rect.y = y;
    text_rect.h = h;
    text_rect.w = w;

    SDL_Surface* surfaceText_new = TTF_RenderText_Blended(font, string, color);

    if (textTexture) SDL_DestroyTexture(textTexture);
    textTexture = SDL_CreateTextureFromSurface(GameSpace::renderer, surfaceText_new);
    SDL_FreeSurface(surfaceText_new);
}

void Text::Draw() {
    SDL_RenderCopy(GameSpace::renderer, textTexture, NULL, &text_rect);
}