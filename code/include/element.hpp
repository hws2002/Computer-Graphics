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
            printf("---------------BEGIN CASE 0 LINE-----------------------\n");
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
            printf("---------------END CASE 0 LINE-----------------------\n");

            break;
        case 1:
            // dy = -dy;
            // for(int i =0; i<=dx; i++)
            // {
            //     img.SetPixel(x, -y, color);
            //     x = x+1, e += 2*dy;
            //     if (e>=0)
            //     {
            //         y++, e-= 2*dx;
            //     }
            // }
            break;
        case 2:
            // k = 1/k;
            // dy = -dy;
            // e = -dy;
            // for(int i =0; i<=dy; i++)
            // {
            //     img.SetPixel(x, -y, color);
            //     y = y+1, e += 2*dx;
            //     if (e>=0)
            //     {
            //         x++, e-= 2*dy;
            //     }
            // }
            break;
        case 3:
            // k = 1/k;
            // dx = -dx;
            // dy = -dy;
            // e = -dy;
            // for(int i =0; i<=dy; i++)
            // {
            //     img.SetPixel(-x, -y, color);
            //     y = y+1, e += 2*dx;
            //     if (e>=0)
            //     {
            //         x++, e-= 2*dy;
            //     }
            // }
            break;  
        case 4:
            // dx = -dx; dy = -dy;
            // e = -dx;
            // for(int i =0; i<=dx; i++)
            // {
            //     img.SetPixel(-x, -y, color);
            //     x = x+1, e += 2*dy;
            //     if (e>=0)
            //     {
            //         y++, e-= 2*dx;
            //     }
            // }
            break;
        case 5:
            printf("---------------BEGIN CASE 5 LINE-----------------------\n");
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
            printf("---------------END CASE 5 LINE-----------------------\n");
            break;  
        case 6:
            // k = 1/k;
            // dx = -dx;
            // e = -dy;
            // for(int i =0; i<=dy; i++)
            // {
            //     img.SetPixel(-x, y, color);
            //     y = y+1, e += 2*dx;
            //     if (e>=0)
            //     {
            //         x++, e-= 2*dy;
            //     }
            // }
            break;
        case 7:
            printf("---------------BEGIN CASE 7 LINE-----------------------\n");
            k = 1/k;
            e = -dy;
            for(int i =0; i<=dy; i++)
            {
                img.SetPixel(x, y, color);
                printf("filled (%d,%d) \n",x,y);
                y = y+1, e += 2*dx;
                if (e>=0)
                {
                    x++, e-= 2*dy;
                }
            }
            printf("---------------BEGIN CASE 7 LINE-----------------------\n");
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
        // TODO: Flood fill
        // // TODO :SCANLINE FILL
        // //STEP1 : initialization
        // // set oldcolor
        // Vector3f oldcolor = img.GetPixel(cx,cy);
        // int xl,xr, i;
        // bool spanNeedFill;
        // std::tuple<int,int> seed(cx,cy);
        // std::tuple<int,int> pt(cx,cy);
        // std::stack< std::tuple<int,int> > Seeds;
        // Seeds.push(seed);
        // //STEP2 : FILL
        // //STEP2_0 : FILL this scanline
        // int x,y;
        // while(!Seeds.empty())
        // {
        //     pt = Seeds.top();
        //     Seeds.pop();
        //     x = std::get<0>(pt);
        //     y = std::get<1>(pt);
        //     while(img.GetPixel(x,y) == oldcolor)
        //     {// right side
        //         img.SetPixel(x,y,color);
        //         x++;
        //     }
        //     xr = x-1;
        //     x = std::get<0>(pt) - 1;
        //     while(img.GetPixel(x,y) == oldcolor)
        //     {// left side
        //         img.SetPixel(x,y,color);
        //         x--;
        //     }
        //     xl = x+1;
        //     //STEP2_1 : FILL UPPER scanline
        //     x = xl;
        //     y = y+1;
        //     while(x <=xr)
        //     {
        //         spanNeedFill = false;
        //         while(img.GetPixel(x,y) == oldcolor)
        //         {
        //             spanNeedFill = true;
        //             x++;
        //         }
        //         if (spanNeedFill)
        //         {
        //             std::get<0>(pt) = x-1; 
        //             std::get<1>(pt) = y;
        //             Seeds.push(pt);
        //             spanNeedFill = false;
        //         }
        //         while (img.GetPixel(x,y)!=oldcolor && x<=xr) x++;
        //     }
        //     //STEP2_2 : FILL LOWER scanline
        //     x = xl;
        //     y = y-2;
        //     while(x<=xr)
        //     {
        //         spanNeedFill = false;
        //         while(img.GetPixel(x,y) == oldcolor)
        //         {
        //             spanNeedFill = true;
        //             x++;
        //         }
        //         if (spanNeedFill)
        //         {
        //             std::get<0>(pt) = x-1;
        //             std::get<0>(pt) = y;
        //             Seeds.push(pt);
        //             spanNeedFill = false;
        //         }
        //         while(img.GetPixel(x,y)!=oldcolor && x<=xr) x++;
        //     }
        // }
    }
};