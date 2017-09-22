#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>

/*Variables globales*/
unsigned int grafica=0;
unsigned int i=0;
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
int gdriver=DETECT;
int gnode;
char *numeros[40]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29"};
char *temperatura[30]={"10","20","30","40","50","60","70","80","90","100","110","120"};
unsigned int OUTBR, DIRBAS, STINR, CTREG, ADHIG, ADLOW;
unsigned char Aalto;
unsigned char Bbajo;
unsigned char Cvalor;
float VD, VA;
float temp;
unsigned char Cvalor2;
float auxad=0;

/*Prototipos de la funciones*/
void confi(void);
void restablecervalores(void);
void interrupt far (*viejo)();
void interrupt funcionInterrunpe();
void coordenadasy(void);

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
	     //printf("\nRestablecemos los valores");
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
void coordenadasy(){
 int x;
       for(x=1;x<13;x++)
	outtextxy(0,x*20,temperatura[x-1]);
}

int main (){
  DIRBAS=0x300; 	  STINR=DIRBAS+0;
  CTREG=DIRBAS+0;         ADHIG=DIRBAS+9;
  ADLOW=DIRBAS+8;         OUTBR=DIRBAS+4;

	printf("Escribe la frecuencia que sea mayor a 18\n");
	scanf("%d", &val);
	printf("Escribe la frecuencia con la que se desea graficar\n");
	scanf("%d",&grafica);
	if(val>18){
	confi();
	printf("\n");
	/*Inicializamos todo para la convercion*/
	   outportb(CTREG,0x00); //Selecciona el canal de 0-5v
	   Cvalor=inportb(ADLOW); //Inicia la conversion
	   initgraph(&gdriver,&gnode,"C:\\BORLANDC\\BGI");
	   coordenadasy();
		while(limitef<21){
			if(limite==1){ //Entra a la bandera de la interrupcion
			    limite=0;
			    limiteaux++;
			    Aalto=inportb(ADHIG);
				Bbajo=inportb(ADLOW);
				VD=Aalto*256+Bbajo;
				VA=(float)VD*5/4096;
				temp=VA*24;
					if(temp<65){
					outportb(OUTBR,0x08);
					outportb(OUTBR,0x01);
					}
					if(temp>=85){  //Posibles datos no se vean en la grafica ya que estan dentro de la interrupcion
					  outportb(OUTBR,0x01);
					  outportb(OUTBR,0x00);
					}else if(temp>=65 && temp<85){
					  outportb(OUTBR,0x09);
					  outportb(OUTBR,0x08);
					}
						    if(limiteaux==grafica){		//Al termino de un segundo grafica
							    line(i*30,auxad,(1+i)*30,temp*2);
							    outtextxy(i*30,0,numeros[i]);
							    auxad=temp*2;
							    limitef++;
							    i++;
							    limiteaux=0;
						    }
			}
		}
	//printf("salimos");
	restablecervalores();
	}else{
	   printf("La frecuencia no es la adecuada");
	}
	getch();
	return 0;
}
