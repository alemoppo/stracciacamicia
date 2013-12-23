#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
#define SPADI 0
#define BASTONI 1
#define COPPE 2
#define DENARI 4
*/

#define MODO_UP 1
#define MODO_DOWN 2

#ifndef EXIT_FAILURE
#define EXIT_FAILURE exit(-1)
#endif

#define MAZZO_STRACCIACAMICIA 1

/*errori: */
#define MAZZO_NON_CONOSCIUTO    1
#define MAZZO_ULTIMA_ERRATA     1<<1
#define MAZZO_NUM_CARTE_ERRATO  1<<2
#define MAZZO_OK                1<<3
#define MAZZO_CARTE_DOPPIE      1<<4
#define MAZZO_CARTA_ERRATA      1<<5

/* */

/*parametri programma*/
#define NUMERO_MISCHIAMENTI 120

/* */

typedef struct
{
    struct carta_t *successiva;
    char numero;

} carta_t;

typedef struct
{
    carta_t *prima;
    carta_t *ultima;

} mazzo_t;

typedef struct
{
    char attuale, successivo;

} turno_t;

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
int RandomIntInRange(int low, int high);
char menu();
int num_partite();
void copia_mazzo(mazzo_t mazzo, mazzo_t *nuovo_mazzo);
char controlla_mazzo(mazzo_t mazzo,char tipo_mazzo);
void verifica_mazzo(mazzo_t mazzo, char tipo_mazzo, char messaggio_errore[255]);

