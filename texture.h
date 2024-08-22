#ifndef TEXTURE_H
#define TEXTURE_H

#include <QPixmap>
#include <QPainter>
#include <vector>
#include <fstream>
#include "util.h"
#include "canvas.h"

struct Texture{
    QPixmap pmap;
    std::string name;
    int offX=0;
    int offY=0;

    void print(Canvas *canvas, int x, int y){

        QPixmap pmap2=pmap;

        //std::cout<<"C "<<x<<" "<<y<<std::endl;

        //point in canvas
        int cx=x*canvas->zoom+canvas->coord.x+canvas->dim.width/2-canvas->offset.x;
        int cy=y*canvas->zoom+canvas->coord.y+canvas->dim.height/2-canvas->offset.y;

        //std::cout<<"D "<<cx<<" "<<cy<<std::endl;

        //distance from center to edges of zoomed image
        int zx=(pmap2.width()/2)*canvas->zoom;
        int zy=(pmap2.height()/2)*canvas->zoom;

        //std::cout<<"E "<<zx<<" "<<zy<<std::endl;

        //where to start drawing the zoomed image
        int px=cx-zx;
        int py=cy-zy;

        //std::cout<<"F "<<px<<" "<<py<<std::endl;

        //canvas->painter->drawLine(canvas->coord.x, canvas->coord.y, canvas->coord.x+canvas->dim.width, canvas->coord.y);
        //canvas->painter->drawLine(canvas->coord.x, canvas->coord.y+canvas->dim.height, canvas->coord.x+canvas->dim.width, canvas->coord.y+canvas->dim.height);
        //canvas->painter->drawLine(canvas->coord.x, canvas->coord.y, canvas->coord.x, canvas->coord.y+canvas->dim.height);
        //canvas->painter->drawLine(canvas->coord.x+canvas->dim.width, canvas->coord.y, canvas->coord.x+canvas->dim.width, canvas->coord.y+canvas->dim.height);

        QSize newSize = pmap2.size() * canvas->zoom;
        pmap2 = pmap2.scaled(newSize, Qt::KeepAspectRatio, Qt::FastTransformation);

        //limits
        int left=px;
        int right=px+pmap2.width();
        int up=py;
        int down=py+pmap2.height();

        //std::cout<<"G "<<left<<" "<<right<<" "<<up<<" "<<down<<std::endl;


        //cut
        int cleft=0;
        int cright=0;
        int cup=0;
        int cdown=0;

        if(left<canvas->coord.x)
            cleft=canvas->coord.x-left;

        if(right>canvas->coord.x+canvas->dim.width)
            cright=right-(canvas->coord.x+canvas->dim.width);

        if(up<canvas->coord.y)
            cup=canvas->coord.y-up;

        if(down>canvas->coord.y+canvas->dim.height)
            cdown=down-(canvas->coord.y+canvas->dim.height);

        if(cleft+cright>=pmap2.width()||cup+cdown>=pmap2.height())
            return;

        //std::cout<<"H "<<cleft<<" "<<cright<<" "<<cup<<" "<<cdown<<std::endl;




        QRect cutRect(cleft, cup, -cright+pmap2.width(), -cdown+pmap2.height());
        pmap2 = pmap2.copy(cutRect);

        canvas->painter->drawPixmap(px+cleft, py+cup, pmap2);
        //std::cout<<"I "<<cx+cleft<<" "<<cy+cup<<std::endl;


    }

};

struct TextureList{
    std::vector<Texture> textures;

    void loadTextures(std::string file){
        int sizeX=0;
        int sizeY=0;
        int offX=0;
        int offY=0;

        std::ifstream inputFile(file);

        if (!inputFile.is_open()) {
            return;
        }

        std::string line;

        while (std::getline(inputFile, line)) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);

                if (key == "SizeX"){
                    sizeX = std::stoi(value);
                }
                if (key == "SizeY"){
                    sizeY =  std::stoi(value);
                }
                if (key == "OffX"){
                    offX =  std::stoi(value);
                }
                if (key == "OffY"){
                    offY =  std::stoi(value);
                }
                if (key == "Done"){
                    break;
                }
                if (key == "Texture"){
                    QPixmap pmap=QPixmap("data/textures/"+QString::fromStdString(value)+".png");
                    int rows=pmap.height()/(sizeY+offY*2);
                    int columns=pmap.width()/(sizeX+offX*2);

                    for(int i=0;i<columns;i++){
                        for(int j=0;j<rows;j++){
                            Texture texture;
                            texture.pmap=pmap.copy(i*(sizeX+offX*2), j*(sizeY+offY*2), sizeX+offX*2, sizeY+offY*2);
                            texture.name=value+"_"+std::to_string(i)+"_"+std::to_string(j);
                            texture.offX=offX;
                            texture.offY=offY;
                            textures.push_back(texture);
                        }
                    }
                }
            }
        }

        inputFile.close();
    }

    Texture* selectTexture(std::string name){
        for(int i=0;i<textures.size();i++)
            if(textures.at(i).name==name)
                return &(textures.at(i));
        return nullptr;
    }

    Texture* selectTexture(int ord){
        if(ord<textures.size())
            return &(textures.at(ord));
        else
            return nullptr;
    }




};

struct AnimatedTextureFrame{
    Texture *texture;
    Coord coord;
    //float orientation;

    AnimatedTextureFrame(){
        coord={0, 0};
        //orientation=0;
    }

    void print(Canvas *canvas, int x, int y){
        texture->print(canvas, x+coord.x, y+coord.y);
    }
};

struct AnimatedTexture{
    std::vector<AnimatedTextureFrame> animatedTextureFrames;
    Coord coord;

    AnimatedTexture(){}

    void addFrame(AnimatedTextureFrame animatedTextureFrame){
        animatedTextureFrames.push_back(animatedTextureFrame);
    }

    void print(Canvas *canvas, int x, int y, int frame){
        animatedTextureFrames.at(frame%animatedTextureFrames.size()).print(canvas, x+coord.x, y+coord.y);
    }

};

#endif // TEXTURE_H
