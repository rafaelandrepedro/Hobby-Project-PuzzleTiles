#ifndef GAME_H
#define GAME_H

#include <QMouseEvent>
#include "map.h"
#include "canvas.h"
#include "texture.h"
#define MAP_LAYERS 3
#define GAME_WINDOW_WIDTH 960-40
#define GAME_WINDOW_HEIGHT 540-40

#define TILE_WIDTH 64
#define TILE_HEIGHT 48




struct Game{
    Map map[MAP_LAYERS];                    //map layers
    TextureList tl[MAP_LAYERS-1];           //textures of each layer
    BuildingInfoList bil[MAP_LAYERS-1];     //infor of buildings of each layer

    TextureList anitl;                      //animations and effects

    Coord cursor;                           //cursor in the canvas
    Coord cursorMap;                        //cursor in the map (TILE_WIDTH x TILE_HEIGHT)

    Canvas canv;                            //canvas


    const std::string objectives[2]={       //objectives
        "Build 20 Terrain and 5 Water",
        "Build 5 Structure"
    };

    int objective;                          //number of the objective

    int tick=0;                             //game tick

    AnimatedTexture atex;                   //animation of cursor tile

    int appNumber;

    Building *options[3];


    void runTick(){
        static int lastObjective=0;

        tick++;

        if(appNumber==0){
            canv.offset={0, 0};
            canv.zoom=2;
        }

        objective=checkObjective(objective);

        if(lastObjective!=objective){
            bil[0].loadBuildingInfo("data/Tiles/Terrain"+std::to_string(objective)+".txt");
            bil[1].loadBuildingInfo("data/Tiles/structures"+std::to_string(objective)+".txt");
        }


        lastObjective=objective;
    }

    Game(){
        objective=0;
        appNumber=0;

        for(int i=0;i<MAP_LAYERS;i++){
            map[i].dim={TILE_WIDTH,TILE_HEIGHT};
            bil[i].setLayer(i);
        }

        tl[0].loadTextures("data/textures/Terrain.txt");
        bil[0].loadBuildingInfo("data/Tiles/Terrain0.txt");
        bil[0].textureList=&tl[0];


        tl[1].loadTextures("data/textures/structures.txt");
        bil[1].loadBuildingInfo("data/Tiles/structures0.txt");
        bil[1].textureList=&tl[1];


        /*
        tl[2].loadTextures("data/textures/weather.txt");
        bil[2].loadBuildingInfo("data/Tiles/weather.txt");
        bil[2].textureList=&tl[2];
*/

        anitl.loadTextures("data/textures/Animation.txt");

        AnimatedTextureFrame atf;
        for(int i=0;i<40;i++){
            atf.texture=anitl.selectTexture(i/10);
            atf.coord={0,0};
            atex.addFrame(atf);
        }





        canv.coord={20,20};
        canv.dim={GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT};
        canv.offset={0,0};
        canv.zoom=1;


        for(int i=0;i<3;i++){
            options[i]=bil[rand()%(MAP_LAYERS-1)].generateBuilding(-1);
            options[i]->coord={(i-1)*2, 0};
        }

        //Building *b=bil[rand()%(MAP_LAYERS-1)].generateBuilding(-1);

        //map[MAP_LAYERS-1].addBuilding(b);
    }


