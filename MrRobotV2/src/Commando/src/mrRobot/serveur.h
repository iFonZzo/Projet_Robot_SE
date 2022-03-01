#if !defined __SERVEUR_H
#define __SERVEUR_H

#include "pilot.h"
#include <netinet/in.h>

#define PORT_DU_SERVEUR (12346)

typedef struct
{
    int socket_ecoute;
    int socket_donnees;
    struct sockaddr_in adresse_du_serveur;

} Server;

typedef enum
{
    DEFAUT = 0,
    ASK_FOR_LOG,
    ASK_MOUV
} Order;

typedef struct
{
    int collision;
    float luminosity;
    int speed;
    Direction dir;
    Order order;
} Data;

extern void Server_new(void);
extern void Server_start(void);
extern void Server_stop(void);
#endif /* __SERVEUR_H*/