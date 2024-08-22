#ifndef BUILDING_H
#define BUILDING_H

#include "shape.h"
#include "score.h"



struct Building:public Shape{
    Coord coord;
    int layer;

    std::string name;
    ScoreCond scoreCond;
    std::vector<std::string> tags;

    Building(){}

    Building(Shape* shape, const Coord& coord) : Shape(*shape), coord(coord) {}

    void print(Canvas *canv, Dim dim){
        int x=coord.x*dim.width;
        int y=coord.y*dim.height;
        std::sort(tiles.begin(), tiles.end(), Tile::compareP);
        for(int i=0;i<tiles.size();i++){
            tiles.at(i)->print(canv, {x, y}, dim);
        }
    }

    static bool colision(Building* b1, Building* b2){
        for (int i=0;i<b1->tiles.size();i++){
            for (int j=0;j<b2->tiles.size();j++){
                int x1=b1->tiles.at(i)->coord.x+b1->coord.x;
                int x2=b2->tiles.at(j)->coord.x+b2->coord.x;
                int y1=b1->tiles.at(i)->coord.y+b1->coord.y;
                int y2=b2->tiles.at(j)->coord.y+b2->coord.y;
                if(x1==x2&&y1==y2){

                    return true;
                }
            }
        }
        return false;
    }


    static int buildingDist(Building* b1, Building* b2){
        int distMin=1000;
        for (int i=0;i<b1->tiles.size();i++){
            for (int j=0;j<b2->tiles.size();j++){
                Tile *t1=b1->tiles.at(i);
                Tile *t2=b2->tiles.at(j);
                int dist=abs((t1->coord.x+b1->coord.x)-(t2->coord.x+b2->coord.x))+abs((t1->coord.y+b1->coord.y)-(t2->coord.y+b2->coord.y));


                if(dist<distMin)
                    distMin=dist;
            }
        }
        return distMin;
    }
};

struct BuildingInfo{
    int size;
    std::string name;
    std::vector<std::string> allowedNames;
    ScoreCond scoreCond;
    std::vector<std::string> tags;
};

struct BuildingInfoList{
    std::vector<BuildingInfo> buildingInfos;
    TextureList *textureList;
    int layer;

    void setLayer(int layer){
        this->layer=layer;
    }

    void loadBuildingInfo(std::string file){

        buildingInfos.clear();

        std::string name="";
        int size=0;

        std::ifstream inputFile(file);

        if (!inputFile.is_open()) {
            return;
        }

        std::string line;

        BuildingInfo buildingInfo;

        ProxPoint proxPoint;

        while (std::getline(inputFile, line)) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);


                if (key == "Name"){
                    buildingInfo.allowedNames.clear();
                    buildingInfo.scoreCond.proxPoints.clear();
                    buildingInfo.tags.clear();
                    buildingInfo.scoreCond.quantityPoints=0;
                    buildingInfo.name = value;
                }
                if (key == "Size"){
                    buildingInfo.size =  std::stoi(value);
                }
                if (key == "Texture"){
                    buildingInfo.allowedNames.push_back(value);
                }
                if (key == "CondName"){
                    proxPoint.name=value;
                }
                if (key == "CondProxDist"){
                    proxPoint.proxDist=std::stoi(value);
                }
                if (key == "CondPoints"){
                    proxPoint.points=std::stoi(value);
                }
                if (key == "CondEnd"){
                    buildingInfo.scoreCond.proxPoints.push_back(proxPoint);
                }
                if (key == "Tag"){
                    buildingInfo.tags.push_back(value);
                }
                if (key == "QuantPoints"){
                    buildingInfo.scoreCond.quantityPoints=std::stoi(value);
                }
                if (key == "End"){
                    buildingInfos.push_back(buildingInfo);
                }
                if (key == "Done"){
                    break;
                }
            }
        }

        inputFile.close();
    }

    BuildingInfo* selectBuilding(std::string name){
        for(int i=0;i<buildingInfos.size();i++)
            if(buildingInfos.at(i).name==name)
                return &(buildingInfos.at(i));
        return nullptr;
    }

    BuildingInfo* selectBuilding(int ord){
        if(ord<buildingInfos.size())
            return &(buildingInfos.at(ord));
        else
            return nullptr;
    }

    Building* generateBuilding(int ord){
        if(ord==-1)
            ord=rand()%buildingInfos.size();
        Building* building=new Building();
        building->scoreCond=selectBuilding(ord)->scoreCond;
        building->name=selectBuilding(ord)->name;
        for(int i=0;i<selectBuilding(ord)->tags.size();i++)
            building->tags.push_back(selectBuilding(ord)->tags.at(i));
        building->layer=layer;
        building->coord={0,0};
        int size=selectBuilding(ord)->size;
        for (int i=0;i<size;){
            Coord coord;
            coord.x=rand()%3-1;
            coord.y=rand()%3-1;
            Texture* texture=textureList->selectTexture(selectBuilding(ord)->allowedNames.at(rand()%selectBuilding(ord)->allowedNames.size()));
            if(texture!=0)
                if(building->addTile(coord, texture))
                    i++;
        }
        return building;
    }

    Building* generateBuilding(std::string name){
        Building* building=new Building();
        building->scoreCond=selectBuilding(name)->scoreCond;
        building->name=name;
        for(int i=0;i<selectBuilding(name)->tags.size();i++)
            building->tags.push_back(selectBuilding(name)->tags.at(i));
        building->layer=layer;
        building->coord={0,0};
        int size=selectBuilding(name)->size;
        for (int i=0;i<size;){
            Coord coord;
            coord.x=rand()%3-1;
            coord.y=rand()%3-1;
            Texture* texture=textureList->selectTexture(selectBuilding(name)->allowedNames.at(rand()%selectBuilding(name)->allowedNames.size()));
            if(texture!=0)
                if(building->addTile(coord, texture))
                    i++;
        }
        return building;
    }

};



#endif // BUILDING_H
