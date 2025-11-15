#pragma once

/*
 * Pin Mapping:
 * X Step: PA10
 * Y Step: PB3
 * Z Step: PB5
 * X Dir: PB4
 * Y Dir: PB10
 * Z Dir: PA8
 * Stepper enable/disable: PA9
 * X Limit: PC7
 * Y Limit: PB6
 * Z Limit: PA7
 * Limit Switches should be configured pull-up
*/

#define X_STEP_BITPOS_GPIOA (1U << 10)
#define Y_STEP_BITPOS_GPIOB (1U << 3)
#define Z_STEP_BITPOS_GPIOB (1U << 5)
#define X_DIR_BITPOS_GPIOB (1U << 4)
#define Y_DIR_BITPOS_GPIOB (1U << 10)
#define Z_DIR_BITPOS_GPIOA (1U << 8)
#define X_LIMIT_BITPOS_GPIOC (1U << 7)
#define Y_LIMIT_BITPOS_GPIOB (1U << 6)
#define Z_LIMIT_BITPOS_GPIOA (1U << 7)
