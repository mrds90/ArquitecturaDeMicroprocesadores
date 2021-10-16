#include "c_func.h"

#define MASK_12_BIT       0x00000FFFUL
#define Saturate12Bit(x)  x > MASK_12_BIT ? MASK_12_BIT : x
#define SAMPLE_RATE     44100
#define ECHO_DELAY      20/1000
#define SAMPLE_DELAY    (SAMPLE_RATE*ECHO_DELAY)
#define HALF_GAIN(x)     (x>>1)

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

void c_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar) {
    uint32_t i;
    for (i = 0; i < longitud; i++) {
        vectorOut[i] = vectorIn[i] * escalar;
    }
}

void c_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar) {
    uint16_t i;
    for (i = 0; i < longitud; i++) {
        vectorOut[i] = vectorIn[i] * escalar;
    }
}

void c_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar) {
    uint16_t i;
    for (i = 0; i < longitud; i++) {
        vectorOut[i] = vectorIn[i] * escalar;
        vectorOut[i] = Saturate12Bit(vectorOut[i]);
    }
}

void c_filtroVentana10 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn)
{
    uint32_t acumulador = 0;
    uint32_t i;
    for (i = 0; i < longitudVectorIn; i++) {
        acumulador += vectorIn[i];
        if (i>8) {
            acumulador -= vectorIn[i-9];
        }
        vectorOut[i] = acumulador / 10;
    }
}

void c_pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud){
    uint32_t i;
    for (i = 0; i < longitud; i++) {
        vectorOut[i] = vectorIn[i] >> 16;
    }
}

int32_t c_max (int32_t * vectorIn, uint32_t longitud) {
    int32_t max = vectorIn[0];
    uint32_t i;
    for (i = 1; i < longitud; i++) {
        if (vectorIn[i] > max) {
            max = vectorIn[i];
        }
    }
    return max;
}

void c_invertir (uint16_t * vector, uint32_t longitud) {
    uint32_t i;
    for (i = 0; i < longitud/2; i++) {
        uint16_t aux = vector[i];
        vector[i] = vector[longitud-1-i];
        vector[longitud-1-i] = aux;
    }
}

void c_eco(int16_t * vector) {
    for(uint32_t i = SAMPLE_DELAY; i < 4096; i++) {
        vector[i] += HALF_GAIN(vector[i - SAMPLE_DELAY]);
    }
}
