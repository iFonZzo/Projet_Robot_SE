#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "RemoteUI.h"
#include "../client/client.h"

#define CHAR_GAUCHE 'q'
#define CHAR_DROITE 'd'
#define CHAR_AVANCER 'z'
#define CHAR_RECULER 's'
#define CHAR_STOP ' '
#define CHAR_EFFACER_LOG 'e'
#define CHAR_AFFICHER_LOG 'r'
#define CHAR_QUITTER 'a'

static Data remoteData; // Instance de Remote
int KeepGoing = 1;      // variable fin de boucle
char key;

// static RemoteUI_setIP(int ip);
static void RemoteUI_captureChoice(void);
static void RemoteUI_askMvt(Direction dir);
static void RemoteUI_ask4Log(void);
static void RemoteUI_askClearLog(void);
static void RemoteUI_eraseLog(void);
static void RemoteUI_quit(void);
static void RemoteUI_run(void);
static void RemoteUI_display(void);

extern void RemoteUI_new()
{
    Client_New();
}
extern void RemoteUI_start()
{
    Client_Start();
    RemoteUI_run();
}

extern void RemoteUI_stop()
{
    Client_stop();
}

static void RemoteUI_eraseLog()
{
    printf("\033[2K\r");
    RemoteUI_display();
}

static void RemoteUI_display()
{
    printf("Bienvenue dans le menu contrôle à distance de RobotV2\n");
    printf("Vous avez le choix entre: \n");
    printf("%c : Gauche\n", CHAR_GAUCHE);
    printf("%c: Droite\n", CHAR_DROITE);
    printf("%c : Avancer\n", CHAR_AVANCER);
    printf("%c : Reculer\n", CHAR_RECULER);
    printf("%c: Stopper le robot\n", CHAR_STOP);
    printf("%c: Effacer les log\n", CHAR_EFFACER_LOG);
    printf("%c : Afficher l'état du robot\n", CHAR_AFFICHER_LOG);
    printf("%c : Quitter\n", CHAR_QUITTER);
}

static void RemoteUI_askClearLog()
{
    RemoteUI_eraseLog();
}

static void RemoteUI_ask4Log()
{
    /*On efface les précédents logs*/
    RemoteUI_askClearLog();
    printf("Demande de récupération de logs\n");

    /*On réinitialise le message à envoyer */
    Data data_to_send = {0, 0, 0, 0, DEFAUT};
    data_to_send.order = ASK_FOR_LOG;
    Client_sendMsg(data_to_send);

    /*Rajouter la partie lecture de données */
    remoteData = Client_readMsg();
    printf("Etat du bolide :\n"
           "Vitesse : %i \n"
           "Collision: %i \n"
           "Luminosité: %f \n",
           remoteData.speed, remoteData.collision, remoteData.luminosity);
}

/*Fonction utiliser pour les ordres de déplacement du robot*/
static void RemoteUI_askMvt(Direction dir)
{
    printf("Asked movement: %d\n", dir);
    /*On réinitialise le message à envoyer */
    Data data_to_send = {0, 0, 0, 0, DEFAUT};

    data_to_send.dir = dir;
    data_to_send.order = ASK_MOUV;
    Client_sendMsg(data_to_send);
}

static void RemoteUI_captureChoice()
{
    // system("stty -icanon -echo");
    key = getchar();
}

static void RemoteUI_run(void)
{
    printf("On rentre dans la fonction RemoteUI_run\n");
    printf("\n");
    /*On affiche le menu */
    RemoteUI_display();

    /*On capture le caractère envoyé par l'utilisateur */

    while (KeepGoing)
    {
        RemoteUI_captureChoice();
        switch (key)
        {

        case CHAR_GAUCHE:
            RemoteUI_askMvt(LEFT);
            break;
        case CHAR_DROITE:
            RemoteUI_askMvt(RIGHT);
            break;
        case CHAR_AVANCER:
            RemoteUI_askMvt(FORWARD);
            break;
        case CHAR_RECULER:
            RemoteUI_askMvt(BACKWARD);
            break;
        case CHAR_EFFACER_LOG:
            RemoteUI_askClearLog();
            RemoteUI_display();
            break;
        case CHAR_STOP:
            RemoteUI_askMvt(ARRET);
            break;
        case CHAR_AFFICHER_LOG:
            RemoteUI_ask4Log();
            break;
        case CHAR_QUITTER:
            RemoteUI_quit();
            break;
        default:
            break;
        }
    }
}

static void RemoteUI_quit(void)
{
    // system("stty icanon echo");
    printf("Close Client\n");
    close(Client_getSocketEcoute());
}