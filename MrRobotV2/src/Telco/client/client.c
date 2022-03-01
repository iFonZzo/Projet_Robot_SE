#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include "client.h"

#define MAX_PENDING_CONNECTIONS (5)

/*Permet de stocker les informations relatives au client */
static Client client;

/*Permet de stocker toutes les valeurs qui transiteront*/
static Data donneesClient;

extern int Client_getSocketEcoute()
{
    return client.socket_ecoute;
}

extern void Client_New()
{

    /* Creation du socket : AF_INET = IP, SOCK_STREAM = TCP */
    client.socket_ecoute = socket(AF_INET, SOCK_STREAM, 0);

    /*On vient tester si la fonction socket s'est exécuter correctement */
    if (client.socket_ecoute < 0)
    {
        printf("ERROR: Initialization of socket_ecoute FAILED --> Ligne %i", __LINE__);
    }
    else
        printf("SUCESS: socket_ecoute Initialize ");

    client.adresse_du_serveur.sin_family = AF_INET;                                                       /* Type d'adresse = IP */
    client.adresse_du_serveur.sin_port = htons(PORT_DU_SERVEUR);                                          /* Port TCP ou le service est accessible */
    client.adresse_du_serveur.sin_addr = *((struct in_addr *)gethostbyname("localhost")->h_addr_list[0]); /* On s'attache a toutes les interfaces */
}

extern void Client_Start()
{
    printf("Création du Client\n");
    int debug = connect(client.socket_ecoute, (struct sockaddr *)&client.adresse_du_serveur, sizeof(client.adresse_du_serveur));
    if (debug < 0)
    {
        printf("ERROR: Client can't connect to the server --> Ligne %i\n", __LINE__);
    }
    else
        printf("SUCESS: Client connected ");
}

extern void Client_stop()
{

    close(client.socket_ecoute);
    printf("Arrêt du client\n");
}

extern Data Client_readMsg()
{

    /*On initialise la vitesse, collision et luminosité à 0*/
    Data datasReceive = {0, 0, 0, 0, DEFAUT};

    /* On utilise la fonction read() pour récupérer les infos transmisent par le socket et les stocker dans la struct datasReceive */
    int quantiteRead = read(client.socket_ecoute, &datasReceive, sizeof(datasReceive));

    if (quantiteRead < 0)
    {
        printf("ERROR: Client.c - read function FAILED --> Ligne %i\n", __LINE__);
    }
    else
        printf("SUCCES: Client.c - function read excecuted\n");

    /* On convertit les données reçu par le socket puis stock les valeurs dans la structure donneesClient */
    donneesClient.collision = datasReceive.collision;
    donneesClient.luminosity = datasReceive.luminosity;
    donneesClient.speed = datasReceive.speed;
    return donneesClient;
}

extern void Client_sendMsg(Data data_to_send)
{
    printf("Order send: %i  Dir send:  %i \n", data_to_send.order, data_to_send.dir);
    /* Le client n'envoie que des ordres représentés ici par des caractères */
    int quantiteEcrite = write(client.socket_ecoute, &data_to_send, sizeof(data_to_send));
    /*On vient tester si la fonction write s'est exécuter sans problème (retourne -1 si c'est le cas) */
    if (quantiteEcrite < 0)
    {
        printf("ERROR: write function FAILED --> Ligne %i\n", __LINE__);
    }
    else
        printf("SUCCES: function write excecuted\n");
}