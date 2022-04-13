#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "shopping.h"

void latoUtente(utente* utenteLoggato, nodoListaUtenti** listaUtenti, nodoListaProdotti** listaProdotti, nodoCarrello** carrello)
{
    //Variabili utilizzate per le scelte nei vari menù
    int scelta;
    int sceltaProdotto;
    int sceltaAcquisto;
    int sceltaCarrello;

    float differenzaImporto;
    prodotto* prodottoDaAcquistare;

    while (1)
    {
        //Effettua le varie stampe
        printf(SEPARATORE);
        printf(CIANO "LATO UTENTE\n" NORMALE);
        printf("Benvenuto/a, %s%s%s\n",VERDE, utenteLoggato->nomeUtente, NORMALE);
        printf("Saldo disponibile: %s%.2f euro%s\n",ROSSO, utenteLoggato->saldo, NORMALE);
        printf("\n1) Aggiungi fondi\n");
        printf("2) Preleva fondi\n");
        printf("3) Acquista un prodotto\n");
        printf("4) Visualizza Carrello\n");
        printf("5) Logout\n");

        scanf("%d",&scelta);

        //Aggiungi fondi:
        if(scelta == 1)
        {
            printf("\nInserire importo da aggiungere in euro (es. 49.99): ");
            scanf("%f",&differenzaImporto);
            utenteLoggato->saldo += differenzaImporto;  //Modifica il saldo dell'utente loggato
            modificaSaldoNellaLista(listaUtenti,utenteLoggato->nomeUtente, utenteLoggato->saldo); //Aggiorna il saldo dell'utente anche nella lista           
            printf(VERDE "Deposito effettuato con successo!\n" NORMALE);
        }
        //Preleva fondi:
        else if(scelta == 2)
        {
            printf("\nInserire importo da prelevare in euro (es. 49.99): ");
            scanf("%f",&differenzaImporto);

            if(differenzaImporto <= utenteLoggato->saldo) //Se l'utente ha abbastanza soldi per fare il prelievo
            {
                utenteLoggato->saldo -= differenzaImporto;
                modificaSaldoNellaLista(listaUtenti,utenteLoggato->nomeUtente, utenteLoggato->saldo);
                printf(VERDE "Prelievo effettuato con successo!\n" NORMALE);
            }
            else
                printf(ROSSO "Il saldo non e' sufficiente per effettuare il prelievo\n" NORMALE);
        }
        //Acquista un prodotto:
        else if(scelta == 3)
        {
            printf(CIANO "\nPRODOTTI DISPONIBILI\n" NORMALE);
            mostraListaProdotti(*listaProdotti, 1); //Stampa l'elenco dei prodotti

            printf("\nSelezionare il prodotto per visualizzare i dettagli: ");
            scanf("%d",&sceltaProdotto);

            prodottoDaAcquistare = ottieniProdottoDaIndice(*listaProdotti,sceltaProdotto - 1); //Ottiene l'elemento della lista scelto

            //Stampa tutte le informazioni del prodotto selezionato
            printf(CIANO "\nINFORMAZIONI PRODOTTO\n" NORMALE);
            printf("Nome: %s %s\n",prodottoDaAcquistare->nomeProdotto,prodottoDaAcquistare->caratterstica);
            printf("Prezzo: %.2f euro\n",prodottoDaAcquistare->prezzo);
            printf("Taglie S disponibili: %d\n",prodottoDaAcquistare->TaglieSdisponibili);
            printf("Taglie M disponibili: %d\n",prodottoDaAcquistare->TaglieMdisponibili);
            printf("Taglie L disponibili: %d\n",prodottoDaAcquistare->TaglieLdisponibili);

            printf("\n1)Acquista taglia S\n");
            printf("2)Acquista taglia M\n");
            printf("3)Acquista taglia L\n");
            printf("4)Annulla Acquisto\n");
            scanf("%d",&sceltaAcquisto);

            //Acquista taglia S:
            if(sceltaAcquisto == 1)
                gestisciAcquisto(carrello, 'S',listaUtenti,utenteLoggato,prodottoDaAcquistare,&(prodottoDaAcquistare->TaglieSdisponibili));
            //Acquista taglia M:
            else if(sceltaAcquisto  == 2)
                gestisciAcquisto(carrello, 'M',listaUtenti,utenteLoggato,prodottoDaAcquistare,&(prodottoDaAcquistare->TaglieMdisponibili));
            //Acquista taglia L:
            else if(sceltaAcquisto == 3)
                gestisciAcquisto(carrello, 'L',listaUtenti, utenteLoggato,prodottoDaAcquistare,&(prodottoDaAcquistare->TaglieLdisponibili)); 
        }
        //Visualizza carrello:
        else if(scelta == 4)
        {
            printf(CIANO "\nCONTENUTO CARRELLO\n" NORMALE);
            mostraProdottiCarrello(*carrello,0);

            printf("1) Procedi all'ordine\n");
            printf("2) Indietro\n");
            scanf("%d",&sceltaCarrello);
        }
        //Logout:
        else if(scelta == 5)
            break;
    }
}

