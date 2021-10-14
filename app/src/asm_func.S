.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_sum
.global asm_svc
.global asm_zeros
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12
.global asm_filtroVentana10
.global asm_pack32to16
.global asm_max
.global asm_invertir

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_zeros (uint32_t *vector, uint32_t longitud)
@
@ Parametros recibidos:
@   r0: vector
@   r1: longitud
@
@
.thumb_func
    asm_zeros:
        mov r2, 0               @ r2 = 0 para tener un cero disponible en un registro
    .asm_zero_loop:
        str r2,[r0],4           @ vector[i] = 0 ó *vector++ = 0
        subs r1, #1             @ longitud--
        bne .asm_zero_loop      @ salir si longitud == 0, saltar su es distinto de 0

        bx lr                   @ vuelve adonde fue llamada

    
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut uint32_t longitud, uint32_t escalar);
@
@ Parametros recibidos:
@   r0: vectorIn
@   r1: vectorOut
@   r2: longitud
@   r3: escalar
@
@
.thumb_func
    asm_productoEscalar32:
        push {r4}               @ r2 = 0 para tener un cero disponible en un registro
    .asm_product_loop:
        ldr r4,[r0], #4       @ vectorIn++
        mul r4, r3, r4        @ vectorOut[i] = escalar * vectorIn[i]
        str r4,[r1],4         @ vector[i] = 0 ó *vector++ = 0
        subs r2, #1           @ longitud--
        bne .asm_product_loop @ salir si longitud == 0
        pop {r4}
        bx lr                 @ vuelve adonde fue llamada


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
@
.thumb_func
	asm_productoEscalar16:
		@ Registros usados
		@ r0: vectorIn
		@ r1: vectorOut
		@ r2: longitud
		@ r3: escalar

		push {r4}

	.productoEscalar16_loop:
		ldrh	r4,[r0],2	                @ r4 = r0* ; r0*+2
		mul		r4,r3		                @ r4 = r4 * r3
		strh		r4,[r1],2	                @ r1* = r4 ; r1*+2
		subs	r2,1		                @ r2 = r2-1 con s actualizamos los flags
		bne		.productoEscalar16_loop	    @ salir si longitud == 0

		pop {r4}

		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
@
.thumb_func
	asm_productoEscalar12:
		@ Registros usados
		@ r0: vectorIn
		@ r1: vectorOut
		@ r2: longitud
		@ r3: escalar

		push {r4}

	.productoEscalar12_loop:
		ldrh	r4,[r0],2	                @ r4 = *r0 ; r0+2
		mul		r4,r3		                @ r4 = r4 * r3
        usat    r4,12,r4                    @ r4 = (r4 > 0xFFF)?0xFFF:r4
		strh	r4,[r1],2	                @ r1* = r4 ; *r1+2
		subs	r2,1		                @ r2 = r2-1 con s actualizamos los flags
		bne		.productoEscalar12_loop	    @ salir si longitud == 0

		pop {r4}

		bx lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@ void asm_filtroVentana10 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn);
@
.thumb_func
	asm_filtroVentana10:
        @ Registros usados
        @ r0: vectorIn
        @ r1: vectorOut
        @ r2: longitudVectorIn
        push {r3} @acumulador
        push {r4} @variable que contiene el dato actual  
        push {r5} @contador y dividendo
        push {r6} @variable donde guardo temporalmente la division
        mov     r3,#0    @ r3 = 0x000 inicio el acumulador
        mov     r5,#0    @ r5 = 0x000 inicio el contador
    .filtroVentana10_loop:
        ldrh	r4,[r0],2    @ r4 = *r0 ; r0+2
        add     r3,r4        @ r3 = r4 + r3
        cmp     r5,#10       @ r5 == 10
        beq     .filtroVentana10_jump_increment @ satura en 10 el contador que se usa posteriormente como dividendo
        add     r5,#1        @ r5 = r5 + 1
    .filtroVentana10_jump_increment:
        cmp     r5,#10       @ r5 == 10?
        bne     .filtroVentana10_window_not_full @if (r5 <10) goto window_not_full
        ldrh    r4,[r0,-20]  @ r4 = *(r0-20) cargo el elemento mas antiguo de la ventana
        subs    r3,r4        @ r3 = r3 - r4  resto al elemento mas antiguo del acumulador
    .filtroVentana10_window_not_full:
        sdiv    r6,r3,r5     @ r6 = r3 / r5 donde r5 incremeneta hasta 10 y ahi queda cuando se llena la ventana
        strh    r6,[r1],2    @ r1* = r6 ; r1+2   
        subs    r2,#1        @ r2 = r2 - 1
        bne     .filtroVentana10_loop @ salir si longitudVectorIn == 0
        pop {r6}
        pop {r5}
        pop {r4}
        bx  lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@ void asm_pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);
@
.thumb_func
	asm_pack32to16:        
        @ Registros usados
        @ r0: vectorIn
        @ r1: vectorOut
        @ r2: longitud
        add     r0,#2
    .pack32to16_loop:
        ldr     r4,[r0],4    @ r4 = *r0 ; r0+2
        strh    r4,[r1],2    @ r1* = r4 ; r1+2
        subs    r2,#1        @ r2 = r2 - 1
        bne     .pack32to16_loop @ salir si longitud == 0
        bx  lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   
@ Prototipo en "C":
@ int32_t asm_max (int32_t * vectorIn, uint32_t longitud);
@
.thumb_func
    asm_max:
        @ Registros usados
        @ r0: vectorIn
        @ r2: longitud

        @r3: max
        @r4: dato actual
        ldr     r3,[r0]      @ r3 = *r0;
        mov     r4,#0        @ r4 = 0x000
    .max_loop:
        ldr     r4,[r0],4    @ r4 = *r0 ; r0++
        cmp     r3,r4        @ r3 > r4?
        bgt     .max_jump_1  @ if (r3 > r4) goto max_jump_1
        mov     r3,r4        @ r3 = r4
    .max_jump_1:
        subs    r1,#1        @ r1 = r1 - 1
        bne     .max_loop    @ salir si longitud == 0
        mov     r0,r3        @ r0 = r3
        bx      lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   
@ Prototipo en "C":
@ void asm_invertir (uint16_t * vector, uint32_t longitud);
@
.thumb_func
    asm_invertir:
        @ Registros usados
        @ r0: vector
        @ r1: longitud
        mov     r2,#2        @ r2 = 0x000
        sub     r1,#1        @ salto = longitud - 1 
        mul     r1,r2        @ r1 = 2*salto //para que este en bytes
    .asm_invertir_loop:
        ldrh    r2,[r0]      @ r2 = *r0;
        ldrh    r3,[r0,r1]   @ r3 = *(r0+salto)
        strh    r2,[r0,r1] @ *(r0+salto) = r2 ;
        strh    r3,[r0],2    @ *(r0) = r3 ; r0+2
        subs    r1,#4        @ r1 = r1 - 4
        cmp     r1,#0        @ r1 > 0?
        bgt     .asm_invertir_loop @ salir si longitud == 0
        bx      lr


