#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <math.h>

unsigned frecuencia=65563;

void interrupt (*viejo)();

/*Configuramos el vector y realizamos el divisor de frecuencia*/
void configuracion(){
	int contador;
	int contador_lo;
	int contador_hi;

		/*Guardamos el viejo vector*/
		enable();
		viejo=getvect(0x1c) //Salvamos el viejo vector 
		setvect(0x1c,funcionInterrupe)
		disable();
		/*Configuramos el timer 8253*/
		contador=65563/(frecuencia/18.2);
		contador_hi=frecuencia/256;
		contador_lo=frecuencia%256;
		/*Ingresamos los valores a el contador*/
		outportb(0x43,0x36); /*CONTROL*/
		outportb(0x40, contador_hi);/*contador en alto*/
		outportb(0x40, contador_lo);/*contador en el byte bajo*/
}

void restablecervalores(){
		// Restablecemos el vector antiguo 
	     disable();
	     setvect(0x1c, viejo);
	     // configuramos el  timer 8253
	     outportb(0x43,0x36);
	     outportb(0x40,0xFF);
	     outportb(0x40,0XFF);

	     enable();
	}

void interrupt funcionInterrupe(){

}

int main()
{   
	int i=0, j=20;
	configuracion();
		while(i<j){

			i++;
		}

	restablecervalores();
 	return 0;
}