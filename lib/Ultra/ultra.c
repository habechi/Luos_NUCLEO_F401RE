/******************************************************************************
 * @file button
 * @brief driver example a simple button
 * @author Luos
 * @version 0.0.0
 ******************************************************************************/
#include "ultra.h"
#include "gpio.h"
#include "ultrasonic.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
linear_position_t data_ready;
/*******************************************************************************
 * Function
 ******************************************************************************/
static void Ultra_MsgHandler(service_t *service, msg_t *msg);
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
void Ultra_Init(void)
{
    revision_t revision = {.major = 1, .minor = 0, .build = 0};

    Luos_CreateService(Ultra_MsgHandler, ULTRA_TYPE, "ultra", revision);
}
/******************************************************************************
 * @brief loop must be call in project loop
 * @param None
 * @return None
 ******************************************************************************/
void Ultra_Loop(void)
{
  data_ready = LinearOD_PositionFrom_mm(calculerrrr());
}
/******************************************************************************
 * @brief Msg Handler call back when a msg receive for this service
 * @param Service destination
 * @param Msg receive
 * @return None
 ******************************************************************************/
static void Ultra_MsgHandler(service_t *service, msg_t *msg)
{
    if (msg->header.cmd == GET_CMD)
    {
        msg_t pub_msg;
        pub_msg.header.target_mode = ID;
        pub_msg.header.target      = msg->header.source;
        LinearOD_PositionToMsg(&data_ready, &pub_msg);
        Luos_SendMsg(service, &pub_msg);
    }
}
