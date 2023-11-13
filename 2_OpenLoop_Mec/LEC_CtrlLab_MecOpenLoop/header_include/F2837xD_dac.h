//###########################################################################
//
// FILE:    F2837xD_dac.h
//
// TITLE:   F2837xD Device DAC Register Definitions.
//
//###########################################################################
// $TI Release: F2837xD Support Library v190 $
// $Release Date: Mon Feb  1 16:51:57 CST 2016 $
// $Copyright: Copyright (C) 2013-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#ifndef __F2837xD_DAC_H__
#define __F2837xD_DAC_H__

#ifdef __cplusplus
extern "C" {
#endif



//#define		CPU_8BIT
#define		CPU_16BIT
//#define		CPU_32BIT

//typedef		char			BOOL;
typedef		char			U8;
typedef		signed char		S8;
typedef		unsigned short	U16;
typedef		short			S16;
#ifdef CPU_32BIT
typedef		unsigned int	U32;
typedef		int				S32;
typedef		unsigned long	U64;
typedef		signed long		S64;
#else
typedef		unsigned long	U32;
typedef		long			S32;
typedef		double			U64;
typedef		double			S64;
#endif
typedef 	float			F32;
typedef 	double			D64;

#define		INT_TYPE		1
#define		FLOAT_TYPE		2

#define		DAC_CH1			1
#define		DAC_CH2			2
#define		DAC_CH3			3
#define		DAC_CH4			4

#define 	DAC1_CS			(GpioDataRegs.GPBDAT.bit.GPIO55)
#define 	DAC2_CS			(GpioDataRegs.GPBDAT.bit.GPIO57)

#define NOP				asm("	NOP")

#define DAC1_TIMER_TBPRD	1000		// Period register
#define DAC1_MAX_CMPA		999		// Period register
#define DAC1_MIN_CMPA		  0		// Period register
#define DAC1_MAX_CMPB		999		// Period register
#define DAC1_MIN_CMPB		   0		// Period register
#define DAC1_MID_CMPA       500        // Period register
#define DAC1_MID_CMPB       500        // Period register
#define _3DAC1_MID_CMPA       500        // Period register
#define _3DAC1_MID_CMPB       500        // Period register
#define _1_5DAC1_CMP      454.5454545        // Period register
#define _1_5DAC1_CMPA      454.5454545        // Period register
#define _1_5DAC1_CMPB      454.5454545        // Period register

#define _3MAX0_5_MIN_0_5            909.0909091
#define _3MAX1_MIN_1                454.5454545
#define _3MAX2_MIN_2                227.2727273
#define _3MAX5_MIN_5                90.90909091
#define _3MAX10_MIN_10              45.45454545
#define _3MAX50_MIN_50              9.090909091
#define _3MAX100_MIN_100            4.545454545
#define _3MAX200_MIN_200            2.272727273
#define _3MAX500_MIN_500            1.262626263
#define _3MAX1000_MIN_10000         0.454545455


#define _3MAX0_5_MIN_0          1818.181818
#define _3MAX1_MIN_0            909.0909091
#define _3MAX2_MIN_0            454.5454545
#define _3MAX5_MIN_0            181.8181818
#define _3MAX2PI_MIN_0          144.6863119
#define _3MAX10_MIN_0           90.90909091
#define _3MAX50_MIN_0           18.18181818
#define _3MAX100_MIN_0         9.090909091
#define _3MAX200_MIN_0         4.545454545
#define _3MAX360_MIN_0         2.525252525
#define _3MAX500_MIN_0         1.818181818
#define _3MAX1000_MIN_0        0.909090909



typedef struct {
    float32     gfDAC_Gain;
    float32     gfDAC_Value;
    float32     gfDAC_Offset;
}DAC_CH;

extern DAC_CH gstDAC_CH1;
extern DAC_CH gstDAC_CH2;
extern DAC_CH gstDAC_CH3;
extern DAC_CH gstDAC_CH4;

//4000/
#define DAC_MAX_3_M3_GAIN       100
#define DAC_MAX_3_M0_GAIN       100
#define DAC_MAX_3_M0_GAIN       100
#define DAC_MAX_100_M100_GAIN   100

/*
********************************************************************************
* GLOBAL VARIABLE
********************************************************************************
*/
extern float32 gfDAC_Val_1;
extern float32 gfDAC_Val_2;
extern float32 gfDAC_Val_3;
extern float32 gfDAC_Val_4;


/*
********************************************************************************
* PROTOTYPES
********************************************************************************
*/
 void InitDACGpio(void);

 void Updata_DACValue(void);

//---------------------------------------------------------------------------
// DAC Individual Register Bit Definitions:

struct DACREV_BITS {                    // bits description
    Uint16 REV:8;                       // 7:0 DAC Revision Register
    Uint16 rsvd1:8;                     // 15:8 Reserved
};

union DACREV_REG {
    Uint16  all;
    struct  DACREV_BITS  bit;
};

struct DACCTL_BITS {                    // bits description
    Uint16 DACREFSEL:1;                 // 0 DAC Reference Select
    Uint16 rsvd1:1;                     // 1 Reserved
    Uint16 LOADMODE:1;                  // 2 DACVALA Load Mode
    Uint16 rsvd2:1;                     // 3 Reserved
    Uint16 SYNCSEL:4;                   // 7:4 DAC PWMSYNC Select
    Uint16 rsvd3:8;                     // 15:8 Reserved
};

union DACCTL_REG {
    Uint16  all;
    struct  DACCTL_BITS  bit;
};

struct DACVALA_BITS {                   // bits description
    Uint16 DACVALA:12;                  // 11:0 DAC Active Output Code
    Uint16 rsvd1:4;                     // 15:12 Reserved
};

union DACVALA_REG {
    Uint16  all;
    struct  DACVALA_BITS  bit;
};

struct DACVALS_BITS {                   // bits description
    Uint16 DACVALS:12;                  // 11:0 DAC Shadow Output Code
    Uint16 rsvd1:4;                     // 15:12 Reserved
};

union DACVALS_REG {
    Uint16  all;
    struct  DACVALS_BITS  bit;
};

struct DACOUTEN_BITS {                  // bits description
    Uint16 DACOUTEN:1;                  // 0 DAC Output Code
    Uint16 rsvd1:15;                    // 15:1 Reserved
};

union DACOUTEN_REG {
    Uint16  all;
    struct  DACOUTEN_BITS  bit;
};

struct DACLOCK_BITS {                   // bits description
    Uint16 DACCTL:1;                    // 0 DAC Control Register Lock
    Uint16 DACVAL:1;                    // 1 DAC Value Register Lock
    Uint16 DACOUTEN:1;                  // 2 DAC Output Enable Register Lock
    Uint16 rsvd1:13;                    // 15:3 Reserved
};

union DACLOCK_REG {
    Uint16  all;
    struct  DACLOCK_BITS  bit;
};

struct DACTRIM_BITS {                   // bits description
    Uint16 OFFSET_TRIM:8;               // 7:0 DAC Offset Trim
    Uint16 rsvd1:4;                     // 11:8 Reserved
    Uint16 rsvd2:4;                     // 15:12 Reserved
};

union DACTRIM_REG {
    Uint16  all;
    struct  DACTRIM_BITS  bit;
};

struct DAC_REGS {
    union   DACREV_REG                       DACREV;                       // DAC Revision Register
    union   DACCTL_REG                       DACCTL;                       // DAC Control Register
    union   DACVALA_REG                      DACVALA;                      // DAC Value Register - Active
    union   DACVALS_REG                      DACVALS;                      // DAC Value Register - Shadow
    union   DACOUTEN_REG                     DACOUTEN;                     // DAC Output Enable Register
    union   DACLOCK_REG                      DACLOCK;                      // DAC Lock Register
    union   DACTRIM_REG                      DACTRIM;                      // DAC Trim Register
    Uint16                                   rsvd1;                        // Reserved
};

//---------------------------------------------------------------------------
// DAC External References & Function Declarations:
//
#ifdef CPU1
extern volatile struct DAC_REGS DacaRegs;
extern volatile struct DAC_REGS DacbRegs;
extern volatile struct DAC_REGS DaccRegs;
#endif
#ifdef CPU2
extern volatile struct DAC_REGS DacaRegs;
extern volatile struct DAC_REGS DacbRegs;
extern volatile struct DAC_REGS DaccRegs;
#endif
#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif

//===========================================================================
// End of file.
//===========================================================================
