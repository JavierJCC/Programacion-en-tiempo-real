#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <math.h>

void confi(void);
void restablecervalores(void);

unsigned frecuencia=65000;
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

int main()
{
	printf("Escribe la frecuencia que sea mayor a 18\n");
	scanf("%d", &val);
	if(val>18){
	confi();
	printf("\n");
		while(limitef<20){
		if(limite==1){
		    limite=0;
		    limiteaux++;
		    printf("Se ha realizado la interrupcion %d\n",limiteaux);
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