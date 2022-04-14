#include <stdio.h>
#include "shopping.h"

int main()
{
    nodoListaUtenti* listaUtenti = NULL; //Lista contentente tutti gli utenti registrati
    nodoListaAmministratori* listaAmministratori = NULL; //Lista contenente tutti gli amministratori
    nodoListaProdotti* listaProdotti = NULL; //Lista contenente tutti i prodotti disponibili
    nodoCarrello* carrello = NULL;  //Lista contenente tutti i prodotti nel carrello
    listaDiAttesa* listaDiAttesa = creaListaDiAttesa();
    utente utenteLoggato;   //Il contenuto verrà modificato per riferimento nella funzione effettuaLogin(). Conterrà i dati dell'utente che ha fatto l'accesso.
    amministratore adminLoggato; 
    int areaDiAccesso; //0 = Registrazione, 1 = Lato Utente, 2 = Lato Amministrazione

    //Popola tutte le liste con il contenuto dei rispettivi file
    listaUtenti = popolaListaUtenti(listaUtenti);
    listaAmministratori = popolaListaAmministratori(listaAmministratori);
    listaProdotti = popolaListaProdotti(listaProdotti);

    //DEBUG

    inserisciInListaDiAttesa(&listaDiAttesa, "Jeans", "Corto", "Mario", 'L');
    inserisciInListaDiAttesa(&listaDiAttesa, "Maglietta", "Blu", "daddi", 'S');
    inserisciInListaDiAttesa(&listaDiAttesa, "Giubbotto", "Nero", "Mario", 'L');
    inserisciInListaDiAttesa(&listaDiAttesa, "Cappello", "Nero", "daddi", 'L');

    //Loop principale della schermata iniziale, permette di poter ritornare a questo punto saltando alla prossima iterazione.
    while (1)
    {
        //Determina in quale lato dell'applicazione entrare e ritorna per riferimento l'utente oppure l'amministratore
        areaDiAccesso = effettuaLogin(listaUtenti, listaAmministratori, &utenteLoggato, &adminLoggato); 

        //REGISTRAZONE:
        if(areaDiAccesso == 0)
        {
            //Inserisce il nuovo utente registrato nella listaUtenti
            listaUtenti = effettuaRegistrazione(listaUtenti);   
        }
        //LATO UTENTE:
        else if(areaDiAccesso == 1)
        {
            /* Avvia la funzione principale del Lato Utente che a sua volta chiamerà molte altre funzioni. Utilizza i doppi puntatori in modo
            da modificare direttamente il contenuto delle liste nel main e per evitare di dover fare una funzione diversa per ogni valore di 
            ritorno.*/
            latoUtente(&utenteLoggato, &listaUtenti, &listaProdotti, &carrello, &listaDiAttesa);
        }
        //lATO AMMINISTRAZIONE:
        else if(areaDiAccesso == 2)
        {
            printf("LATO AMMINISTRAZIONE\n");
        }

    }
    

}