    void mouseEvent(QMouseEvent* event){

        static Coord offsetStartPoint={0,0};
        static Coord offsetOriginalPoint={0,0};

        int x, y;
        x = event->pos().x();
        y = event->pos().y();

        Coord coord = canv.click(x,y);

        int coordX=std::floor(((float)(coord.x))/TILE_WIDTH+0.5);
        int coordY=std::floor(((float)(coord.y))/TILE_HEIGHT+0.5);

        cursor={coord.x, coord.y};
        cursorMap={coordX, coordY};

        Qt::MouseButton button=event->button();

        QEvent::Type cursorEventType=event->type();

        switch(cursorEventType){

        case QEvent::MouseButtonPress:
            if(appNumber==0){
                for(int i=0;i<3;i++)
                    if(cursorMap.x==options[i]->coord.x&&cursorMap.y==options[i]->coord.y){
                        map[MAP_LAYERS-1].addBuilding(options[i]);
                        canv.zoom=1;
                        appNumber=1;
                    }
                break;
            }
            if(appNumber==1){
                if (button == Qt::MiddleButton){
                    map[MAP_LAYERS-1].at(0)->rotate();
                }
                if (button == Qt::LeftButton){

                    if(layerTransfer(map[MAP_LAYERS-1].at(0), map[MAP_LAYERS-1].at(0)->layer, MAP_LAYERS-1)){
                        play("card3.mp3");
                        for(int i=0;i<3;i++){
                            options[i]=bil[rand()%(MAP_LAYERS-1)].generateBuilding(-1);
                            options[i]->coord={(i-1)*2, 0};
                        }
                        appNumber=0;
                    }
                    else{
                        play("card1.mp3");
                    }
                }
                if (button == Qt::RightButton){
                    map[MAP_LAYERS-1].remove(map[MAP_LAYERS-1].at(0));
                    Building *b=bil[rand()%(MAP_LAYERS-1)].generateBuilding(-1);
                    map[MAP_LAYERS-1].addBuilding(b);
                    play("card1.mp3");
                }
                if (button == Qt::XButton1){
                    offsetStartPoint = canv.click(x,y);
                    offsetOriginalPoint = canv.offset;
                }

                cursorEventType=QEvent::MouseMove;
                //dont break;
            }



        case QEvent::MouseButtonRelease:
            if(appNumber==0){
                break;
            }
            if(appNumber==1){
                if (button == Qt::XButton2){
                    canv.offset={0, 0};
                }

                cursorEventType=QEvent::MouseMove;
                //dont break;
            }



        case QEvent::MouseMove:
            if(appNumber==0){
                break;
            }
            if(appNumber==1){
                moveBuilding(MAP_LAYERS-1, 0, cursorMap.x, cursorMap.y);

                if(event->buttons() & Qt::XButton1){
                    std::cout<<canv.offset.x<<" "<<canv.offset.y<<std::endl;
                    canv.offset={
                        offsetOriginalPoint.x-(canv.click(x,y).x-offsetStartPoint.x),
                        offsetOriginalPoint.y-(canv.click(x,y).y-offsetStartPoint.y)};
                    offsetStartPoint = canv.click(x,y);
                    offsetOriginalPoint = canv.offset;
                }
                break;
            }

        default:
            break;
        }
    }

    void wheelEvent(QWheelEvent* event){
        if(appNumber==0){

        }
        if(appNumber==1){
            int delta=event->angleDelta().y();
            if(delta>0)
                canv.zoom*=2;
            else
                canv.zoom/=2;
        }
    }



    void moveBuilding(int layer, int i, int x, int y){
        map[layer].buildings.at(i)->coord={x, y};
    }

    bool layerTransfer(Building* building, int newLayer, int oldLayer){
        if(map[newLayer].addBuilding(building)){
            map[oldLayer].remove(building);
            return 1;
        }
        else{
            return 0;
        }
    }

    int gameScore(){
        int score=0;

        for(int i=0;i<MAP_LAYERS-1;i++){                                          //check all layers 1
            std::vector<Building*> *bs1 = &(map[i].buildings);              //list of buildings 1
            for(int j=0;j<bs1->size();j++){                                     //check all buildings 1
                Building* b1=bs1->at(j);                                    //building 1

                for(int k=0;k<MAP_LAYERS-1;k++){                                  //check all layers 2
                    std::vector<Building*> *bs2 = &(map[k].buildings);      //list of buildings 2
                    for(int l=0;l<bs2->size();l++){                             //check all buildings 2
                        Building* b2=bs2->at(l);                            //building 2

                        if(b1->name==b2->name&&b1!=b2)
                            score+=b1->scoreCond.quantityPoints;

                        for(int h=0;h<b1->scoreCond.proxPoints.size();h++){      //check all conditions 1
                            ProxPoint pp=b1->scoreCond.proxPoints.at(h);    //condition 1
                            if(pp.name==b2->name)
                                if(Building::buildingDist(b1, b2)<=pp.proxDist)
                                    score+=pp.points;
                            for(int g=0;g<b2->tags.size();g++){
                                if(pp.name==b2->tags.at(g))
                                    if(Building::buildingDist(b1, b2)<=pp.proxDist)
                                        score+=pp.points;
                            }
                        }



                    }
                }
            }
        }
        return score;
    }

