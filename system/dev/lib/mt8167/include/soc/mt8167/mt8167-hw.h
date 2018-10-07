// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

/* Peripheral Memory */
#define MT8167_XO_BASE                                      0x10000000
#define MT8167_XO_SIZE                                      0x624

#define MT8167_GPIO_BASE                                    0x10005000
#define MT8167_GPIO_SIZE                                    0x700

#define MT8167_SOC_BASE                                     0x10200000
#define MT8167_SOC_SIZE                                     0x1D00

// SOC Interrupt polarity registers start
#define MT8167_SOC_INT_POL                                  0x620

#define MT8167_I2C0_BASE                                    0x11009000
#define MT8167_I2C0_SIZE                                    0x8c

#define MT8167_I2C1_BASE                                    0x1100a000
#define MT8167_I2C1_SIZE                                    0x8c

#define MT8167_I2C2_BASE                                    0x1100b000
#define MT8167_I2C2_SIZE                                    0x8c

#define MT8167_USB0_BASE                                    0x11100000
#define MT8167_USB0_LENGTH                                  0x1000

#define MT8167_USBPHY_BASE                                  0x11110000
#define MT8167_USBPHY_LENGTH                                0x1000

/* Interrupts */
#define MT8167_USB0_IRQ                                     104
#define MT8167_I2C0_IRQ                                     112
#define MT8167_I2C1_IRQ                                     113
#define MT8167_I2C2_IRQ                                     114
