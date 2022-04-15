#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "shopping.h"

void latoUtente(utente* utenteLoggato, nodoListaUtenti** listaUtenti, nodoListaProdotti** listaProdotti, nodoCarrello** carrello, listaDiAttesa** listaDiAttesa)
{
    //Variabili utilizzate per le scelte nei vari menù
    int scelta;
    int sceltaProdotto;
    int sceltaAcquisto;
    int sceltaCarrello;

    float prezzoTotaleCarrello = 0;
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
        printf("5) Visualizza Notifiche liste di attesa\n");
        printf("6) Logout\n");

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
            printf("Nome: %s %s\n",prodottoDaAcquistare->nomeProdotto,prodottoDaAcquistare->caratteristica);
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
                gestisciAcquisto(carrello, 'S',listaUtenti, listaDiAttesa, utenteLoggato,prodottoDaAcquistare,&(prodottoDaAcquistare->TaglieSdisponibili));
            //Acquista taglia M:
            else if(sceltaAcquisto  == 2)
                gestisciAcquisto(carrello, 'M',listaUtenti, listaDiAttesa, utenteLoggato,prodottoDaAcquistare,&(prodottoDaAcquistare->TaglieMdisponibili));
            //Acquista taglia L:
            else if(sceltaAcquisto == 3)
                gestisciAcquisto(carrello, 'L',listaUtenti, listaDiAttesa, utenteLoggato,prodottoDaAcquistare,&(prodottoDaAcquistare->TaglieLdisponibili)); 
        }
        //Visualizza carrello:
        else if(scelta == 4)
        {
            printf(CIANO "\nCONTENUTO CARRELLO\n" NORMALE);
            prezzoTotaleCarrello = mostraProdottiCarrello(*carrello,0); //Si salva il prezzo totale e stampa il carrello

            printf("1) Procedi all'ordine\n");
            printf("2) Indietro\n");
            scanf("%d",&sceltaCarrello);

            //Procedi all'ordine:
            if(sceltaCarrello == 1)
            {
                if(utenteLoggato->saldo >= prezzoTotaleCarrello) //Se l'utente ha abbastanza soldi per acquistare TUTTO il carrello
                {
                    acquistaCarrello(utenteLoggato,listaUtenti,carrello,listaProdotti);
                    printf(VERDE "Ordine completato con successo!\n" NORMALE); 
                }
                else //Se non ha abbastanza soldi
                    printf(ROSSO "Il tuo saldo non e' sufficiente per completare l'ordine, effettua una ricarica.\n" NORMALE);
            }
        }
        //Visualizza liste di attesa:
        else if(scelta == 5)
        {
            printf(CIANO "\nNOTIFICHE LISTE DI ATTESA\n" NORMALE);

            gestisciListaDiAttesa(listaDiAttesa, listaProdotti, carrello, utenteLoggato);        
        }
        //Logout:
        else if(scelta == 6)
            break;
    }
}

