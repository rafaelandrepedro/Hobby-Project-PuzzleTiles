#ifndef ENTITY_H
#define ENTITY_H

struct Entity{
    Entity(){
        static int IdCount=1;
        Id=IdCount++;
    }
    int Id;
};

#endif // ENTITY_H
