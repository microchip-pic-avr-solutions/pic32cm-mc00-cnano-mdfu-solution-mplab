/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "config/default/bootloader/library/core/ftp/bl_ftp.h"
#include "config/default/bootloader/library/core/bl_core.h"
#include "config/default/bootloader/library/core/bl_app_verify.h"
#include "config/default/bootloader/example/bl_example.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void)
{
    /* Initialize all modules */
    SYS_Initialize(NULL);

    BL_ExampleInitialize();
    
    while (true)
    {
        BL_Example();
    }

    /* Execution should not come here during normal operation */
    return (EXIT_FAILURE);
}


/*******************************************************************************
 End of File
 */

