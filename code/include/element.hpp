#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>

class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Bresenham Algorithm
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
                color.x(), color.y(), color.z());
        // RECOMMEND : Check the direction of canvas FIRST!!!
        // STEP -1 : HANDLE SPECIAL CASES
        if (xA == xB)
        {
            if(yA > yB)
            {
                std::swap(yA, yB);
            }
            for(int i=yA; i<=yB; i++)
            {
                img.SetPixel(xA, i, color);
            }
            return ;
        }
        if (yA == yB)
        {
            if (xA > xB)
            {
                std::swap(xA, xB);
            }
            for(int i=xA; i<=xB; i++)
            {
                img.SetPixel(i, yA, color);
            }
            return ;
        }
        // STEP 0 : Prepare Variables
        int dx = xB - xA, dy = yB - yA;
        float k = abs((float)dy/(float)dx);
        // e' = 2*e*dx, e = d - 0.5;
        int e = -dx;
        int x = xA, y = yA;
        // STEP 1 : Determine the position of xB,yB Compared to xA, yA
        int CASE = 0;
        if (xB > xA && yB > yA && k <=1){
            CASE = 0;
        } else if (xB > xA && yB < yA && k <= 1){
            CASE = 1;
        } else if (xB > xA && yB < yA && k > 1){
            CASE = 2;
        } else if (xB < xA && yB < yA && k > 1){
            CASE = 3;
        } else if (xB < xA && yB < yA && k <= 1){
            CASE = 4;
        } else if (xB < xA && yB > yA && k <= 1){
            CASE = 5;
        } else if (xB < xA && yB > yA && k > 1){
            CASE = 6;
        } else if (xB > xA && yB > yA && k > 1){
            CASE = 7;
        }
        // STEP2 : different position, different SetPixel!
        switch (CASE)
        {
        case 0:
            for(int i =0; i<=dx; i++)
            {
                img.SetPixel(x, y, color);
                x = x+1, e += 2*dy;
                if (e>=0)
                {
                    y++, e-= 2*dx;
                }
                //else do nothing!
            }
            break;
        case 1:
            dy = -dy;
            for(int i =0; i<=dx; i++)
            {
                img.SetPixel(x, -y, color);
                x = x+1, e += 2*dy;
                if (e>=0)
                {
                    y++, e-= 2*dx;
                }
            }
            break;
        case 2:
            k = 1/k;
            dy = -dy;
            e = -dy;
            for(int i =0; i<=dy; i++)
            {
                img.SetPixel(x, -y, color);
                y = y+1, e += 2*dx;
                if (e>=0)
                {
                    x++, e-= 2*dy;
                }
            }
            break;
        case 3:
            k = 1/k;
            dx = -dx;
            dy = -dy;
            e = -dy;
            for(int i =0; i<=dy; i++)
            {
                img.SetPixel(x, y, color);
                y = y+1, e += 2*dx;
                if (e>=0)
                {
                    x++, e-= 2*dy;
                }
            }
            break;  
        case 4:
            dx = -dx; dy = -dy;
            e = -dx;
            for(int i =0; i<=dx; i++)
            {
                img.SetPixel(-x, -y, color);
                x = x+1, e += 2*dy;
                if (e>=0)
                {
                    y++, e-= 2*dx;
                }
            }
            break;
        case 5:
            dx = -dx;
            e = -dx;
            for(int i =0; i<=dx; i++)
            {
                img.SetPixel(-x, y, color);
                x = x+1, e += 2*dy;
                if (e>=0)
                {
                    y++, e-= 2*dx;
                }
            }
            break;  
        case 6:
            k = 1/k;
            dx = -dx;
            e = -dy;
            for(int i =0; i<=dy; i++)
            {
                img.SetPixel(-x, y, color);
                y = y+1, e += 2*dx;
                if (e>=0)
                {
                    x++, e-= 2*dy;
                }
            }
            break;
        case 7:
            k = 1/k;
            e = -dy;
            for(int i =0; i<=dy; i++)
            {
                img.SetPixel(x, y, color);
                y = y+1, e += 2*dx;
                if (e>=0)
                {
                    x++, e-= 2*dy;
                }
            }
            break;
        default:
            break;
        }
    }
};

class Circle : public Element {

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Implement Algorithm to draw a Circle
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Flood fill
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
    }
};