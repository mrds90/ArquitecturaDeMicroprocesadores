
# Guía de preguntas orientadoras
## Diferencias entre familias de ARM

|  | Application| Real time | Microcontollers |
|--|------------|-----------|-----------------|
|                                                      Descripción                                                              | Pensado para utilizar con sistemas operativos en sistemas con alta performance y ejecutando varios procesos en simultaneo       |Utilizado para sistemas donde la latencia y velocidad de procesamiento son prioridad                                            | Orientado a sistemas de uso masivo, bajo costo con alta densidad de código, buena eficiencia energética y optimizado para  programación en C                                                                                                               |
|                                                     Caracteristicas                                                           | Existen versiones con nucleo de 32 y 64 bit RISC y los de 32 en su mayoría incluyen MMU                                         | Posee caracteristicas que lo vuelve mas tolerante y apto para aplicaciones de seguridad críticas. Incluye cache, MPU, mayores manejos de excepciones por hardware, memorias de rapido acceso (no cache), interrupciones determinísticas, etc                  | No posee MMU, por lo que no es posible implementar sistemas operativos completamente desarrollados                              |

## Densidad de código con Thumb