    int PossibleScore(){
        int score=0;

        for(int i=0;i<MAP_LAYERS;i++){                                          //check all layers 1
            std::vector<Building*> *bs1 = &(map[i].buildings);              //list of buildings 1
            for(int j=0;j<bs1->size();j++){                                     //check all buildings 1
                Building* b1=bs1->at(j);                                    //building 1

                for(int k=0;k<MAP_LAYERS;k++){                                  //check all layers 2
                    std::vector<Building*> *bs2 = &(map[k].buildings);      //list of buildings 2
                    for(int l=0;l<bs2->size();l++){                             //check all buildings 2
                        Building* b2=bs2->at(l);                            //building 2

                        if(b1->name==b2->name&&b1!=b2)
                            score+=b1->scoreCond.quantityPoints;

                        for(int h=0;h<b1->scoreCond.proxPoints.size();h++){      //check all conditions 1
                            ProxPoint pp=b1->scoreCond.proxPoints.at(h);    //condition 1
                            if(pp.name==b2->name)
                                if(Building::buildingDist(b1, b2)<=pp.proxDist)
                                    score+=pp.points;
                            for(int g=0;g<b2->tags.size();g++){
                                if(pp.name==b2->tags.at(g))
                                    if(Building::buildingDist(b1, b2)<=pp.proxDist)
                                        score+=pp.points;
                            }
                        }



                    }
                }
            }
        }
        return score;
    }

    int NumTags(std::string name){
        int count=0;
        for(int i=0;i<MAP_LAYERS-1;i++){
            std::vector<Building*> *bs = &(map[i].buildings);
            for(int j=0;j<bs->size();j++){
                if(name==bs->at(j)->name)
                    count++;
                for(int k=0;k<bs->at(j)->tags.size();k++)
                    if(name==bs->at(j)->tags.at(k))
                        count++;
            }
        }
        return count;
    }

    int checkObjective(int i){
        switch(i){
        case 0:
            if(NumTags("Terrain")>=20&&NumTags("Terrain")>=5)
                i++;
            break;
        case 1:
            if(NumTags("Structure")>=5)
                i++;
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        }
        return i;
    }



    void paint(QPainter &painter){
        canv.painter=&painter;
        if(appNumber==0){
            Dim dim={TILE_WIDTH, TILE_HEIGHT};
            for(int i=0;i<3;i++)
                options[i]->print(&canv, dim);
            optionInfo(painter);
        }
        if(appNumber==1){
            for(int i=0;i<MAP_LAYERS;i++)
                map[i].print(&canv);

            painter.setPen(Qt::black);
            painter.setFont(QFont("Arial", 12));


            QString str="Score: "+QString::number(gameScore())+" Possible score: "+QString::number(PossibleScore());
            painter.drawText(20, 18, str);

            str="Objetive: "+QString::fromStdString(objectives[objective]);
            painter.drawText(300, 18, str);

            if(PossibleScore()-gameScore()>100)
                play("epic.mp3");

            buildingInfo(painter);
            buildingInfo(painter, 0);


            int x=(cursorMap.x)*TILE_WIDTH;
            int y=(cursorMap.y)*TILE_HEIGHT;

            atex.print(&canv, x, y, tick);
        }

    }

    void optionInfo(QPainter &painter){
        QString str;
        for (int h=0;h<3;h++){

            int line=1;
            //Name
            str="Name: "+QString::fromStdString(options[h]->name);
            painter.drawText(20+h*300, GAME_WINDOW_HEIGHT+18+line*20, str);
            line++;

            //tags
            str="Tags: ";
            for(int i=0;i<options[h]->tags.size();i++){
                str+=QString::fromStdString(options[h]->tags.at(i));
            }
            painter.drawText(20+h*300, GAME_WINDOW_HEIGHT+18+line*20, str);
            line++;

            //Points same building
            if(options[h]->scoreCond.quantityPoints){
                str="Points for other "+QString::fromStdString(options[h]->name)+": "+QString::number(options[h]->scoreCond.quantityPoints);
                painter.drawText(20+h*300, GAME_WINDOW_HEIGHT+18+line*20, str);
                line++;
            }

            for(int i=0;i<options[h]->scoreCond.proxPoints.size();i++){
                int points=options[h]->scoreCond.proxPoints.at(i).points;
                int proxDist=options[h]->scoreCond.proxPoints.at(i).proxDist;
                std::string name=options[h]->scoreCond.proxPoints.at(i).name;
                str="Points for every "+QString::fromStdString(name)+" "+QString::number(proxDist)+" tiles away: "+QString::number(points);
                painter.drawText(20+h*300, GAME_WINDOW_HEIGHT+18+line*20, str);
                line++;
            }
        }
    }

