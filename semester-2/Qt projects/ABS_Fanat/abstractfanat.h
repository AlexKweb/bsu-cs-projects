#ifndef ABSTRACTFANAT_H
#define ABSTRACTFANAT_H

#include <QTextStream>

class ABS_Fanat{
public:
    virtual void GetFanats(QTextStream&) = 0;
    virtual void PutFanats(QTextStream&) = 0;
    virtual void SortUp() = 0;
    virtual void SortDown() = 0;

    virtual ~ABS_Fanat() = default;
};

#endif // ABSTRACTFANAT_H
