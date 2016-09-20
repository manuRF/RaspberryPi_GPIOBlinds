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
# include "persianas.h"
# include "gpio.h"
ofstream logfile;
/*
* ***********************************************************************
*/ 
int main(int argc, char *argv[])
{
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = kb_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
	int sockfd, newsockfd, portno;
	pid_t pid_net, pid_ir;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	struct lirc_config *config;
	
	logfile.open (LOGFILE);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
	// Inicializacion para el GPIO de salida de RF
    if (wiringPiSetupGpio () == -1)
		{
			logfile<<"Fallo al activar libreria wiringPi"<<endl;
			exit (EXIT_FAILURE) ;
		} else {
				setupPins();
				cleanUp();
		}
       	
	// Para matar los zombies del forck
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) 
		{
			logfile<< "error" << SIG_ERR <<endl;
			exit(EXIT_FAILURE);
		}
		// Initiate net socket for Android clients.
	if (sockfd < 0)
		{
			logfile<<"ERROR abriendo el socket"<<endl;
			exit(EXIT_FAILURE);
		}   
		
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = PORT;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        logfile<<"ERROR enlazando al puerto :"<< portno<<endl;
	else logfile<< "Socket creado y enlazado en el puerto :"<< portno<<endl;	
    listen(sockfd,5);
    clilen = sizeof(cli_addr);	
			logfile<< "net socket inicializado OK"<<endl;
	//Initiate LIRC. Exit on failure
    if(lirc_init((char*)"lirc",1)==-1)
		{
		logfile<<"Fallo inicializando lirc"<<endl;
		exit(EXIT_FAILURE);
		}
     //Read the default LIRC config at /etc/lirc/lircd.conf  This is the config for your remote.
	bool lircdresult = lirc_readconfig(NULL,&config,NULL);
	if(lircdresult==0)		
		logfile<<"Lectura de lircd.conf cargada y OK:"<<lircdresult<<endl;		
 	else logfile<<"fallo en la lectura de lircd.conf"<<lircdresult<<endl;
	// Read push-buttons INTERRUPTS
	wiringPiISR(ENTRADA_SUBIR_FRONTAL, INT_EDGE_BOTH, &handle_ENTRADA_SUBIR_FRONTAL);
	wiringPiISR(ENTRADA_BAJAR_FRONTAL, INT_EDGE_BOTH, &handle_ENTRADA_BAJAR_FRONTAL);
	wiringPiISR(ENTRADA_SUBIR_LATERAL, INT_EDGE_BOTH, &handle_ENTRADA_SUBIR_LATERAL);
	wiringPiISR(ENTRADA_BAJAR_LATERAL, INT_EDGE_BOTH, &handle_ENTRADA_BAJAR_LATERAL);

	pid_ir = fork();
	if(pid_ir < 0)
		logfile<<"ERROR en la creacion de proc_net IR"<<endl;
	// Lanza el proceso LIRC de entrada del DIODO IR
	if(pid_ir == 0)
		{
			proc_lirc();
			exit(0);
		}
	while (1) 
	{
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		logfile<<"aceptada la conexion"<<endl;
		if (newsockfd < 0)
			logfile<<"ERROR en la aceptacion"<<endl;
		pid_net = fork();
		if (pid_net < 0)
			logfile<<"ERROR en la creacion de proc_net NET"<<endl;
		if (pid_net == 0)  
			{
			close(sockfd);
			proc_net(newsockfd);
			exit(0);
		}else close(newsockfd);
				
	} // end of while
		//Frees the data structures associated with config.
		lirc_freeconfig(config);
		exit(EXIT_FAILURE);
}
/*
* ***********************************************************************
*/
void proc_net (int sock)
{
	logfile << "entra en tarea" <<endl ;
   int n;
   char comando[14];
   bzero(comando,14);
   n = read(sock,comando,sizeof(comando));
   if (n < 0) logfile<<"ERROR reading from socket"<<endl;
   else logfile<<"tarea comando :"<<comando<<endl;
   string COMANDO = comando;
   //Obtengo una subcadena a partir del caracter 0 y con una longitud de 9
   string TIPO=COMANDO.substr(0,9);
   if(TIPO=="GETSTATUS"){
		string value = GetStatus();
		logfile <<"GETSTATUS:"<<value.c_str()<<endl;
		send(sock,value.c_str(),value.size(),MSG_CONFIRM);    
   }else{
		string TIPO=COMANDO.substr(0,9);
		string PIN_ON=COMANDO.substr(9,2);
		string PIN_OFF=COMANDO.substr(11,2);
		logfile<<"COMANDO:"<<COMANDO<<"PIN_ON:"<<PIN_ON<<" PIN_OFF:"<<PIN_OFF<<endl;
		if (TIPO=="PUSHPULL "){
				string value = PusPull(PIN_ON,PIN_OFF);
				logfile <<"PUSHPULL :"<<value.c_str()<<endl;
				send(sock,value.c_str(),value.size(),MSG_CONFIRM);    
		}else if (TIPO=="PULLBUTT "){
				string value = PullButt(PIN_ON);
				logfile <<"PULLBUTT:"<<value.c_str()<<endl;
				send(sock,value.c_str(),value.size(),MSG_CONFIRM);    
		}
   }
} 	
/*
* ***********************************************************************
*/
void proc_lirc(void)
{
	int buttonTimer = millis();
	char *code;
    char *c;
	
	while(lirc_nextcode(&code)==0)
	{	
	    //If code = NULL, meaning nothing was returned from LIRC socket,
        //then skip lines below and start while loop again.		
        if(code==NULL) continue;
		{
            //Make sure there is a 400ms gap before detecting button presses.
        if (millis() - buttonTimer  > 400)
		    {
				logfile<<"codigo encontrado:"<<code<<endl;
                if(strstr (code,"KEY_GREEN")) 
					{
						logfile<<"Econtrado  KEY_GREEN: SUBIR FRONTAL "<<endl;
						digitalWrite(SALIDA_BAJAR_FRONTAL,0);
						digitalWrite(SALIDA_SUBIR_FRONTAL,!digitalRead(SALIDA_SUBIR_FRONTAL));
						buttonTimer = millis();
                    }
                else if(strstr (code,"KEY_RED"))
					{
						logfile<<"Econtrado  KEY_RED: BAJAR FRONTAL"<<endl;
						digitalWrite(SALIDA_SUBIR_FRONTAL,0);
						digitalWrite(SALIDA_BAJAR_FRONTAL,!digitalRead(SALIDA_BAJAR_FRONTAL));
						buttonTimer = millis();
					}
                else if(strstr (code,"KEY_BLUE"))
					{
						logfile<<"Econtrado  KEY_BLUE: SUBIR LATERAL"<<endl;
						digitalWrite(SALIDA_BAJAR_LATERAL,0);
						digitalWrite(SALIDA_SUBIR_LATERAL,!digitalRead(SALIDA_SUBIR_LATERAL));
						buttonTimer = millis();
                    }
				else if(strstr (code,"KEY_YELLOW"))
					{
						logfile<<"Econtrado  KEY_YELLOW :BAJAR LATERAL"<<endl;
						digitalWrite(SALIDA_SUBIR_LATERAL,0);
						digitalWrite(SALIDA_BAJAR_LATERAL,!digitalRead(SALIDA_BAJAR_LATERAL));						
						buttonTimer = millis();
					}
			}
			logfile<<"saliendo de en proc_lirc() desde code = NULL"<<endl;
           //Need to free up code before the next loop
		}
            free(code);		
    } 
	logfile<<"Saliendo solo si se a pulsado IR"<<endl;
}
/*
* ***********************************************************************
*/
void kb_handler(int s){
           printf("Interrupcion de teclado %d\n",s);
		   cleanUp();
		   logfile.close();
           exit(1); 
}
/*
* ***********************************************************************
*/

