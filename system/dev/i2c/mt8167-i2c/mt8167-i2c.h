// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <ddk/mmio-buffer.h>
#include <ddk/protocol/i2c.h>

#include <ddktl/device.h>
#include <ddktl/protocol/i2c-impl.h>

#include <fbl/array.h>
#include <fbl/atomic.h>
#include <fbl/unique_ptr.h>

namespace mt8167_i2c {

class Mt8167I2c;
using DeviceType = ddk::Device<Mt8167I2c>;

class Mt8167I2c : public DeviceType,
                     public ddk::I2cImplProtocol<Mt8167I2c> {
public:
    Mt8167I2c(zx_device_t* parent)
        : DeviceType(parent) {}
    ~Mt8167I2c();
    zx_status_t Init();

    // Methods required by the ddk mixins
    void DdkRelease();
    uint32_t I2cImplGetBusCount();
    zx_status_t I2cImplGetMaxTransferSize(uint32_t bus_id, size_t* out_size);
    zx_status_t I2cImplSetBitRate(uint32_t bus_id, uint32_t bitrate);
    zx_status_t I2cImplTransact(uint32_t bus_id, i2c_impl_op_t* ops, size_t count);

private:
    enum class Wait {
        kBusy,
        kIdle,
        kInterruptPending
    };
    static constexpr const char* WaitStr(Wait type) {
        switch (type) {
        case Wait::kBusy:
            return "BUSY";
        case Wait::kIdle:
            return "IDLE";
        case Wait::kInterruptPending:
            return "INTERRUPT_PENDING";
        }
        return "UNKNOWN";
    }
    uint32_t bus_count_;
    fbl::Array<mmio_buffer_t> mmios_;
    mmio_buffer_t xo_mmio_;

    void Reset();
    zx_status_t Read(uint8_t addr, void* buf, size_t len, bool stop);
    zx_status_t Write(uint8_t addr, const void* buf, size_t len, bool stop);
    zx_status_t Start();
    void Stop();
    zx_status_t RxData(uint8_t* buf, size_t length, bool stop);
    zx_status_t TxData(const uint8_t* buf, size_t length, bool stop);
    zx_status_t TxAddress(uint8_t addr, bool is_read);
    zx_status_t WaitFor(Wait type);
};
} // namespace mt8167_i2c
