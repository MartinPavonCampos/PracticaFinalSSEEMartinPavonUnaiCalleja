#include "hal_data.h"
#include "r_adc_api.h"
#include "ADC_pot.h"
#include "i2c_master0.h"
#include "r_iic_master.h"
#include "hal_entry.h"
#include <stdio.h>
#include "UART_config.h"




int estadoSistema = 0;
int filtro[5] = {0}; //filtro de 5 valores
uint8_t fila1[0x02]={0x00, 0x80};
uint8_t fila2[0x02]={0x00, 0xC0};
uint8_t mensaje1[0x08]={0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
uint8_t mensaje2[0x08]={0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};



FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

void encenderLed(){
    fsp_err_t err;

        /* Inicializar el módulo IOPORT y configurar los pines */

        err = R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);

        /* Maneja cualquier error. Esta función debe ser definida por el usuario. */
        assert(FSP_SUCCESS == err);

        /* Configurar Pin como salida*/
        /* Llamar al R_IOPORT_PinCfg si la configuración no formaba parte de las
        configuraciones iniciales realizadas en open */
        err = R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_06,
        IOPORT_CFG_PORT_DIRECTION_OUTPUT);
        assert(FSP_SUCCESS == err);

        /* Escribimos el valor alto al pin 106 */
        bsp_io_level_t level = BSP_IO_LEVEL_HIGH;
        err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_06, level);
        assert(FSP_SUCCESS == err);
}

void apagarLed(){
    fsp_err_t err;

        /* Inicializar el módulo IOPORT y configurar los pines */

        err = R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);

        /* Maneja cualquier error. Esta función debe ser definida por el usuario. */
        assert(FSP_SUCCESS == err);

        /* Configurar Pin como salida*/
        /* Llamar al R_IOPORT_PinCfg si la configuración no formaba parte de las
        configuraciones iniciales realizadas en open */
        err = R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_06,
        IOPORT_CFG_PORT_DIRECTION_OUTPUT);
        assert(FSP_SUCCESS == err);

        /* Escribimos el valor bajo al pin 106 */
        err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_06, BSP_IO_LEVEL_LOW);
}

void encenderZumb(){
    fsp_err_t err;

        /* Inicializar el módulo IOPORT y configurar los pines */

        err = R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);

        /* Maneja cualquier error. Esta función debe ser definida por el usuario. */
        assert(FSP_SUCCESS == err);

        /* Configurar Pin como salida*/
        /* Llamar al R_IOPORT_PinCfg si la configuración no formaba parte de las
        configuraciones iniciales realizadas en open */
        err = R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_13,
        IOPORT_CFG_PORT_DIRECTION_OUTPUT);
        assert(FSP_SUCCESS == err);

        /* Escribimos el valor alto al pin 106 */
        bsp_io_level_t level = BSP_IO_LEVEL_HIGH;
        err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_13, level);
        assert(FSP_SUCCESS == err);
}

//Funcion para apagar el zumbador
void apagarZumb(){
    fsp_err_t err;

        /* Inicializar el módulo IOPORT y configurar los pines */

        err = R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);

        /* Maneja cualquier error. Esta función debe ser definida por el usuario. */
        assert(FSP_SUCCESS == err);

        /* Configurar Pin como salida*/
        /* Llamar al R_IOPORT_PinCfg si la configuración no formaba parte de las
        configuraciones iniciales realizadas en open */
        err = R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_13,
        IOPORT_CFG_PORT_DIRECTION_OUTPUT);
        assert(FSP_SUCCESS == err);

        /* Escribimos el valor bajo al pin 106 */
        err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_13, BSP_IO_LEVEL_LOW);
}
fsp_err_t icu_init(void){
    fsp_err_t err = FSP_SUCCESS;

    /* Open ICU module */
    err = R_ICU_ExternalIrqOpen(&g_external_irq0_ctrl, &g_external_irq0_cfg);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        /* ICU Open failure message */
        //APP_ERR_PRINT ("\r\n**R_ICU_ExternalIrqOpen API FAILED**\r\n");
    }
    return err;
}
//Permitir a una interrupcion externa al canal especificado
fsp_err_t icu_enable(void){
    fsp_err_t err = FSP_SUCCESS;

    /* Enable ICU module */
    err = R_ICU_ExternalIrqEnable(&g_external_irq0_ctrl);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        /* ICU Enable failure message */
       // APP_ERR_PRINT ("\r\n**R_ICU_ExternalIrqEnable API FAILED**\r\n");
    }
    return err;
}
//Cerrar el modulo ICU
void icu_deinit(void){
    fsp_err_t err = FSP_SUCCESS;

    /* Close ICU module */
    err = R_ICU_ExternalIrqClose(&g_external_irq0_ctrl);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        /* ICU Close failure message */
        //APP_ERR_PRINT("\r\n**R_ICU_ExternalIrqClose API FAILED**\r\n");
    }
}


void callback_boton(external_irq_callback_args_t *p_args){
    estadoSistema = 1;

}


