;Ubicación de los pines de EN y RS en Miuva
#define LCD_EN   PORTD,4
#define LCD_RS   PORTD,5

    CBLOCK 
	NIBBLE  ;Variable para escribir en la LCD
	Ret1    ;Variables para crear retardos
	Ret2
    ENDC
    
;*****************************************************************************;
;Ejemplo de envío de instrucción					      ;
;									      ;
;   RS	    _________________________	1: DATOS | 0: INSTRUCCIONES	      ;
;									      ;   
;   RW	    _________________________	1: LECTURA  |	0: ESCRITURA          ;
;		  ___      ___                                                ;
;   EN	    _____/   \____/   \______	LEE EN FLANCOS DE BAJADA              ;
;									      ;
;	    _________________________					      ;
;   D7:D4   ___/_4 MSB_\/_4_LSB_\____	PARA ENVIAR LOS 8 BITS DE INSTRUCCION ;
;					PRIMERO SE ENVIAN LOS 4 MÁS           ;
;					SIGNIFICATIVOS Y POSTERIORMENTE LOS   ;
;					4 MENOS SIGNIFICATIVOS                ;
;									      ;
;*****************************************************************************;
;Se carga D7:D4 de la LCD en los bits D3:D0 de Miuva    

;*****************************************************************************;
;				    LCD_Init				      ;
;   Inicializa la LCD, esta función se debe ejecutar antes de cualquier otra  ;
;   que involucre el uso de la LCD.					      ;
;									      ;
;   Ejemplo de uso:							      ;
;	call    LCD_Init						      ;
;								              ;
;*****************************************************************************;

LCD_Init    
    bcf     LCD_EN		;Enable = LOW
    call    Retardo15ms		
    bcf     LCD_RS		;RS = LOW
    
    ;Inicia secuencia de inicialización
    
    ;Function set
    movlw   b'00000011'	;Carga el valor del puerto D
    movwf   PORTD	;Manda el valor al puerto
    bsf	    LCD_EN	;Activa el bit de ENABLE
    call    Retardo100us    
    bcf	    LCD_EN	;Lee Function set
    call    Retardo4_1ms
	
    ;Function set
    movlw   b'00000011'
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	;Lee Function set
    call    Retardo100us
	
    ;Function set
    movlw   b'00000011'
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	;Lee Function set
    call    Retardo4_1ms
    
    ;Function set
    movlw   b'00000010'
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	;Lee Function set
    call    Retardo4_1ms
	
    ;Function set
    movlw   b'00000010' ;MSB Función set
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
    movlw   b'00001110' ;LSB Función set
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
	
    ;Display off
    movlw   b'00000000'	;MSB Funcion display
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
    movlw   b'00001000'	;LSB Funcion display
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
	
    ;Display clear
    movlw   b'00000000'	;MSB Funcion clear
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
    movlw   b'00000001'	;LSB Funcion clear
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms

    ;Entry mode set
    movlw   b'00000000'	;MSB Funcion mode set
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
    movlw   b'00000110'	;LSB Funcion mode set
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
	
    ;DDRAM ACCES
    movlw   b'00001000'	;MSB DDRAM Access
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
    movlw   b'00000000'	;LSB DDRAM Access
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
	
    ;Display on
    movlw   b'00000000'	;MSB Display ON
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
    movlw   b'00001100'	;LSB Display ON
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	
    call    Retardo4_1ms
	
    return
    
;*****************************************************************************;
;   Ejemplo de envío del caracter H
;   Ubicación del caracter H: 0100 1000
;   Se tiene que enviar primero 0100 y posteriormente 1000
;   1. NIBBLE almacena el valor 0100 1000
;   2. Intercambia los nibbles y los almacena en W -> W = 0100 
;   3. Realiza una AND con 0x0F para tener solamente 4 bits
;	W = 0000 0100
;   4. Le suma 00100000 para habilitar RS como datos -> W = 0010 0100
;   5. Se envía a la LCD (PUERTO D)
;   6. Mueve al registro de trabajo la variable NIBBLE -> W = 0100 1000
;   7. Realiza una AND con 0x0F para tener solamente 4 bits
;	W = 0000 1000
;   8. Le suma 00100000 para habilitar RS como datos -> W = 0010 1000
;   9. Se envía a la LCD (PUERTO D)
;******************************************************************************;
    
