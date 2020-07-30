/*
 * ENCODER_SENZORI.c
 *
 * Created: 1/11/2020 7:03:34 PM
 * Author : Tomi Vlad
 */ 

#define F_CPU 16000000                              /* Frecventa de lucru microcontroler */

#define Baud 9600                                   /* Rata transmitere biti */
#define Baud_BRR ((F_CPU/16/Baud)-1)                /* UBRR (Valoare incarcata in registru )*/


#include <avr/io.h>
#include <util/delay1.h>                           /* Biblioteca modificata pentru delay */

/*Functii */

void Initializare_Porturi ();                     /* Portul D ca si Input */
void Initializare_Serial ();                      /*Initializare Serial */
void Trimitere_Mesaj ();                          /* Transmitere Vlaoare */



/* Cod Functii*/
void Initializare_Porturi()
{
	/* Setare intrare Pinii  PD4 si PD5 */
	
	DDRD =0x00;
		
}

void Initializare_Serial()
{
	/* Incarcare valoare Baud Rate */
	UBRR0H = (Baud_BRR<<8);
	UBRR0L = Baud_BRR;
	
	/*Setare Registrii */
	
	UCSR0B = ( 1<<TXEN0);  /*enable registru transmitere date */
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01) ;  /* * biti per caracter */
	
}

void Trimitere_Mesaj(const unsigned char *pointer )
{
	const unsigned char *String;
	String = pointer;
	while (*String != 0)
	{
		while ((UCSR0A & (1 << UDRE0)) == 0) {};
		UDR0 = *String++;
	}
}


int main(void)
{
	char buffer [20];                                     /* sir de caractere pentru a transforma din HEXA in Zecimal cu ajutorul functiei itoa() */
	int counter=0;                                        /* Constanta prin care numaram numarul de rotiri */
    Initializare_Porturi();                               /* Initializam portul D */
	Initializare_Serial();                                /* Initilaizam transmiterea seriala */
	int valoare_initiala_CLK= bit_is_clear(PIND,4);        /*Salvam intr-o constanta valoare initiala a portului A*/
	
	
	/*Bucla infinita de executie a programului */
    while (1) 
    {
		_delay_ms(1);
		int valoare_curenta_CLK=bit_is_clear(PIND,4);         /*Salvam din nou valoarea CLK-ului si verificam daca s-a facut vreo miscare */
		if(valoare_curenta_CLK!=valoare_initiala_CLK)          /*Daca s-a inregistrat ca s-a modificat valoarea Pinului D ,Atunci verificam in care parte s-a invartit */
		{
			if(bit_is_clear(PIND,5)!=valoare_curenta_CLK)       /* Daca valoarea pinului 2 e egala cu a Clock-ului e Clockwise ,altfel ConterClockwise*/
			{
				counter--;
			}
			else
			{
				counter++;
			}
			
			itoa(counter,buffer,10);                         /*Functie pentru a transforma valoarea hexazecimala a counter-ului intr-o valoare zecimala*/
			Trimitere_Mesaj(buffer);                         /*Transmiterea valorii din buffer */                        
			
			valoare_initiala_CLK=valoare_curenta_CLK;        /*Valoarea initiala a CLK-ului se va inlocui cu cea actuala */
		}
    }
}                                                        /* Sfarsitul programului */