void gestisciAcquisto(nodoCarrello** carrello, char tagliaRichiesta, nodoListaUtenti** listaUtenti, utente* utenteLoggato, prodotto* prodottoDaAcquistare, int* numeroTaglieDisponibili)
{
    if(utenteLoggato->saldo >= prodottoDaAcquistare->prezzo && *numeroTaglieDisponibili > 0) //Se l'utente ha abbastanza soldi e c'è almeno una taglia disponibile
    {
        utenteLoggato->saldo -= prodottoDaAcquistare->prezzo; //Decrementa il saldo dell'utente
        (*numeroTaglieDisponibili)--; //Decrementa il numero di taglie S del prodotto
        modificaSaldoNellaLista(listaUtenti,utenteLoggato->nomeUtente, utenteLoggato->saldo); //Aggiorna il saldo anche nella lista

        printf(VERDE "Acquisto effettuato con successo!\n" NORMALE);
    }
    else if(utenteLoggato->saldo < prodottoDaAcquistare->prezzo) //Se l'utente non ha abbastanza soldi
    {
        printf(ROSSO "Non hai abbastanza fondi per completare l'acquisto, effettua una ricarica\n" NORMALE);
        //Inserisce il prodotto nella lista carrello
        *carrello = inserisciInCodaListaCarrello(*carrello,prodottoDaAcquistare->nomeProdotto,prodottoDaAcquistare->caratterstica,tagliaRichiesta, prodottoDaAcquistare->prezzo);

        printf(VERDE "Il prodotto desiderato e' stato aggiunto al carrello\n" NORMALE);
    }
    else if(*numeroTaglieDisponibili == 0)  //Se non ci sono abbastanza taglie disponibili
    {
        printf(ROSSO "Impossibile completare l'acquisto perche' la taglia scelta non e' disponibile\n" NORMALE);
        // TODO: Aggiungi utente nella lista di attesa
        printf(VERDE "Sei stato aggiunto ad una lista d'attesa\n" NORMALE);
    }
}

nodoListaUtenti* effettuaRegistrazione(nodoListaUtenti* listaUtenti)
{
    char nomeUtente[20];
    char password[20];
    char confermaPassword[20];
    FILE* fileUtenti = fopen("utenti.txt","a");

    while(1)
    {
        //Effettua tutte le varie stampe
        printf(SEPARATORE); 
        printf(CIANO "REGISTRAZIONE UTENTE\n" NORMALE);
        printf("Inserisci nome utente: ");
        scanf("%s",nomeUtente);
        printf("Inserisci password: ");
        scanf("%s",password);
        printf("Conferma password: ");
        scanf("%s",confermaPassword);

        if(strcmp(password,confermaPassword) == 0) //Se le password inserite coincidono
        {
            if(ricercaUtentePerNome(listaUtenti, nomeUtente) == 0) //Se l'utente non è già registrato
            {
                listaUtenti = inserisciInCodaListaUtenti(listaUtenti, nomeUtente, password,0); //Inserisce il nuovo utente nella lista
                printf(VERDE "Registrazione effettuata correttamente\n" NORMALE);

                fprintf(fileUtenti,"\n%s %s %.2f",nomeUtente, password, 0); //Scrive il nuovo utente nel file utenti.txt
                fclose(fileUtenti);
                return listaUtenti;
            }
            else
                printf(ROSSO "L'utente inserito e gia registrato\n" NORMALE);
        }
        else
            printf(ROSSO "Le password inserite non coincidono\n" NORMALE);
    }
}


