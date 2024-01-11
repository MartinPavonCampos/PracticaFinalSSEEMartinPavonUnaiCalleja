#include "ADC_pot.h"
void ADCInit(void){
    fsp_err_t err = FSP_SUCCESS;
    /* Inicializar ADC */
   err = R_ADC_Open(&g_adc0_pot_ctrl, &g_adc0_pot_cfg);
   assert(FSP_SUCCESS == err);
   err = R_ADC_ScanCfg(&g_adc0_pot_ctrl, &g_adc0_pot_channel_cfg);
   assert(FSP_SUCCESS == err);
}

void ADCStartScan (void){
   /* In software trigger mode, start a scan by calling R_ADC_ScanStart(). In other modes, enable external
    * triggers by calling R_ADC_ScanStart(). */
    R_ADC_ScanStart(&g_adc0_pot_ctrl);
}

void ADCWaitConversion (void){
   /* Wait for conversion to complete. */
    adc_status_t status;
    R_ADC_StatusGet(&g_adc0_pot_ctrl, &status);

    if (status.state == ADC_STATE_SCAN_IN_PROGRESS) {
        return(0);
    }
    return(1);
}



 uint16_t ReadADC (adc_channel_t Achan){
   fsp_err_t err = FSP_SUCCESS;
   uint16_t channel4_conversion_result;
   /*Lectura del valor del potenciometro, conectado en el canal 4*/
   err = R_ADC_Read(&g_adc0_pot_ctrl, Achan, &channel4_conversion_result);
   assert(FSP_SUCCESS == err);
   return (channel4_conversion_result);
   }




 static fsp_err_t ADC_Initialize(void)
 {
     fsp_err_t fsp_err = FSP_SUCCESS;

     fsp_err = R_ADC_Open(&g_adc0_pot_ctrl, &g_adc0_pot_cfg);
     if(FSP_SUCCESS != fsp_err)
         return fsp_err;

     fsp_err = R_ADC_ScanCfg(&g_adc0_pot_ctrl, &g_adc0_pot_channel_cfg);
     if(FSP_SUCCESS != fsp_err)
         return fsp_err;

     fsp_err = R_ADC_ScanStart(&g_adc0_pot_ctrl);
     if(FSP_SUCCESS != fsp_err)
         return fsp_err;

     return fsp_err;
 }
