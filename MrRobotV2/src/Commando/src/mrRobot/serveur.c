#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "serveur.h"
#include "pilot.h"

#define MAX_PENDING_CONNECTIONS (5)
#define CHAR_STOP_REMOTE 'a'
#define IP_SERV gethostbyname("localhost")

static int KeepGoing = 1;

static Server server;

static Data donneesServer;

static void Server_sendMsg(void);
static void Server_readMsg(void);
static void Server_run(void);
static VelocityVector Server_translate(Direction dir);

extern void Server_new()
{
    /* Creation du socket : AF_INET = IP, SOCK_STREAM = TCP */
    server.socket_ecoute = socket(AF_INET, SOCK_STREAM, 0);

    /*On vient tester si la fonction socket s'est exécuter correctement */
    if (server.socket_ecoute < 0)
    {
        printf("ERROR: Initialization of socket_ecoute FAILED --> Ligne %i", __LINE__);
    }
    else
        printf("SUCESS: socket_ecoute Initialize ");

    server.adresse_du_serveur.sin_family = AF_INET;                                    /* Type d'adresse = IP */
    server.adresse_du_serveur.sin_port = htons(PORT_DU_SERVEUR);                       /* Port TCP ou le service est accessible */
    server.adresse_du_serveur.sin_addr = *((struct in_addr *)IP_SERV->h_addr_list[0]); /* On s'attache a toutes les interfaces */

    Pilot_new();
}

extern void Server_start()
{
    /*On met à 0 les données du server */
    donneesServer.collision = 0;
    donneesServer.luminosity = 0;
    donneesServer.speed = 0;
    donneesServer.order = DEFAUT;
    donneesServer.dir = ARRET;

    Pilot_start();

    printf("Port du Server : %d, Adresse du Server: %s", PORT_DU_SERVEUR, IP_SERV->h_addr_list[0]);
    bind(server.socket_ecoute, &server.adresse_du_serveur, sizeof(server.adresse_du_serveur));
    if (listen(server.socket_ecoute, MAX_PENDING_CONNECTIONS) < 0)
    {
        printf("ERROR: socket can't listen\n");
        Server_stop();
    }
    else
    {
        printf("SUCCES: socket is listening\n");
        Pilot_start();
        Server_run();
    }
}

extern void Server_stop()
{
    printf("Fin de la session\n Fermeture du serveur");
    close(server.socket_ecoute);
}

static void Server_run()
{
    printf("Démarrage de la fonction run\n");

    /*On vient tester la connexion entre le client et le server */
    server.socket_donnees = accept(server.socket_ecoute, NULL, 0);
    if (server.socket_donnees < 0)
    {
        printf("ERROR: Impossible pour le client de se connecter\n");
        Server_stop();
    }
    else
    {
        printf("SUCCES: Client connecter au serveur\n");
        /* Une fois la connexion établie on peut rentrer dans la boucle */

        while (KeepGoing)
        {
            Server_readMsg();
            printf("Ordre reçu par le client: %d\n", donneesServer.order);
            printf("\n");

            switch (donneesServer.order)
            {
            case ASK_FOR_LOG:
                donneesServer.collision = Pilot_getState().collision;
                donneesServer.luminosity = Pilot_getState().luminosity;
                donneesServer.speed = Pilot_getState().speed;
                Server_sendMsg();
                break;

            case ASK_MOUV:
                Pilot_setVelocity(Server_translate(donneesServer.dir));
                break;
            case DEFAUT:

                break;
            default:

                break;
            }
        }
    }
}

static void Server_readMsg()
{
    Data dataReceives = {0, 0, 0, 0, DEFAUT};

    /* On utilise la fonction read() pour récupérer les infos transmisent par le socket et les stocker dans la struct dataClient */
    int quantiteRead = read(server.socket_donnees, &dataReceives, sizeof(dataReceives));

    if (quantiteRead < 0)
    {
        printf("ERROR: Server.c - read function FAILED --> Ligne %i\n", __LINE__);
    }
    else
        printf("SUCCES: Server.c - function read excecuted\n");

    /* On convertit les valeurs */
    donneesServer.dir = dataReceives.dir;
    donneesServer.order = dataReceives.order;
}

static void Server_sendMsg()
{
    /* Le client n'envoie que des ordres représentés ici par des caractères */
    int quantiteEcrite = write(server.socket_donnees, &donneesServer, sizeof(donneesServer));
    /*On vient tester si la fonction write s'est exécuter sans problème (retourne -1 si c'est le cas) */
    if (quantiteEcrite < 0)
    {
        printf("ERROR: write function FAILED --> Ligne %i\n", __LINE__);
    }
    else
        printf("SUCCES: function write excecuted\n");
}

static VelocityVector Server_translate(Direction dir)
{
    VelocityVector vel;
    vel.dir = dir;
    vel.power = POWER;
    return vel;
}