void creacionMensajefila1(uint8_t mensaje1[], int peligro, uint16_t valor){

    if(peligro == 0){
        mensaje1[0x00] = 0x40;
        mensaje1[0x01] = 0x53;
        mensaje1[0x02] = 0x59;
        mensaje1[0x03] = 0x53;
        mensaje1[0x04] = 0x54;
        mensaje1[0x05] = 0x45;
        mensaje1[0x06] = 0x4D;
        mensaje1[0x07] = 0x4F;
    }else{
        char str[100];
            char numero_char;
            unsigned char asciiValue1, asciiValue2, asciiValue3, asciiValue4, asciiValue5;


        sprintf(str, "%i", valor);


        numero_char = str[0];
        asciiValue1 = (unsigned char)numero_char;
        numero_char = str[1];
        asciiValue2 = (unsigned char)numero_char;
        numero_char = str[2];
        asciiValue3 = (unsigned char)numero_char;
        numero_char = str[3];
        asciiValue4 = (unsigned char)numero_char;
        numero_char = str[4];
        asciiValue5 = (unsigned char)numero_char;

            mensaje1[0x00] = 0x40;
            mensaje1[0x01] = asciiValue1;
            mensaje1[0x02] = asciiValue2;
            mensaje1[0x03] = asciiValue3;
            mensaje1[0x04] = asciiValue4;
            mensaje1[0x05] = asciiValue5;
            mensaje1[0x06] = 0x80;
            mensaje1[0x07] = 0x80;
        }


}


void creacionMensajefila2(uint8_t mensaje2[], int peligro){
    if(peligro == 0){

        mensaje2[0x00] = 0x40;
        mensaje2[0x01] = 0x80;
        mensaje2[0x02] = 0x80;
        mensaje2[0x03] = 0x80;
        mensaje2[0x04] = 0x80;
        mensaje2[0x05] = 0x80;
        mensaje2[0x06] = 0x80;
        mensaje2[0x07] = 0x80;
    }else{
        mensaje2[0x00] = 0x40;
        mensaje2[0x01] = 0x50;
        mensaje2[0x02] = 0x45;
        mensaje2[0x03] = 0x4C;
        mensaje2[0x04] = 0x49;
        mensaje2[0x05] = 0x47;
        mensaje2[0x06] = 0x52;
        mensaje2[0x07] = 0x4F;
    }

}


void actualizarFiltro(uint16_t valorNuevo){
    // Se mueven los anteriores registros del filtro al siguiente registro
    filtro[4] = filtro[3];
    filtro[3] = filtro[2];
    filtro[2] = filtro[1];
    filtro[1] = filtro[0];
    // Se asigna el valor del potenciómetro al primer registro
    filtro[0] = valorNuevo;

}

//Funcion que devuelve la media de los ultimos 5 valores
uint16_t mediaFiltro(){
    int suma = 0;
    // Se recorren el filtro
    for (uint16_t i = 0; i < 5; i++) {
        // Se agregan los valores del filtro
        suma = suma + filtro[i];
    }
    // Devolver la media
    int media1= suma / 5;
    uint16_t media = (uint16_t)media1;
    return media;
}


/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{

    //Inicializacion de la interrupcion
           fsp_err_t err= FSP_SUCCESS;
           // 1. Inicializar el driver IRQ
           err = icu_init();
           // Handle error
           if(FSP_SUCCESS != err){
                          //fallo inicializacion
                      }
           // 2. Habilitar el IRQ driver
           err = icu_enable();
           // Handle error
           if(FSP_SUCCESS != err){
               // Cerrar el modulo IRQ .
               icu_deinit();
           }
           fsp_err_t i2c = init_i2c();
               clear_i2c();
               initialice_LCD();
               ADCInit();
               int peligro = 0;
               init_uart();
               err = R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
               assert(FSP_SUCCESS == err);
               err = R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_13, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
               assert(FSP_SUCCESS == err);
               uint16_t velocidad;
               char str[3];


    while(1){
        write_LCD(fila1, mensaje1);
        write_LCD(fila2, mensaje2);



        if(estadoSistema == 1){
            //encender();
            creacionMensajefila1(mensaje1, peligro, 0);
            ADCStartScan();
            ADCWaitConversion();

            uint16_t adc_result = ReadADC(ADC_CHANNEL_4);

            actualizarFiltro(adc_result);
            uint16_t valorFiltro = mediaFiltro();

            velocidad = (uint16_t) adc_result;
            sprintf(str, "%i", velocidad);
            uart_write(str);



            if(valorFiltro>12288){
                peligro = peligro + 1;
                encenderLed();
                creacionMensajefila1(mensaje1, peligro, valorFiltro);
                creacionMensajefila2(mensaje2, peligro);
                R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS); // NOLINT
                apagarLed();
                R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS); // NOLINT
                if (peligro == 1){
                  encenderZumb();
                  R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS); // NOLINT
                  apagarZumb();

                }


            }else{
                peligro = 0;
                creacionMensajefila2(mensaje2, peligro);
            }


        }else{
           // apagar();


        }

    }
    /* TODO: add your own code here */

#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&g_ioport_ctrl, g_ioport.p_cfg);
    }
}

#if BSP_TZ_SECURE_BUILD

BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{

}
#endif