void gestisciAcquisto(nodoCarrello** carrello, char tagliaRichiesta, nodoListaUtenti** listaUtenti, listaDiAttesa** listaDiAttesa,utente* utenteLoggato, prodotto* prodottoDaAcquistare, int* numeroTaglieDisponibili)
{
    if(utenteLoggato->saldo >= prodottoDaAcquistare->prezzo && *numeroTaglieDisponibili > 0) //Se l'utente ha abbastanza soldi e c'è almeno una taglia disponibile
    {
        utenteLoggato->saldo -= prodottoDaAcquistare->prezzo; //Decrementa il saldo dell'utente
        (*numeroTaglieDisponibili)--; //Decrementa il numero di taglie S del prodotto
        modificaSaldoNellaLista(listaUtenti,utenteLoggato->nomeUtente, utenteLoggato->saldo); //Aggiorna il saldo anche nella lista

        printf(VERDE "Acquisto effettuato con successo!\n" NORMALE);
    }
    else if(utenteLoggato->saldo < prodottoDaAcquistare->prezzo && *numeroTaglieDisponibili > 0) //Se l'utente non ha abbastanza soldi
    {
        printf(ROSSO "Non hai abbastanza fondi per completare l'acquisto, effettua una ricarica.\n" NORMALE);
        //Inserisce il prodotto nella lista carrello
        *carrello = inserisciInCodaListaCarrello(*carrello,prodottoDaAcquistare->nomeProdotto,prodottoDaAcquistare->caratteristica,tagliaRichiesta, prodottoDaAcquistare->prezzo);

        printf(VERDE "Il prodotto desiderato e' stato spostato nel carrello.\n" NORMALE);
    }
    else if(*numeroTaglieDisponibili == 0)  //Se non ci sono abbastanza taglie disponibili
    {
        printf(ROSSO "Impossibile completare l'acquisto perche' la taglia scelta non e' disponibile.\n" NORMALE);
        //Inserisce l'utente nella lista di attesa con il prodotto e la taglia desiderata
        inserisciInListaDiAttesa(listaDiAttesa, prodottoDaAcquistare->nomeProdotto, prodottoDaAcquistare->caratteristica, utenteLoggato->nomeUtente, tagliaRichiesta);
        printf(VERDE "Sei stato aggiunto ad una lista d'attesa per tenere traccia della disponibilita'\n" NORMALE);
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

prodotto* ottieniProdottoPerNome(nodoListaProdotti* listaProdotti, char nomeProdotto[], char caratteristica[])
{
    if(listaProdotti == NULL)
        return NULL;
    //Se trova il prodotto nella lista lo ritorna 
    else if(strcmp(listaProdotti->prodotto.nomeProdotto,nomeProdotto) == 0 && strcmp(listaProdotti->prodotto.caratteristica,caratteristica) == 0)
        return &(listaProdotti->prodotto);
    else
        //Procede ricorsivamente sul resto della lista
        return ottieniProdottoPerNome(listaProdotti->next, nomeProdotto, caratteristica);
}

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
        printf("%d) %s %s %.2f euro\n", indice, listaProdotti->prodotto.nomeProdotto,listaProdotti->prodotto.caratteristica,listaProdotti->prodotto.prezzo);

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
        printf("%s %s %.2f %d %d %d\n",listaProdotti->prodotto.nomeProdotto,listaProdotti->prodotto.caratteristica,listaProdotti->prodotto.prezzo,
            listaProdotti->prodotto.TaglieSdisponibili,listaProdotti->prodotto.TaglieMdisponibili,listaProdotti->prodotto.TaglieLdisponibili);

        stampaListaProdotti(listaProdotti->next);
    }    
}

