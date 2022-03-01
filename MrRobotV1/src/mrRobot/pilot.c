/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */  
// a completer
#include "pilot.h"
#include "robot.h"
#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>






static void Pilot_run(Event event, VelocityVector vel);
static bool Pilot_hasBumped();
static void Pilot_sendMvt(VelocityVector vel);
   
static Pilot pilot;


extern void Pilot_new(){
    printf("Création d'un pilote\n");
    Robot_new();
    
}

extern void Pilot_start(){
    Robot_start();
}

extern void Pilot_stop(){
    Robot_stop();
}

extern void Pilot_free(){
    Robot_free();
}

static void Pilot_sendMvt(VelocityVector vel){

    switch(vel.dir){
        case LEFT:
            Robot_setWheelsVelocity(vel.power,-vel.power);
            break;
        case RIGHT:
            Robot_setWheelsVelocity(-vel.power,vel.power);
            break;
        case FORWARD:
            Robot_setWheelsVelocity(vel.power,vel.power);
            break;
        case BACKWARD:
            Robot_setWheelsVelocity(-vel.power,-vel.power);
            break;
        case ARRET:
            Robot_setWheelsVelocity(DEFAUT_POWER,DEFAUT_POWER);
            break;
        default:
            Robot_setWheelsVelocity(DEFAUT_POWER,DEFAUT_POWER);
            break;
        }
}

extern void Pilot_setVelocity(VelocityVector vel){
    Pilot_run(SET_VELOCITY,vel);
   
}

static bool Pilot_hasBumped(){
    return Robot_getSensorState().collision;
}

extern PilotState Pilot_getState(){
    pilot.pilotState.luminosity=Robot_getSensorState().luminosity;
    pilot.pilotState.speed=Robot_getRobotSpeed();
    pilot.pilotState.collision=Robot_getSensorState().collision;
    return pilot.pilotState;
}

extern void Pilot_check(void){
    VelocityVector defaut_vel;
    defaut_vel.dir=ARRET;
    defaut_vel.power=DEFAUT_POWER;
    Pilot_run(CHECK, defaut_vel);
}

static void Pilot_run(Event event, VelocityVector vel){
    switch(pilot.current_state){
       
        case IDLE:
            printf("IDLE\n");

            switch(event){

                case STOP:
                    vel.dir=ARRET;
                    Pilot_sendMvt(vel);
                    Pilot_stop();
                    break;

                case SET_VELOCITY:
                    pilot.current_state=RUNNING;
                    Pilot_sendMvt(vel);
                    break;
                
                default:
                    break;

            }
        
        case RUNNING:
            printf("Running\n");
            switch(event){

                case STOP:
                    vel.dir=ARRET;
                    Pilot_sendMvt(vel);
                    Pilot_stop();
                    break;
                
                case CHECK:
                    if(Pilot_hasBumped()){
                        pilot.pilotState.collision=1;
                        pilot.current_state=IDLE;
                    }
                    break;
                
                case SET_VELOCITY:
                printf("SET VELOCITY\n");
                    if(vel.dir==ARRET){
                        pilot.current_state=IDLE;
                        break;
                    }
                    else{
                        printf("Puissance des moteurs mise à %i\n",vel.power);
                        Pilot_sendMvt(vel);
                        break;
                    }      

            }
    }
}
