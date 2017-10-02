/*
 * Copyright (C) 2017 Kernkonzept GmbH.
 * Author(s): Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 *            Alexander Warg <alexander.warg@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include "generic_cpu_dev.h"

#include <cstdio>

extern __thread unsigned vmm_current_cpu_id;

namespace Vmm {

class Cpu_dev : public Generic_cpu_dev
{
public:
  enum
  {
    Flags_default_32 = 0x1d3,
    Flags_default_64 = 0x1c5,
    Flags_mode_32 = (1 << 4)
  };

  Cpu_dev(unsigned idx, unsigned phys_id, Vdev::Dt_node const *)
  : Generic_cpu_dev(idx, phys_id)
  {}

  void show_state_registers(FILE *f);

  void
  start_vcpu()
  {
    Dbg(Dbg::Cpu, Dbg::Info)
      .printf("Initiating cpu startup @ 0x%lx\n", _vcpu->r.ip);
    reschedule();
  }

  void init_vgic(Vmm::Arm::State::Gic *iface);

  /**
   * Enter the virtual machine
   *
   * We assume an already setup register state that can be used as is
   * to enter the virtual machine (it was not changed by
   * vcpu_control_ext()). The virtualization related state is set to
   * default values, therefore we have to initialize this state here.
   */
  void reset() override;

  /**
   * Translate a device tree "reg" value to an internally usable CPU id.
   *
   * For most architectures this is NOP, but some archictures like ARM
   * might encode topology information into this value, which needs to
   * be translated.
   */
  static unsigned dtid_to_cpuid(l4_int32_t prop_val)
  { return prop_val & 0xf; }
};

}
