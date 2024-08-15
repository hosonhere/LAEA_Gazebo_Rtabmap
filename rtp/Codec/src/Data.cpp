#include "Data.h"

void destroy_data(struct Data *data){
    free(data->buffer);
}
