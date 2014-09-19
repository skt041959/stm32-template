#include    "hw_config.h"
#include    "usb_lib.h"
#include    "usb_desc.h"
#include    "usb_pwr.h"

GPIO_InitTypeDef  GPIO_InitStructure; 
NVIC_InitTypeDef NVIC_InitStructure;
USART_InitTypeDef USART_InitStructure;
I2C_InitTypeDef I2C_InitStructure;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
u16 PrescalerValue = 0;

DMA_InitTypeDef DMA_InitStructure;

extern __IO uint8_t Receive_Buffer[64];
extern __IO uint32_t Receive_length;
extern __IO uint32_t length;
uint8_t Send_Buffer[64];
uint32_t packet_sent = 1;
uint32_t packet_receive = 1;

vu32 TimeDelay=0;
vu8 d=0;

u8 Delay(u32 nms)
{
    if(!d)
    {
        d=1;
        TimeDelay=nms;
        while (TimeDelay);
        d=0;
        return 0;
    }
    else
        return 1;
}

void NVIC_Config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /*NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;*/
    /*NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;*/
    /*NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;*/
    /*NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;*/
    /*NVIC_Init(&NVIC_InitStructure);*/

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|\
            RCC_APB2Periph_GPIOB|\
            RCC_APB2Periph_GPIOC|\
            RCC_APB2Periph_GPIOD|\
            RCC_APB2Periph_GPIOE|\
            RCC_APB2Periph_AFIO\
            ,ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void TIM3_Config() 
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    /* Compute the prescaler value */
    PrescalerValue = 7200 - 1;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 100; /*ARR*/
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; /*RSC*/
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
}

void USART_Config()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate=9600;
    USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
    USART_Init(USART1,&USART_InitStructure);
    /*USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); */
    USART_Cmd(USART1,ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);
}

void hex(s16 d, char * str)
{
    u8 t;
    t=(d>>12)&0x000f;
    str[0] = (t<=9)?(t+0x30):(t+0x37);
    t=(d>>8)&0x000f;
    str[1] = (t<=9)?(t+0x30):(t+0x37);
    t=(d>>4)&0x000f;
    str[2] = (t<=9)?(t+0x30):(t+0x37);
    t=d&0x000f;
    str[3] = (t<=9)?(t+0x30):(t+0x37);
    /*str[4] = '\0';*/
}

int main()
{
    SysTick_Config(SystemCoreClock/1000);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    Set_System();
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

/*
 *    GPIO_Config();
 *
 *    GPIOA->BSRR = GPIO_Pin_15;
 *
 *    while (1)
 *    {
 *        if (bDeviceState == CONFIGURED) {
 *            CDC_Receive_DATA();
 *            [>Check to see if we have data yet <]
 *            if (Receive_length != 0) {
 *                if (packet_sent == 1)
 *                    CDC_Send_DATA((unsigned char*) Receive_Buffer, Receive_length);
 *                Receive_length = 0;
 *            }
 *        }
 *    }
 */
    while(1);
}

void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);

        if(GPIOE->IDR & GPIO_Pin_8)
            GPIOE->BRR=GPIO_Pin_8;
        else
            GPIOE->BSRR=GPIO_Pin_8;
    }
}

void SysTick_Handler(void)
{
    TimeDelay--;
}
