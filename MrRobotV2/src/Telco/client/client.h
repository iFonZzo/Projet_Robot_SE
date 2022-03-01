#ifndef _CLIENT_H
#define _CLIENT_H

#include <netinet/in.h>

#define PORT_DU_SERVEUR (12346)

typedef enum
{
    LEFT = 0,
    RIGHT,
    FORWARD,
    BACKWARD,
    ARRET
} Direction;

typedef struct
{
    int socket_ecoute;
    struct sockaddr_in adresse_du_serveur;

} Client;

typedef enum
{
    DEFAUT = 0,
    ASK_FOR_LOG,
    ASK_MOUV,

} Order;

typedef struct
{
    int collision;
    float luminosity;
    int speed;
    Direction dir;
    Order order;
} Data;

extern void Client_New(void);
extern void Client_Start(void);
extern void Client_stop(void);
extern void Client_sendMsg(Data data_to_sent);
extern Data Client_readMsg(void);
extern int Client_getSocketEcoute(void);

#endif /* __CLIENT_H*/