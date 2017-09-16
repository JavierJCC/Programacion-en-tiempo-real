#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

/*Variables globales*/
unsigned int i=1;
unsigned int segundos=0;
unsigned int limite;
unsigned int limitef=0;
unsigned int limiteaux=0;
unsigned int ciclos=0;
unsigned int anterior=0;
unsigned int Port;
unsigned int val=19;
unsigned char convh;
unsigned char convl;

/*Variables para el adq12*/
unsigned int OUTBR, DIRBAS, STINR, CTREG, ADHIG, ADLOW;
unsigned char Aalto;
unsigned char Bbajo;
unsigned char Cvalor;
float VD, VA;
unsigned char Cvalor2;

/*Prototipos de la funciones*/
void confi(void);
void restablecervalores(void);
void interrupt far (*viejo)();
void interrupt funcionInterrunpe();

/*Configuramos el vector y realizamos el divisor de frecuencia*/
void confi(){
	unsigned char contador_lo;
	unsigned char contador_hi;
	int contador;
		printf("configuramos");
		/*Guardamos el viejo vector*/
		disable();
		viejo=getvect(0x1c); //Salvamos el viejo vector
		setvect(0x1c,funcionInterrunpe);
		enable();
		/*Configuramos el timer 8253*/
		contador=1193800/val;
		contador_hi=contador>>8;
		contador_lo=contador;
		/*Ingresamos los valores a el contador*/
		outportb(0x43,0x36); /*CONTROL*/
		convl=inportb(0x40);
		convh=inportb(0x40);
		outportb(0x40, contador_lo);/*contador en alto*/
		outportb(0x40, contador_hi);/*contador en el byte bajo*/
}

void restablecervalores(){
	     printf("\nRestablecemos los valores");
		// Restablecemos el vector antiguo
	     disable();
	     setvect(0x1c, viejo);
	     // configuramos el  timer 8253
	     outportb(0x40,convl);
	     outportb(0x40,convh);

}

void interrupt funcionInterrunpe(){
	    limite=1;
}

int main (){
  DIRBAS=0x300; 	  STINR=DIRBAS+0;
  CTREG=DIRBAS+0;         ADHIG=DIRBAS+9;
  ADLOW=DIRBAS+8;         OUTBR=DIRBAS+4;

	printf("Escribe la frecuencia que sea mayor a 18\n");
	scanf("%d", &val);
	if(val>18){
	confi();
	printf("\n");
	/*Inicializamos todo para la convercion*/
	   outportb(CTREG,0x00); //Selecciona el canal de 0-5v
	   Cvalor=inportb(ADLOW); //Inicia la conversion
		while(limitef<20){
		if(limite==1){
		    limite=0;
		    limiteaux++;
		    Aalto=inportb(ADHIG);
		    Bbajo=inportb(ADLOW);
		    VD=Aalto*256+Bbajo;
		    VA=(float)VD*5/4096;
		    printf("Se ha realizado la interrupcion %d y el voltaje es de %f\n",limiteaux,VA);
		    if(limiteaux==val){
		    printf("Se han realizado %d interrupciones en un segundo %d \n", limiteaux, i);
		    limitef++;
		    i++;
		    limiteaux=0;
		    }

		}
		}
	printf("salimos");
	restablecervalores();
	}else{
	   printf("La frecuencia no es la adecuada");
	}
	getch();
	return 0;
}