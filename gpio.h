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
#ifndef _gpio_h
#define _gpio_h
/*
* ***********************************************************************
*/
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>
#include <sstream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
# include <signal.h>
#define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str() 
/*
* ***********************************************************************
*/
using namespace std;
/*
* ***********************************************************************
*/
#define SALIDA_BAJAR_FRONTAL  17
#define SALIDA_SUBIR_FRONTAL  27
#define SALIDA_BAJAR_LATERAL  22
#define SALIDA_SUBIR_LATERAL  23
#define ENTRADA_SUBIR_FRONTAL 24
#define ENTRADA_BAJAR_FRONTAL 25
#define ENTRADA_SUBIR_LATERAL 8
#define ENTRADA_BAJAR_LATERAL 7
#define LOGFILE	"login.txt" 

void setupPins(); 
void cleanUp();
void handle_ENTRADA_SUBIR_FRONTAL();
void handle_ENTRADA_BAJAR_FRONTAL();
void handle_ENTRADA_SUBIR_LATERAL();
void handle_ENTRADA_BAJAR_LATERAL();
string PusPull(string PIN_ON,string PIN_OFF);
string PullButt(string PIN_ON);
string ToggleBtt(string PIN_ON);
string GetStatus();
/*
* ***********************************************************************
*/
#endif
