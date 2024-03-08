//
// Created by joaosousa on 28-02-2024.
//

#ifndef PROJDA_WATERRESERVOIR_H
#define PROJDA_WATERRESERVOIR_H

#include <iostream>
#include <string>

using namespace std;

    class waterReservoir {
    private:
        string reservoir;
        string municipality;
        int id;
        string code;
        int maxCapacity;

    public:

        waterReservoir(const string &reservoir, const string &municipality, int id, const string &code,
                       int maxCapacity);

        const string &getReservoir() const;

        void setReservoir(const string &reservoir);

        const string &getMunicipality() const;

        void setMunicipality(const string &municipality);

        int getId() const;

        void setId(int id);

        const string &getCode() const;

        void setCode(const string &code);

        int getMaxCapacity() const;

        void setMaxCapacity(int maxCapacity);
    };


#endif //PROJDA_WATERRESERVOIR_H
