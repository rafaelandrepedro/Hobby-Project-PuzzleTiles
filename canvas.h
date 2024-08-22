#ifndef CANVAS_H
#define CANVAS_H

#include "util.h"
#include <QPainter>
#include <iostream>

struct Canvas
{
    QPainter *painter;
    Coord coord;
    Dim dim;
    float zoom;
    Coord offset;

    Coord click(int x, int y){
        //std::cout<<"A "<<x<<" "<<y<<std::endl;
        //point in canvas
        int cx=x-(coord.x+dim.width/2)+offset.x;
        int cy=y-(coord.y+dim.height/2)+offset.y;
        //distance from center of canvas to cursor
        int zx=cx/zoom;
        int zy=cy/zoom;
        //std::cout<<"B "<<zx<<" "<<zy<<std::endl;
        return {zx, zy};
    }

 /*   void print(int x, int y, Texture *texture){

        //std::cout<<"C "<<x<<" "<<y<<std::endl;

        //point in canvas
        int cx=x*zoom+coord.x+dim.width/2-offset.x;
        int cy=y*zoom+coord.y+dim.height/2-offset.y;

        //std::cout<<"D "<<cx<<" "<<cy<<std::endl;

        //distance from center to edges of zoomed image
        int zx=(texture->pmap.width()/2+texture->offX)*zoom;
        int zy=(texture->pmap.height()/2+texture->offY)*zoom;

        //std::cout<<"E "<<zx<<" "<<zy<<std::endl;

        //where to start drawing the zoomed image
        int px=cx-zx;
        int py=cy-zy;

        //std::cout<<"F "<<px<<" "<<py<<std::endl;

        painter->drawLine(coord.x, coord.y, coord.x+dim.width, coord.y);
        painter->drawLine(coord.x, coord.y+dim.height, coord.x+dim.width, coord.y+dim.height);
        painter->drawLine(coord.x, coord.y, coord.x, coord.y+dim.height);
        painter->drawLine(coord.x+dim.width, coord.y, coord.x+dim.width, coord.y+dim.height);

        QSize newSize = texture->pmap.size() * zoom;
        QPixmap pmap = texture->pmap.scaled(newSize, Qt::KeepAspectRatio, Qt::FastTransformation);

        //limits
        int left=px;
        int right=px+pmap.width();
        int up=py;
        int down=py+pmap.height();

        //std::cout<<"G "<<left<<" "<<right<<" "<<up<<" "<<down<<std::endl;


        //cut
        int cleft=0;
        int cright=0;
        int cup=0;
        int cdown=0;

        if(left<coord.x)
            cleft=coord.x-left;

        if(right>coord.x+dim.width)
            cright=right-(coord.x+dim.width);

        if(up<coord.y)
            cup=coord.y-up;

        if(down>coord.y+dim.height)
            cdown=down-(coord.y+dim.height);

        if(cleft+cright>=pmap.width()||cup+cdown>=pmap.height())
            return;

        //std::cout<<"H "<<cleft<<" "<<cright<<" "<<cup<<" "<<cdown<<std::endl;




        QRect cutRect(cleft, cup, -cright+pmap.width(), -cdown+pmap.height());
        pmap = pmap.copy(cutRect);

        painter->drawPixmap(px+cleft, py+cup, pmap);
        //std::cout<<"I "<<cx+cleft<<" "<<cy+cup<<std::endl;


    }
*/
};

#endif // CANVAS_H
