#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "shopping.h"


int effettuaLogin(nodoListaUtenti* listaUtenti,nodoListaAmministratori* listaAmministratori, utente* utenteLoggato, amministratore* adminLoggato)
{
    int scelta;
    char nomeUtente[20];
    char password[20];

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





//FUNZIONI PER LA LISTA DI PRODOTTI

void stampaListaProdotti(nodoListaProdotti* listaProdotti)
{
    if(listaProdotti == NULL)
        printf("");
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