int effettuaLogin(nodoListaUtenti* listaUtenti,nodoListaAmministratori* listaAmministratori, utente* utenteLoggato, amministratore* adminLoggato)
{
    int scelta;
    char nomeUtente[20];
    char password[20];

    printf(SEPARATORE);
    printf(MAGENTA "BENVENUTO NELLO SHOPPING ONLINE\n" NORMALE);
    printf("1) Accedi come utente\n");
    printf("2) Accedi come amministratore\n");
    printf("3) Non hai un account? Registrati\n");
    scanf("%d",&scelta);

    //Accedi come utente:
    if(scelta == 1)
    {
        while(1) //Continua a chiedere le credenziali fin quando non inserisce quelle giuste.
        {
            printf(SEPARATORE);
            printf(CIANO "LOGIN UTENTE\n" NORMALE);
            printf("Nome utente: ");
            scanf("%s",nomeUtente);
            printf("Password: ");
            scanf("%s",password);

            //Controlla che i dati inseriti siano presenti nella lista e contemporaneamente ritorna per riferimento l'utente che ha fatto l'accesso
            if(ricercaUtenteNellaLista(listaUtenti, utenteLoggato,nomeUtente, password) == 1)
            {
                printf(VERDE "Accesso eseguito con successo!\n" NORMALE);
                return 1;  //Farà aprire il lato utente nel main    
            }  
            else
                printf(ROSSO "Credenziali errate, inserisci correttamente i dati\n" NORMALE);
        }
    }
    //Accedi come amministratore:
    else if(scelta == 2)
    {
        while(1)
        {
            printf(SEPARATORE);
            printf(CIANO "LOGIN AMMINISTRAZIONE\n" NORMALE);
            printf("Nome amministratore: ");
            scanf("%s",nomeUtente);
            printf("Password: ");
            scanf("%s",password);

            if(ricercaAmministratoreNellaLista(listaAmministratori, adminLoggato, nomeUtente, password) == 1)
            {
                printf(VERDE "Accesso eseguito con successo!\n" NORMALE);
                return 2;
            }
            else
                printf(ROSSO "Credenziali errate, inserisci correttamente i dati\n" NORMALE);
        }
    }
    //Registrati:
    else if(scelta == 3)
    {
        return 0;
    }
}

//-----------------------------------------------------------------------//
//FUNZIONI PER LA LISTA DI PRODOTTI

prodotto* ottieniProdottoDaIndice(nodoListaProdotti* listaProdotti, int posizione)
{
    for(int i=0; i<posizione; i++)
        listaProdotti = listaProdotti->next;
    return &(listaProdotti->prodotto);
}

void mostraListaProdotti(nodoListaProdotti* listaProdotti, int indice)
{
    if(listaProdotti == NULL)
        printf("");
    else
    {
        //Stampa la lista in un formato ad elenco, più sensato nell'interfaccia
        printf("%d) %s %s %.2f euro\n", indice, listaProdotti->prodotto.nomeProdotto,listaProdotti->prodotto.caratterstica,listaProdotti->prodotto.prezzo);

        mostraListaProdotti(listaProdotti->next,indice + 1);
    }
}

void stampaListaProdotti(nodoListaProdotti* listaProdotti)
{
    if(listaProdotti == NULL)
        printf("NULL");
    else
    {
        //Stampa semplicemente tutti i campi del nodo
        printf("%s %s %.2f %d %d %d\n",listaProdotti->prodotto.nomeProdotto,listaProdotti->prodotto.caratterstica,listaProdotti->prodotto.prezzo,
            listaProdotti->prodotto.TaglieSdisponibili,listaProdotti->prodotto.TaglieMdisponibili,listaProdotti->prodotto.TaglieLdisponibili);

        stampaListaProdotti(listaProdotti->next);
    }    
}

