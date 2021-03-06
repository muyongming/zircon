// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <fs-management/ram-nand.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fbl/unique_fd.h>
#include <lib/fzl/fdio.h>
#include <zircon/device/ram-nand.h>
#include <zircon/types.h>
#include <zircon/nand/c/fidl.h>

namespace {

constexpr char kBasePath[] = "/dev/misc/nand-ctl";

} // namespace

int create_ram_nand(const ram_nand_info_t* config, char* out_path) {
    fbl::unique_fd control(open(kBasePath, O_RDWR));
    if (!control) {
        fprintf(stderr, "Could not open nand-ctl\n");
        return -1;
    }

    ram_nand_name_t response = {};
    ssize_t rc = (config->vmo == ZX_HANDLE_INVALID)
                     ? ioctl_ram_nand_create(control.get(), config, &response)
                     : ioctl_ram_nand_create_vmo(control.get(), config, &response);
    if (rc < 0) {
        fprintf(stderr, "Could not create ram_nand device\n");
        return -1;
    }

    strcpy(out_path, kBasePath);
    out_path[sizeof(kBasePath) - 1] = '/';
    strcpy(out_path + sizeof(kBasePath), response.name);
    return 0;
}

int destroy_ram_nand(const char* ram_nand_path) {
    fbl::unique_fd ram_nand(open(ram_nand_path, O_RDWR));
    if (!ram_nand) {
        fprintf(stderr, "Could not open ram_nand\n");
        return -1;
    }
    fzl::FdioCaller caller(fbl::move(ram_nand));

    zx_status_t status;
    if (zircon_nand_RamNandUnlink(caller.borrow_channel(), &status) != ZX_OK || status != ZX_OK) {
        fprintf(stderr, "Could not shut off ram_nand\n");
        return -1;
    }
    return 0;
}
