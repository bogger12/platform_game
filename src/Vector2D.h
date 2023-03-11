#pragma once
#include "utils.cpp"


class Vector2D {
    public:
        float x;
        float y;

        Vector2D(float x=0, float y=0) { this->x = x; this->y = y; }

        operator const char* () {
            const char* orig = "Vector2D(%g, %g)";
            return formatstring(orig, x, y);
        }
        operator std::string () {
            return std::string(operator const char* ());
        }
        Vector2D vround () {
            return Vector2D(round(x),round(y));
        }
        Vector2D vceil () {
            return Vector2D(ceil(x),ceil(y));
        }
        Vector2D absv () {
            return Vector2D(abs(x),abs(y));
        }
        Vector2D bin () {
            Vector2D result;
            result.x = (x > 0) ? 1 : (x < 0) ? -1 : 0;
            result.y = (y > 0) ? 1 : (y < 0) ? -1 : 0;
            return result;
        }
        bool isnotzero () {
            return (x != 0 || y != 0);
        }

        Vector2D invbin () {
            return Vector2D(!x, !y);
        }


        Vector2D operator + (const Vector2D &vector){
            Vector2D result;
            result.x = x + vector.x;
            result.y = y + vector.y;
            return result;
        }
        Vector2D operator - (const Vector2D &vector){
            Vector2D result;
            result.x = x - vector.x;
            result.y = y - vector.y;
            return result;
        }
        Vector2D operator * (const Vector2D &vector){
            Vector2D result;
            result.x = x * vector.x;
            result.y = y * vector.y;
            return result;
        }
        Vector2D operator / (const Vector2D &vector){
            Vector2D result;
            result.x = x / vector.x;
            result.y = y / vector.y;
            return result;
        }

        Vector2D operator + (const double &num){
            Vector2D result;
            result.x = x + num;
            result.y = y + num;
            return result;
        }
        Vector2D operator - (const double &num){
            Vector2D result;
            result.x = x - num;
            result.y = y - num;
            return result;
        }
        Vector2D operator * (const double &num){
            Vector2D result;
            result.x = x * num;
            result.y = y * num;
            return result;
        }
        Vector2D operator / (const double &num){
            Vector2D result;
            result.x = x / num;
            result.y = y / num;
            return result;
        }

        void operator += (const Vector2D &vector){
            x += vector.x;
            y += vector.y;
        }
        void operator -= (const Vector2D &vector){
            x -= vector.x;
            y -= vector.y;
        }
        void operator *= (const Vector2D &vector){
            x *= vector.x;
            y *= vector.y;
        }
        void operator /= (const Vector2D &vector){
            x /= vector.x;
            y /= vector.y;
        }

        bool operator == (const Vector2D &vector) {
            return (x == vector.x && y == vector.y);
        }
        Vector2D operator > (const Vector2D &vector) {
            Vector2D result;
            result.x = abs(x) > abs(vector.x);
            result.y = abs(y) > abs(vector.y);
            return result;
        }
        Vector2D operator < (const Vector2D &vector) {
            Vector2D result;
            result.x = abs(x) < abs(vector.x);
            result.y = abs(y) < abs(vector.y);
            return result;
        }
        Vector2D operator >= (const Vector2D &vector) {
            Vector2D result;
            result.x = abs(x) >= abs(vector.x);
            result.y = abs(y) >= abs(vector.y);
            return result;
        }
        Vector2D operator <= (const Vector2D &vector) {
            Vector2D result;
            result.x = abs(x) <= abs(vector.x);
            result.y = abs(y) <= abs(vector.y);
            return result;
        }

};