nodoListaProdotti* creaNodoListaProdotti(char nomeProdotto[], char caratteristica[], float prezzo, int taglieS, int taglieM, int taglieL) 
{
    nodoListaProdotti* tmp = (nodoListaProdotti*)malloc(sizeof(nodoListaProdotti)); //Alloca memoria per il nuovo nodo
    strcpy(tmp->prodotto.nomeProdotto,nomeProdotto); //Riempie tutti i campi del nuovo nodo con le informazioni in input
    strcpy(tmp->prodotto.caratterstica,caratteristica);
    tmp->prodotto.prezzo = prezzo;
    tmp->prodotto.TaglieSdisponibili = taglieS;
    tmp->prodotto.TaglieMdisponibili = taglieM;
    tmp->prodotto.TaglieLdisponibili = taglieL;
    tmp->next = NULL;

    return tmp;
}

//Funzione ricorsiva che inserisce l'elemento in coda alla lista, in questo modo l'ordine dei prodotti sarà lo stesso del file.
nodoListaProdotti* inserisciInCodaListaProdotti(nodoListaProdotti* lista, char nomeProdotto[], char caratterstica[], float prezzo, int taglieS, int taglieM, int taglieL)
{
    if(lista == NULL)
        return creaNodoListaProdotti(nomeProdotto, caratterstica, prezzo, taglieS, taglieM, taglieL);    
    else
    {
        lista->next = inserisciInCodaListaProdotti(lista->next, nomeProdotto, caratterstica, prezzo, taglieS, taglieM, taglieL);
        return lista;
    }
}

nodoListaProdotti* popolaListaProdotti(nodoListaProdotti* listaProdotti)
{
    FILE* fileProdotti = fopen("prodotti.txt","r"); //Apre il file prodotti.txt in modalità lettura

    char nomeProdottoTmp[20], caratteristicaTmp[20];    //Variabili temporanee per memorizzare il contenuto della fscanf
    float prezzoTmp;
    int taglieS_tmp, taglieM_tmp, taglieL_tmp;

    //Fin quando trova righe nel file prodotti.txt
    while(fscanf(fileProdotti,"%s %s %f %d %d %d",nomeProdottoTmp, caratteristicaTmp, &prezzoTmp, &taglieS_tmp, &taglieM_tmp, &taglieL_tmp) == 6) 
    {
        listaProdotti = inserisciInCodaListaProdotti(listaProdotti, nomeProdottoTmp, caratteristicaTmp, prezzoTmp, taglieS_tmp, taglieM_tmp, taglieL_tmp); 
    }
    fclose(fileProdotti);

    return listaProdotti;
}

//FUNZIONI LISTA CARRELLO



void mostraProdottiCarrello(nodoCarrello* carrello, float prezzoIniziale)
{
    float prezzoTotale = prezzoIniziale;    

    if(carrello == NULL) //Quando non trova piu prodotti nel carrello stampa il prezzo totale
        printf("\nPrezzo totale: %s%.2f euro%s\n",ROSSO, prezzoTotale, NORMALE);
    else
    {
        //Stampa semplicemente tutti i campi del nodo
        printf("- %s %s Taglia %c %.2f euro\n", carrello->nomeProdotto, carrello->caratteristica, carrello->tagliaRichiesta, carrello->prezzo);

        mostraProdottiCarrello(carrello->next, prezzoTotale + carrello->prezzo); 
    }    
}

nodoCarrello* creaNodoListaCarrello(char nomeProdotto[], char caratteristica[], char tagliaRichiesta, float prezzo) 
{
    nodoCarrello* tmp = (nodoCarrello*)malloc(sizeof(nodoCarrello)); //Alloca memoria per il nuovo nodo
    strcpy(tmp->nomeProdotto,nomeProdotto); //Riempie tutti i campi del nuovo nodo con le informazioni in input
    strcpy(tmp->caratteristica,caratteristica);
    tmp->tagliaRichiesta = tagliaRichiesta;
    tmp->prezzo = prezzo;
    tmp->next = NULL;

    return tmp;
}

