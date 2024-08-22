#ifndef MAP_H
#define MAP_H

#include "building.h"
#include "canvas.h"

struct Map{
    std::vector<Building*> buildings;
    Dim dim;

    bool addBuilding(Building *building){
        for(int i=0;i<buildings.size();i++)
            if(Building::colision(building, buildings.at(i))){
                return false;
            }
        buildings.push_back(building);
        return true;
    }



    void print(Canvas *canv){

        std::vector<Tile*> tiles;
        for(int i=0;i<buildings.size();i++){
            Building *building=buildings.at(i);
            for(int j=0;j<building->tiles.size();j++){
                Tile *tile=new Tile(building->tiles.at(j));
                tile->coord={tile->coord.x+building->coord.x, tile->coord.y+building->coord.y};
                tiles.push_back(tile);
            }
        }

        std::sort(tiles.begin(), tiles.end(), Tile::compareP);
        for(int i=0;i<tiles.size();i++){
            tiles.at(i)->print(canv, {0, 0}, dim);
        }
    }

    Building* at(int i){
        return buildings.at(i);
    }

    void remove(Building* building){
        for(int i=0;i<buildings.size();i++)
            if(buildings.at(i)==building){
                buildings.erase(buildings.begin()+i);
                return;
            }

    }




};
#endif // MAP_H
