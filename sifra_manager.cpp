#include "sifra_manager.h"
#include "generichwchanmanager.h"


STRUCTSTATUSCMD StatusCmd;
ChannelsBackupParam Chan[_MAX_LOAD_CHAN_];
// GenericHwChanManager <dword, _WEIGHT_BUFFER_LENGTH_> *weightChan;
extern GenericHwChanManager  *weightChan;


SIFRA_Manager::SIFRA_Manager()
{
   //  weightChan = new GenericHwChanManager();
            // <dword, _WEIGHT_BUFFER_LENGTH_> (_MAX_LOAD_CHAN_, _HW_CHAN_VV_TYPE_, _24_BITS_RESOLUTION_, 10);

}

bool SIFRA_Manager::readLoadInLine15()
{
    int adc_gain_cell_8 = 0;
    int adc_gain_cell_15 = 0;
    dword  d_adc_gain_cell_8 =0;
    dword  d_adc_gain_cell_15 = 0;
    //float this_gain_8;
    //float this_gain_15;
    float cal_adc_system;

 #ifdef VERIFY_NEGATIVE
    if (m_WeightSample[_ADC1_] >= weightChan->getOffset(_ADC1_)){
            d_adc_gain_cell_8 = m_WeightSample[_ADC1_] - weightChan->getOffset(_ADC1_);
            adc_gain_cell_8 = (int)d_adc_gain_cell_8 ;
    } else {
            /*d_adc_gain_cell_8 = weightChan->getOffset(_ADC1_) - m_WeightSample[_ADC1_] ;
            adc_gain_cell_8 = (int)d_adc_gain_cell_8 ;		*/
            return false;
    }


    if (m_WeightSample[_ADC2_] >= weightChan->getOffset(_ADC2_)){
            d_adc_gain_cell_15 = m_WeightSample[_ADC2_] - weightChan->getOffset(_ADC2_);
            adc_gain_cell_15= (int)d_adc_gain_cell_15 ;
    } else {
            /*d_adc_gain_cell_15 = weightChan->getOffset(_ADC2_) - m_WeightSample[_ADC2_] ;
            adc_gain_cell_15 = (int)d_adc_gain_cell_15 ;*/
            return false;
    }
#endif // VERIFY_NEGATIVE

    adc_gain_cell_8 = (int)m_WeightSample[_ADC1_] - (int)weightChan->getOffset(_ADC1_);
    adc_gain_cell_15 = (int)m_WeightSample[_ADC2_] - (int)weightChan->getOffset(_ADC2_);

    //this_gain_8 = (2000.0 /(float)((int)m_WeightSample[_ADC1_] - (int)weightChan->getOffset(_ADC1_)));
    //this_gain_15 = (2000.0 /(float)((int)m_WeightSample[_ADC2_] - (int)weightChan->getOffset(_ADC2_)));

    if(StatusCmd.status == STATO_CALIBRAZIONE_FABBRICA)
    {
//        if(Chan[_ADC1_].AreCalibrate && Chan[_ADC1_].AreCalibrate) - Of course it is an error
        if(Chan[_ADC1_].AreCalibrate && Chan[_ADC2_].AreCalibrate)
        {
            if((adc_gain_cell_8 > ((int)Chan[_ADC1_].AdcTo2Kg_dx - _DELTA_ADC_0Kg_DEF)) &&
                    (adc_gain_cell_8 < ((int)Chan[_ADC1_].AdcTo2Kg_dx + _DELTA_ADC_0Kg_DEF)))
            {
                if((adc_gain_cell_15 > ((int)Chan[_ADC2_].AdcTo2Kg_dx - _DELTA_ADC_0Kg_DEF)) &&
                    (adc_gain_cell_15 < ((int)Chan[_ADC2_].AdcTo2Kg_dx + _DELTA_ADC_0Kg_DEF)))
                {
                    // salvo i valori ADC di guadagno con 2 Kg su cella destra per entrambi i canali
                    Chan[_ADC1_].AdcTo2Kg_dx = adc_gain_cell_8;
                    Chan[_ADC2_].AdcTo2Kg_dx = adc_gain_cell_15;

                    // calcolo un guadagno comune ad entrambi i canali. Tale valore incide sul calcolo di m_loadsystem2 che a sua volta incide sul valore di peso rilevato
                    // questo calcolo � stato mantenuto dal firmware originale per non modificare il metodo di rilevazione del peso sulla rastrelliera
                    if((adc_gain_cell_15 -Chan[_ADC2_].AdcTo2Kg) != 0)
                    {
                        cal_adc_system = (float)((Chan[_ADC1_].AdcTo2Kg - adc_gain_cell_8)/(float)(adc_gain_cell_15 -Chan[_ADC2_].AdcTo2Kg));
                        Chan[_ADC2_].WeightFactoryGain = Chan[_ADC1_].WeightFactoryGain = cal_adc_system;
                        Chan[_ADC2_].Weightgain = Chan[_ADC1_].Weightgain = cal_adc_system;
                        weightChan->setGain(_ADC1_, cal_adc_system);
                        weightChan->setGain(_ADC2_, cal_adc_system);

                        backup_factory_gain_param(_ADC1_, Chan[_ADC1_].WeightFactoryGain, Chan[_ADC1_].AdcTo2Kg, Chan[_ADC1_].AdcTo2Kg_dx);
                        backup_factory_gain_param(_ADC2_, Chan[_ADC2_].WeightFactoryGain, Chan[_ADC2_].AdcTo2Kg, Chan[_ADC2_].AdcTo2Kg_dx);
                        return true;
                    }
                    else
                    {
                        StatusCmd.error_monitor = ERR_CASE_FACTORY_CALIB_DIVIDE_BY_ZERO;
                    }
                }
                else
                {
                    StatusCmd.error_monitor = ERR_CASE_FACTORY_CALIB_GAIN_DX_OUT_CELL_15;
                }
            }
            else
            {
                StatusCmd.error_monitor = ERR_CASE_FACTORY_CALIB_GAIN_DX_OUT_CELL_8;
            }
        }
        else
        {
            // salvo i valori ADC di guadagno con 2 Kg su cella destra per entrambi i canali
            Chan[_ADC1_].AdcTo2Kg_dx = adc_gain_cell_8;
            Chan[_ADC2_].AdcTo2Kg_dx = adc_gain_cell_15;

            // calcolo un guadagno comune ad entrambi i canali. Tale valore incide sul calcolo di m_loadsystem2 che a sua volta incide sul valore di peso rilevato
            // questo calcolo � stato mantenuto dal firmware originale per non modificare il metodo di rilevazione del peso sulla rastrelliera
            cal_adc_system = (float)((Chan[_ADC1_].AdcTo2Kg - adc_gain_cell_8)/(float)(adc_gain_cell_15 -Chan[_ADC2_].AdcTo2Kg));
            Chan[_ADC2_].WeightFactoryGain = Chan[_ADC1_].WeightFactoryGain = cal_adc_system;
            Chan[_ADC2_].Weightgain = Chan[_ADC1_].Weightgain = cal_adc_system;
            weightChan->setGain(_ADC1_, cal_adc_system);
            weightChan->setGain(_ADC2_, cal_adc_system);

            backup_factory_gain_param(_ADC1_, Chan[_ADC1_].WeightFactoryGain, Chan[_ADC1_].AdcTo2Kg, Chan[_ADC1_].AdcTo2Kg_dx);
            backup_factory_gain_param(_ADC2_, Chan[_ADC2_].WeightFactoryGain, Chan[_ADC2_].AdcTo2Kg, Chan[_ADC2_].AdcTo2Kg_dx);
            return true;
        }
    }
    else
    {
        if((adc_gain_cell_8 > ((int)Chan[_ADC1_].AdcTo2Kg_dx - _DELTA_ADC_0Kg)) &&
                    (adc_gain_cell_8 < ((int)Chan[_ADC1_].AdcTo2Kg_dx + _DELTA_ADC_0Kg)))
        {
            if((adc_gain_cell_15 > ((int)Chan[_ADC2_].AdcTo2Kg_dx - _DELTA_ADC_0Kg)) &&
                (adc_gain_cell_15 < ((int)Chan[_ADC2_].AdcTo2Kg_dx + _DELTA_ADC_0Kg)))
            {
                // salvo i valori ADC di guadagno con 2 Kg su cella destra per entrambi i canali
                Chan[_ADC1_].AdcTo2Kg_dx = adc_gain_cell_8;
                Chan[_ADC2_].AdcTo2Kg_dx = adc_gain_cell_15;

                // calcolo un guadagno comune ad entrambi i canali. Tale valore incide sul calcolo di m_loadsystem2 che a sua volta incide sul valore di peso rilevato
                // questo calcolo � stato mantenuto dal firmware originale per non modificare il metodo di rilevazione del peso sulla rastrelliera
                cal_adc_system = (float)((Chan[_ADC1_].AdcTo2Kg - adc_gain_cell_8)/(float)(adc_gain_cell_15 -Chan[_ADC2_].AdcTo2Kg));
                Chan[_ADC2_].Weightgain = Chan[_ADC1_].Weightgain = cal_adc_system;
                weightChan->setGain(_ADC1_, cal_adc_system);
                weightChan->setGain(_ADC2_, cal_adc_system);

                backup_new_gain_value(_ADC1_, Chan[_ADC1_].Weightgain);
                backup_new_gain_value(_ADC2_, Chan[_ADC2_].Weightgain);
                return true;
            }
            else
            {
                StatusCmd.error_monitor = ERR_CASE_CALIB_GAIN_DX_OUT_CELL_15;
            }
        }
        else
        {
            StatusCmd.error_monitor = ERR_CASE_CALIB_GAIN_DX_OUT_CELL_8;
        }
    }

    return false;

}


