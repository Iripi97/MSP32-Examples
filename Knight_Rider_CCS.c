/* "Knight Rider" TI LaunchPad Code Composer Studio Program
 * Created by: Justin Russo
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/driverlib/pcm.h>

int main(void) {

    WDT_A->CTL = WDT_A_CTL_PW |                                 // Stop WDT
                 WDT_A_CTL_HOLD;

       P2->SEL0 &= ~0x20;                                       // Pin 2.5 --> 00100000
       P2->SEL1 &= ~0x20;
       P2->DIR |= 0x20;
       P3->SEL0 &= ~0x01;                                       // Pin 3.0 --> 00000001
       P3->SEL1 &= ~0x01;
       P3->DIR |= 0x01;
       P5->SEL0 &= ~0x80;                                       // Pin 5.7 --> 10000000
       P5->SEL1 &= ~0x80;
       P5->DIR |= 0x80;
       P1->SEL0 &= ~0x40;                                       // Pin 1.6 --> 01000000
       P1->SEL1 &= ~0x40;
       P1->DIR |= 0x40;
       P1->SEL0 &= ~0x80;                                       // Pin 1.7 --> 10000000
       P1->SEL1 &= ~0x80;
       P1->DIR |= 0x80;
       P5->SEL0 &= ~0x01;                                       // Pin 5.0 --> 00000001
       P5->SEL1 &= ~0x01;
       P5->DIR |= 0x01;
       P3->SEL0 &= ~0x20;                                       // Pin 3.5 --> 00100000
       P3->SEL1 &= ~0x20;
       P3->DIR |= 0x20;
       P3->SEL0 &= ~0x40;                                       // Pin 3.6 --> 01000000
       P3->SEL1 &= ~0x40;
       P3->DIR |= 0x40;
       GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
                                                                // Configuring S2 as input
       GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
       GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
                                                                // Configuring interrupt for button S2
       GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
                                                                // Enable interrupts for GPIO pin
       Interrupt_enableInterrupt(INT_PORT1);                    // Enable interrupts on Port 1
       Interrupt_enableMaster();                                // Enable interrupts globally

       PCM_gotoLPM3();                                          // Stay in low power mode when not handling interrupt
}

void PORT1_IRQHandler(void)                                     // Beginning of ISR
{
    uint32_t status;                                            // ISR variable for detecting button pushes
    do
    {
        status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
        if (status & GPIO_PIN4)                                 // If button is pushed
        {
            P2->OUT |= 0x20;                                    // Cycle forward through LEDs
            __delay_cycles(80000);
            P2->OUT &= ~0x20;
            __delay_cycles(80000);
            P3->OUT |= 0x1;
            __delay_cycles(80000);
            P3->OUT &= ~0x1;
            __delay_cycles(80000);
            P5->OUT |= 0x80;
            __delay_cycles(80000);
            P5->OUT &= ~0x80;
            __delay_cycles(80000);
            P1->OUT |= 0x40;
            __delay_cycles(80000);
            P1->OUT &= ~0x40;
            __delay_cycles(80000);
            P1->OUT |= 0x80;
            __delay_cycles(80000);
            P1->OUT &= ~0x80;
            __delay_cycles(80000);
            P5->OUT |= 0x1;
            __delay_cycles(80000);
            P5->OUT &= ~0x1;
            __delay_cycles(80000);
            P3->OUT |= 0x20;
            __delay_cycles(80000);
            P3->OUT &= ~0x20;
            __delay_cycles(80000);
            P3->OUT |= 0x40;
            __delay_cycles(80000);
            P3->OUT &= ~0x40;
            __delay_cycles(120000);
                P3->OUT |= 0x40;                                // Cycle backwards through LEDs
                __delay_cycles(80000);
                P3->OUT &= ~0x40;
                __delay_cycles(80000);
                P3->OUT |= 0x20;
                __delay_cycles(80000);
                P3->OUT &= ~0x20;
                __delay_cycles(80000);
                P5->OUT |= 0x1;
                __delay_cycles(80000);
                P5->OUT &= ~0x1;
                __delay_cycles(80000);
                P1->OUT |= 0x80;
                __delay_cycles(80000);
                P1->OUT &= ~0x80;
                __delay_cycles(80000);
                P1->OUT |= 0x40;
                __delay_cycles(80000);
                P1->OUT &= ~0x40;
                __delay_cycles(80000);
                P5->OUT |= 0x80;
                __delay_cycles(80000);
                P5->OUT &= ~0x80;
                __delay_cycles(80000);
                P3->OUT |= 0x1;
                __delay_cycles(80000);
                P3->OUT &= ~0x1;
                __delay_cycles(80000);
                P2->OUT |= 0x20;
                __delay_cycles(80000);
                P2->OUT &= ~0x20;
                __delay_cycles(80000);
            GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);   // Reset interrupt flag so sequence can be repeated
        }
    }
    while(status);
}                                                               // End of ISR
