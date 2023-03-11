#pragma once

class Text {
    public:
        Text(const char* font_string, int ptsize, const char* in_string, SDL_Color in_color, int x, int y, bool alignleft=true);
        ~Text();
        void UpdateText(const char* new_text, bool alignleft=true);
        void Draw();
        
        operator const char* () {
            return string;
        }

    private:
        int x;
        int y;

        bool alignleft;

        SDL_Rect text_rect;
        TTF_Font* font;
        const char* string;
        SDL_Color color;
        SDL_Texture* textTexture = nullptr;

};