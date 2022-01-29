#ifndef SIFRA_MANAGER_H
#define SIFRA_MANAGER_H

#define _MAX_LOAD_CHAN_		2	// max 2celle nel m3300 con misura differenziale
/* delta adc di differenza accettabile sul valore di zero della cella */
#define _DELTA_ADC_1_PERC_FS_		656			// 1 % del fondo scala della cella di carico
#define _DELTA_ADC_0Kg				(_DELTA_ADC_1_PERC_FS_ * 3)	// 3% del fondo scala
#define _DELTA_ADC_0Kg_DEF			(_DELTA_ADC_1_PERC_FS_ * 5)	// 5% del fondo scala

#define CHAN_CALIB_CHECK_FACTORY_VAL		0xAA
#define CHAN_CALIB_RESET_VALUE			0x55

#define WEIGHT_DEFAULT_OFFSET		25000
#define WEIGHT_DEFAULT_GAIN		1.03

#define WEIGHT_DEFAULT_2Kg_LOAD			21000
#define WEIGHT_DEFAULT_2Kg_NO_LOAD		100

#define WEIGHT_GAIN_LIMIT_LOW				0.0
#define WEIGHT_GAIN_LIMIT_HIGH			2.0

#define WEIGHT_OFFSET_LIMIT_LOW			1000
#define WEIGHT_OFFSET_LIMIT_HIGH			40000

typedef unsigned long	dword;
typedef	unsigned short	word;
typedef unsigned char	byte;

enum errorStatus_2_t
{
    E_ERROR_NONE = 0,
    E_ERROR_ENCODER,
    E_ERROR_WEIGHT,
    ERR_EEPROM,
    ERR_RAM_READ_BK,
    ERR_RAM_READ_ENC,		// 5
    ERR_RAM_WRITE_BK,
    ERR_RAM_WRITE_ENC,
    ERR_SYSTEM,
    ERR_MSG_NOT_EXPECTED,
    ERR_OPCODE_UNKNOWN,		//10
    ERR_START_MESSAGE,
    ERR_START_NOT_HANDLED,
    ERR_ESD_DAMAGE,
    ERR_RESTORE_DATA,
    ERR_READ_EEPROM,			// 15
    ERR_CASE_SACCA_MNGR,
    ERR_CASE_MANUAL_MNGR,
    ERR_CASE_RT_MNGR,
    ERR_CASE_RISCIACQUO_MNGR,
    ERR_CASE_ST_MNGR,			// 20
    ERR_CASE_SAMPLE_MNGR,
    ERR_CASE_AIR_MNGR,
    ERR_CASE_CALIB_MNGR,
    ERR_CASE_SERVICE_MNGR,
    ERR_CASE_EMC_MNGR,		// 25
    ERR_CASE_POMPE_MNGR,
    ERR_CASE_LED_MNGR,
    ERR_CASE_PINCH_TEST_MNGR,
    ERR_CASE_VOL_CONTROL_RT,
    ERR_CASE_PESO_INIT_SACCA,	// 30
    ERR_CASE_PESO_FINALE_SACCA,
    ERR_CASE_PESO_PARZIALE_SACCA,
    ERR_CASE_CODA_SAMPLE_MNGR,
    ERR_MSG_VIA_DEBUG,
    ERR_CASE_VUOTO,				// 35
    ERR_CASE_STATO_EROGAZIONE_SACCA,
    ERR_CASE_STATO_ENCODER_SACCA,
    ERR_CASE_TEST_DEBUG,
    ERR_CASE_PESO_FINALE_RT_MNGR,
    ERR_CASE_PESO_EROGATO_RT_MNGR,	// 40
    ERR_CASE_PESO_DA_EROGARE_SACCA_MNGR,
    ERR_CASE_PESO_FINALE_SACCA_MNGR,
    ERR_CASE_PESO_PARZIALE_MANUAL_MNGR,
    ERR_CASE_CHIUSURA_MANUAL_MNGR,
    ERR_CASE_PESO_PARZIALE_RINSE_MNGR,	// 45
    ERR_CASE_CHIUSURA_RINSE_MNGR,
    ERR_CASE_TEST_FLOW_PUMP,
    ERR_CASE_VARIAZIONE_PESO,
    ERR_CASE_TOT_EN_LINES,
    ERR_CASE_SET_STABILITY_LEVEL,			// 50
    ERR_CASE_NON_CALA,
    ERR_CASE_FACTORY_CALIB_OFFSET_OUT_CELL_8,
    ERR_CASE_FACTORY_CALIB_OFFSET_OUT_CELL_15,
    ERR_CASE_FACTORY_CALIB_OFFSET_LOW,
    ERR_CASE_CALIB_OFFSET_OUT_CELL_8,				// 55
    ERR_CASE_CALIB_OFFSET_OUT_CELL_15,
    ERR_CASE_FACTORY_CALIB_GAIN_SX_OUT_CELL_8,
    ERR_CASE_FACTORY_CALIB_GAIN_SX_OUT_CELL_15,
    ERR_CASE_CALIB_GAIN_SX_OUT_CELL_8,
    ERR_CASE_CALIB_GAIN_SX_OUT_CELL_15,				// 60
    ERR_CASE_FACTORY_CALIB_GAIN_DX_OUT_CELL_8,
    ERR_CASE_FACTORY_CALIB_GAIN_DX_OUT_CELL_15,
    ERR_CASE_CALIB_GAIN_DX_OUT_CELL_8,
    ERR_CASE_CALIB_GAIN_DX_OUT_CELL_15,
    ERR_CASE_READ_ADC_VALUES,						// 65
    ERR_CASE_TEST_OPEN_MORE_EV,
    ERR_RESET_CALIB_DATA_FAIL,
    ERR_CASE_CALC_PUMP_NOT_MOVE,
    ERR_CASE_FACTORY_CALIB_DIVIDE_BY_ZERO,
    ERR_CASE_DOWNLOAD_STATE,
    ERR_CASE_START_FAILED,
    ERR_CASE_NUM
};


