#include "hal_data.h"
#include <stdio.h>
#include <stdint.h>
#include "i2c_master0.h"

/* Reading I2C call back event through i2c_Master callback */
static volatile i2c_master_event_t i2c_event = I2C_MASTER_EVENT_ABORTED;
/*
 * private function declarations
 */
fsp_err_t err     = FSP_SUCCESS;
/*******************************************************************************************************************//**
 *  @brief       initialize IIC master module and set up PMOD ACL sensor
 *  @param[IN]   None
 *  @retval      FSP_SUCCESS                  Upon successful open and start of timer
 *  @retval      Any Other Error code apart from FSP_SUCCESS is  Unsuccessful open or start
 **********************************************************************************************************************/

fsp_err_t init_i2c(void)
{
   uint32_t direccion = 0x3C;
   err = R_IIC_MASTER_SlaveAddressSet(&g_i2c_master0_ctrl, direccion,I2C_MASTER_ADDR_MODE_7BIT );
    /* opening IIC master module */
    err = R_IIC_MASTER_Open(&g_i2c_master0_ctrl, &g_i2c_master0_cfg);
    return err;
}


//Comandos inicializacion
uint8_t init1[0x02]={0x00, 0x38}; //Function Set  DL=1: 8 bits; N=1: 2 line; F=0: 5 x 8dot
uint8_t init2[0x02]={0x00,0x0C}; //DISPLAY ON D=1, display on; C=B=0; cursor off; blinking off;
uint8_t init3[0x02]={0x00,0x06}; //ENTRY MODE I/D=1: Increment by 1; S=0: No shift


//SEGUIR LA PAGINA 18 DEL PDF DEL DATASHEET.
// El significado de cada bit esta en la tabla 12
// Como se envian datos de 16 bits, se rellena con 0 a la izquierda.
void initialice_LCD(void)
{
    // Function Set: 00111000
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, init1, 0x02, false);
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS); // DELAY

    // DISPLAY ON: 00001100
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, init2, 0x02, false);
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS); // DELAY

    // ENTRY MODE: 00000110
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, init3, 0x02, false);
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS); // DELAY
}


uint8_t clear[0x02]={0x00,0x01};//limpiar el LCD
void clear_i2c(void)
 {
    err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, clear, 0x02, false);
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS); // DELAY
}

 fsp_err_t write_LCD(uint8_t line[], uint8_t msg[])
 {
     err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, line, 0x02, false);
     R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS); // DELAY

     err = R_IIC_MASTER_Write(&g_i2c_master0_ctrl, msg, 0x08, false);
      R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS); // DELAY

    return err;
}


/*******************************************************************************************************************//**
 *  @brief      User callback function
 *  @param[in]  p_args
 *  @retval None
 **********************************************************************************************************************/
//nombre de mi funcion callback: i2c_lcd_callback
void i2c_master_callback(i2c_master_callback_args_t *p_args)
{
if (NULL != p_args)
    {
        /* capture callback event for validating the i2c transfer event*/
        i2c_event = p_args->event;
    }
}

