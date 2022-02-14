#include "main.h"
#include "system.h"
#include "bsp_uart1.h"
#include "app_info.h"
#include "app_jump.h"
#define OUTPUT_TYPE_PP 0
#define OUTPUT_TYPE_OD 1
typedef enum
{
    DO_LED_BLUE = 0,
    DO_LED_GREEN = 1,
    DO_LED_RED = 2,
} do_map_enum;
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
typedef struct
{
    GPIO_TypeDef *gpio;  // GPIO address
    uint16_t      pin;   // pin num
    uint8_t       state; // IO state
    uint8_t       type;
} GPIO_DEFINE_TYPE;

// IO MAP
static GPIO_DEFINE_TYPE bsp_do_map[] = {
    {GPIOB, GPIO_PIN_5, 0, OUTPUT_TYPE_PP}, // DO_LED_RED
    {GPIOB, GPIO_PIN_0, 0, OUTPUT_TYPE_PP}, // DO_LED_GREEN
    {GPIOB, GPIO_PIN_1, 0, OUTPUT_TYPE_PP}, // DO_LED_BLUE
};
void bsp_write_do(uint8_t n, uint8_t state)
{
    GPIO_DEFINE_TYPE *io;

    if (n < ARRAY_LEN(bsp_do_map))
    {
        io        = bsp_do_map + n;
        io->state = state;
        HAL_GPIO_WritePin(io->gpio, io->pin, (GPIO_PinState)state);
    }

}
void bsp_write_do_toggle(uint8_t n)
{
    GPIO_DEFINE_TYPE *io;

    if (n < ARRAY_LEN(bsp_do_map))
    {
        io        = bsp_do_map + n;
        io->state = !io->state;
        HAL_GPIO_WritePin(io->gpio, io->pin, (GPIO_PinState)io->state);
        return;
    }
}
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
    for (uint16_t i = 0; i < ARRAY_LEN(bsp_do_map); i++)
    {
        if (bsp_do_map[i].type == OUTPUT_TYPE_PP)
        {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        }
        else
        {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        }

        GPIO_InitStruct.Pin = bsp_do_map[i].pin;
        HAL_GPIO_Init(bsp_do_map[i].gpio, &GPIO_InitStruct);
    }
}

void vTaskLED1(void * pvParameters)
{ 
  LED_Init();
  while(1)
  {
    bsp_write_do_toggle(DO_LED_RED);
    //printf("hello world %d !\r\n",xTaskGetTickCount());
    vTaskDelay( ( 1000 / portTICK_RATE_MS ) ); 
  }
} 

void vTaskLED2(void * pvParameters)
{ 
  LED_Init(); 
  while(1)
  {
    bsp_write_do_toggle(DO_LED_BLUE);
    //printf("hello world %d !\r\n",xTaskGetTickCount());
    vTaskDelay(( 1000 / portTICK_RATE_MS )); 
  }
} 

void vTaskLED3(void * pvParameters)
{ 
  LED_Init();
  while(1)
  {
    bsp_write_do_toggle(DO_LED_GREEN);
    //printf("hello world %d !\r\n",xTaskGetTickCount());
    vTaskDelay( 1000 / portTICK_RATE_MS); 
    //static int8_t cont = 0;
    //cont++;
    //if(cont > 10)
    //{
    //    cont = 0;
    //    app_jump_to_boot(MAGIC_NUM_UART_BOOT);
    //}
  }
} 
void test_led(void)
{
    xTaskCreate(vTaskLED1, "LED1", 500, NULL, 0, NULL);
    xTaskCreate(vTaskLED2, "LED2", 500, NULL, 0, NULL);
    xTaskCreate(vTaskLED3, "LED3", 500, NULL, 0, NULL); 
}
void uart1_read(const uint8_t data)
{
    static uint8_t status = 0;
    if( 4 == status )
    {
        return;
    }
    if(data < 0)
    {
        return;
    }
    switch (status)
    {
    case 0:
        if(data == 'b')
        {
            status  = 1;
        }
        else
        {
            status = 0;
        }
         break;
    case 1:
        if(data == 'o')
        {
            status = 2;
        }
        else
        {
            status = 0;
        }
        break;
    case 2:
        if(data == 'o')
        {
            status = 3 ;
        }
        else
        {
            status = 0;
        }
        break;
    case 3:
        if(data == 't')
        {
            status = 4;
        }
        else
        {
            status = 0;
        }
        break;
    default:
        break;
    }
    if(4 == status)
    {
      //uint8_t data[2] = {0x12,0x10}; 
      //bsp_uart1_send(data,2);
      HAL_Delay(1000);
      app_jump_to_boot(MAGIC_NUM_UART_BOOT);
      return ;
    }
}
int main( void ) 
{
    sys_init();
    test_led();
    bsp_uart1_init(115200);
    bsp_uart1_install_rx_callback(uart1_read);
    vTaskStartScheduler(); 
    while(1);
}   

/*************************** End of file ****************************/
