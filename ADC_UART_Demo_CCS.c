//******************************************************************************
//  "ADC/UART Demo" TI LaunchPad Code Composer Studio Program
//  Created by: Justin Russo
//
//  Description: This demo allows the user to utilize the on-board temperature
//  sensor through ADC14 and capture the ambient temperature and transmits
//  it through the UART to a PC.
//
//                   MSP43P432P401
//             -----------------------
//         /|\|                       |
//          | |                       |
//          --|RST                    |
//            |                       |
//            |                       |
//            |     P1.3/EUSCI_A0->TXD|---->
//            |                       |
//            |     P1.2/EUSCI_A0->RXD|<----
//            |                       |
//            |                       |
//
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/driverlib/pcm.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
const eUSCI_UART_ConfigV1 uartConfig =      // Settings to configure the UART
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     78,
     2,
     0,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
};
volatile uint32_t cal30;                    // variables for calibrating the temperature sensor
volatile uint32_t cal85;
volatile float calDifference;
static float TXData;                        // variables for storing the temperature data
static char TXStr[10];
                                            // variables for temperature data
volatile float tempC;                       // Celsius
int i=0,j=0;                                // variables for indexing
int main(void)
{
    MAP_WDT_A_holdTimer();
    Interrupt_enableSleepOnIsrExit();
    ADC14_enableModule();                   // Initializing ADC with temperature sensor
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_TEMPSENSEMAP);
                                            // Configuring ADC memory in repeat mode
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A22, false);
                                            // Configuring the sample/hold time for 192
    ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192, ADC_PULSE_WIDTH_192);
                                            // Enabling sample timer in auto iteration mode and interrupts
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableInterrupt(ADC_INT0);
                                            // Enabling Interrupts
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();
                                            // Setting reference voltage to 2.5V and enabling temperature sensor
    REF_A_enableTempSensor();
    REF_A_setReferenceVoltage(REF_A_VREF2_5V);
    REF_A_enableReferenceVoltage();
    cal30 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_30_DEGREES_C);
    cal85 = SysCtl_getTempCalibrationConstant(SYSCTL_2_5V_REF, SYSCTL_85_DEGREES_C);
    calDifference = cal85 - cal30;

    CS_setDCOCenteredFrequency(CS_48MHZ);   // baud rate 19200
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
                                            // if receiving and sending modify both pins, if only receiving configure 1.2, and vice versa
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    MAP_UART_enableModule(EUSCI_A0_BASE);
    //MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);  there wont be any info received by the MSP432
    MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
    MAP_Interrupt_enableMaster();
}

void ADC14_IRQHandler(void)
{
    uint64_t status;
    int16_t conRes;
    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if (status & ADC_INT0)
    {
                                            // Converting raw ADC values into actual temperatures
        conRes = ((ADC14_getResult(ADC_MEM0) - cal30) * 55);
                                            // Calculating Celsius temperature
        tempC = (conRes / calDifference) + 30.0f;
                                            // Calculating Fahreneheit temperature
        TXData = tempC * 9.0f / 5.0f + 32.0f;
        sprintf(TXStr, "%f", TXData);       // Converting the temperature data to string format
    }
}

void EUSCIA0_IRQHandler(void)
{
    for(i = 200000; i > 0; i--){            // Added delay so the UART can be read
               if(i == 1){
                   UART_transmitData(EUSCI_A0_BASE, TXStr[j]);
                                            // Transmit the first character of data
                   j++;
                                            // Index to the next character
                   if (j == 9)              // Reserve 8 characters for temperature data
                       UART_transmitData(EUSCI_A0_BASE, '\n');
                   if (j == 10){
                       j = 0;               // Reset index for next temperature
                       UART_transmitData(EUSCI_A0_BASE, '\r');
                                            // Toggle next sample from ADC14
                       ADC14_enableConversion();
                       ADC14_toggleConversionTrigger();
                   }
               }
            }
}
