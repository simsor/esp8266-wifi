#ifndef SERVICE_H
#define SERVICE_H

class Service {
public:
    virtual bool init() = 0;
    virtual void update() = 0;
};

#endif