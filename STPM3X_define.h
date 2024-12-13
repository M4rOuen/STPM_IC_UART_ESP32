//Marouennnnnnnnnnnnn

#ifndef STPM_DEFINE_h
#define STPM_DEFINE_h
#define STPM3x_FRAME_LEN 5
#define CRC_8 (0x07)
typedef union {
  struct {
    unsigned CLRSS_T01 : 4;
    unsigned ClearSS1  : 1;
    unsigned ENVREF1   : 1;  
    unsigned TC1       : 3;  
    unsigned           : 7;
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned LSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR100bits_t;

typedef union {
  struct {
    unsigned      : 1;
    unsigned AEM1 : 1; 
    unsigned APM1 : 1; 
    unsigned BHPFV1 : 1; 
    unsigned BHPFC1 : 1; 
    unsigned ROC1   : 1; 
    unsigned BLPFV1 : 1; 
    unsigned BLPFC1 : 1; 
    unsigned LPW1   : 4; 
    unsigned LPC1   : 2; 
    unsigned LCSI   : 2; 
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR101bits_t;

typedef union {
  struct {
    unsigned CLRSS_T02 : 4;
    unsigned ClearSS2  : 1;
    unsigned ENVREF2   : 1; 
    unsigned TC2       : 3;  
    unsigned           : 7;
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned LSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR200bits_t;

typedef union {
  struct {
    unsigned        : 1;
    unsigned AEM2   : 1; 
    unsigned APM2   : 1; 
    unsigned BHPFV2 : 1; 
    unsigned BHPFC2 : 1; 
    unsigned ROC2   : 1; 
    unsigned BLPFV2 : 1; 
    unsigned BLPFC2 : 1; 
    unsigned LPW2   : 4; 
    unsigned LPC2   : 2; 
    unsigned LCS2   : 2; 
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR201bits_t;

typedef union {
  struct {
    unsigned SAG_TIME_THR : 14;
    unsigned ZCR_SEL      : 2;        
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned LSW    : 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR300bits_t;

typedef union {
  struct {
    unsigned ZCR_EN   : 1;    
    unsigned TMP_TOL  : 2;
    unsigned TMP_EN   : 1;
    unsigned SW_Reset : 1;   
    unsigned SW_Latch1: 1;  
    unsigned SW_Latch2: 1; 
    unsigned SWAuto_Latch : 1; 
    unsigned LED_OFF1 : 1;  
    unsigned LED_OFF2 : 1;   
    unsigned EN_CUM   : 1;  
    unsigned REFFREQ  : 1;  
    unsigned          : 4;
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR301bits_t;
typedef union {
  struct {
    unsigned PHC2: 10;         
    unsigned PHV2: 2;          
    unsigned PHC1: 10;         
    unsigned PHV1: 2;          
    unsigned     : 8;
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR400bits_t;
typedef union {
  struct {
    unsigned CHV1    : 12;          
    unsigned SWV_THR1: 10;         
    unsigned SAG_THR1: 10;          
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR500bits_t;
typedef union {
  struct {
    unsigned CHC1    : 12;          
    unsigned SWV_THR1: 10;         
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 6;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR600bits_t;
typedef union {
  struct {
    unsigned CHV2    : 12;          
    unsigned SWV_THR2: 10;          
    unsigned SAG_THR2: 10;         
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR700bits_t;
typedef union {
  struct {
    unsigned CHV2    : 12;          
    unsigned SWV_THR2: 10;          
    unsigned SAG_THR2: 10;          
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR800bits_t;
typedef union {

  struct {
    unsigned AH_UP1  : 12;
    unsigned OFA1    : 10;
    unsigned OFAF1   : 10;
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR900bits_t;
typedef union {
  struct {
    unsigned AH_DOWN1  : 12;
    unsigned OFR1     : 10;
    unsigned OFAS1    : 10;
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR1000bits_t;
typedef union {
  struct {
    unsigned AH_UP2   : 12;
    unsigned OFA2     : 10;
    unsigned OFAF2    : 10;
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR1100bits_t;
typedef union {
  struct {
    unsigned AH_DOWN2  : 12;
    unsigned OFR2     : 10;
    unsigned OFAS2    : 10;
  };
  struct {
    unsigned LSW: 16;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DSP_CR1200bits_t;
typedef union {
  struct {
    unsigned  : 16;
  };
} DFE_CR100bits_t;
typedef union {
  struct {
    unsigned        : 10;
    unsigned GAIN1  : 2;
    unsigned        : 4;
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DFE_CR101bits_t;
typedef union {
  struct {
    unsigned        : 10;
    unsigned GAIN1  : 2;
    unsigned        : 4;
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned MSW: 16;
  };
  struct {
    unsigned short Address;
  };
} DFE_CR201bits_t;
typedef union {
  struct {
    unsigned CRC_Pol   : 8;
    unsigned Noise_EN  : 1;
    unsigned BRK_ERR   : 1;
    unsigned           : 4;
    unsigned CRC_EN    : 1;
    unsigned ISB_FIRST : 1;
  };
  struct {
    unsigned LSB: 8;
    unsigned MSB: 8;
    unsigned LSW: 16;
  };
  struct {
    unsigned short Address;
  };
} US1_REG100bits_t;
typedef union {
  struct {
    unsigned TIME_OUT : 8;
  };
  struct {
    unsigned LSB: 8;
  };
  struct {
    unsigned short Address;
  };
} US1_REG101bits_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH1_ReactEvergy_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} __PH1_AppartEvergy;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH1_ActivePower_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH1_FundPower_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH1_ReactPower_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH1_AppartPower_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH2_ActivePower_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH2_FundPower_t;
typedef struct {
  unsigned int DATA;
  unsigned short Row;
  unsigned short Address;
} PH2_ReactPower_t;
typedef struct {
  unsigned int* dataBuffer;
  unsigned short Row;
  unsigned short Address;
} parm_Reg_t;
#define Period_Address 0x2E
#define V1_Data_Address 0x30
#define C1_Data_Address 0x32
#define V2_Data_Address 0x34
#define C2_Data_Address 0x36
#define V1_Fund_Address 0x38
#define C1_Fund_Address 0x3A
#define V2_Fund_Address 0x3C
#define C2_Fund_Address 0x3E
#define C1_RMS_Data_Address 0x48
#define C2_RMS_Data_Address 0x4A
#define SAG1_SWV1_TIME_Address 0x4C;
#define C1PHA_SWC1_TIME_Address 0x4E
#define SAG2_SWV2_TIME_Address 0x50
#define C2PHA_SWC2_TIME_Address 0x52
#define PH1_Active_Energy_Address 0x54
#define PH1_Fundamental_Energy_Address 0x56
#define PH1_Reactive_Energy_Address 0x58
#define PH1_Apparent_Energy_Address 0x5A
#define PH1_Active_Power_Address 0x5C
#define PH1_Fundamental_Power_Address 0x5E
#define PH1_Reactive_Power_Address 0x60
#define PH1_Apparent_RMS_Power_Address 0x62
#define PH1_Apparent_Vectorial_Power_Address 0x64
#define PH1_Momentary_Active_Power_Address 0x66
#define PH1_Momentary_Fundamental_Power_Address 0x68
#define PH1_AH_ACC_Address 0x6A
#define PH2_Active_Energy_Address 0x6C
#define PH2_Fundamental_Energy_Address 0x6E
#define PH2_Reactive_Energy_Address 0x70
#define PH2_Apparent_RMS_Energy_Address 0x72
#define PH2_Active_Power_Address 0x74
#define PH2_Fundamental_Power_Address 0x76
#define PH2_Reactive_Power_Address 0x78
#define PH2_Apparent_RMS_Power_Address 0x7A
#define PH2_Apparent_Vectorial_Power_Address 0x7C
#define PH2_Momentary_Active_Power_Address 0x7E
#define PH2_Momentary_Fundamental_Power_Address 0x80
#define PH2_AH_ACC_Address 0x84
#define Tot_Active_Energy_Address 0x84
#define Tot_Fundamental_Energy_Address 0x86
#define Tot_Reactive_Energy_Address 0x88
#define Tot_Apparent_Energy_Address 0x8A
#endif
