#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <math.h>

void confi(void);
void restablecervalores(void);

unsigned frecuencia=65000;
unsigned int i=0;
unsigned int segundos=0;
unsigned int limite=0;
unsigned int limitef=0;
unsigned int ciclos=0;
unsigned int anterior=0;
unsigned int Port;
static int k=1;

void interrupt far (*viejo)();
void interrupt funcionInterrunpe();

/*Configuramos el vector y realizamos el divisor de frecuencia*/
void confi(){
	int contador_lo;
	int contador_hi;
	int contador;
		printf("configuramos");
		/*Guardamos el viejo vector*/
		disable();
		viejo=getvect(0x1c); //Salvamos el viejo vector
		setvect(0x1c,funcionInterrunpe);
		enable();
		/*Configuramos el timer 8253*/
		contador=65563/(frecuencia/18.2);
		contador_hi=contador&0xff00;
		contador_hi=contador_hi/256;
		contador_lo=frecuencia&0x00ff;
		/*Ingresamos los valores a el contador*/
		outportb(0x43,0x36); /*CONTROL*/
		outportb(0x40, contador_hi);/*contador en alto*/
		outportb(0x40, contador_lo);/*contador en el byte bajo*/
}

void restablecervalores(){
	     printf("\nRestablecemos los valores");
		// Restablecemos el vector antiguo
	     disable();
	     setvect(0x1c, viejo);
	     // configuramos el  timer 8253
	     outportb(0x43,0x36);
	     outportb(0x40,0xFF);
	     outportb(0x40,0XFF);

	}

void interrupt funcionInterrunpe(){
	if (k==1)
	{
	    outportb(Port,0x01);
	    //printf("Es 1- %d", inportb(Port));
	    if(limite==17){
		limitef++;
		limite=0;
	    }
	    limite++;
	    k=0;
	}else{
	    outportb(Port,0);
	    //printf("Es 2- %d", inportb(Port));
	    if(limite==17){
		limitef++;
		limite=0;
	    }
	    limite++;
	    k=1;
	}
}

int main()
{
	Port=peek(0x40,0x08);
	confi();
	anterior=segundos+1;
		while(limitef<20){
		   while((inportb(Port)&(0x01))==0);
		   while((inportb(Port)&(0x01))==1);

		}
	printf("salimos");
	restablecervalores();
	return 0;
}
