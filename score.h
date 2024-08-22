#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <vector>

enum ProxType{
    SAME,
    ORT,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,
    D7,
    D8,
    D9,
    D10
};

struct ProxPoint{
    std::string name;
    int proxDist;
    int points;

    ProxPoint(){}

    ProxPoint(std::string name, int proxDist, int points){
        this->name=name;
        this->proxDist=proxDist;
        this->points=points;
    }
};

struct ScoreCond{
    int quantityPoints;
    std::vector<ProxPoint> proxPoints;

    ScoreCond(){
        this->quantityPoints=0;
    }

    ScoreCond(int quantityPoints){
        this->quantityPoints=quantityPoints;
    }

    void add(ProxPoint proxPoint){
        proxPoints.push_back(proxPoint);
    }

};

#endif // SCORE_H