;*****************************************************************************;
;				    LCD_Char				      ;
;   Imprime cualquier caracter en la posición actual de la LCD.		      ;
;									      ;
;   Ejemplo de uso:							      ;
;	movlw	'H'		;Caracter a escribir			      ;
;	call    LCD_Char						      ;
;								              ;
;*****************************************************************************;
LCD_Char
    bsf	    LCD_RS	;Se enviará un dato
    movwf   NIBBLE	;Almacenamos el valor en la variable NIBBLE
    swapf   NIBBLE,w	;Intercambia los NIBBLE
    andlw   0x0F	;AND con 0x0F
    addlw   b'00100000'	;Suma 00100000
    movwf   PORTD	;Se envía al puerto D
    bsf	    LCD_EN	;ENABLE = HIGH
    call    Retardo100us
    bcf	    LCD_EN	;ENABLE = LOW
    call    Retardo4_1ms
    movf    NIBBLE,w	;Mueve la variable NIBBLE a W
    andlw   0x0F	;AND con 0x0F
    addlw   b'00100000'	;Suma 00100000
    movwf   PORTD	;Se envía al puerto D
    bsf	    LCD_EN	;ENABLE = HIGH
    call    Retardo100us
    bcf	    LCD_EN	;ENABLE = LOW
    call    Retardo4_1ms
    return	
    
;*****************************************************************************;
;				    LCD_Clear				      ;
;   Limpia todo lo que está escrito en la LCD y posteriormente regresa el     ;
;   cursor a Home.
;									      ;
;   Ejemplo de uso:							      ;
;	call    LCD_Clear						      ;
;								              ;
;*****************************************************************************;
LCD_Clear
    bcf	    LCD_RS
    movlw	b'00000000'
    movwf	PORTD
    bsf	LCD_EN
    call	Retardo100us
    bcf	LCD_EN	
    call	Retardo4_1ms
    movlw	b'00000001'
    movwf	PORTD
    bsf	LCD_EN
    call	Retardo100us
    bcf	LCD_EN	
    call	Retardo4_1ms
    return
    
;*****************************************************************************;
;				    LCD_Home				      ;
;   Envia el cursor a la posición inicial de la LCD			      ;
;									      ;
;   Ejemplo de uso:							      ;
;	call    LCD_Home						      ;
;								              ;
;*****************************************************************************;
LCD_Home
    bcf	    LCD_RS
    movlw	b'00000000'
    movwf	PORTD
    bsf	LCD_EN
    call	Retardo100us
    bcf	LCD_EN	;Lee Function set
    call	Retardo4_1ms
    movlw	b'00000010'
    movwf	PORTD
    bsf	LCD_EN
    call	Retardo100us
    bcf	LCD_EN	;Lee Function set
    call	Retardo4_1ms
    return
   
;*****************************************************************************;
;				    LCD_GoTo				      ;
;   Inicializa la LCD, esta función se debe ejecutar antes de cualquier otra  ;
;   que involucre el uso de la LCD.					      ;
;									      ;
;   Ejemplo de uso:							      ;
;	movlw	0x03	    ;Posición de la LCD		        	      ;
;       call    LCD_GoTo						      ;
;								              ;
;*****************************************************************************;
;			POSICIONES DE LA LCD 2X16			      ;
;									      ;
;          00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F                    ;
;	   40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F                    ;
;									      ;
;*****************************************************************************;
LCD_GoTo
    bcf	    LCD_RS
    movwf   NIBBLE
    swapf   NIBBLE,w
    andlw   0x0F
    addlw   b'0001000'
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	;Lee Function set
    call    Retardo4_1ms
    movf    NIBBLE,w
    andlw   0x0F
    movwf   PORTD
    bsf	    LCD_EN
    call    Retardo100us
    bcf	    LCD_EN	;Lee Function set
    call    Retardo4_1ms
    return

