#ifndef _MSPM0_CONFIG_H
#define _MSPM0_CONFIG_H

#include "ti_mspm0_include.h"

// -------------------- UART1 配置 --------------------
#define UART_1_INST                                                        UART1
#define UART_1_INST_FREQUENCY                                            4000000
#define UART_1_INST_IRQHandler                                  UART1_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                         DL_GPIO_PIN_9
#define GPIO_UART_1_TX_PIN                                         DL_GPIO_PIN_8
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM20)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM19)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM20_PF_UART1_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM19_PF_UART1_TX
#define UART_1_BAUD_RATE                                                (115200)
#define UART_1_IBRD_4_MHZ_115200_BAUD                                        (2)
#define UART_1_FBRD_4_MHZ_115200_BAUD                                       (11)

// -------------------- UART2 配置 --------------------
#define UART_2_INST                                                        UART2
#define UART_2_INST_FREQUENCY                                            4000000
#define UART_2_INST_IRQHandler                                  UART2_IRQHandler
#define UART_2_INST_INT_IRQN                                      UART2_INT_IRQn
#define GPIO_UART_2_RX_PORT                                                GPIOB
#define GPIO_UART_2_TX_PORT                                                GPIOB
#define GPIO_UART_2_RX_PIN                                        DL_GPIO_PIN_16
#define GPIO_UART_2_TX_PIN                                        DL_GPIO_PIN_15
#define GPIO_UART_2_IOMUX_RX                                     (IOMUX_PINCM33)
#define GPIO_UART_2_IOMUX_TX                                     (IOMUX_PINCM32)
#define GPIO_UART_2_IOMUX_RX_FUNC                      IOMUX_PINCM33_PF_UART2_RX
#define GPIO_UART_2_IOMUX_TX_FUNC                      IOMUX_PINCM32_PF_UART2_TX
#define UART_2_BAUD_RATE                                                (115200)
#define UART_2_IBRD_4_MHZ_115200_BAUD                                        (2)
#define UART_2_FBRD_4_MHZ_115200_BAUD                                       (11)

// -------------------- Timer2 配置 --------------------
#define TIMER2_INST                                                       TIMER2
#define TIMER2_INT_IRQN                                           TIMER2_INT_IRQn
#define TIMER2_IRQHandler                                         TIMER2_IRQHandler

#endif 