// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <ddk/debug.h>
#include <ddk/device.h>
#include <ddk/platform-defs.h>
#include <ddk/protocol/platform-bus.h>

#include <soc/aml-s905d2/s905d2-gpio.h>
#include <soc/aml-s905d2/s905d2-hw.h>

#include <limits.h>

#include "astro.h"

static const pbus_mmio_t display_mmios[] = {
    {
        // DSI Host Controller
        .base = S905D2_MIPI_DSI_BASE,
        .length = S905D2_MIPI_DSI_LENGTH,
    },
    {
        // DSI PHY
        .base = S905D2_DSI_PHY_BASE,
        .length = S905D2_DSI_PHY_LENGTH,
    },
    {
        // HHI
        .base = S905D2_HIU_BASE,
        .length = S905D2_HIU_LENGTH,
    },
    {
        // VBUS/VPU
        .base = S905D2_VPU_BASE,
        .length = S905D2_VPU_LENGTH,
    },
    {
        // AOBUS
        .base = S905D2_AOBUS_BASE,
        .length = S905D2_AOBUS_LENGTH,
    },
    {
        // CBUS
        .base = S905D2_CBUS_BASE,
        .length = S905D2_CBUS_LENGTH,
    },
};

static const pbus_irq_t display_irqs[] = {
    {
        .irq = S905D2_VIU1_VSYNC_IRQ,
        .mode = ZX_INTERRUPT_MODE_EDGE_HIGH,
    },
};

static const pbus_gpio_t display_gpios[] = {
    {
        // Backlight Enable
        .gpio = S905D2_GPIOA(10),
    },
    {
        // LCD Reset
        .gpio = S905D2_GPIOH(6),
    },
    {
        // Panel detection
        .gpio = S905D2_GPIOH(5),
    },
};

static const pbus_bti_t display_btis[] = {
    {
        .iommu_index = 0,
        .bti_id = BTI_DISPLAY,
    },
};

static const pbus_i2c_channel_t display_i2c_channels[] = {
    {
        .bus_id = ASTRO_I2C_3,
        .address = I2C_BACKLIGHT_ADDR,
    },
};

static const uint32_t display_protocols[] = {
    ZX_PROTOCOL_AMLOGIC_CANVAS,
};

static pbus_dev_t display_dev = {
    .name = "display",
    .vid = PDEV_VID_AMLOGIC,
    .pid = PDEV_PID_AMLOGIC_S905D2,
    .did = PDEV_DID_AMLOGIC_DISPLAY,
    .mmios = display_mmios,
    .mmio_count = countof(display_mmios),
    .irqs = display_irqs,
    .irq_count = countof(display_irqs),
    .gpios = display_gpios,
    .gpio_count = countof(display_gpios),
    .i2c_channels = display_i2c_channels,
    .i2c_channel_count = countof(display_i2c_channels),
    .btis = display_btis,
    .bti_count = countof(display_btis),
    .protocols = display_protocols,
    .protocol_count = countof(display_protocols),
};

zx_status_t aml_display_init(aml_bus_t* bus) {
    zx_status_t status = pbus_device_add(&bus->pbus, &display_dev);
    if (status != ZX_OK) {
        zxlogf(ERROR, "%s: Could not add display dev: %d\n", __FUNCTION__, status);
        return status;
    }
    return ZX_OK;
}
