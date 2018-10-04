// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mt-usb-dci.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ddk/binding.h>
#include <ddk/debug.h>
#include <ddk/device.h>
#include <ddk/driver.h>
#include <ddk/platform-defs.h>
#include <ddk/protocol/gpio.h>
#include <ddk/protocol/platform-device.h>
#include <fbl/algorithm.h>
#include <fbl/unique_ptr.h>

namespace mt_usb_dci {

zx_status_t MtUsbDci::Create(zx_device_t* parent) {
    platform_device_protocol_t pdev;
    zx_handle_t bti;

    auto status = device_get_protocol(parent, ZX_PROTOCOL_PLATFORM_DEV, &pdev);
    if (status != ZX_OK) {
        return status;
    }

    status = pdev_get_bti(&pdev, 0, &bti);
    if (status != ZX_OK) {
        return status;
    }

    fbl::AllocChecker ac;
    auto dci = fbl::make_unique_checked<MtUsbDci>(&ac, parent, &pdev, bti);
    if (!ac.check()) {
        return ZX_ERR_NO_MEMORY;
    }

    status = dci->DdkAdd("mt-usb-dci");
    if (status != ZX_OK) {
        return status;
    }

    // devmgr is now in charge of the device.
    __UNUSED auto* dummy = dci.release();
    return ZX_OK;
}

void MtUsbDci::DdkRelease() {
    delete this;
}

 void MtUsbDci::UsbDciRequestQueue(usb_request_t* req) {
 printf("%s\n", __func__);
 }
 
 zx_status_t MtUsbDci::UsbDciSetInterface(const usb_dci_interface_t* interface) {
    memcpy(&dci_intf_, interface, sizeof(dci_intf_));
    return ZX_OK;
}

 zx_status_t MtUsbDci::UsbDciConfigEp(const usb_endpoint_descriptor_t* ep_desc, const
                            usb_ss_ep_comp_descriptor_t* ss_comp_desc) {
    return ZX_OK;
}

 zx_status_t MtUsbDci::UsbDciDisableEp(uint8_t ep_address) {
    return ZX_OK;
}

 zx_status_t MtUsbDci::UsbDciEpSetStall(uint8_t ep_address) {
    return ZX_OK;
}

 zx_status_t MtUsbDci::UsbDciEpClearStall(uint8_t ep_address) {
    return ZX_OK;
}

 zx_status_t MtUsbDci::UsbDciGetBti(zx_handle_t* out_bti) {
    *out_bti = bti_.get();
    return ZX_OK;
}

} // namespace mt_usb_dci

zx_status_t mt_usb_dci_bind(void* ctx, zx_device_t* parent) {
    return mt_usb_dci::MtUsbDci::Create(parent);
}