nodoListaProdotti* creaNodoListaProdotti(char nomeProdotto[], char caratteristica[], float prezzo, int taglieS, int taglieM, int taglieL) 
{
    nodoListaProdotti* tmp = (nodoListaProdotti*)malloc(sizeof(nodoListaProdotti)); //Alloca memoria per il nuovo nodo
    strcpy(tmp->prodotto.nomeProdotto,nomeProdotto); //Riempie tutti i campi del nuovo nodo con le informazioni in input
    strcpy(tmp->prodotto.caratteristica,caratteristica);
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

/*Scorre tutta la lista carrello: Ad ogni iterazione si ricava il corrispondente prodotto nella ListaProdotti, successivamente va a 
verificare la taglia richiesta ed effettua l'acquisto, modificando opportunamente tutte i campi dei nodi. Infine dealloca la lista Carrello */
void acquistaCarrello(utente* utenteLoggato, nodoListaUtenti** listaUtenti, nodoCarrello** carrello, nodoListaProdotti** listaProdotti)
{
    prodotto* prodottoCorrente;

    while(*carrello != NULL)    //Scorre tutto il carrello
    {
        //Va a cercarsi il prodotto del carrello nella lista di prodotti
        prodottoCorrente = ottieniProdottoPerNome(*listaProdotti, (*carrello)->nomeProdotto, (*carrello)->caratteristica);

        //Se il prodotto nel carrello è di taglia S
        if((*carrello)->tagliaRichiesta == 'S')
        {
            utenteLoggato->saldo -= prodottoCorrente->prezzo; //Decrementa il saldo dell'utente
            modificaSaldoNellaLista(listaUtenti,utenteLoggato->nomeUtente, utenteLoggato->saldo);
            prodottoCorrente->TaglieSdisponibili--; //Decrementa il numero di taglie S del prodotto
        }
        else if ((*carrello)->tagliaRichiesta == 'M')
        {
            utenteLoggato->saldo -= prodottoCorrente->prezzo;
            modificaSaldoNellaLista(listaUtenti,utenteLoggato->nomeUtente, utenteLoggato->saldo);
            prodottoCorrente->TaglieMdisponibili--; //Decrementa taglie M
        }
        else if ((*carrello)->tagliaRichiesta == 'L')
        {
            utenteLoggato->saldo -= prodottoCorrente->prezzo;
            modificaSaldoNellaLista(listaUtenti,utenteLoggato->nomeUtente, utenteLoggato->saldo);
            prodottoCorrente->TaglieLdisponibili--; //Decrementa taglie L
        }
        
        nodoCarrello* tmp = *carrello; //Salva la posizione del nodo in una variabile temporanea
        *carrello = (*carrello)->next; //Scorre avanti la lista
        free(tmp); //Svuota la zona di memoria del prodotto ;
    }
}

//Stampa in modo corretto tutto il contenuto del carrello e ritorna il prezzo totale
float mostraProdottiCarrello(nodoCarrello* carrello, float prezzoIniziale)
{
    if(carrello == NULL) //Quando non trova piu prodotti nel carrello stampa il prezzo totale
    {
        printf("\nPrezzo totale: %s%.2f euro%s\n",ROSSO, prezzoIniziale, NORMALE);
        return prezzoIniziale;
    }
    else
    {
        //Stampa semplicemente tutti i campi del nodo
        printf("- %s %s Taglia %c %.2f euro\n", carrello->nomeProdotto, carrello->caratteristica, carrello->tagliaRichiesta, carrello->prezzo);

        return mostraProdottiCarrello(carrello->next, prezzoIniziale + carrello->prezzo); 
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

//FUNZIONI LISTA DI ATTESA

/* Si occupa di fare tutta una serie di controlli per verificare se l'utenteLoggato ha la priorita sui prodotti nella sua lista di attesa e nel
caso lo notifica se il prodotto è tornato disponibile */
void gestisciListaDiAttesa(listaDiAttesa** listaDiAttesa, nodoListaProdotti** listaProdotti, nodoCarrello** carrello, utente* utenteLoggato)
{
    nodoListaDiAttesa* elementoCorrente = (*listaDiAttesa)->front;
    prodottoCoda prodottiInteressati[20]; //Vettore che conterrà tutti i prodotti che l'utente ha nella lista di attesa
    int grandezzaVettore = 0;
    int i = 0;
    char sceltaCarrello;

    printf("Prodotti per i quali sei in una lista di attesa:\n");

    //Scorre tutti gli elementi della lista di attesa in modo da trovare a che prodotti è interessato l'utente e li mette in un array
    while (elementoCorrente != NULL)
    {
        if(strcmp(elementoCorrente->elemento.nomeUtente,utenteLoggato->nomeUtente) == 0)
        {
            printf("- %s %s Taglia %c\n",elementoCorrente->elemento.nomeProdotto, elementoCorrente->elemento.caratteristica,elementoCorrente->elemento.taglia);
            prodottiInteressati[i] = elementoCorrente->elemento; //Aggiunge il prodotto al vettore
            i++;
            grandezzaVettore++; //Tiene traccia di quanti elementi ci sono nel vettore
        }
        elementoCorrente = elementoCorrente->next;
    }

    //Per ogni prodotto va a controllare nella coda se l'utente ha la priorità
    for(int i=0; i<grandezzaVettore; i++)
    {
        elementoCorrente = (*listaDiAttesa)->rear; //Vado al rear della coda ed inizio a scorrere al contrario 
        //Scorro fin quando non trovo il primo elemento che ha i campi che mi interessano, che sarà quindi quello con piu priorità
        while(strcmp(elementoCorrente->elemento.nomeProdotto,prodottiInteressati[i].nomeProdotto) != 0 ||
                strcmp(elementoCorrente->elemento.caratteristica,prodottiInteressati[i].caratteristica) != 0 ||
                elementoCorrente->elemento.taglia != prodottiInteressati[i].taglia)
                {
                    elementoCorrente = elementoCorrente->prev;
                }
        
        //A questo punto ho trovato l'ELEMENTO CON PIU PRIORITA' che corrisponde a quello che mi interessa
        if(strcmp(elementoCorrente->elemento.nomeUtente,utenteLoggato->nomeUtente) == 0) 
        {
            //Se entro in questo if vuol dire che l'utente ha effettivamente la priorità su quel prodotto, quindi deve essere notificato nel caso sia tornato disponibile

             //Si va a prendere il prodotto nella lista prodotti
            prodotto* prodottoNellaLista = ottieniProdottoPerNome((*listaProdotti),elementoCorrente->elemento.nomeProdotto,elementoCorrente->elemento.caratteristica);
            
            //FINALMENTE NOTIFICA L'UTENTE
            if(elementoCorrente->elemento.taglia == 'S')
            {
                if(prodottoNellaLista->TaglieSdisponibili > 0) //Se ora il prodotto è tornato disponibile può aggiungerlo al carrello
                {
                    printf(VERDE "\nIl prodotto %s %s Taglia %c e' tornato disponibile!%s\n",elementoCorrente->elemento.nomeProdotto,elementoCorrente->elemento.caratteristica,elementoCorrente->elemento.taglia, NORMALE);
                    printf("Vuoi aggiungerlo al carrello?(s/n) \n");
                    getchar();
                    scanf("%c",&sceltaCarrello);
                }
            }
            else if(elementoCorrente->elemento.taglia == 'M')
            {
                if(prodottoNellaLista->TaglieMdisponibili > 0)
                {
                    printf(VERDE "\nIl prodotto %s %s Taglia %c e' tornato disponibile!%s\n",elementoCorrente->elemento.nomeProdotto,elementoCorrente->elemento.caratteristica,elementoCorrente->elemento.taglia, NORMALE);
                    printf("Vuoi aggiungerlo al carrello?(s/n) \n");
                    getchar();
                    scanf("%c",&sceltaCarrello);
                }
            }   
            else if(elementoCorrente->elemento.taglia == 'L')
            {
                if(prodottoNellaLista->TaglieLdisponibili > 0)
                {
                    printf(VERDE "\nIl prodotto %s %s Taglia %c e' tornato disponibile!%s\n",elementoCorrente->elemento.nomeProdotto,elementoCorrente->elemento.caratteristica,elementoCorrente->elemento.taglia, NORMALE);
                    printf("Vuoi aggiungerlo al carrello?(s/n) \n");
                    getchar();
                    scanf("%c",&sceltaCarrello);
                }
            }           
            //Se l'utente preme s il prodotto viene spostato nel carrello
            if(sceltaCarrello == 's' || sceltaCarrello == 'S')
            {
                *carrello = inserisciInCodaListaCarrello(*carrello,elementoCorrente->elemento.nomeProdotto, elementoCorrente->elemento.caratteristica, elementoCorrente->elemento.taglia, prodottoNellaLista->prezzo);   
                //Rimuove elemento dalla coda
                rimuoviElementoListaDiAttesa(listaDiAttesa,elementoCorrente);
            }                       
        }                 
    }
}


void rimuoviElementoListaDiAttesa(listaDiAttesa** listaDiAttesa,  nodoListaDiAttesa* elemento)
{
    if(elemento == (*listaDiAttesa)->front)
    {
        (*listaDiAttesa)->front = elemento->next;
        (*listaDiAttesa)->front->prev = NULL;
        free(elemento);
    }
    else if (elemento == (*listaDiAttesa)->rear)
    {
        (*listaDiAttesa)->rear = elemento->prev;
        (*listaDiAttesa)->rear->next = NULL;
        free(elemento);
    }
    else
    {
        elemento->prev->next = elemento->next;
        elemento->next->prev = elemento->prev;
        free(elemento);
    }
}

listaDiAttesa* creaListaDiAttesa()
{
    listaDiAttesa* tmp = (listaDiAttesa*)malloc(sizeof(listaDiAttesa));

    if(tmp == NULL)
    {
        printf("Errore: impossibile allocare memoria per la coda\n");
        return NULL;
    }

    tmp->front = NULL; //Imposta sia il fronte che il retro della coda a NULL
    tmp->rear = NULL;

    return tmp;
}

void stampaListaDiAttesa(nodoListaDiAttesa* frontlistaDiAttesa)
{
    if(frontlistaDiAttesa == NULL)
        printf("NULL");
    else
    {
        //Stampa semplicemente tutti i campi del nodo
        printf("%s %s %s %c\n",frontlistaDiAttesa->elemento.nomeProdotto, frontlistaDiAttesa->elemento.caratteristica,frontlistaDiAttesa->elemento.nomeUtente,
        frontlistaDiAttesa->elemento.taglia);

        stampaListaDiAttesa(frontlistaDiAttesa->next);
    }    
}

//Inserisce l'elemento in testa alla lista di attesa utilizzando i doppi puntatori
void inserisciInListaDiAttesa(listaDiAttesa** listaDiAttesa, char nomeProdotto[20], char caratteristica[20], char nomeUtente[20], char taglia)
{
    nodoListaDiAttesa* tmp = (nodoListaDiAttesa*)malloc(sizeof(nodoListaDiAttesa));

    if(tmp == NULL)
        printf("Errore: impossibile allocare memoria per il nuovo nodo della coda\n");
    
    //Riempie tutti i campi del nuovo nodo con le informazioni in input
    strcpy(tmp->elemento.nomeProdotto, nomeProdotto);
    strcpy(tmp->elemento.caratteristica,caratteristica);
    strcpy(tmp->elemento.nomeUtente,nomeUtente);
    tmp->elemento.taglia = taglia;
    tmp->next = (*listaDiAttesa)->front;
    tmp->prev = NULL;

    if((*listaDiAttesa)->front == NULL) //Se la lista di attesa è vuota imposta sia il fronte che il retro al nuovo nodo
    {
        (*listaDiAttesa)->front = tmp;
        (*listaDiAttesa)->rear = tmp;
    }
    else    //Se non è vuoto
    {
        (*listaDiAttesa)->front->prev = tmp; //Il prev del front diventa il nuovo nodo
        (*listaDiAttesa)->front = tmp; //Il nuovo nodo diventa il front
    }
}

listaDiAttesa* popolaListaDiAttesa(listaDiAttesa* listaDiAttesa)
{
    FILE* fileListaDiAttesa = fopen("listaDiAttesa.txt","r"); //Apre il file listaDiAttesa.txt in modalità lettura

    char nomeProdottoTmp[20], caratteristicaTmp[20];    //Variabili temporanee per memorizzare il contenuto della fscanf
    char nomeUtenteTmp[20];
    char tagliaTmp;

    //Fin quando trova righe nel file listaDiAttesa.txt
    while(fscanf(fileListaDiAttesa,"%s %s %s %c",nomeUtenteTmp, nomeProdottoTmp, caratteristicaTmp, &tagliaTmp) == 4) 
    {
        inserisciInListaDiAttesa(&listaDiAttesa, nomeProdottoTmp, caratteristicaTmp, nomeUtenteTmp, tagliaTmp);
    }
    fclose(fileListaDiAttesa);

    return listaDiAttesa;
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