int main()
{
    mazzo_t mazzo1, mazzo2, tavola, mazzo_intero, mazzo_temp, mazzo_max;
    int ritorno, num_partite_var=1, i, max_lunghezza=0;
    char scelta, convalida;

    scelta = menu();

    if(scelta == '1')
    {
        carica_mazzo(&mazzo_intero);
        verifica_mazzo(mazzo_intero, MAZZO_STRACCIACAMICIA, "Mazzo caricato non valido!");
    }
    else
    {
        num_partite_var = num_partite();
    }

    for(i=0; i<num_partite_var; i++)
    {
        if(scelta != '1')
        {
            nuovo_mazzo(&mazzo_intero);
            verifica_mazzo(mazzo_intero, MAZZO_STRACCIACAMICIA, "Mazzo generato non valido!");
            mischiamazzo(&mazzo_intero);
            verifica_mazzo(mazzo_intero, MAZZO_STRACCIACAMICIA, "Mazzo mischiato non valido!");
//            copia_mazzo(mazzo_intero, &mazzo_temp);
//            verifica_mazzo(mazzo_temp, MAZZO_STRACCIACAMICIA, "Mazzo copiato non valido!");
        }
        alzamazzo(&mazzo_intero, &mazzo1, &mazzo2, 20);
        ritorno = stracciacamicia(&mazzo1, &mazzo2, &tavola,  1);
        printf("giocate: %d\n", ritorno);
        if(ritorno > max_lunghezza)
        {
            max_lunghezza = ritorno;
 //           mazzo_max = mazzo_temp;
        }
    }
    printf("\nmassimo: %d\n", max_lunghezza);
 //   verifica_mazzo(mazzo_temp, MAZZO_STRACCIACAMICIA, "Mazzo max non valido!");
//    mostramazzo(mazzo_max);
//    salvamazzo(mazzo_max);
    deallocamazzo(&mazzo1);
    deallocamazzo(&mazzo2);
    deallocamazzo(&tavola);
    //deallocamazzo(&mazzo_intero); //inutile: le carte son le stesse!
//    deallocamazzo(&mazzo_temp);
//    deallocamazzo(&mazzo_max);


    return 0;
}
void verifica_mazzo(mazzo_t mazzo, char tipo_mazzo, char messaggio_errore[255])
{
    char convalida = controlla_mazzo(mazzo, tipo_mazzo);
    if(convalida != MAZZO_OK)
    {
        printf("\n\nErrore: %s. Riferimento: %d\n\n",messaggio_errore, convalida);
        exit(convalida);
    }
}
char controlla_mazzo(mazzo_t mazzo,char tipo_mazzo)
{
    char i=0, carte[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    carta_t *corrente = NULL;
    switch(tipo_mazzo)
    {
    case MAZZO_STRACCIACAMICIA:
        do
        {
            i++;
            if(corrente == NULL)
                corrente = mazzo.prima;

            if(corrente->numero < 0 || corrente->numero > 39)
            {
                printf("Carta non conosciuta: %d", corrente->numero);
                return MAZZO_CARTA_ERRATA;
            }


            if(carte[corrente->numero] == 0)
            {
                carte[corrente->numero] = 1;
            }
            else
            {
                printf("Carta doppia: %d", corrente->numero);
                return MAZZO_CARTE_DOPPIE;
            }

            if(corrente->successiva == NULL)
                if(mazzo.ultima != corrente)
                    return MAZZO_ULTIMA_ERRATA;

            corrente = corrente->successiva;

        }
        while(corrente != NULL);
        if(i != 40)
        {
            printf("Numero carte invalido: %d", i);
            return MAZZO_NUM_CARTE_ERRATO;
        }

        return MAZZO_OK;

        break;
    default:
        return MAZZO_NON_CONOSCIUTO;
        break;
    }
    return 0;
}

void copia_mazzo(mazzo_t mazzo, mazzo_t *nuovo_mazzo)
{
    carta_t *precedente=NULL, *corrente, *nuova;

    corrente = mazzo.prima;

    while(corrente->successiva != NULL)
    {
        nuova = (carta_t*) malloc(sizeof(carta_t));
        nuova->numero = corrente->numero;

        if(precedente != NULL)
            precedente->successiva = nuova;
        else
            nuovo_mazzo->prima = nuova;

        precedente = nuova;

        if(corrente->successiva != NULL)
            corrente = corrente->successiva;

        if(corrente->successiva == NULL)
        {
            nuovo_mazzo->ultima = nuova;
            nuova->successiva = NULL;
        }
    }
}

char menu()
{
    char scelta;
    printf("1: carica mazzo\n2: genera mazzo\n\n scelta: ");
    scanf("%c", &scelta);

    return scelta;
}

int num_partite()
{
    int partite;
    printf("\nNumero di round da giocare: ");
    scanf("%d", &partite);

    return partite;
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

    for(i=0; i<40; i++)
    {
        if((nuova = (carta_t*) malloc(sizeof(carta_t))))
        {
            nuova->numero = i;
            if(!i)
            {
                mazzo->prima = nuova;
                mazzo->ultima = nuova;
            }
            else
            {
                mazzo->ultima->successiva = nuova;
                mazzo->ultima = nuova;
                mazzo->ultima->successiva = NULL;
            }
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
    static char randomincrementale=0;
    carta_t *carte[40], *carta_corrente;
    char i=0, temp, a, b;

    carta_corrente = (carta_t*) malloc(sizeof(carta_t));
    carta_corrente = mazzo->prima;

    while(1)
    {
        carte[(int)(carta_corrente->numero)] = carta_corrente;
        if(carta_corrente->successiva != NULL)
            carta_corrente = carta_corrente->successiva;
        else break;
    }

    /* */
    srand(time(NULL)*clock()+randomincrementale);        //rifare meglio il generatore random!
    if(randomincrementale++ > 100)
        randomincrementale = 0;
    /* */

    for(i=0; i<NUMERO_MISCHIAMENTI; i++) //numero mischiamenti
    {
        a = RandomIntInRange(0,40);
        b = RandomIntInRange(0,40);

        temp = carte[(int)a]->numero;
        carte[(int)a]->numero = carte[(int)b]->numero;
        carte[(int)b]->numero = temp;
    }
}

void alzamazzo(mazzo_t *mazzo_intero,mazzo_t *mazzo1,mazzo_t *mazzo2, int cartealzate)
{
    char i;
    carta_t *carta_corrente;
    carta_corrente = (carta_t*) malloc(sizeof(carta_t));

    mazzo1->prima = carta_corrente->successiva = mazzo_intero->prima;
    for(i=0; i<cartealzate; i++)
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
    if(mazzo->prima != NULL)
    {
        attuale = mazzo->prima;
        successiva = mazzo->prima->successiva;
        while(successiva != NULL)
        {
            free(attuale);
            attuale = successiva;
            successiva = successiva->successiva;
        }
    }
}
char valore(char carta)
{
    return (carta%10)+1;
}
char seme(char carta)
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
        if(turno.attuale == 1)
            aggiungi_carta(tavola, pesca(mazzo1), MODO_DOWN);
        else
            aggiungi_carta(tavola, pesca(mazzo2), MODO_DOWN);
        switch(numero(tavola->ultima->numero))
        {
        case 1:
        case 2:
        case 3:
            carte_debito[(int)turno.attuale] = 0;
            carte_debito[(int)turno.successivo] = numero(tavola->ultima->numero);
            prossimo_turno(&turno);
            break;
        default:

            if(carte_debito[(int)turno.attuale] > 0)
            {
                carte_debito[(int)turno.attuale]--;
                if(carte_debito[(int)turno.attuale] == 0 && numero(tavola->ultima->numero%10) > 3)
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
    }
    while((mazzo1->prima != NULL) && (mazzo2->prima != NULL));      //il gioco finisce quando non ci son più carte in tavola! (ovvero quando dopo una ripescata, il prossimo giocatore gioca la carta NULL


    return passi;
}

void salvamazzo(mazzo_t mazzo)
{
    FILE *fpout;
    carta_t *carta;
    char nomefile[40];

    char i=0;

    printf("Nome file da salvare: ");
    scanf("%s", nomefile);

    fpout = fopen(nomefile, "w");
    if(!fpout)
    {
        printf("impossibile accedere al file");
        EXIT_FAILURE;
    }
    carta = mazzo.prima;

    while(carta->successiva != NULL)
    {
        i++;
        fprintf(fpout, "%d ", carta->numero);
        carta = carta->successiva;
    }
    printf("Salvati %d elementi", i);
    fprintf(fpout, "%d ", carta->numero);   //nel while controlla la successiva, ma questa attuale devo salvarla :v

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
    for(i=0; i<40 && !feof(fpin); i++)
    {
        if((nuova = (carta_t*) malloc(sizeof(carta_t))))
        {
            fscanf(fpin,"%d ", &(nuova->numero));

            if(!i)
            {
                mazzo->prima = nuova;
                mazzo->ultima = nuova;
            }
            else
            {
                mazzo->ultima->successiva = nuova;
                mazzo->ultima = nuova;
                mazzo->ultima->successiva = NULL;
            }
        }
        else EXIT_FAILURE;
    }
}

