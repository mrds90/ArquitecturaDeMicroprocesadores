#include "board.h"
#include "c_func.h"
#include "asm_func.h"

#include <stdlib.h>
#include <stdnoreturn.h>

#define DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
#define DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
#define DWT_CYCCNT_ENA_BIT      (1UL<<0)
#define DWT_DEMCR               (*((volatile uint32_t*)0xE000EDFC))
#define DWT_DEMCR_TRC_ENA_BIT   (1UL<<24)
#define EnableCycleCounter()    DWT_CONTROL |= DWT_CYCCNT_ENA_BIT ; DWT_DEMCR |= DWT_DEMCR_TRC_ENA_BIT;
#define GetCycleCounter()       DWT_CYCCNT
#define ResetCycleCounter()     DWT_CYCCNT = 0
#define DisableCycleCounter()   DWT_CONTROL &= ~DWT_CYCCNT_ENA_BIT

// Variable que se incrementa cada vez que se llama al handler de interrupcion
// del SYSTICK.
static volatile uint32_t s_ticks = 0;


// Inicia soporte de la placa y periodo de la interrupcion del SYSTICK
// cada 1 milisegundo.
static void Inicio (void)
{
    Board_Init ();
    SystemCoreClockUpdate ();
    SysTick_Config (SystemCoreClock / 1000);
}


// Segun la configuracion realizada en Inicio(), este handler de interrupcion
// se ejecutara cada 1 milisegundo.
void SysTick_Handler (void)
{
    ++ s_ticks;
}

static void Zeros (void)
{
    uint32_t vector[0xF] = {0xC};
    asm_zeros(vector, 0xF);
    c_zeros(vector, 0xF);
}

static void Product32 (void)
{
    uint32_t vectorIn[1000];
    uint32_t vectorOut[1000];
    uint32_t i;
    for (i = 0; i < 1000; ++i) {
        vectorIn[i] = i;
        vectorOut[i] = 0;
    }
    asm_productoEscalar32(vectorIn, vectorOut, 1000, 2);
    c_productoEscalar32(vectorIn, vectorOut, 1000, 2);
}

static void Product16 (void)
{   volatile uint32_t lectura;
    
    uint16_t vectorIn[1000];
    uint16_t vectorOut[1000];
    uint32_t i;
    for (i = 0; i < 1000; ++i) {
        vectorIn[i] = i;
        vectorOut[i] = 0;
    }

    EnableCycleCounter();
    ResetCycleCounter();
    asm_productoEscalar16(vectorIn, vectorOut, 1000, 2);
    lectura = GetCycleCounter();
    ResetCycleCounter();
    c_productoEscalar16(vectorIn, vectorOut, 1000, 2);
    lectura = GetCycleCounter();
    ResetCycleCounter();
    DisableCycleCounter();
}


static void Product12 (void)
{   volatile uint32_t lectura;
    
    uint16_t vectorIn[1000];
    uint16_t vectorOut[1000];
    uint32_t i;
    for (i = 0; i < 1000; ++i) {
        vectorIn[i] = i;
        vectorOut[i] = 0;
    }

    EnableCycleCounter();
    ResetCycleCounter();
    asm_productoEscalar12(vectorIn, vectorOut, 1000, 2);
    lectura = GetCycleCounter();
    ResetCycleCounter();
    c_productoEscalar12(vectorIn, vectorOut, 1000, 2);
    lectura = GetCycleCounter();
    ResetCycleCounter();
    DisableCycleCounter();
}


static void MovingAverage(void) {
    volatile uint32_t asm_counts = 0;
    volatile uint32_t c_counts = 0;
    uint16_t vectorIn[1000];
    uint16_t vectorOut[1000];
    uint16_t vectorOut2[1000];
    uint32_t i;
    for (i = 0; i < 1000; ++i) {
        vectorIn[i] = i;
        vectorOut[i] = 0;
    }

    EnableCycleCounter();
    ResetCycleCounter();
    asm_filtroVentana10(vectorIn, vectorOut, 1000);
    asm_counts = GetCycleCounter();
    ResetCycleCounter();
    c_filtroVentana10(vectorIn, vectorOut2, 1000);
    c_counts = GetCycleCounter();
    ResetCycleCounter();
    DisableCycleCounter();

}

static void pack32to16(void) {
    volatile uint32_t asm_counts = 0;
    volatile uint32_t c_counts = 0;
    uint32_t vectorIn[1000];
    uint16_t vectorOut[1000];
    uint32_t i;
    for (i = 0; i < 1000; ++i) {
        vectorIn[i] = i;
        vectorOut[i] = 0;
    }

    EnableCycleCounter();
    ResetCycleCounter();
    asm_pack32to16(vectorIn, vectorOut, 1000);
    asm_counts = GetCycleCounter();
    ResetCycleCounter();
    c_pack32to16(vectorIn, vectorOut, 1000);
    c_counts = GetCycleCounter();
    ResetCycleCounter();
    DisableCycleCounter();
}

