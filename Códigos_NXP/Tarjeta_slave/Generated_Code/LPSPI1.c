/*
 * LPSPI1.c

 *
 *  Created on: 07/11/2019
 *      Author: alabe
 */
/*============================================================================*/
/* Project    =   Window Lifter 						                      */
/* Module     =   LPSPI1.c                                                      */
/*============================================================================*/
/*                        OBJECT SPECIFICATION                                */
/*============================================================================*/
/* Layer:            Application, Service, Abstraction, Drivers               */
/* %version:         1 %                                                      */
/* %created_by:      uid02495 %                                               */
/* %date_created:    Tue Dec 10 13:41:01 2019 %                               */
/*============================================================================*/
/* DESCRIPTION : This file contains macros, WindowLifter type definitions,    */
/* enumerations, data types and API function prototypes of WindowLifter Driver*/
/*============================================================================*/
/*                             OBJECT HISTORY                                 */
/*============================================================================*/
/*  REVISION |   DATE      |                               |      AUTHOR      */
/*----------------------------------------------------------------------------*/
/*  1.0      | DD/MM/YYYY  | SAR/SIF/SCN_xxx               | Mr. Template     */
/* Integration under Continuous CM                                            */
/*============================================================================*/

/******************************************************************************/
/*                      Include Section                                       */
/*============================================================================*/
#include "S32K144.h"           /* include peripheral declarations S32K144 */
#include "LPSPI1.h"
#include <window.h>

/*******************************************************************************
 **                      API Service Id Macros                                 **
 *******************************************************************************/

#define LPSPI_ul_PCC_CLOCKS_DISABLED	( (unsigned long) 0x00000000 )
#define LPSPI_ul_PCC_PCS_OPTION_6       ( (unsigned long) 0x06000000 )
#define LPSPI_ul_PCC_CGC_CLOCK_ENABLED  ( (unsigned long) 0x40000000 )
#define LPSPI_ul_PCC_PR_PERIPH_PRESENT  ( (unsigned long) 0x80000000 )

#define LPSPI_ul_CR_CLEAR  				( (unsigned long) 0x00000000 )
#define LPSPI_ul_CR_MODULE_ENABLE       ( (unsigned long) 0x00000001 )
#define LPSPI_ul_CR_DEBUG_ENABLED       ( (unsigned long) 0x00000008 )

#define LPSPI_ul_IER_CLEAR				( (unsigned long) 0x00000000 )

#define LPSPI_ul_DER_CLEAR				( (unsigned long) 0x00000000 )

#define LPSPI_ul_CFGR0_CLEAR			( (unsigned long) 0x00000000 )

#define LPSPI_ul_CFGR1_MASTER			( (unsigned long) 0x000000000 )

#define LPSPI_ul_TCR_FRAMESZ_16_BIT		( (unsigned long) 0x0000000F )
#define LPSPI_ul_TCR_FRAMESZ_8_BIT		( (unsigned long) 0x00000007 )
#define LPSPI_ul_TCR_PCS_3				( (unsigned long) 0x03000000 )
#define LPSPI_ul_TCR_PRES_DIV_4			( (unsigned long) 0x10000000 )
#define LPSPI_ul_TCR_CPHA				( (unsigned long) 0x40000000 )
#define LPSPI_ul_TCR_CPOL				( (unsigned long) 0x80000000 )

#define LPSPI_ul_CCR_SCKPCS_5_DELAYS	( (unsigned long) 0x04000000 )
#define LPSPI_ul_CCR_PCSSCK_10_DELAYS	( (unsigned long) 0x00090000 )
#define LPSPI_ul_CCR_PCSSCK_20_DELAYS   ( (unsigned long) 0x00130000 )
#define LPSPI_ul_CCR_DBT_10_DELAYS		( (unsigned long) 0x00000800 )
#define LPSPI_ul_CCR_SCKDIV_10_DELAYS   ( (unsigned long) 0x00000008 )

#define LPSPI_ul_FCR_3_WATERMARK		( (unsigned long) 0x00000003 )

#define LPSPI_ul_SR_NO_TRANSMISSION		( (unsigned long) 0x00000000 )
#define LPSPI_ul_SR_TRANSMISSION		( (unsigned long) 0x00000001 )

#define MAIN_ub_PORT_PCR_ALTERNATIVE_3 ( (unsigned long) 3)