;*****************************************************************************;
;			Funciones de retardos				      ;
;   Estas funciones se utilizaron para los tiempos requeridos mientras se     ;
;   envían los datos o instrucciones a la LCD, está predefinidos para el      ;
;   oscilador de Miuva (8MHz). En caso de usar otro oscilador se tendrían     ;
;   que ajustar los retardos para satisfacer los tiempo solicitados.	      ;
;									      ;
;		fosc = 8MHz						      ;
;		Ciclo Reloj = 1/fosc = 1/8M = 125ns			      ;
;		Ciclo Instruccion = 4*Ciclo Reloj = 500ns		      ;
;		La funcion DECFSZ tarda 3 ciclos en ejecutarse		      ;
;		Retardo 15ms						      ;
;		Tiempo = Ret1*Ret2*(3*500ns)				      ;
;		Tiempo = Ret1*Ret2*(1.5us)				      ;
;		Ret1 = 255						      ;
;		Ret2 = 40						      ;
;		Tiempo = (255*40)(1.5us) = .0153 = 15.3ms		      ;
;*****************************************************************************;

Retardo15ms
    movlw 	.255
    movwf 	Ret1
    movlw 	.40
    movwf	Ret2
Ret15ms
    decfsz	Ret1, F
    bra	Ret15ms
    decfsz	Ret2, F
    bra	Ret15ms
    return
    
;Retardo 40ms
;Tiempo = Ret1*Ret2*(3*500ns) 
;Tiempo = Ret1*Ret2*(1.5us)
;Ret1 = 255
;Ret2 = 105
;Tiempo = (255*105)(1.5us) = .0402 = 40.2ms
Retardo40ms
    movlw 	.255
    movwf 	Ret1
    movlw 	.105
    movwf	Ret2
Ret40ms
    decfsz	Ret1, F
    bra	Ret15ms
    decfsz	Ret2, F
    bra	Ret15ms
    return

;Retardo 5ms
;Tiempo = Ret1*Ret2*(3*500ns) 
;Tiempo = Ret1*Ret2*(1.5us)
;Ret1 = 255
;Ret2 = 14
;Tiempo = (255*14)(1.5us) = .005355 = 5.355ms
Retardo5ms
    movlw   .255
    movwf   Ret1
    movlw   .14
    movwf   Ret2
Ret5ms
    decfsz  Ret1,F
    bra	    Ret5ms
    decfsz  Ret2,F
    bra	    Ret5ms
    return
    
;Retardo 4.1ms
;Tiempo = Ret1*Ret2*(3*500ns) 
;Tiempo = Ret1*Ret2*(1.5us)
;Ret1 = 255
;Ret2 = 11
;Tiempo = (255*11)(1.5us) = .0042 = 4.2ms
Retardo4_1ms
    movlw 	.255
    movwf 	Ret1
    movlw 	.11
    movwf	Ret2
Ret4_1ms
    decfsz	Ret1, F
    bra	Ret15ms
    decfsz	Ret2, F
    bra	Ret15ms
    return
    
    
;Retardo 100us
;Tiempo = Ret1*(3*500ns) 
;Tiempo = Ret1*(1.5us)
;Ret1 = 67
;Tiempo = (67)(1.5us) = .0001005 = 100.5us
Retardo100us
    movlw 	.67
    movwf 	Ret1
Ret100us
    decfsz	Ret1, F
    return
    
;Retardo 40us
;Tiempo = Ret1*(3*500ns) = Ret1*(1.5us)
;Ret1 = 27
;Tiempo = (27)(1.5us) = .0000405 = 40.5us
Retardo40us
    movlw	.27
    movwf	Ret1
Ret40us
    decfsz	Ret1,F
    return
    
    END