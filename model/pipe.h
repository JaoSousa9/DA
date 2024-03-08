//
// Created by joaosousa on 28-02-2024.
//

#ifndef PROJDA_PIPE_H
#define PROJDA_PIPE_H

#include <string>
using namespace std;

class pipe {

    private:
        string source;
        string dest;
        int capacity;
        bool direction;

public:
    pipe(const string &source, const string &dest, int capacity, bool direction);

    const string &getSource() const;

    void setSource(const string &source);

    const string &getDest() const;

    void setDest(const string &dest);

    int getCapacity() const;

    void setCapacity(int capacity);

    bool isDirection() const;

    void setDirection(bool direction);


};


#endif //PROJDA_PIPE_H