#define MAIN_ul_PIN_14 ( (unsigned long) 14 )
#define MAIN_ul_PIN_15 ( (unsigned long) 15 )
#define MAIN_ul_PIN_16 ( (unsigned long) 16 )
#define MAIN_ul_PIN_17 ( (unsigned long) 17 )

/*******************************************************************************
 **                      Function Definitions                                  **
 *******************************************************************************/
/**************************************************************
 *  Name                 : PORT_v_InitializeSPI_Pins
 *  Description          : Initialize pins and ports for communication SPI
 *  Input Parameters     : None (name of input parameters)
 *  Return Parameter     : None (name of return variable)
 *  Preconditions        : [yes / No]
 **************************************************************/

void PORT_v_InitializeSPI_Pins(void) {
	/* Description of PCC_PORTB register can be found in section 29.6.23 */
	PCC->PCCn[ PCC_PORTB_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clock for PORTB */

	/* Description of PORT_PCR register can be found in section 12.6.1 Pin Control Register n (PORT_PCRn)  */
	PORTB->PCR[ MAIN_ul_PIN_14] |= PORT_PCR_MUX(MAIN_ub_PORT_PCR_ALTERNATIVE_3); /* Port B14: MUX = ALT3, LPSPI1_SCK */
	PORTB->PCR[ MAIN_ul_PIN_15] |= PORT_PCR_MUX(MAIN_ub_PORT_PCR_ALTERNATIVE_3); /* Port B15: MUX = ALT3, LPSPI1_SIN */
	PORTB->PCR[ MAIN_ul_PIN_16] |= PORT_PCR_MUX(MAIN_ub_PORT_PCR_ALTERNATIVE_3); /* Port B16: MUX = ALT3, LPSPI1_SOUT */
	PORTB->PCR[ MAIN_ul_PIN_17] |= PORT_PCR_MUX(MAIN_ub_PORT_PCR_ALTERNATIVE_3); /* Port B17: MUX = ALT3, LPSPI1_PCS3 */
}

/**************************************************************
 *  Name                 : LPSPI1_v_Initialization
 *  Description          : Initialize modes for communication SPI
 *  Input Parameters     : None (name of input parameters)
 *  Return Parameter     : None (name of return variable)
 *  Preconditions        : [yes / No]
 **************************************************************/
void LPSPI1_v_Initialization(void) {
	PCC->PCCn[PCC_LPSPI1_INDEX] = LPSPI_ul_PCC_CLOCKS_DISABLED; /* Disable clocks to modify PCS ( default) */
	PCC->PCCn[PCC_LPSPI1_INDEX] = LPSPI_ul_PCC_PCS_OPTION_6
			| LPSPI_ul_PCC_CGC_CLOCK_ENABLED | LPSPI_ul_PCC_PR_PERIPH_PRESENT; /* Enable PCS=SPLL_DIV2 (40 MHz func'l clock) */

	LPSPI1->CR = LPSPI_ul_CR_CLEAR; /* Disable module for configuration and clear previous configurations */

	LPSPI1->IER = LPSPI_ul_IER_CLEAR; /* Interrupts not used */

	LPSPI1->DER = LPSPI_ul_DER_CLEAR; /* DMA not used */

	LPSPI1->CFGR0 = LPSPI_ul_CFGR0_CLEAR; /* Defaults: */
	/* RDM0=0: rec'd data to FIFO as normal */
	/* CIRFIFO=0; Circular FIFO is disabled */
	/* HRSEL, HRPOL, HREN=0: Host request disabled */

	LPSPI1->CFGR1 = LPSPI_ul_CFGR1_MASTER; /* Configurations: master mode*/

	/* PCSCFG=0: PCS[3:2] are enabled */
	/* OUTCFG=0: Output data retains last value when CS negated */
	/* PINCFG=0: SIN is input, SOUT is output */
	/* MATCFG=0: Match disabled */
	/* PCSPOL=0: PCS is active low */
	/* NOSTALL=0: Stall if Tx FIFO empty or Rx FIFO full */
	/* AUTOPCS=0: does not apply for master mode */
	/* SAMPLE=0: input data sampled on SCK edge */
	/* MASTER=1: Master mode */

	LPSPI1->TCR = LPSPI_ul_TCR_FRAMESZ_8_BIT | LPSPI_ul_TCR_PCS_3
			| LPSPI_ul_TCR_PRES_DIV_4 | LPSPI_ul_TCR_CPHA;
	/* Transmit cmd: PCS3, 16 bits, prescale func'l clk by 4, etc*/
	/* CPOL=0: SCK inactive state is low */
	/* CPHA=1: Change data on SCK lead'g, capture on trail'g edge*/
	/* PRESCALE=2: Functional clock divided by 2**2 = 4 */
	/* PCS=3: Transfer using PCS3 */
	/* LSBF=0: Data is transfered MSB first */
	/* BYSW=0: Byte swap disabled */
	/* CONT, CONTC=0: Continuous transfer disabled */
	/* RXMSK=0: Normal transfer: rx data stored in rx FIFO */
	/* TXMSK=0: Normal transfer: data loaded from tx FIFO */
	/* WIDTH=0: Single bit transfer */
	/* FRAMESZ=15: # bits in frame = 15+1=16 */
	/* Peripheral bus clock is configured at 40 Mhz clock.
	 * In TCR register the input bus clock is divided by 4, so SPI is operating at 10 MHz (100 nanoseconds)
	 *  */
	LPSPI1->CCR = LPSPI_ul_CCR_SCKPCS_5_DELAYS | LPSPI_ul_CCR_PCSSCK_20_DELAYS
			| LPSPI_ul_CCR_DBT_10_DELAYS | LPSPI_ul_CCR_SCKDIV_10_DELAYS; /* Clock dividers based on prescaled func'l clk of 100 nsec */
	/* SCKPCS=4: SCK to PCS delay = 4+1 = 5 (500 nsec) */
	/* PCSSCK=4: PCS to SCK delay = 9+1 = 10 (1 usec) */
	/* DBT=8: Delay between Transfers = 8+2 = 10 (1 usec) */
	/* SCKDIV=8: SCK divider =8+2 = 10 (1 usec: 1 MHz baud rate) */

	LPSPI1->FCR = LPSPI_ul_FCR_3_WATERMARK; /* RXWATER=0: Rx flags set when Rx FIFO >0 */
	/* TXWATER=3: Tx flags set when Tx FIFO <= 3 */

	LPSPI1->CR = LPSPI_ul_CR_MODULE_ENABLE | LPSPI_ul_CR_DEBUG_ENABLED;
	/* Enable module for operation */
	/* DBGEN=1: module enabled in debug mode */
	/* DOZEN=0: module enabled in Doze mode */
	/* RST=0: Master logic not reset */
	/* MEN=1: Module is enabled */
}

/**************************************************************
 *  Name                 : LPSPI1_v_Send_8_bits_Data
 *  Description          :Function for send data through SPI
 *  Input Parameters     : ui_Data
 *  Return Parameter     : None (name of return variable)
 *  Preconditions        : [yes / No]
 **************************************************************/
void LPSPI1_v_Send_8_bits_Data(char ui_Data) {
	while ((( LPSPI1->SR & LPSPI_SR_TDF_MASK) >> LPSPI_SR_TDF_SHIFT)
			== LPSPI_ul_SR_NO_TRANSMISSION) {
		/* Do nothing if a frame is being transmitted */
	}

	LPSPI1->TDR = (unsigned long) ui_Data; /* Put data in TDR; SPI peripheral will automatically send data */
	LPSPI1->SR |= LPSPI_SR_TDF_MASK; /* Clear TDF flag */
}

/**************************************************************
 *  Name                 : LPSPI1_v_Receive_16bits_Data
 *  Description          :Function for receive data through SPI
 *  Input Parameters     : none
 *  Return Parameter     : ui_returnValue
 *  Preconditions        : [yes / No]
 **************************************************************/
uint16_t LPSPI1_v_Receive_16bits_Data(void) {
	uint16_t ui_returnValue;

	while (((LPSPI1->SR & LPSPI_SR_RDF_MASK) >> LPSPI_SR_RDF_SHIFT)
			== LPSPI_ul_SR_NO_TRANSMISSION) {
		/* Do nothing while a data frame is being transmitted */

	}
	/* Wait at least one RxFIFO entry */
	ui_returnValue = (uint16_t) LPSPI1->RDR; /* Read received data */
	LPSPI1->SR |= LPSPI_SR_RDF_MASK; /* Clear RDF flag */
	return ui_returnValue; /* Return received data */
}

