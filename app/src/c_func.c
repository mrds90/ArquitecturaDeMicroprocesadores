#include "c_func.h"


uint32_t c_sum (uint32_t firstOperand, uint32_t secondOperand)
{
    return (firstOperand + secondOperand);
}

void c_zeros (uint32_t *vector, uint32_t longitud) {
    uint32_t i;
    for (i = 0; i < longitud; i++) {
        vector[i] = 0;
    }
}
