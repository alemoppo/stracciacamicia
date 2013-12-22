#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SPADI 0
#define BASTONI 1
#define COPPE 2
#define DENARI 4

#define MODO_UP 1
#define MODO_DOWN 2

typedef struct
{
    struct carta_t *successiva;
    char numero;
}carta_t;

typedef struct
{
    carta_t *prima;
    carta_t *ultima;

}mazzo_t;

void aggiungi_carta(mazzo_t *mazzo, carta_t *carta, char modo);
void mixamazzi(mazzo_t *mazzo1, mazzo_t *mazzo2);
int nuovo_mazzo(mazzo_t *mazzo);
void mostramazzo(mazzo_t mazzo);
void alzamazzo(mazzo_t *mazzo_intero,mazzo_t *mazzo1,mazzo_t *mazzo2, int cartealzate);
void deallocamazzo(mazzo_t *mazzo);
char stracciacamicia(mazzo_t *mazzo1,mazzo_t *mazzo2,mazzo_t *tavola, char giocatore_iniziale);
carta_t *pesca(mazzo_t *mazzo);
void mischiamazzo(mazzo_t *mazzo);

int main()
{
    mazzo_t mazzo1, mazzo2, tavola, mazzo_intero;

    nuovo_mazzo(&mazzo_intero);
    mischiamazzo(&mazzo_intero);
    alzamazzo(&mazzo_intero, &mazzo1, &mazzo2, 20);
    stracciacamicia(&mazzo1, &mazzo2, &tavola,  1);

	//dealloco: non è necessario deallocare gli altri mazzi perché son le stesse carte!
    deallocamazzo(&mazzo_intero);

    return 0;
}

int nuovo_mazzo(mazzo_t *mazzo)
{
    carta_t *nuova;
    char i;

    for(i=0;i<40;i++)
    {
        if((nuova = (carta_t*) malloc(sizeof(carta_t))))
        {
            nuova->numero = i+1;
            if(!i)
            {
                mazzo->prima = nuova;
                mazzo->ultima = nuova;
            }
            mazzo->ultima->successiva = nuova;
            mazzo->ultima = nuova;
            mazzo->ultima->successiva = NULL;
        }
        else EXIT_FAILURE;
    }
    return 0;
}

void mostramazzo(mazzo_t mazzo)
{
    char i=0;

    carta_t *corrente;
    corrente = mazzo.prima;
    printf("Mazzo: ");
    while(corrente != NULL)
    {
        i++;
        printf("%d", corrente->numero);
        if(corrente->successiva != NULL)
            printf(", ");
        corrente = corrente->successiva;
    }
    printf("\n\n----------\n\n");
}
void mischiamazzo(mazzo_t *mazzo)
{
    carta_t *carte[40], *carta_corrente;
    char i=0, temp, a, b;

    carta_corrente = (carta_t*) malloc(sizeof(carta_t));
    carta_corrente = mazzo->prima;

    while(1)
    {
        carte[(carta_corrente->numero)-1] = carta_corrente;
        if(carta_corrente->successiva != NULL)
            carta_corrente = carta_corrente->successiva;
        else break;
    }
    srand(time(0));
    for(i=0;i<120;i++)   //numero mischiamenti
    {
        a = RandomIntInRange(0,40);
        b = RandomIntInRange(0,40);

        temp = carte[a]->numero;
        carte[a]->numero = carte[b]->numero;
        carte[b]->numero = temp;
    }
    free(carta_corrente);

}

void alzamazzo(mazzo_t *mazzo_intero,mazzo_t *mazzo1,mazzo_t *mazzo2, int cartealzate)
{
    char i;
    carta_t *carta_corrente;
    carta_corrente = (carta_t*) malloc(sizeof(carta_t));

    mazzo1->prima = carta_corrente->successiva = mazzo_intero->prima;
    for(i=0;i<cartealzate;i++)
        carta_corrente = carta_corrente->successiva;

    mazzo1->ultima = carta_corrente;

    mazzo2->prima = carta_corrente->successiva;

    mazzo1->ultima->successiva = NULL;

    mazzo2->ultima = mazzo_intero->ultima;

    mazzo2->ultima->successiva = NULL; //dovrebbe funzionare anche senza questa riga
}

void deallocamazzo(mazzo_t *mazzo)
{
    carta_t *successiva, *attuale;
    successiva = (carta_t*) malloc(sizeof(carta_t));
    attuale = mazzo->prima;
    successiva = mazzo->prima->successiva;
    while(successiva != NULL)
    {
        free(attuale);
        attuale = successiva;
        successiva = successiva->successiva;
    }
}
char valore(char carta)
{
    return (carta%10)+1;
}
 char seme(char carta)  //penso funzione inutile
 {
     return carta/10;
 }

int RandomIntInRange(int low, int high)
{
   return (int)(rand() / (RAND_MAX / (double)(high - low))) + low;
}

carta_t *pesca(mazzo_t *mazzo)
{
	carta_t *pescata = mazzo->prima;
	mazzo->prima = mazzo->prima->successiva;

/*
    char i;
    carta_t *attuale;
    carta_t *pescata;
    carta_t *successiva;

    pescata = mazzo->ultima;
    attuale = mazzo->prima;

    do
    {
        if(attuale->successiva != NULL)
        {
            successiva = attuale->successiva;
            if(successiva->successiva == NULL)
                attuale->successiva = NULL;
        }
		*/

        /*
        if(attuale->successiva != NULL)
            if(attuale->successiva->successiva == NULL)
                attuale->successiva = NULL;
        */
        /*
        if(attuale->successiva == NULL)
            attuale = NULL;
        else attuale = attuale->successiva;
        *
    } while(attuale->successiva != NULL);
	*/
    pescata->successiva = NULL;

    return pescata;
}

void aggiungi_carta(mazzo_t *mazzo, carta_t *carta, char modo)
{
    if(modo == MODO_DOWN)
    {
        if(mazzo->ultima == NULL)
        {
            mazzo->ultima = carta;
            mazzo->prima = carta;
            carta->successiva = NULL;
        }
        carta->successiva = mazzo->prima;
        mazzo->prima = carta;
    }
    else if(modo == MODO_UP)
    {
        if(mazzo->ultima == NULL)
        {
            mazzo->ultima = carta;
            mazzo->prima = carta;
        }
        else
            mazzo->ultima->successiva = carta;
        carta->successiva = NULL;
    }
}

void mixamazzi(mazzo_t *mazzo1, mazzo_t *mazzo2)
{
    mazzo1->ultima->successiva = mazzo2->prima;
}

char stracciacamicia(mazzo_t *mazzo1,mazzo_t *mazzo2,mazzo_t *tavola, char giocatore_iniziale)
{
    mazzo_t *temp;

    tavola->prima = NULL;
    tavola->ultima = NULL;

    if(giocatore_iniziale == 2)
    {
        temp = mazzo1;
        mazzo1 = mazzo2;
        mazzo2 = temp;
    }

    mostramazzo(*mazzo1);

    aggiungi_carta(tavola, pesca(mazzo1), MODO_UP); 
  

    mostramazzo(*mazzo1);
    mostramazzo(*tavola);
	return 0;
}
