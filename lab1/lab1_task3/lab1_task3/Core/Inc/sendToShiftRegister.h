#define SER_PORT GPIOB
#define SER_PIN GPIO_PIN_15

#define SRCLK_PORT GPIOC
#define SRCLK_PIN GPIO_PIN_0

#define RCLK_PORT GPIOB
#define RCLK_PIN GPIO_PIN_7

void init();
void sendToShiftRegister(uint16_t);
