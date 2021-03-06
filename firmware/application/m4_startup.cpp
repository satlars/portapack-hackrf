/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "m4_startup.hpp"

#include "hal.h"

#include "message.hpp"
#include "portapack_shared_memory.hpp"

#include <cstring>

/* TODO: OK, this is cool, but how do I put the M4 to sleep so I can switch to
 * a different image? Other than asking the old image to sleep while the M0
 * makes changes?
 *
 * I suppose I could force M4MEMMAP to an invalid memory reason which would
 * cause an exception and effectively halt the M4. But that feels gross.
 */
void m4_init(const portapack::spi_flash::region_t from, const portapack::memory::region_t to) {
	/* Initialize M4 code RAM */
	std::memcpy(reinterpret_cast<void*>(to.base()), from.base(), from.size);

	/* M4 core is assumed to be sleeping with interrupts off, so we can mess
	 * with its address space and RAM without concern.
	 */
	LPC_CREG->M4MEMMAP = to.base();

	/* Reset M4 core */
	LPC_RGU->RESET_CTRL[0] = (1 << 13);
}

void m4_request_shutdown() {
	ShutdownMessage shutdown_message;
	shared_memory.baseband_queue.push(shutdown_message);
}
