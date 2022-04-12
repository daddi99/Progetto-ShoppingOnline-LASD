#include <stdio.h>
#include "shopping.h"

int main()
{
    nodoListaUtenti* listaUtenti = NULL; //Lista contentente tutti gli utenti registrati
    nodoListaAmministratori* listaAmministratori = NULL; //Lista contenente tutti gli amministratori
    nodoListaProdotti* listaProdotti = NULL; //Lista contenente tutti i prodotti disponibili
    utente utenteLoggato;   //Il contenuto verrà modificato per riferimento nella funzione effettuaLogin(). Conterrà i dati dell'utente che ha fatto l'accesso.
    amministratore adminLoggato; 
    int areaDiAccesso; //1 = Lato Utente, 2 = Lato Amministrazione

    //Popola tutte le liste con il contenuto dei rispettivi file
    listaUtenti = popolaListaUtenti(listaUtenti);
    listaAmministratori = popolaListaAmministratori(listaAmministratori);
    listaProdotti = popolaListaProdotti(listaProdotti);

    //Loop principale della schermata iniziale, permette di poter ritornare a questo punto saltando alla prossima iterazione.
    while (1)
    {
        areaDiAccesso = effettuaLogin(listaUtenti, listaAmministratori, &utenteLoggato, &adminLoggato);
        
        //REGISTRAZONE:
        if(areaDiAccesso == 0)
        {
            listaUtenti = effettuaRegistrazione(listaUtenti);
        }
        //LATO UTENTE:
        else if(areaDiAccesso == 1)
        {
            printf("LATO UTENTE\n");
        }
        //lATO AMMINISTRAZIONE:
        else if(areaDiAccesso == 2)
        {
            printf("LATO AMMINISTRAZIONE\n");
        }

    }
    

}