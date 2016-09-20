/*
************************************************************************
	Manuel Rodriguez 27/07/2016
  Instalacion general: RASPBIAN
  Se han añadido las librerias wiringPi y lirc.
  persianas funciona como un proceso arrancado en systemctl y
  que espera conexiones en un socket de red puerto 104 y señales
  IR que provienen de un mando EUSKALTEL para controlar a las dos 
  por GPIO y SSR las persianas del SALON 
  28/08/2016	
  Añadidos cuatro pulsadores para manejo manual (INTERRPCIONES)
********************* PINES USADOS EN ESTE PROYECTO	*********************
	3.3VDC	01  VDC IR					5VDC	02  VCC SALIDAS
	------  03  ---------				5VDC	04  VCC SALIDAS
	------  05  ---------				0.0VDC	06	GND SALIDAS	
	GPIO04 	07  ENTRADA_IR				------  08  ---------
	0.0VDC	06	GND IR					------  10  ---------
	GPIO17 	11 	SALIDA_BAJAR_FRONTAL	------  12  ---------
	GPIO27 	13 	SALIDA_SUBIR_FRONTAL	0.0VDC	14	GND SALIDAS	
	GPIO22 	15  SALIDA_BAJAR_LATERAL	GPIO23 	16	SALIDA_SUBIR_LATERAL	
	------  17	-----------				GPIO24  18  ENTRADA_SUBIR_FRONTAL
	------  19  -----------				0.0VDC	20	GND ENTRADAS
	------  21  -----------				GPIO25	22	ENTRADA_BAJAR_FRONTAL
	------  23  -----------			 	GPIO08	24	ENTRADA_SUBIR_LATERAL
	------  25  -----------				GPIO07	26  ENTRADA_BAJAR_LATERAL 
************************************************************************
*/
#include "gpio.h"
/*
************************************************************************
*/
void handle_ENTRADA_SUBIR_FRONTAL(){
		bool valor = digitalRead(ENTRADA_SUBIR_FRONTAL);
		if(valor == 0){
		digitalWrite(SALIDA_BAJAR_FRONTAL,0);
		digitalWrite(SALIDA_SUBIR_FRONTAL,1);
		}else{
			digitalWrite(SALIDA_BAJAR_FRONTAL,0);
			digitalWrite(SALIDA_SUBIR_FRONTAL,0);
		}
}
/*
* ***********************************************************************
*/
void handle_ENTRADA_BAJAR_FRONTAL(){
		bool valor = digitalRead(ENTRADA_BAJAR_FRONTAL);
		if(valor == 0){
		digitalWrite(SALIDA_SUBIR_FRONTAL,0);
		digitalWrite(SALIDA_BAJAR_FRONTAL,1);	
		}else{
			digitalWrite(SALIDA_BAJAR_FRONTAL,0);
			digitalWrite(SALIDA_SUBIR_FRONTAL,0);
		}	
}
/*
************************************************************************
*/
void handle_ENTRADA_SUBIR_LATERAL(){
		bool valor = digitalRead(ENTRADA_SUBIR_LATERAL);
		if(valor == 0){
		digitalWrite(SALIDA_BAJAR_LATERAL,0);
		digitalWrite(SALIDA_SUBIR_LATERAL,1);
		}else{
			digitalWrite(SALIDA_BAJAR_LATERAL,0);
			digitalWrite(SALIDA_SUBIR_LATERAL,0);
		}	
}
/*
************************************************************************
*/
void handle_ENTRADA_BAJAR_LATERAL(){
		bool valor = digitalRead(ENTRADA_BAJAR_LATERAL);
		if(valor == 0){
		digitalWrite(SALIDA_SUBIR_LATERAL,0);
		digitalWrite(SALIDA_BAJAR_LATERAL,1);
		}else{
			digitalWrite(SALIDA_BAJAR_LATERAL,0);
			digitalWrite(SALIDA_SUBIR_LATERAL,0);
		}		
}
/*
************************************************************************
*/
void setupPins(){
	pinMode (SALIDA_BAJAR_FRONTAL, OUTPUT) ;
	pinMode (SALIDA_SUBIR_FRONTAL, OUTPUT) ;
	pinMode (SALIDA_BAJAR_LATERAL, OUTPUT) ;
	pinMode (SALIDA_SUBIR_LATERAL, OUTPUT) ;	
	pullUpDnControl (ENTRADA_SUBIR_FRONTAL,PUD_UP);
	pullUpDnControl (ENTRADA_BAJAR_FRONTAL,PUD_UP);
	pullUpDnControl (ENTRADA_SUBIR_LATERAL,PUD_UP);
	pullUpDnControl (ENTRADA_BAJAR_LATERAL,PUD_UP);
}
/*
**********************Siempre arranca/para apagando todo*********************
*/
void cleanUp(){
	digitalWrite(SALIDA_BAJAR_FRONTAL, 0);
	digitalWrite(SALIDA_SUBIR_FRONTAL, 0);
	digitalWrite(SALIDA_BAJAR_LATERAL, 0);
	digitalWrite(SALIDA_SUBIR_LATERAL, 0);
}
/*
* ***********************************************************************
*/
string PusPull(string PIN_ON,string PIN_OFF){
		digitalWrite(atoi(PIN_OFF.c_str()),0);
		digitalWrite(atoi(PIN_ON.c_str()),1);
	    return(SSTR(digitalRead(atoi(PIN_ON.c_str()) ) ));
}
/*
* ***********************************************************************
*/
string PullButt(string PIN_ON){
		digitalWrite(atoi(PIN_ON.c_str()),0);
	    return(SSTR(digitalRead(atoi(PIN_ON.c_str()))));
}
/*
* ***********************************************************************
*/
string ToggleBtt(string PIN_ON){
	   digitalWrite(atoi(PIN_ON.c_str()),!digitalRead(atoi(PIN_ON.c_str())));
	   return(SSTR(digitalRead(atoi(PIN_ON.c_str()))));
}
/*
* ***********************************************************************
*/
string GetStatus(){
	string value = SSTR(digitalRead(12))+SSTR(digitalRead(16))+SSTR(digitalRead(20))
			+SSTR(digitalRead(21))
			+SSTR(digitalRead(SALIDA_BAJAR_FRONTAL))+SSTR(digitalRead(SALIDA_SUBIR_FRONTAL))
			+SSTR(digitalRead(SALIDA_BAJAR_LATERAL))+SSTR(digitalRead(SALIDA_SUBIR_LATERAL));
	return value;
}
/*
* ***********************************************************************
*/

