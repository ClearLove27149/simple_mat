//
// Created by 26685 on 2023/6/3.
//

#ifndef SIMPLEMAT_POINT_H
#define SIMPLEMAT_POINT_H

#include <iostream>

template<typename T>
class Point {
public:
    T x,y;
    Point() = default;
    Point(T _x, T _y) : x(_x), y(_y) {}
    Point operator+ (const Point& other) {
        Point tmp;
        tmp.x = x + other.x;
        tmp.y = y + other.y;
        return tmp;
    }
};

#endif //SIMPLEMAT_POINT_H
