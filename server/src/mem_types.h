/*
 * Copyright (C) 2018 Kernkonzept GmbH.
 * Author(s): Sarah Hoffmann <sarah.hoffmann@kernkonzept.com>
 *
 * This file is distributed under the terms of the GNU General Public
 * License, version 2.  Please see the COPYING-GPL-2 file for details.
 */
#pragma once

#include <l4/sys/consts.h>
#include <l4/sys/l4int.h>

namespace Vmm {

/**
 * A guest-physical address.
 */
class Guest_addr
{
public:
  Guest_addr() : _addr(0) {}
  explicit Guest_addr(l4_addr_t addr) : _addr(addr) {}

  friend bool operator == (Guest_addr lhs, Guest_addr rhs) noexcept
  { return lhs._addr == rhs._addr; }

  friend bool operator != (Guest_addr lhs, Guest_addr rhs) noexcept
  { return lhs._addr != rhs._addr; }

  friend bool operator > (Guest_addr lhs, Guest_addr rhs) noexcept
  { return lhs._addr > rhs._addr; }

  friend bool operator >= (Guest_addr lhs, Guest_addr rhs) noexcept
  { return lhs._addr >= rhs._addr; }

  friend bool operator < (Guest_addr lhs, Guest_addr rhs) noexcept
  { return lhs._addr < rhs._addr; }

  friend bool operator <= (Guest_addr lhs, Guest_addr rhs) noexcept
  { return lhs._addr <= rhs._addr; }

  friend Guest_addr operator + (Guest_addr lhs, l4_size_t rhs) noexcept
  { return Guest_addr(lhs._addr + rhs); }

  friend Guest_addr operator + (l4_size_t lhs, Guest_addr rhs) noexcept
  { return Guest_addr(lhs + rhs._addr); }

  friend Guest_addr operator - (Guest_addr lhs, l4_size_t rhs) noexcept
  { return Guest_addr(lhs._addr - rhs); }

  friend Guest_addr operator - (l4_size_t lhs, Guest_addr rhs) noexcept
  { return Guest_addr(lhs - rhs._addr); }

  friend l4_size_t operator - (Guest_addr lhs, Guest_addr rhs) noexcept
  { return lhs._addr - rhs._addr; }

  l4_addr_t get() const noexcept
  { return _addr; }

  Guest_addr round_page() const noexcept
  { return Guest_addr(l4_round_page(_addr)); }

private:
  l4_addr_t _addr;
};

/**
 * Type of region in a VM area map
 *
 * This type is used to keep track of regions in an vm area map. It is used to
 * check whether regions are compatible with each other when regions overlap and
 * when someone searches for a region of a specific type..
 */
enum class Region_type
{
  None,         ///< No type specified
  Ram,          ///< Physical RAM
  Vbus,         ///< Mmio resource specified on Vbus
  Kernel,       ///< Mmio resources provided by the kernel
  Virtual       ///< Mmio resource provided by a virtual device
};

template <typename T>
struct Generic_region
{
  T start;
  T end; // inclusive
  Region_type type;

  Generic_region() = delete;
  explicit Generic_region(T a) : start(a), end(a), type(Region_type::None) {}
  Generic_region(T a, Region_type type) : start(a), end(a), type(type) {}
  Generic_region(T s, T e, Region_type type) : start(s), end(e),
    type(type) {}

  static Generic_region ss(T start, l4_size_t size, Region_type type)
  { return Generic_region(start, start + size - 1, type); }

  bool operator < (Generic_region const &r) const { return end < r.start; }

  bool contains(Generic_region const &r) const
  {
    // [ start [ r.start ... r.end ] end ]
    return (start <= r.start) && (r.end <= end) && (type == r.type);
  }
};

using Region = Generic_region<Guest_addr>;
using Io_region = Generic_region<l4_addr_t>;
} // namespace
