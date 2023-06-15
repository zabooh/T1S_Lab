/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "config/default/peripheral/port/plib_port.h"
#include "config/default/peripheral/sercom/usart/plib_sercom1_usart.h"
#include "gfx_mono/gfx_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void) {

    {
        static int old_but1 = 0;
        int temp_but1 = BUTTON1_Get();
        if (temp_but1 && !old_but1) {
            LED1_Set();
        }
        if (!temp_but1 && old_but1) {
            LED1_Clear();
            SERCOM1_USART_Virtual_Receive("iperf -s\n");
            gfx_mono_print_scroll("iperf TCP server");
        }
        old_but1 = temp_but1;

        static int old_but2 = 0;
        int temp_but2 = BUTTON2_Get();
        if (temp_but2 && !old_but2) {
            LED2_Set();
        }
        if (!temp_but2 && old_but2) {
            LED2_Clear();
            SERCOM1_USART_Virtual_Receive("iperf -s -u\n");
            gfx_mono_print_scroll("iperf UDP server");
        }
        old_but2 = temp_but2;
        
        static int old_but3 = 0;
        int temp_but3 = BUTTON3_Get();
        if (temp_but3 && !old_but3) {
            LED3_Set();
        }
        if (!temp_but3 && old_but3) {
            LED3_Clear();
            SERCOM1_USART_Virtual_Receive("iperfk\n");
            gfx_mono_print_scroll("iperf kill server");
        }
        old_but3 = temp_but3;
    }


    /* Check the application's current state. */
    switch (appData.state) {
            /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized) {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {

            break;
        }

            /* TODO: implement your application state machine.*/


            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
