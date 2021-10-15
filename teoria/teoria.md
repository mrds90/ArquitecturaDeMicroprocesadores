
# Guía de preguntas orientadoras
## Cortex M
1. Diferencias entre familias de ARM

|  | Application| Real time | Microcontollers |
|--|------------|-----------|-----------------|
|                                                      Descripción                                                              | Pensado para utilizar con sistemas operativos en sistemas con alta performance y ejecutando varios procesos en simultaneo       |Utilizado para sistemas donde la latencia y velocidad de procesamiento son prioridad                                            | Orientado a sistemas de uso masivo, bajo costo con alta densidad de código, buena eficiencia energética y optimizado para  programación en C                                                                                                               |
|                                                     Caracteristicas                                                           | Existen versiones con nucleo de 32 y 64 bit RISC y los de 32 en su mayoría incluyen MMU                                         | Posee caracteristicas que lo vuelve mas tolerante y apto para aplicaciones de seguridad críticas. Incluye cache, MPU, mayores manejos de excepciones por hardware, memorias de rapido acceso (no cache), interrupciones determinísticas, etc                  | No posee MMU, por lo que no es posible implementar sistemas operativos completamente desarrollados                              |

2. Densidad de código con Thumb
    Se dice que Thumb permite mayor densidad de código porque son instrucciones de 16bit y ARM ejecuta instrucciones de 32bit. Por lo que es posible duplicar la cantidad de instrucciones en el mismo espacio de memoria.

3. Arquitectura Load-Store
    Se entiende por arquitectura Load-Store que no es posible operar directamente con la posición de memoria. Se debe Cargar el dato en un registro de proposito general del µP y almacenar nuevamente en la posición de memoria correspondiente luego de haber ejecutado la operacion deseada. 
    
    La arquitectura no dispone de instrucciones que permitan realizar cambios en memoria evitando el load-store.

