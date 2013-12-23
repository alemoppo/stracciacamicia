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

typedef struct
{
    char attuale, successivo;

}turno_t;

void aggiungi_carta(mazzo_t *mazzo, carta_t *carta, char modo);
void mixamazzi(mazzo_t *mazzo1, mazzo_t *mazzo2);
int nuovo_mazzo(mazzo_t *mazzo);
void mostramazzo(mazzo_t mazzo);
void alzamazzo(mazzo_t *mazzo_intero,mazzo_t *mazzo1,mazzo_t *mazzo2, int cartealzate);
void deallocamazzo(mazzo_t *mazzo);
int stracciacamicia(mazzo_t *mazzo1,mazzo_t *mazzo2,mazzo_t *tavola, char giocatore_iniziale);
carta_t *pesca(mazzo_t *mazzo);
void mischiamazzo(mazzo_t *mazzo);
void prossimo_turno(turno_t *turno);
char numero(char num_carta);
void salvamazzo(mazzo_t mazzo);
void carica_mazzo(mazzo_t *mazzo);

/*

fare i controlli nelle varie funzioni se il mazzo non ha 40 carte!

*/


int main()
{
    mazzo_t mazzo1, mazzo2, tavola, mazzo_intero;

    char scelta;
    int massimo = 0, i, ritorno;

    printf("1: carica mazzo\n2: genera mazzo\n\n scelta: ");
    scanf("%c", &scelta);
    if(scelta == '1')
        carica_mazzo(&mazzo_intero);
    else
        nuovo_mazzo(&mazzo_intero);

        mischiamazzo(&mazzo_intero);
        if(scelta != '1')
            salvamazzo(mazzo_intero);

        alzamazzo(&mazzo_intero, &mazzo1, &mazzo2, 20);
        ritorno = stracciacamicia(&mazzo1, &mazzo2, &tavola,  1);
        if(ritorno > massimo)
            massimo = ritorno;

    deallocamazzo(&mazzo_intero);
    printf("massimo: %d", massimo);

    return 0;
}

char numero(char num_carta)
{
    if(num_carta % 10 == 0)
        return 10;
    else return num_carta % 10;
}
int nuovo_mazzo(mazzo_t *mazzo)
{
    carta_t *nuova;
    char i;

    for(i=0;i<40;i++)
    {
        if((nuova = (carta_t*) malloc(sizeof(carta_t))))
        {
            nuova->numero = i;
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
        carte[(carta_corrente->numero)] = carta_corrente;
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

    pescata->successiva = NULL;

    return pescata;
}

void aggiungi_carta(mazzo_t *mazzo, carta_t *carta, char modo)
{
    if(modo == MODO_UP)
    {
        if(mazzo->ultima == NULL)
        {
            mazzo->ultima = carta;
            mazzo->prima = carta;
            carta->successiva = NULL;
        }
        else
        {
            carta->successiva = mazzo->prima;
            mazzo->prima = carta;
        }
    }
    else if(modo == MODO_DOWN)
    {
        if(mazzo->ultima == NULL)
        {
            mazzo->ultima = carta;
            mazzo->prima = carta;
        }
        else
            mazzo->ultima->successiva = carta;
            mazzo->ultima = carta;
            /*
            mazzo->ultima->successiva = carta;
        carta->successiva = NULL;
        */
    }
}

void mixamazzi(mazzo_t *mazzo1, mazzo_t *mazzo2)
{
    mazzo1->ultima->successiva = mazzo2->prima;
    mazzo1->ultima = mazzo2->ultima;
}

void prossimo_turno(turno_t *turno)
{
    char t;
    t = turno->attuale;
    turno->attuale = turno->successivo;
    turno->successivo = t;
}

int stracciacamicia(mazzo_t *mazzo1,mazzo_t *mazzo2,mazzo_t *tavola, char giocatore_iniziale)
{
    mazzo_t *temp;
    char carte_debito[2] = {0,0};
    turno_t turno;
    int passi=0;

    turno.attuale = 1;
    turno.successivo = 2;

    tavola->prima = NULL;
    tavola->ultima = NULL;

    if(giocatore_iniziale == 2)
    {
        temp = mazzo1;
        mazzo1 = mazzo2;
        mazzo2 = temp;
    }

    do
    {
        passi++;
        /**/
        if(turno.attuale == 1)
        {
            aggiungi_carta(tavola, pesca(mazzo1), MODO_DOWN);
        }
        else
        {
            aggiungi_carta(tavola, pesca(mazzo2), MODO_DOWN);
        }
        switch(numero(tavola->ultima->numero))
        {
            case 1:
            case 2:
            case 3:
                carte_debito[turno.attuale] = 0;
                carte_debito[turno.successivo] = numero(tavola->ultima->numero);
                prossimo_turno(&turno);
                break;
            default:

                if(carte_debito[turno.attuale] > 0)
                {
                    carte_debito[turno.attuale]--;
                    if(carte_debito[turno.attuale] == 0 && numero(tavola->ultima->numero%10) > 3)
                    {
                        if(turno.attuale == 1)
                            mixamazzi(mazzo2, tavola);
                        else mixamazzi(mazzo1, tavola);
                        tavola->ultima = NULL;
                        prossimo_turno(&turno);
                    }
                }
                else prossimo_turno(&turno);
                break;
        }

      /*  mostramazzo(*mazzo1);
        mostramazzo(*mazzo2);
        mostramazzo(*tavola);
        printf("\nProssima mano: \n");
*/
        /**/
    } while((mazzo1->prima != NULL) && (mazzo2->prima != NULL));    //il gioco finisce quando non ci son più carte in tavola! (ovvero quando dopo una ripescata, il prossimo giocatore gioca la carta NULL


	return passi;
}

void salvamazzo(mazzo_t mazzo)
{
    FILE *fpout;
    carta_t *carta;
    char nomefile[40];

    printf("Nome file da salvare: ");
    scanf("%s", nomefile);
    fpout = fopen(nomefile, "w");
    if(!fpout)
        printf("impossibile accedere al file");

    carta = mazzo.prima;

    while(carta->successiva != NULL)
    {
        fprintf(fpout, "%d ", carta->numero);
        carta = carta->successiva;
    }
    fclose(fpout);
}

void carica_mazzo(mazzo_t *mazzo)
{
    FILE *fpin;
    carta_t *nuova;
    char i, nomefile[40];

    printf("Nome file da caricare: ");
    scanf("%s", nomefile);

    fpin = fopen(nomefile, "r");
    if(!fpin)
    {
        printf("impossibile accedere al file");
        EXIT_FAILURE;
    }
    for(i=0;i<40 && !feof(fpin);i++)
    {
        if((nuova = (carta_t*) malloc(sizeof(carta_t))))
        {
            fscanf(fpin,"%d ", &(nuova->numero));

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
}
 /*
 int nuovo_mazzo(mazzo_t *mazzo)
{
    carta_t *nuova;
    char i;

    for(i=0;i<40;i++)
    {
        if((nuova = (carta_t*) malloc(sizeof(carta_t))))
        {
            nuova->numero = i;
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
} */
