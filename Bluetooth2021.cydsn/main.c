/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "LiquidCrystal_I2C.h"

uint8_t Addr = 0x27;/*Direccion del modulo I2C es 0x27 si usa el integrado PCF8574 ó 0x3F si usa el PCF8574A 
                    ver Datasheet del modulo*/
uint8_t fila = 0, columna = 0;
char dato;


CY_ISR(InterruptRX) /*Genero la rutina de interrupcion cuyo nombre de vector de interrupcion es InterruptRX*/
{
    if((fila == 0)&&(columna == 16))
    {
        fila = 1;   /*al finalizar la primera fila, inica la segunda fila */
        columna = 0;
    }
    if((fila == 1)&&(columna == 16))
    {
        fila = 0;   /*al finalizar en la segunda fila, y ultima columna regresa al inicio */
        columna = 0;
        clearDisplay();    /*funcion de la libreria LiquidCrystal_I2C.h*/
    }
    dato = UART_GetChar(); /*Toma el dato recibido por la UART desde el modulo Bluetooth y lo guarda en dato */
    setCursor(columna, fila); /*funcion de la libreria LiquidCrystal_I2C.h*/
    LCD_printChar(dato); /*funcion de la libreria LiquidCrystal_I2C.h: imprime el dato recibido en el LCD*/
    columna = columna + 1;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    LiquidCrystal_I2C_init(Addr, 16, 2, 0); /*Inicializa configuracion del modulo LCD para la API LiquidCrystal
                                            Parámetros de la funcion: Direccion, Columnas, Filas, Tamaño de caracter*/
    Clock_Start();
    PWM_Start();
    I2C_Start();
    UART_Start();
    RXisr_StartEx(InterruptRX); /*Configura la interrupción para que funcione y establece la dirección 
                                como el vector ISR para la interrupción: Nombre del vector de interrupcion se define 
                                InterruptRX como*/
    begin(); /*inicializa el LCD con el modulo I2C */
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
