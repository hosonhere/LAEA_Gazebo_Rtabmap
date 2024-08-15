#ifndef _Data_H_
#define _Data_H_
#include "Common.h"
struct Data{
    uint8_t *buffer;
    size_t size;
};

void destroy_data(struct Data *data);

#endif