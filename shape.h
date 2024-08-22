#ifndef SHAPE_H
#define SHAPE_H

#include "tile.h"
#include "texture.h"


struct Shape{
    std::vector<Tile*> tiles;

    bool addTile(Coord coord, Texture *texture){
        for (int i=0;i<tiles.size();i++)
            if(coord.x==tiles.at(i)->coord.x&&coord.y==tiles.at(i)->coord.y)
                return false;

        Tile *tile= new Tile(coord, texture);

        bool connect=false;
        if(tiles.size()==0){
            tile->coord={0,0};
            connect=true;
        }
        for (int i=0;i<tiles.size();i++){
            if(Tile::connected(tiles.at(i), tile)){
                connect=true;
            }
        }

        if(connect)
            tiles.push_back(tile);
        else
            return false;
        return true;
    }

    void print(Canvas *canv, Dim dim, Coord coord){
        std::sort(tiles.begin(), tiles.end(), Tile::compareP);
        for(int i=0;i<tiles.size();i++){
            tiles.at(i)->print(canv, coord, dim);
        }
    }

    void rotate(){
        for(int i=0;i<tiles.size();i++){
            int x=tiles.at(i)->coord.x;
            int y=tiles.at(i)->coord.y;
            tiles.at(i)->coord.x=y;
            tiles.at(i)->coord.y=-x;
        }
    }

};

#endif // SHAPE_H
