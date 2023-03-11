#pragma once
#include <stdlib.h>
#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>

const char* formatstring(const char* formatstring, Args ... args) {

    int size_s = std::snprintf( nullptr, 0, formatstring, args ... )+1;
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    char* buf = new char[ size_s ];
    std::snprintf( buf, size_s, formatstring, args ... );

    return buf;
}

SDL_Rect makeSDL_Rect(int x, int y, int w, int h) {
    SDL_Rect result;
    result.x = x;
    result.y = y;
    result.w = w;
    result.h = h;
    return result;
}