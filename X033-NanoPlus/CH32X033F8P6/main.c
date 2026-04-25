/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/04/06
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 *GPIO routine:
 *PA0 push-pull output.
 *
 ***Only PA0--PA15 and PC16--PC17 support input pull-down.
 */


#include "ch32x035.h"
#include "debug.h"
#include "PWMOP.h"
#include "LED.h"
#include "Key.h"
#include "keyEXTI.h"
#include "EZTimer.h"
/* Global define */

/* Global Variable */

#include <ch32x035_usbfs_device.h>
#include "usbd_composite_km.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{

    LEDGPIO_INIT();
    LED_OFF();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    /* Initialize USART2 for receiving the specified keyboard data */
    USART2_Init( 115200 );
    printf( "USART2 Init OK!\r\n" );

    /* Initialize GPIO for keyboard scan */
    KB_Scan_Init( );
    KB_Sleep_Wakeup_Cfg( );
    printf( "KB Scan Init OK!\r\n" );

    /* Initialize GPIO for mouse scan */
    MS_Scan_Init( );
    MS_Sleep_Wakeup_Cfg( );
    printf( "MS Scan Init OK!\r\n" );

    /* Initialize timer for Keyboard and mouse scan timing */
    TIM3_Init( 1, SystemCoreClock / 10000 - 1 );
    printf( "TIM3 Init OK!\r\n" );

    /* Usb Init */
    USBFS_RCC_Init( );
    USBFS_Device_Init( ENABLE , PWR_VDD_SupplyVoltage());
    USB_Sleep_Wakeup_CFG( );
    while(1)
    {
        //KB_Scan();
        /* Determine if enumeration is complete, perform data transfer if completed */
        if(USBFS_DevEnumStatus)
        {
            if( USBFS_DevEnumStatus )
            {
                /* Handle keyboard scan data */
                KB_Scan_Handle(  );
                //LED_ON();
                /* Handle keyboard lighting */
                //KB_LED_Handle( );

                /* Handle mouse scan data */
                //MS_Scan_Handle( );

                /* Handle USART2 receiving data */
                //USART2_Receive_Handle( );

            }
            //LED_ON();
        }
        //LED_OFF();

    }
}