bool backup_factory_gain_param(byte numchan, float FactoryGain, int adc2Kg, int adc2Kgdx)
{
    byte type_of_calibration = CHAN_CALIB_CHECK_FACTORY_VAL;

#ifdef INCLUDE_EE
    if(!EE_write(&FactoryGain, sizeof(float), (ADDRESS_FACTORY_GAIN + numchan*BASESTRUCTADDR)))
    {
        return false;
    }
    if(!EE_write(&FactoryGain, sizeof(float), (ADDRESS_GAIN + numchan*BASESTRUCTADDR))) // se factory, aggiorno anche il valore user
    {
        return false;
    }
    if(!EE_write(&adc2Kg, sizeof(int), (ADDRESS_2Kg_READ + numchan*BASESTRUCTADDR)))
    {
        return false;
    }
    if(!EE_write(&adc2Kgdx, sizeof(int), (ADDRESS_2KgDx_READ + numchan*BASESTRUCTADDR)))
    {
        return false;
    }
    if(!EE_byte_write((ADDRESS_CHAN_ARE_MEM_CALIBRATE + 1 + numchan*BASESTRUCTADDR), type_of_calibration))
    {
        return false;
    }
#endif
    return true;

}

bool backup_new_gain_value(byte numchan, float NewGain)
{
#ifdef INCLUDE_EE
    if(!EE_write(&NewGain, sizeof(float), (ADDRESS_GAIN + numchan*BASESTRUCTADDR)))
    {
        return false;
    }
#endif
    return true;

}

