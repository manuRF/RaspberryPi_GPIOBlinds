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
***********************************************************************
*/
#ifndef _persianas_h
#define _persianas_h
/*
* ***********************************************************************
*/
#include <wiringPi.h>
#include <lirc/lirc_client.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#define PORT 104
/*
* ***********************************************************************
*/
using namespace std;
/*
* ***********************************************************************
*/
void proc_net (int sock);
void kb_handler(int s);
void proc_lirc(void);
/*
* ***********************************************************************
*/
#endif
