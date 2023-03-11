#pragma once


class Map {
    public:
        Map();
        void Draw(bool replace);
    private:
        SDL_Rect src, dest;
        // int maptiles[20][25];
        int tilecolnum = 0;

        SDL_Texture *platform, *background;
};