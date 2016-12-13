; ***********************************************************
;   INTESC electronics & embedded
;
;   Curso básico de microcontroladores en ensamblador	    
;
;   Práctica 3: Uso de librería de LCD
;   Objetivo: Conocer el funcionamiento de una librería de LCD
;
;   Fecha: 05/Jun/16
;   Creado por: Daniel Hernández Rodríguez
; ************************************************************
    
    LIST    P = 18F4550	;PIC a utilizar
    INCLUDE <P18F4550.INC>

;************************************************************
;Configuración de fusibles
    CONFIG  FOSC = HS
    CONFIG  PWRT = ON
    CONFIG  BOR = OFF
    CONFIG  WDT = OFF
    CONFIG  MCLRE = ON
    CONFIG  PBADEN = OFF
    CONFIG  LVP = OFF
    CONFIG  DEBUG = OFF
    CONFIG  XINST = OFF

;***************************************************
    
    ORG 0x00    ;Iniciar el programa en el registro 0x00
	goto    INICIO

INICIO
	movlw   0x0A    
	movwf   ADCON1  ;Configuración de entradas digitales
	movlw   0x00
	movwf   TRISD   ;Configurar puerto D como salida

	call    LCD_Init	;Inicialización de la LCD
	movlw   0x05
	call    LCD_GoTo	;Ir a la posición 0x05 de la LCD
	movlw   'I'
	call    LCD_Char	;Imprime caracter "I"
	movlw   'N'
	call    LCD_Char	;Imprime caracter "N"
	movlw   'T'
	call    LCD_Char	;Imprime caracter "T"
	movlw   'E'
	call    LCD_Char	;Imprime caracter "E"
	movlw   'S'
	call    LCD_Char	;Imprime caracter "S"
	movlw   'C'
	call    LCD_Char	;Imprime caracter "C"
	movlw   0x42
	call    LCD_GoTo	;Ir a la posición 0x42
	movlw   'M'
	call    LCD_Char	;Imprime caracter "M"
	movlw   'I'
	call    LCD_Char	;Imprime caracter "I"
	movlw   'U'
	call    LCD_Char	;Imprime caracter "U"
	movlw   'V'
	call    LCD_Char	;Imprime caracter "V"
	movlw   'A'
	call    LCD_Char	;Imprime caracter "A"
	movlw   ' '
	call    LCD_Char	;Imprime caracter " "
	movlw   '-'
	call    LCD_Char	;Imprime caracter "-"
	movlw   ' '
	call    LCD_Char	;Imprime caracter " "
	movlw   'L'
	call    LCD_Char	;Imprime caracter "L"
	movlw   'C'
	call    LCD_Char	;Imprime caracter "C"
	movlw   'D'
	call    LCD_Char	;Imprime caracter "D"

BUCLE
	goto	BUCLE

	INCLUDE <MiuvaLCD.inc>

    END


