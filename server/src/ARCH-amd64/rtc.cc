/*
 * Copyright (C) 2017 Kernkonzept GmbH.
 * Author(s): Philipp Eppelt <philipp.eppelt@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */

#include "device_factory.h"
#include "guest.h"
#include "device.h"
#include "io_device.h"

namespace Vdev {

class Rtc : public Vmm::Io_device, public Vdev::Device
{
  void io_out(unsigned, Vmm::Mem_access::Width, l4_uint32_t) override {}

  void io_in(unsigned, Vmm::Mem_access::Width, l4_uint32_t *value) override
  { *value = 0; }
};

} // namespace Vdev

namespace {

struct F : Vdev::Factory
{
  cxx::Ref_ptr<Vdev::Device> create(Vdev::Device_lookup *devs,
                                    Vdev::Dt_node const &) override
  {
    auto dev = Vdev::make_device<Vdev::Rtc>();
    auto region = Vmm::Io_region(0x70, 0x71, Vmm::Region_type::Virtual);
    devs->vmm()->register_io_device(region, dev);

    return dev;
  }
}; // struct F

static F f;
static Vdev::Device_type t = {"virt-rtc", nullptr, &f};

} // namespace
