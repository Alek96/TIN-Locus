#ifndef LOCUS_DATABASE_H
#define LOCUS_DATABASE_H

#include "ClientDataManager.h"

class Database {
    Database();

public:
    virtual ~Database();

    static Database& getInstance();

    ClientDataManager &getClientDataManager();

private:
    ClientDataManager clientDataManager;
};


#endif //LOCUS_DATABASE_H
