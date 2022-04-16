#ifndef SHOPPING_H
#define SHOPPING_H

//Codici per i colori del terminale
#define VERDE "\x1B[32m"
#define NORMALE "\x1B[0m"
#define ROSSO "\x1B[31m"
#define CIANO "\x1B[36m"
#define MAGENTA  "\x1B[35m"

#define SEPARATORE "--------------------------------------\n"

typedef struct utente
{
    char nomeUtente[20];
    char password[20];
    float saldo;
}utente;

typedef struct nodoListaUtenti
{
    struct utente utente;
    struct nodoListaUtenti* next;
}nodoListaUtenti;

typedef struct prodotto
{
    char nomeProdotto[20];
    char caratteristica[20]; //Può essere ad esempio il colore,la misura (es. verde,corto,lungo,rosa ecc..)
    float prezzo;
    int TaglieSdisponibili;
    int TaglieMdisponibili;
    int TaglieLdisponibili;
}prodotto;

typedef struct nodoListaProdotti
{
    struct prodotto prodotto;
    struct nodoListaProdotti* next;
}nodoListaProdotti;

typedef struct amministratore
{
    char nomeUtente[20];
    char password[20];
}amministratore;

typedef struct nodoListaAmministratori
{
    struct amministratore amministratore;
    struct nodoListaAmministratori* next;
}nodoListaAmministratori;

typedef struct nodoCarrello
{
    char nomeProdotto[20];
    char caratteristica[20];
    char tagliaRichiesta;
    float prezzo;
    struct nodoCarrello* next;
}nodoCarrello;

typedef struct elementoListaDiAttesa
{
    char nomeUtente[20];
    char nomeProdotto[20];
    char caratteristica[20];
    char taglia;
}prodottoCoda;

typedef struct nodoListaDiAttesa
{
    struct elementoListaDiAttesa elemento;
    struct nodoListaDiAttesa* next;
    struct nodoListaDiAttesa* prev;
}nodoListaDiAttesa;

typedef struct listaDiAttesa
{
    struct nodoListaDiAttesa* front;
    struct nodoListaDiAttesa* rear;
}listaDiAttesa;

//FUNZIONI PER L'INTERFACCIA
//Si occupa di gestire la schermata di login/registrazione, e ritorna per riferimento l'utente/amministratore che ha fatto l'accesso
int effettuaLogin(nodoListaUtenti* listaUtenti, nodoListaAmministratori* listaAmministratori,utente* utenteLoggato, amministratore* adminLoggato);
nodoListaUtenti* effettuaRegistrazione(nodoListaUtenti* listaUtenti); //Dopo aver fatto tutti i controlli aggiunge il nuovo utente alla listaUtenti
//Gestisce tutto il lato utente dell'applicazione
void latoUtente(utente* utenteLoggato,nodoListaUtenti** listaUtenti, nodoListaProdotti** listaProdotti, nodoCarrello** carrello, listaDiAttesa** listaDiAttesa);
//Gestisce tutto il lato amministrazione dell'applicazione
void LatoAmministrazione(amministratore* adminLoggato, nodoListaProdotti** listaProdotti);
/*Gestisce tutti i casi possibili quando si effettua un acquisto (Non si hanno abbastanza soldi, non ci sono taglie disponibli ecc.) e si occupa
di effettuare tutte le modifiche necessarie al saldo dell'utente, numero di taglie dei prodotti ecc. */
void gestisciAcquisto(nodoCarrello** carrello, char tagliaRichiesta, nodoListaUtenti** listaUtenti, listaDiAttesa** listaDiAttesa, utente* utenteLoggato, prodotto* prodottoDaAcquistare, int* numeroTaglieDisponibili);


//FUNZIONI LISTA PRODOTTI
prodotto* ottieniProdottoPerNome(nodoListaProdotti* listaProdotti, char nomeProdotto[], char caratteristica[]);
prodotto* ottieniProdottoDaIndice(nodoListaProdotti* listaProdotti, int posizione);
void mostraListaProdotti(nodoListaProdotti* listaProdotti, int indice); //Stampa la lista sotto forma di elenco 
void mostraListaProdottiAdmin(nodoListaProdotti* listaProdotti, int indice); //Stampa la lista sotto forma di elenco ma senza prezzo
void stampaListaProdotti(nodoListaProdotti* listaProdotti); //Stampa tutti gli elememti della lista
nodoListaProdotti* creaNodoListaProdotti(char nomeProdotto[], char caratteristica[], float prezzo, int taglieS, int taglieM, int taglieL);
nodoListaProdotti* inserisciInCodaListaProdotti(nodoListaProdotti* lista, char nomeProdotto[], char caratterstica[], float prezzo, int taglieS, int taglieM, int taglieL);
nodoListaProdotti* popolaListaProdotti(nodoListaProdotti* listaProdotti);


