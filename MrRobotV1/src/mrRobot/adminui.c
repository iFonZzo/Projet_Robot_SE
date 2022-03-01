/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer
   
#include "adminui.h"
#include "pilot.h"
#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>



#define CHAR_GAUCHE 'q'
#define CHAR_DROITE 'd'
#define CHAR_AVANCER 'z'
#define CHAR_RECULER 's'
#define CHAR_STOP ' '
#define CHAR_EFFACER_LOG 'e'
#define CHAR_AFFICHER_LOG 'r'
#define CHAR_QUITTER 'a'
#define CHAR_TELCO 't'



static void AdminUI_captureChoice(void);
static void AdminUI_askMvt(Direction dir);
static VelocityVector AdminUI_translate(Direction dir);
static void AdminUI_ask4Log(void);
static void AdminUI_askClearLog(void);
static void AdminUI_eraseLog(void);
static void AdminUI_quit(void);
static void AdminUI_run(void);
static void AdminUI_display(void);



static Admin admin;


extern void AdminUI_new(){
    printf("Création d'un AdminUI\n");
    //AdminUI_ask4Log();
    Pilot_new();
    telcoUI_new();
}
extern void AdminUI_free(){
    Pilot_free();
}

extern void AdminUI_stop(){
    Pilot_stop();
    telcoUI_stop();
}


extern void AdminUI_start(){
    Pilot_start();
    AdminUI_run();
}

static void AdminUI_display(){
    printf("Bienvenue dans le menu de Robot\n");
    printf("Vous avez le choix entre: \n");
    printf("q : Aller à gauche\n");
    printf("d : Aller à droite\n");
    printf("z : Avancer\n");
    printf("s : Reculer\n");
    printf(" : Stopper le robot\n");
    printf("e: Effacer les log\n");
    printf("r : Afficher l'état du robot\n");
    printf("t: mode contrôle à distance");
    printf("a : Quitter\n");
}

static void AdminUI_askClearLog(){
    AdminUI_eraseLog();
}
static void AdminUI_ask4Log(){
    Pilot_check();
    printf("Vitesse : %d,", Pilot_getState().speed);
    printf("Collision : %i,", Pilot_getState().collision);
    printf("Luminosité %f", Pilot_getState().luminosity); 
}

static VelocityVector AdminUI_translate(Direction dir){
    VelocityVector vel;
    vel.dir=dir;
    vel.power=POWER;
    return vel;
}

static void AdminUI_askMvt(Direction dir){
    //printf("%i\n",Pilot_getState().collision);

    /*if(!Pilot_getState().collision){
        printf("ERROR: Le robot est bloqué à cause d'une collision\n");
    }
    else{
        */
       printf("La puissance donnée aux roues est %i\n", AdminUI_translate(dir).power);
       printf("La direction donnée aux roues est %i\n",AdminUI_translate(dir).dir);
       Pilot_setVelocity(AdminUI_translate(dir));

}

static void AdminUI_captureChoice(){
    admin.choice=getchar();
}


static void AdminUI_eraseLog(){
    printf("\033[2K\r");
}

static void AdminUI_quit(){
    printf("Fin de la session\n");
    Pilot_stop();
}

static void AdminUI_captureRemoteChoice(){
    int un_socket;
}

static void AdminUI_run(){
   AdminUI_display();
   AdminUI_captureChoice();
    while(admin.choice!=CHAR_QUITTER){
        AdminUI_captureChoice();
        switch(admin.choice){

            case CHAR_GAUCHE:
                AdminUI_askMvt(LEFT);
                break;
            case CHAR_DROITE:
                AdminUI_askMvt(RIGHT);
                break;
            case CHAR_AVANCER:
                printf("Avancer\n");
                AdminUI_askMvt(FORWARD);
                break;
            case CHAR_RECULER:
                AdminUI_askMvt(BACKWARD);
                break;
            case CHAR_EFFACER_LOG:
                AdminUI_askClearLog();
                AdminUI_display();
                break;
            case CHAR_STOP:
                AdminUI_askMvt(ARRET);
                break;
            case CHAR_AFFICHER_LOG:
                printf("LOG\n");
                AdminUI_ask4Log();
                break;
            case CHAR_QUITTER:
                AdminUI_quit();
                break;
            case CHAR_TELCO:
                TelcoUI_start();
                TelcoUI_stop();
            default:
                break;
        }
    }



  
}


