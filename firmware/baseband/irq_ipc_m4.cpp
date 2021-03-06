/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
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

#include "irq_ipc_m4.hpp"

#include "ch.h"
#include "hal.h"

#include "event_m4.hpp"

#include "lpc43xx_cpp.hpp"
using namespace lpc43xx;

void m0apptxevent_interrupt_enable() {
	nvicEnableVector(M0CORE_IRQn, CORTEX_PRIORITY_MASK(LPC43XX_M0APPTXEVENT_IRQ_PRIORITY));
}

void m0apptxevent_interrupt_disable() {
	nvicDisableVector(M0CORE_IRQn);
}

extern "C" {

CH_IRQ_HANDLER(MAPP_IRQHandler) {
	CH_IRQ_PROLOGUE();

	chSysLockFromIsr();
	events_flag_isr(EVT_MASK_BASEBAND);
	chSysUnlockFromIsr();

	creg::m0apptxevent::clear();

	CH_IRQ_EPILOGUE();
}

}