nodoCarrello* inserisciInCodaListaCarrello(nodoCarrello* lista, char nomeProdotto[], char caratterstica[], char tagliaRichiesta, float prezzo)
{
    if(lista == NULL)
        return creaNodoListaCarrello(nomeProdotto, caratterstica, tagliaRichiesta, prezzo);    
    else
    {
        lista->next = inserisciInCodaListaCarrello(lista->next, nomeProdotto, caratterstica, tagliaRichiesta, prezzo);
        return lista;
    }
}

//FUNZIONI PER LA LISTA DI AMMINISTRATORI

//Funzione ricorsiva che ricerca un amministratore nella lista di amministratori
int ricercaAmministratoreNellaLista(nodoListaAmministratori* listaAmministratori, amministratore* amministratoreOutput,char nomeUtente[], char password[])
{
    if(listaAmministratori == NULL)
        return 0;
    //Se trova un nodo che ha il nome utente e la password richiesta allora l'amministratore è presente
    else if(strcmp(listaAmministratori->amministratore.nomeUtente,nomeUtente) == 0 && strcmp(listaAmministratori->amministratore.password,password) == 0)
    {
        //Ritorna per riferimento al main l'admin che ha fatto l'accesso
        strcpy(amministratoreOutput->nomeUtente,listaAmministratori->amministratore.nomeUtente);
        strcpy(amministratoreOutput->password,listaAmministratori->amministratore.password);

        return 1;
    }
    else
        return ricercaAmministratoreNellaLista(listaAmministratori->next, amministratoreOutput, nomeUtente, password);  
}

void stampaListaAmministratori(nodoListaAmministratori* listaAmministratori)
{
    if(listaAmministratori == NULL)
        printf("");
    else
    {
        printf("%s %s\n",listaAmministratori->amministratore.nomeUtente, listaAmministratori->amministratore.password);
        stampaListaAmministratori(listaAmministratori->next);
    }    
}

nodoListaAmministratori* creaNodoListaAmministratori(char nomeUtente[], char password[]) 
{
    nodoListaAmministratori* tmp = (nodoListaAmministratori*)malloc(sizeof(nodoListaAmministratori)); //Alloca memoria per il nuovo nodo
    strcpy(tmp->amministratore.nomeUtente,nomeUtente); //Riempie tutti i campi del nuovo nodo con le informazioni in imput
    strcpy(tmp->amministratore.password,password);
    tmp->next = NULL;

    return tmp;
}

//Funzione ricorsiva che inserisce l'elemento in coda alla lista, in questo modo l'ordine degli amministratori sarà lo stesso del file.
nodoListaAmministratori* inserisciInCodaListaAmministratori(nodoListaAmministratori* lista, char nomeUtente[], char password[])
{
    if(lista == NULL)
        return creaNodoListaAmministratori(nomeUtente, password);    
    else
    {
        lista->next = inserisciInCodaListaAmministratori(lista->next, nomeUtente, password);
        return lista;
    }
}

nodoListaAmministratori* popolaListaAmministratori(nodoListaAmministratori* listaAmministratori)
{
    FILE* fileAmministratori = fopen("amministratori.txt","r"); //Apre il file utenti.txt in modalità lettura

    char nomeUtenteTmp[20], passwordTmp[20];    //Variabili temporanee per memorizzare il contenuto della fscanf

    while(fscanf(fileAmministratori,"%s %s",nomeUtenteTmp,passwordTmp) == 2) //Fin quando trova righe nel file amministratori.txt
    {
        listaAmministratori = inserisciInCodaListaAmministratori(listaAmministratori,nomeUtenteTmp, passwordTmp); 
    }
    fclose(fileAmministratori);

    return listaAmministratori;
}