4. Mapa de memoria
    El Mapa de memoria es un mapa plano con direcciones de 32 bit. Dentro de este se ubican todas las zonas de memorias del µ.
    
    En la imagen se puede ver como se divide la misma en el cortex M3:

    ![alt text](https://images4.programmerclick.com/755/43/43e5cd691713b3a2cabdc17a96ffe4e3.gif)

5. Shadow pointers del PSP y MSP
    La existencia de dos punteros al Stack con saltos por HW (en modo privilegiado y no privilegiado) permite que las tareas de un sistema operativo operen con el PSP mientras el kernel use el MSP.

6. Modos de privilegio y operación en Cortex M
    En la siguiente tabla se pueden observar los modos de privilegio y operación del Cortex M

    ![alt text](https://www.researchgate.net/profile/Sandro-Pinto/publication/339784921/figure/fig1/AS:866952901840901@1583709087725/Armv7-M-operation-modes-and-privileged-levels.png)

    Lo que representa esta imagen es que existen dos modos de privilegio (privilegiado y no privilegiado). Las excepciones solo se corren en modo privilegiado. En modo thread es posible estar en cualquiera de los dos modos de privilegio.
    Para conmutar de un estado a otro la siguiente imagen es bastante descriptiva:

    ![alt text](https://community.arm.com/cfs-file/__key/communityserver-blogs-components-weblogfiles/00-00-00-21-42/6470.handler_2D00_thread.PNG)

    Se puede observar que por defecto (en reset) se inicia en modo Thread Privilegiado y que por software podemos conmutar a modo no privilegiado.
    Desde cualquier modo de privilegio en el Thread podemos conmutar al modo Handler mediante una excepción y este retornará a su punto anterior una vez finalizada la rutina del Handler.
    Un ejemplo del flujo de privilegiado->no privilegiado->privilegiado es el switch de privilegiado a no privilegiado cuando el kernel de un SO le da espacio a una tarea y durante la ejecucion de la tarea sucede una interrupción que conmuta al mismo a modo Handler privilegiado.

7. Modelo de registro ortogonal
    Esto significa que todos los registros en todas las combinaciones pueden ser utilizado para ejecutar una instrucción.

8. Instrucciones e ejecucion condicional (IT)
    Estas instrucciones poseen la ventaja de que permiten generar un flujo de programa bifurcado sin romper el pipeline. Como el pipeline permite realizar de forma veloz el acceso a las instrucciones en espera, evitar la rotura del mismo implica ahorros de tiempos.

    Ejemplo:
```
CMP   R2,R1
ITETE NE
ANDNE r0,r0,r1
MOVEQ r2,r3
ADDNE r2,r2,#1
MOVEQ r5,#3
```
9. Excepciones de mayor prioridad (reset, NMI y Hardfault)

| Excepción | Prioridad | Descripción                                                                                  |
| --------- | --------- | -------------------------------------------------------------------------------------------- |
| Reset     | 3         | Se invoca al encender e inicializa el core                                                   |
| NMI       | 2         | Interrupción no enmascarable                                                                 |
| Hardfault | 1         | Interrupción por alguna falla.  Acceso a un lugar de memoria protegido o una división por 0. |

10. Pila o Stack

    La pila tiene como función conservar el contexto cuando se realiza un salto dentro del flujo del programa (Ej: llamado a función). Se utiliza como un buffer FIFO. Esto permite tener varios saltos consecutivos y en cada regreso se recupera lo ultimo (lo mas reciente) que se envió a la pila.
    Tambien se utiliza para pasar argumentos a las funciones.

    La arquitectura realiza un PUSH del los primeros 4 registros del µP y el Program Counter (PC) a la pila cuando se realiza un cambio de contexto.

11. Secuencia de Reset

    La secuencia de Reset inicializa leyendo la dirección 0 de la memoria donde se encuentra el valor del MSP, posteriormente se lee la siguiente dirección 0x04 (32 bit posteriores) cuyo valor indica la posición inicial del vector de reset. Desde ahi se salta a esta posición de memoria indicada anteriormente. En esta ultima posición de memoria se obtiene la primera instrucción del vector de Reset.

12. Core peripherals

    Son los perifericos que define ARM para sus Cortex y en consecuencia son necesarios para su funcionamiento. Aunque los distintos fabricantes puede incorporar varios perifericos a sus procesadores, todos tienen en comun los core peripherals.

13. Prioridad de las interrupciones
    Se utiliza el NVIC para priorizar las interrupciones. La arquitectura Cortex-M proporciona a los usuarios una función para priorizar las interrupciones de acuerdo con su uso y necesidad. Las primeras 16 excepciones no se pueden priorizar ya que son excepciones del sistema y el sistema no permitirá que el usuario cambie la prioridad de sus interrupciones. Además, estas interrupciones solo son accesibles en modo privilegiado. Sin embargo, las interrupciones del usuario (16-255) son programables por el usuario. Estas interrupciones, ya sea de software o hardware, pueden ser priorizadas por el usuario y NVIC decodifica la prioridad de las interrupciones antes de procesarlas.

14. CMSIS

    CMSIS es un estandar provisto por ARM que deben cumplir todos los fabricantes. Esta es una capa de abstracción de hardware que permite que los usuarios de las arquitecturas de ARM portar sus implementaciones entre fabricantes con mayor facilidad.

15. Secuencia de atencion de interrupción
    En la siguiente imagen se observa la secuencia de atención de dos interrupciones.
    [!alt text](https://microcontrollerslab.com/wp-content/uploads/2020/09/Tail-Chaining-NVIC-ARM-CortexM.jpg)
    La imagen muestra que se apenas se realiza el pedido de interrupción se genera una señal que se mantiene hasta que la interrupción es atendida. El NVIC compara los niveles de prioridad y comienza la rutina de ingreso a la excepción mas prioritaria. Para esto realiza un push al stack del contexto y posteriormente ejecuta la subrutina de interrupción.

16. Tail Chainig y late arrival
    
    - Tail Chainig ayuda a ejecutar las interrupciones una tras otra sin el problema del cambio de contexto. Sin un controlador de interrupciones anidado, la próxima interrupción pasa al estado pendiente si ya se está ejecutando una interrupción, a menos que esa interrupción en ejecución complete su rutina de servicio y regrese al programa y complete el cambio de contexto. En las interrupciones anidadas, sin embargo, no tenemos que hacer esto, las siguientes interrupciones se ejecutaran dentro de la primera interrupción antes de devolver el control al programa que llama.

    - Late arrival: Es el proceso de cambiar de una excepción a otra de mayor prioridad cuando ambos handler se encuentran temporalmente. Para que se de esta situación la interrupción de menor prioridad debe ser invocada primero. EN este caso se realiza un nuevo push al stack para preservar el contexto.

17. SisTick
    El systick es un timer descendente de 24 bits incorporado que es comun a todos los fabricantes. Debido a que todos los fabricantes deben incorporarlos y es el utilizado como base de tiempo para los sistemas operativos, la implementación del mismo facilita la portabilidad entre fabricantes.

18. MPU

    El MPU es un componente para proteger la memoria. Las discrepancias de MPU y las violaciones de permisos invocan el controlador de fallas de prioridad programable, es decir que ejecutan una excepción de falla.
    Se puede utilizar para
    - Establecer reglas de privilegios
    - Separar procesos
    - Establecer reglas de acceso.

19.  ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las regiones definidas?
    
    Se pueden configurar hasta 8 regiones. En caso de haber solapamientos la región es accesible para la ultima región definida. Las zonas de memoria no definidas son accesibles en modo privilegiado. Puede configurarse para que sean accesible en modo no privilegiado.

20. PendSV
    
    PendSV (llamada de servicio pendiente) funciona con SVC en el sistema operativo. Aunque SVC (por instrucción SVC) no se puede suspender (una aplicación que llame a SVC esperará que la tarea requerida se realice de inmediato), PendSV se puede suspender y es útil para que un sistema operativo suspenda una excepción. Esto permite realizar una accion despues de que tareas importantes sean completadas.

21. SCV

    SVC se utiliza para generar llamadas a funciones del sistema. Por ejemplo, en lugar de permitir que los programas de usuario accedan directamente al hardware, un sistema operativo puede proporcionar acceso al hardware a través de un SVC. Entonces, cuando un programa de usuario desea usar cierto hardware, genera la excepción SVC usando instrucciones SVC, y luego se ejecuta el controlador de excepciones de software en el sistema operativo y proporciona el servicio solicitado por la aplicación del usuario.
    
    Esto permite proveer de un sistema mas robusto al prevenir que las aplicaciones de usuario acceda directamente al hardware.

[!alt text](https://ars.els-cdn.com/content/image/3-s2.0-B9781856179638000107-f07-15.jpg)



## ISA

1. Sufijos
    Son etiquetas claves que sirven para ejecutar la instrucción con condiciones adicionales.   Facilita al programador ya que permite interpretar las instrucciones como una instrucción mas un  "argumento" que genera un comportamiento particular.
    Por ejemplo utilizar 16bit en vez de 32bit o actualizar los flags.

   
2. Sufijo s
    Se utiliza para actualizar los flags (bit de status). Permite evaluar condiciones para definir el flujo del programa.
    ejemplo:
```
.productoEscalar16_loop:
    ldrh	r4,[r0],2	                @ r4 = r0* ; r0*+2
    mul		r4,r3		                @ r4 = r4 * r3
    strh    r4,[r1],2	            @ r1* = r4 ; r1*+2
    subs	r2,1		                @ r2 = r2-1 con s actualizamos los flags
    bne		.productoEscalar16_loop	    @ salir si longitud == 0
```
3. Aritmetica saturada

    Permite acotar el valor de una variable a la cantidad de 2^bits que se desee y evitar que un overflow me produzca un reinicio del valor de la variable.

ejemplo 8 bit:
```
.productoEscalar8_loop:
	ldrh	r4,[r0],2	                @ r4 = *r0 ; r0+2
	mul		r4,r3		                @ r4 = r4 * r3
     usat    r4,8,r4                     @ r4 = (r4 > 0xFF)?0xFF:r4 ;r4 = (r4 < 0)?0:r4
	strh	r4,[r1],2	                @ r1* = r4 ; *r1+2
	subs	r2,1		                @ r2 = r2-1 con s actualizamos los flags
	bne		.productoEscalar8_loop	    @ salir si longitud == 0
```

4. Interfaz Asm y C.

    Los argumentos se reciben desde r0 a r3 y si hay mas se stackean en la pila.
    La devolución del resultado se realiza mediante el registro R0.
    Todos los registros posteriores al r3 deben guardarse en la pila antes de modificarse.

5. SIMD

    SIMD son instrucciones que aplican una misma operación sobre un conjunto de datos. En una instrucción convencional se aplican a un paquete de datos reducido. En cambio con SIMD podemos ahorrar tiempos al aplicar la misma operación a varios datos en simultaneo.