static void Max (void) {
    volatile uint32_t asm_counts = 0;
    volatile uint32_t c_counts = 0;
    int16_t c_result;
    int16_t asm_result;
    uint32_t vectorIn[1000];
    uint32_t i;
    for (i = 0; i < 1000; ++i) {
        vectorIn[i] = i;
    }
    EnableCycleCounter();
    ResetCycleCounter();
    asm_result = asm_max(vectorIn, 1000);
    asm_counts = GetCycleCounter();
    ResetCycleCounter();
    c_result = c_max(vectorIn, 1000);
    c_counts = GetCycleCounter();
    ResetCycleCounter();
    DisableCycleCounter();
}

static void Invertir (void) {
    volatile uint32_t asm_counts = 0;
    volatile uint32_t c_counts = 0;
    uint16_t vectorIn[50];
    uint32_t i;
    for (i = 0; i < 50; ++i) {
        vectorIn[i] = i;
    }
    EnableCycleCounter();
    ResetCycleCounter();
    asm_invertir(vectorIn, 50);
    asm_counts = GetCycleCounter();
    ResetCycleCounter();
    c_invertir(vectorIn, 50);
    c_counts = GetCycleCounter();
    ResetCycleCounter();
    DisableCycleCounter();
}
static void Suma (void)
{
    const uint32_t A = 20;
    const uint32_t B = 30;

    const uint32_t SumResult_C = c_sum (A, B);
    const uint32_t SumResult_Asm = asm_sum (A, B);

    // Actividad de debug: SumResult_C y SumResult_Asm deberian contener el
    // mismo valor.
    __BKPT (0);

    (void) SumResult_C;
    (void) SumResult_Asm;
}


static void LlamandoAMalloc (void)
{
    // De donde saca memoria malloc?
    // (se vera en clase)
    void * ptr = malloc (2048);

    (void) ptr;
}


static void PrivilegiosSVC (void)
{
    // Obtiene valor del registro de 32 bits del procesador llamado "control".
    // El registro guarda los siguientes estados:
    // bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
    // bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
    // bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
    //        Recordar que este valor solo se usa en modo Thread. Las
    //        interrupciones siempre se ejecutan en modo Handler con total
    //        privilegio.
    uint32_t x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    __BKPT (0);

    x |= 1;
    // bit 0 a modo No privilegiado.
    __set_CONTROL (x);

    // En este punto se estaria ejecutando en modo No privilegiado.
    // Lectura del registro "control" para confirmar.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    __BKPT (0);

    x &= ~1u;
    // Se intenta volver a modo Privilegiado (bit 0, valor 0).
    __set_CONTROL (x);

    // Confirma que esta operacion es ignorada por estar ejecutandose en modo
    // Thread no privilegiado.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    __BKPT (0);

    // En este punto, ejecutando en modo Thread no privilegiado, la unica forma
    // de volver a modo privilegiado o de realizar cualquier cambio que requiera
    // modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
    // opertivo de tiempo real.
    // Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
    // utilizando la instruccion "svc".
    // No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
    // es necesario implementar una funcion en assembler. Ver el archivo suma.S.
    asm_svc ();

    // El sistema operativo (el handler de SVC) deberia haber devuelto el modo
    // de ejecucion de Thread a privilegiado (bit 0 en valor 0).
    x = __get_CONTROL ();

    // Fin del ejemplo de SVC
}


// Handler de la interrupcion "SVC" (Supervisor Call).
// Usado por el ejemplo "EjemploPrivilegiosSVC".
void SVC_Handler (void)
{
    // Se obtiene el valor del registro "control". El bit 0 indica el nivel
    // de privilegio en modo "Thread". Deberia ser 1: No privilegiado.
    uint32_t x = __get_CONTROL ();

    // Borra el bit 0. Nuevo valor 0: privilegiado.
    x &= ~1u;

    // Asigna el nuevo valor al registro "control". Esta operacion deberia
    // ejecutarse ya que todo manejador de interrupciones se ejecuta en modo
    // "Handler" con total privilegio.
    __set_CONTROL (x);
}


noreturn void LoopInfinito (void)
{
    while (1)
    {
        // Procesador en modo espera hasta que ocurra una interrupcion
        // (Bajo consumo)
        __WFI();
    }
}


int main (void)
{
    Inicio ();

    // Zeros();
    // Suma ();
    // MovingAverage();
    Invertir();
    PrivilegiosSVC ();

    LlamandoAMalloc ();

    LoopInfinito ();
}