//FUNZIONI PER LA LISTA DI UTENTI
//Funzione ricosiva che utlizza il doppio puntatore per modificare direttamente il saldo dell'utente all'interno della lista senza doverla ritornare
void modificaSaldoNellaLista(nodoListaUtenti** listaUtenti, char nomeUtente[], float nuovoSaldo)
{
    if(*listaUtenti == NULL)
        return;
    else if(strcmp((*listaUtenti)->utente.nomeUtente,nomeUtente) == 0)  //Se trova l'utente
    {
        (*listaUtenti)->utente.saldo = nuovoSaldo;  //Modifica il saldo
    }
    else
        modificaSaldoNellaLista((&(*listaUtenti)->next), nomeUtente, nuovoSaldo);
}

int ricercaUtentePerNome(nodoListaUtenti* listaUtenti, char nomeUtente[])
{
    if(listaUtenti == NULL)
        return 0;
    //Se trova il nome utente nella lista 
    else if(strcmp(listaUtenti->utente.nomeUtente,nomeUtente) == 0)
        return 1;
    else
        //Procede ricorsivamente sul resto della lista
        return ricercaUtentePerNome(listaUtenti->next, nomeUtente);
}

//Funzione ricorsiva che ricerca un utente nella lista di utenti
int ricercaUtenteNellaLista(nodoListaUtenti* listaUtenti, utente* utenteOutput,char nomeUtente[], char password[])
{
    if(listaUtenti == NULL)
        return 0;
    //Se trova un nodo che ha il nome utente e la password richiesta allora l'utente è presente
    else if(strcmp(listaUtenti->utente.nomeUtente,nomeUtente) == 0 && strcmp(listaUtenti->utente.password,password) == 0)
    {
        //Ritorna per riferimento al main l'utente che ha fatto l'accesso
        strcpy(utenteOutput->nomeUtente,listaUtenti->utente.nomeUtente);
        strcpy(utenteOutput->password,listaUtenti->utente.password);
        utenteOutput->saldo = listaUtenti->utente.saldo;

        return 1;
    }
    else
        return ricercaUtenteNellaLista(listaUtenti->next, utenteOutput,nomeUtente, password);  
}

nodoListaUtenti* popolaListaUtenti(nodoListaUtenti* listaUtenti)
{
    FILE* fileUtenti = fopen("utenti.txt","r"); //Apre il file utenti.txt in modalità lettura

    char nomeUtenteTmp[20], passwordTmp[20];    //Variabili temporanee per memorizzare il contenuto della fscanf
    float saldoTmp;

    while(fscanf(fileUtenti,"%s %s %f",nomeUtenteTmp,passwordTmp,&saldoTmp) == 3) //Fin quando trova righe nel file utenti.txt
    {
        listaUtenti = inserisciInCodaListaUtenti(listaUtenti,nomeUtenteTmp, passwordTmp, saldoTmp); 
    }
    fclose(fileUtenti);

    return listaUtenti;
}

nodoListaUtenti* creaNodoListaUtenti(char nomeUtente[], char password[], float saldo) 
{
    nodoListaUtenti* tmp = (nodoListaUtenti*)malloc(sizeof(nodoListaUtenti)); //Alloca memoria per il nuovo nodo
    strcpy(tmp->utente.nomeUtente,nomeUtente); //Riempie tutti i campi del nuovo nodo con le informazioni in imput
    strcpy(tmp->utente.password,password);
    tmp->utente.saldo = saldo;
    tmp->next = NULL;

    return tmp;
}

//Funzione ricorsiva che inserisce l'elemento in coda alla lista, in questo modo l'ordine degli utenti sarà lo stesso del file.
nodoListaUtenti* inserisciInCodaListaUtenti(nodoListaUtenti* lista, char nomeUtente[], char password[], float saldo)
{
    if(lista == NULL)
        return creaNodoListaUtenti(nomeUtente, password, saldo);    
    else
    {
        lista->next = inserisciInCodaListaUtenti(lista->next, nomeUtente, password, saldo);
        return lista;
    }
}

void stampaListaUtenti(nodoListaUtenti* listaUtenti)
{
    if(listaUtenti == NULL)
        printf("");
    else
    {
        printf("%s %s %.2f\n",listaUtenti->utente.nomeUtente,listaUtenti->utente.password, listaUtenti->utente.saldo);
        stampaListaUtenti(listaUtenti->next);
    }    
}