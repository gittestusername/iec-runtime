/*
 * Copyright (C) 2001,2002,2003,2004,2005 Philippe Gerum <rpm@xenomai.org>.
 *
 * Xenomai is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Xenomai is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Xenomai; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef _XENO_ASM_X86_CALIBRATION_H
#define _XENO_ASM_X86_CALIBRATION_H

#ifndef _XENO_ASM_X86_BITS_INIT_H
#error "please don't include asm/calibration.h directly"
#endif

#include <asm/processor.h>

static inline unsigned long xnarch_get_sched_latency (void)
{
	unsigned long sched_latency;

#if CONFIG_XENO_OPT_TIMING_SCHEDLAT != 0
	sched_latency = CONFIG_XENO_OPT_TIMING_SCHEDLAT;
#else /* !CONFIG_XENO_OPT_TIMING_SCHEDLAT */

	if (strcmp(RTHAL_TIMER_DEVICE, "lapic") == 0) {
#ifdef CONFIG_SMP
		if (num_online_cpus() > 1)
			sched_latency = 3350;
		else
			sched_latency = 2000;
#else /* !SMP */
		sched_latency = 1000;
#endif /* !SMP */
	} else if (strcmp(RTHAL_TIMER_DEVICE, "pit")) { /* HPET */
#ifdef CONFIG_SMP
		if (num_online_cpus() > 1)
			sched_latency = 3350;
		else
			sched_latency = 1500;
#else /* !SMP */
		sched_latency = 1000;
#endif /* !SMP */
	} else {
		/*
		 * Use the bogomips formula to identify low-end x86 boards
		 * when using the 8254 PIT. The following is still grossly
		 * experimental and needs work (i.e. more specific cases), but
		 * the approach is definitely saner than previous attempts to
		 * guess such value dynamically.
		 */
#ifdef CONFIG_IPIPE_CORE
#define __bogomips (this_cpu_read(cpu_info.loops_per_jiffy)/(500000/HZ))
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38)
#define __bogomips (cpu_info.loops_per_jiffy/(500000/HZ))
#else
#define __bogomips (current_cpu_data.loops_per_jiffy/(500000/HZ))
#endif

		sched_latency = (__bogomips < 250 ? 17000 :
				 __bogomips < 2500 ? 4200 :
				 3500);
#undef __bogomips

#ifdef CONFIG_SMP
		sched_latency += 1000;
#endif /* CONFIG_SMP */
	}
#endif /* !CONFIG_XENO_OPT_TIMING_SCHEDLAT */

	return sched_latency;
}

#endif /* !_XENO_ASM_X86_CALIBRATION_H */
