/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include <stdio.h>
#include <stdlib.h>
#include "robot.h"
#include "./infox_prose-x86_64-v0.3/include/infox/prose/prose.h"

#define LEFT_MOTOR MD
#define RIGHT_MOTOR MA
#define LIGHT_SENSOR S1
#define FRONT_BUMPER S3
#define FLOOR_SENSOR S2

static Robot robot;

extern void Robot_start()
{
    printf("Création du robot\n");
    robot.mD = Motor_open(MD);
    robot.mG = Motor_open(MA);
    if (robot.mD == NULL)
        PProseError("ERROR: Problème d'ouverture du moteur droit (port MD)");
    if (robot.mG == NULL)
        PProseError("ERROR: Problème d'ouverture du moteur gauche (port MA)");
    robot.floorSensor = ContactSensor_open(FLOOR_SENSOR);
    robot.frontSensor = ContactSensor_open(FRONT_BUMPER);
    robot.lightSensor = LightSensor_open(LIGHT_SENSOR);
    if (robot.floorSensor == NULL)
        PProseError("Problème initialisation floorSensor");
    if (robot.frontSensor == NULL)
        PProseError("Problème initialisation frontSensor");
    if (robot.lightSensor == NULL)
        PProseError("Problème initialisation lightSensor");
    printf("SUCESS: Robot créé\n");
}

extern void Robot_stop()
{
    Motor_setCmd(robot.mD, 0);
    Motor_setCmd(robot.mG, 0);
}

extern void Robot_new()
{
    int i = ProSE_Intox_init("127.0.0.1", 12345);
    if (i == 0)
    {
        printf("Connexion établie avec le simulateur\n");
    }
    else
        printf("ERROR: Connexion impossible\n");
}

extern void Robot_free()
{
    Motor_close(robot.mD);
    Motor_close(robot.mG);
    ContactSensor_close(FLOOR_SENSOR);
    ContactSensor_close(FRONT_BUMPER);
    LightSensor_close(LIGHT_SENSOR);
    ProSE_Intox_close();
}

extern int Robot_getRobotSpeed()
{
    return (Motor_getCmd(robot.mD) + Motor_getCmd(robot.mG)) / 2;
}

extern SensorState Robot_getSensorState()
{
    robot.sensorState.collision = ContactSensor_getStatus(robot.floorSensor) || ContactSensor_getStatus(robot.frontSensor);
    robot.sensorState.luminosity = LightSensor_getStatus(robot.lightSensor);
    return robot.sensorState;
}

void Robot_setWheelsVelocity(int mr, int ml)
{
    Motor_setCmd(robot.mD, mr);
    Motor_setCmd(robot.mG, ml);
}