//FUNZIONI LISTA DI ATTESA (CODA)
void gestisciListaDiAttesa(listaDiAttesa** listaDiAttesa, nodoListaProdotti** listaProdotti, nodoCarrello** carrello,utente* utenteLoggato);
listaDiAttesa* creaListaDiAttesa(); //Crea la lista di attesa come una coda impostando front e rear a NULL
void inserisciInListaDiAttesa(listaDiAttesa** listaDiAttesa, char nomeProdotto[20], char caratteristica[20], char nomeUtente[20], char taglia);
void rimuoviElementoListaDiAttesa(listaDiAttesa** listaDiAttesa,  nodoListaDiAttesa* elemento);
void stampaListaDiAttesa(nodoListaDiAttesa* frontlistaDiAttesa);
listaDiAttesa* popolaListaDiAttesa(listaDiAttesa* listaDiAttesa);


//FUNZIONI LISTA CARRELLO
//Si occupa di far acquistare TUTTO il contenuto del carrello, modificando opportunamente tutti i campi. Successivamente dealloca la lista Carrello
void acquistaCarrello(utente* utenteLoggato, nodoListaUtenti** listaUtenti, nodoCarrello** carrello, nodoListaProdotti** listaProdotti);
float mostraProdottiCarrello(nodoCarrello* carrello, float prezzoIniziale); //Stampa il carrello e ritorna il prezzo totale
nodoCarrello* creaNodoListaCarrello(char nomeProdotto[], char caratteristica[], char tagliaRichiesta, float prezzo);
nodoCarrello* inserisciInCodaListaCarrello(nodoCarrello* lista, char nomeProdotto[], char caratterstica[], char tagliaRichiesta, float prezzo); 


//FUNZIONI LISTA AMMINISTRATORI
nodoListaAmministratori* popolaListaAmministratori(nodoListaAmministratori* listaAmministratori);
nodoListaAmministratori* inserisciInCodaListaAmministratori(nodoListaAmministratori* lista, char nomeUtente[], char password[]);
nodoListaAmministratori* creaNodoListaAmministratori(char nomeUtente[], char password[]);
void stampaListaAmministratori(nodoListaAmministratori* listaAmministratori);
int ricercaAmministratoreNellaLista(nodoListaAmministratori* listaAmministratori, amministratore* amministratoreOutput,char nomeUtente[], char password[]);


//FUNZIONI LISTA UTENTI
nodoListaUtenti* popolaListaUtenti(nodoListaUtenti* listaUtenti); //Inserisce all'interno della listaUtenti il contenuto del file utenti.txt
nodoListaUtenti* creaNodoListaUtenti(char nomeUtente[], char password[], float saldo);
nodoListaUtenti* inserisciInCodaListaUtenti(nodoListaUtenti* lista, char nomeUtente[], char password[], float saldo);
void stampaListaUtenti(nodoListaUtenti* listaUtenti);
int ricercaUtenteNellaLista(nodoListaUtenti* listaUtenti,utente* utenteOutput ,char nomeUtente[], char password[]); //Ricerca un utente nella lista di utenti
int ricercaUtentePerNome(nodoListaUtenti* listaUtenti, char nomeUtente[]);
//Utilizza il doppio puntatore in modo da modificare direttamente il saldo dell'utente nella lista senza doverla ritornare
void modificaSaldoNellaLista(nodoListaUtenti** listaUtenti, char nomeUtente[], float nuovoSaldo);

//FUNZIONI PERSISTENZA DEI DATI
void aggiornaFileUtenti(nodoListaUtenti* listaUtenti);
void aggiornaFileProdotti(nodoListaProdotti* listaProdotti);
void aggiornaFileListaDiAttesa(nodoListaDiAttesa* rearListaDiAttesa);



#endif