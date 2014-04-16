#ifndef PST05QUERY_H
#define PST05QUERY_H

#include "pst05data.h"

/**
 * @brief The PST05Query class
 *
 * Should be inherited by PST05 class
 */
class PST05Query
{
public:
    virtual PST05Data queryDevice() = 0;
};

#endif // PST05QUERY_H