enum COMMANDSTATUS
{
    STATO_IDLE = 0x0001,
    STATO_ERRORE = 0x0002,
    STATO_CALIBRAZIONE_FABBRICA = 0x0004,
    STATO_STOP = 0x0008,
    STATO_START = 0x0010,
    STATO_FINE_SEQ = 0x0020,
    STATO_VERIFICA_BILANCE = 0x0040,
    STATO_CALIBRAZIONE_UTENTE = 0x0080
};

enum kindOfWeightChan{
    _ADC1_= 0,
    _ADC2_,
    _NUM_OF_WEIGHT_CHAN_
};

typedef struct
{
    float			WeightFactoryGain;		// 4 byte
    word			WeightFactoryOffset;		// 2 byte
    float			Weightgain;				// 4 byte
    word			Weightoffset;			// 2 byte
    int			AdcTo2Kg;				// 4 byte, può essere occasionalmente negativa
    int			AdcTo2Kg_dx;			// 4 byte, può essere occasionalmente negativa
    byte			typeOfOffsetCal;			// 1 byte, usato per leggere il valore del byte che indica se i valori di offset associati alla cella corrispondono ad una calibrazione valida
    byte			typeOfGainCal;			// 1 byte, usato per leggere il valore del byte che indica se i valori di guadagno associati alla cella corrispondono ad una calibrazione valida
    bool 		AreCalibrate;				// 1 byte
} ChannelsBackupParam;

struct STRUCTSTATUSCMD
{
    bool air_block_en;		// attivazione del blocco motore per aria
    bool m_FillingOnCourse;
    byte status;
    byte flagsErrorLine;		// segnalazione errore bloccante rilevato
    byte tot_enabled_lines;
    int next_line;
    /*
    statusChan è un vettore nel caso del modulo M3300 di 2 word.
    _ADC1_ e _ADC2_ fanno parte di un enum: _ADC1_ corrisponde a 0 mentre _ADC2_ corrisponde a 1.
    La prima word (statusChan[_ADC1_]) memorizza nei due byte che la costituiscono i vari errori che possono presentarsi sul  modulo ed elencati nell'enum ERRORSTATUS
    riportato sotto.
    La seconda word (statusChan[_ADC2_]) memorizza invece le vie (per l'M3300 sono 8) che hanno preentato gli errori di aria o prodotto.
    In particolare il primo byte della word più significativa (8 bit più a sinistra) ha settato a 1 le vie che hanno presentato errore prodotto: ad esempio se il primo byte è 01001001
    l'errore prodotto è stato rilevato sulle vie 8, 11 e 14 del modulo M3300 (le vie vanno da 8 a 15).
    Il secondo byte invece (8 bit più a destra) ha settato a 1 le vie che hanno presentato errore aria: ad esempio se il primo byte è 00100100 l'errore prodotto è stato rilevato
    sulle vie 10 e 13.
    */
    byte statusAirSensors;
    word statusChan[_MAX_LOAD_CHAN_];
    word SIFRAComPacketsRx;
    word SIFRAComHwErrors;
    word SIFRAComProtocolErrors;
    word SIFRAComUnknownErrors;
    byte error_stop;
    short variazione_peso;
    byte test_pinch_done;
    byte error_monitor;
    byte state;
    byte prevState;
    byte phase;
};





class SIFRA_Manager
{
public:
    SIFRA_Manager();

    bool readLoadInLine15();


    dword m_WeightSample[_MAX_LOAD_CHAN_];


private:

};

bool backup_factory_gain_param(byte numchan, float FactoryGain, int adc2Kg, int adc2Kgdx);
bool backup_new_gain_value(byte numchan, float NewGain);


#endif // SIFRA_MANAGER_H