    void buildingInfo(QPainter &painter){
        QString str;
        int line=1;
        //Name
        str="Name: "+QString::fromStdString(map[MAP_LAYERS-1].buildings.at(0)->name);
        painter.drawText(20, GAME_WINDOW_HEIGHT+18+line*20, str);
        line++;

        //tags
        str="Tags: ";
        for(int i=0;i<map[MAP_LAYERS-1].buildings.at(0)->tags.size();i++){
            str+=QString::fromStdString(map[MAP_LAYERS-1].buildings.at(0)->tags.at(i));
        }
        painter.drawText(20, GAME_WINDOW_HEIGHT+18+line*20, str);
        line++;

        //Points same building
        if(map[MAP_LAYERS-1].buildings.at(0)->scoreCond.quantityPoints){
            str="Points for other "+QString::fromStdString(map[MAP_LAYERS-1].buildings.at(0)->name)+": "+QString::number(map[MAP_LAYERS-1].buildings.at(0)->scoreCond.quantityPoints);
            painter.drawText(20, GAME_WINDOW_HEIGHT+18+line*20, str);
            line++;
        }

        for(int i=0;i<map[MAP_LAYERS-1].buildings.at(0)->scoreCond.proxPoints.size();i++){
            int points=map[MAP_LAYERS-1].buildings.at(0)->scoreCond.proxPoints.at(i).points;
            int proxDist=map[MAP_LAYERS-1].buildings.at(0)->scoreCond.proxPoints.at(i).proxDist;
            std::string name=map[MAP_LAYERS-1].buildings.at(0)->scoreCond.proxPoints.at(i).name;
            str="Points for every "+QString::fromStdString(name)+" "+QString::number(proxDist)+" tiles away: "+QString::number(points);
            painter.drawText(20, GAME_WINDOW_HEIGHT+18+line*20, str);
            line++;
        }
    }

    void buildingInfo(QPainter &painter, int layer){
        int COLUMN=300;
        int ord=-1;
        for(int i=0;i<map[layer].buildings.size();i++)
            for(int j=0;j<map[layer].buildings.at(i)->tiles.size();j++){
                int x=map[layer].buildings.at(i)->tiles.at(j)->coord.x+map[layer].buildings.at(i)->coord.x;
                int y=map[layer].buildings.at(i)->tiles.at(j)->coord.y+map[layer].buildings.at(i)->coord.y;
                if(x==cursor.x&&y==cursor.y){
                    ord=i;
                    break;
                }
            }
        if(ord==-1)
            return;

        QString str;
        int line=1;
        //Name
        str="Name: "+QString::fromStdString(map[layer].buildings.at(ord)->name);
        painter.drawText(20+COLUMN*1, GAME_WINDOW_HEIGHT+18+line*20, str);
        line++;

        //tags
        str="Tags: ";
        for(int i=0;i<map[layer].buildings.at(ord)->tags.size();i++){
            str+=QString::fromStdString(map[layer].buildings.at(ord)->tags.at(i))+" ";
        }
        painter.drawText(20+COLUMN*1, GAME_WINDOW_HEIGHT+18+line*20, str);
        line++;

        //Points same building
        if(map[layer].buildings.at(ord)->scoreCond.quantityPoints){
            str="Points for other "+QString::fromStdString(map[layer].buildings.at(ord)->name)+": "+QString::number(map[layer].buildings.at(ord)->scoreCond.quantityPoints);
            painter.drawText(20+COLUMN*1, GAME_WINDOW_HEIGHT+18+line*20, str);
            line++;
        }

        for(int i=0;i<map[layer].buildings.at(ord)->scoreCond.proxPoints.size();i++){
            int points=map[layer].buildings.at(ord)->scoreCond.proxPoints.at(i).points;
            int proxDist=map[layer].buildings.at(ord)->scoreCond.proxPoints.at(i).proxDist;
            std::string name=map[layer].buildings.at(ord)->scoreCond.proxPoints.at(i).name;
            str="Points for every "+QString::fromStdString(name)+" "+QString::number(proxDist)+" tiles away: "+QString::number(points);
            painter.drawText(20+COLUMN*1, GAME_WINDOW_HEIGHT+18+line*20, str);
            line++;
        }
    }
};

#endif // GAME_H
