#ifndef PST05QUERY_H
#define PST05QUERY_H

#include "pst05data.h"

class PST05Query
{
public:
    virtual PST05Data queryDevice() = 0;
};

#endif // PST05QUERY_H
