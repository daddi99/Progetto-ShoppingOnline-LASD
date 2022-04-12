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
    char caratterstica[20]; //Può essere ad esempio il colore,la misura (es. verde,corto,lungo,rosa ecc..)
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

//FUNZIONI PER L'INTERFACCIA
//Si occupa di gestire la schermata di login/registrazione, e ritorna per riferimento l'utente/amministratore che ha fatto l'accesso
int effettuaLogin(nodoListaUtenti* listaUtenti, nodoListaAmministratori* listaAmministratori,utente* utenteLoggato, amministratore* adminLoggato);
nodoListaUtenti* effettuaRegistrazione(nodoListaUtenti* listaUtenti); //Dopo aver fatto tutti i controlli aggiunge il nuovo utente alla listaUtenti
void latoUtente(utente* utenteLoggato,nodoListaUtenti** listaUtenti, nodoListaProdotti** listaProdotti);

//FUNZIONI LISTA PRODOTTI
void mostraListaProdotti(nodoListaProdotti* listaProdotti, int indice); //Stampa la lista sotto forma di elenco 
void stampaListaProdotti(nodoListaProdotti* listaProdotti); //Stampa tutti gli elememti della lista
nodoListaProdotti* creaNodoListaProdotti(char nomeProdotto[], char caratteristica[], float prezzo, int taglieS, int taglieM, int taglieL);
nodoListaProdotti* inserisciInCodaListaProdotti(nodoListaProdotti* lista, char nomeProdotto[], char caratterstica[], float prezzo, int taglieS, int taglieM, int taglieL);
nodoListaProdotti* popolaListaProdotti(nodoListaProdotti* listaProdotti);

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


#endif