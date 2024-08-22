#ifndef TILE_H
#define TILE_H

#include "texture.h"
#include "canvas.h"
#include "util.h"

struct Tile{
    Coord coord;
    Ori ori;
    Texture* textureUp;
    Texture* textureDown;
    Texture* textureLeft;
    Texture* textureRight;

    Tile(Tile *tile){
        this->coord=tile->coord;
        this->ori=tile->ori;
        this->textureUp=tile->textureUp;
        this->textureDown=tile->textureDown;
        this->textureLeft=tile->textureLeft;
        this->textureRight=tile->textureRight;
    }


    Tile(Coord coord, Texture *texture){
        this->coord=coord;
        this->ori=Ori::UP;
        this->textureUp=texture;
        this->textureDown=texture;
        this->textureLeft=texture;
        this->textureRight=texture;
    }

    Tile(Coord coord, Ori ori, Texture *texture){
        this->coord=coord;
        this->ori=ori;
        this->textureUp=texture;
        this->textureDown=texture;
        this->textureLeft=texture;
        this->textureRight=texture;
    }

    Tile(Coord coord, Texture *textureVer, Texture *textureHor){
        this->coord=coord;
        this->ori=Ori::UP;
        this->textureUp=textureVer;
        this->textureDown=textureVer;
        this->textureLeft=textureHor;
        this->textureRight=textureHor;
    }

    Tile(Coord coord, Ori ori, Texture *textureVer, Texture *textureHor){
        this->coord=coord;
        this->ori=ori;
        this->textureUp=textureVer;
        this->textureDown=textureVer;
        this->textureLeft=textureHor;
        this->textureRight=textureHor;
    }

    Tile(Coord coord, Texture *textureUp, Texture *textureDown, Texture *textureLeft, Texture *textureRight){
        this->coord=coord;
        this->ori=Ori::UP;
        this->textureUp=textureUp;
        this->textureDown=textureDown;
        this->textureLeft=textureLeft;
        this->textureRight=textureRight;
    }

    Tile(Coord coord, Ori ori, Texture *textureUp, Texture *textureDown, Texture *textureLeft, Texture *textureRight){
        this->coord=coord;
        this->ori=ori;
        this->textureUp=textureUp;
        this->textureDown=textureDown;
        this->textureLeft=textureLeft;
        this->textureRight=textureRight;
    }

    void print(Canvas *canv, Coord coord, Dim dim){
        int x=(this->coord.x)*dim.width+coord.x;
        int y=(this->coord.y)*dim.height+coord.y;

        switch(ori){
        case UP:
            textureUp->print(canv, x, y);
            break;
        case DOWN:
            textureDown->print(canv, x, y);
            break;
        case LEFT:
            textureLeft->print(canv, x, y);
            break;
        case RIGHT:
            textureRight->print(canv, x, y);
            break;
        }

    }

    static bool connected(Tile* t1, Tile* t2){
        int x1=t1->coord.x;
        int x2=t2->coord.x;
        int y1=t1->coord.y;
        int y2=t2->coord.y;
        if((abs(x1-x2)==1&&abs(y1-y2)==0)||(abs(x1-x2)==0&&abs(y1-y2)==1))
            return true;
        return false;
    }

    static bool compare(const Tile& t1, const Tile& t2) {
        return Coord::compare(t1.coord, t2.coord);
    }

    static bool compareP(const Tile *t1, const Tile *t2) {
        return Coord::compare(t1->coord, t2->coord);
    }
};
#endif // TILE_H
