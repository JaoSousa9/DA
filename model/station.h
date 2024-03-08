//
// Created by joaosousa on 28-02-2024.
//

#ifndef PROJDA_STATION_H
#define PROJDA_STATION_H

#include <string>
 using namespace std;

class station {

    private:
        int id;
        string code;

public:
    station(int id, const string &code);

    int getId() const;

    void setId(int id);

    const string &getCode() const;

    void setCode(const string &code);
};


#endif //PROJDA_STATION_H
