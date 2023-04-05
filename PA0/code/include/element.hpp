#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>

#include <stack>
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
                // printf("filled (%d,%d)",x,y);
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
                img.SetPixel(x, y, color);
                x = x+1, e += 2*dy;
                if (e>=0)
                {
                    y--, e-= 2*dx;
                }
            }
            break;
        case 2:
            k = 1/k;
            dy = -dy;
            e = -dy;
            for(int i =0; i<=dy; i++)
            {
                img.SetPixel(x, y, color);
                y = y-1, e += 2*dx;
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
                y = y-1, e += 2*dx;
                if (e>=0)
                {
                    x--, e-= 2*dy;
                }
            }
            break;  
        case 4:
            dx = -dx; dy = -dy;
            e = -dx;
            for(int i =0; i<=dx; i++)
            {
                img.SetPixel(x, y, color);
                x = x-1, e += 2*dy;
                if (e>=0)
                {
                    y--, e-= 2*dx;
                }
            }
            break;
        case 5:
            dx = -dx;
            e = -dx;
            for(int i =0; i<=dx; i++)
            {
                img.SetPixel(x, y, color);
                // printf("filled (%d,%d)",x,y);
                x = x-1, e += 2*dy;
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
                img.SetPixel(x, y, color);
                y = y+1, e += 2*dx;
                if (e>=0)
                {
                    x--, e-= 2*dy;
                }
            }
            break;
        case 7:
            k = 1/k;
            e = -dy;
            for(int i =0; i<=dy; i++)
            {
                img.SetPixel(x, y, color);
                // printf("filled (%d,%d) \n",x,y);
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
        // TODO: Mid-point Algorithm
        float d;
        int x = 0, y = radius; int e = 5-4*radius;
        // 8-way symmetry
        while (x <= y)
        {
            img.SetPixel(cx + x, cy + y, color);
            img.SetPixel(cx + x, cy - y, color);
            img.SetPixel(cx - x, cy + y, color);
            img.SetPixel(cx - x, cy - y, color);
            img.SetPixel(cx + y, cy + x, color);
            img.SetPixel(cx + y, cy - x, color);
            img.SetPixel(cx - y, cy + x, color);
            img.SetPixel(cx - y, cy - x, color);
            if (e < 0)
            {
                e += 8 * x + 12;
            }
            else
            {
                e += 8 * (x - y) + 20;
                y--;
            }
            x++;
        }
    }
};


class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override {
        // TODO: Flood fill 非递归版本
        // 种子点 ： std::tuple<int,int> Seed(cx,cy);
        //STEP1 : initialization
        Vector3f oldcolor = img.GetPixel(cx,cy);
        std::tuple<int,int> Seed(cx,cy);
        std::stack< std::tuple<int,int> > Seeds;
        Seeds.push(Seed);
        //STEP2 : FILL
        while(!Seeds.empty())
        {
            std::tuple<int,int> pt = Seeds.top(); Seeds.pop();
            int x = std::get<0>(pt);
            int y = std::get<1>(pt);
            //STEP2_0 : FILL THIS SCANLINE
            while(img.GetPixel(x,y) == oldcolor && 0<=x && x< img.Width())
            {//right side
                img.SetPixel(x,y,color);
                x++;
            }
            int xr = x-1;
            x = std::get<0>(pt)-1;
            while(img.GetPixel(x,y) == oldcolor && 0<=x && x< img.Width())
            {// left side
                img.SetPixel(x,y,color);
                x--;
            }
            int xl = x+1;
            //STEP2_1 : FILL UPPER SCANLINE
            x = xl;
            y += 1;
            if (0<=y && y<img.Height()){
                while( x <= xr)
                {
                    bool needFill = false;
                    while(img.GetPixel(x,y) == oldcolor && x < img.Width())
                    {
                        needFill = true;
                        x++;
                    }
                    if(needFill)
                    {
                        std::get<0>(Seed) = x-1;
                        std::get<1>(Seed) = y;
                        Seeds.push(Seed);
                        // needFill = false; // 没必要？
                    } else {
                        //do nothing!
                    }
                    while(img.GetPixel(x,y)!= oldcolor && x <=xr) x++;
                }
            } else{
                //do nothing!
            }

            //STEP2_2 : FILL LOWER SCANLINE
            x = xl;
            y -= 2;
            if(0<=y && y<img.Height()){
                while(x<=xr)
                {
                    bool needFill = false;
                    while(img.GetPixel(x,y) == oldcolor && x < img.Width())
                    {
                        needFill = true;
                        x++;
                    }
                    if (needFill)
                    {
                        std::get<0>(Seed) = x-1;
                        std::get<1>(Seed) = y;
                        Seeds.push(Seed);
                        // needFill = false; // 没必要？
                    }
                    while(img.GetPixel(x,y)!= oldcolor && x <=xr) x++;
                }
            } else {
                //do nothing!
            }
        }
    }
};