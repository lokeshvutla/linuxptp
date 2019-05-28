/**
 * @file missing.h
 * @note Copyright (C) 2011 Richard Cochran <richardcochran@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * When glibc offers the syscall, this will go away.
 */
#ifndef HAVE_MISSING_H
#define HAVE_MISSING_H

#include <time.h>
#include <sys/syscall.h>
#include <sys/timex.h>
#include <unistd.h>

#ifndef ADJ_TAI
#define ADJ_TAI 0x0080
#endif

#ifndef ADJ_NANO
#define ADJ_NANO 0x2000
#endif

#ifndef ADJ_SETOFFSET
#define ADJ_SETOFFSET 0x0100
#endif

#ifndef CLOCK_INVALID
#define CLOCK_INVALID -1
#endif

#define CLOCKFD 3
#define FD_TO_CLOCKID(fd)	((clockid_t) ((((unsigned int) ~fd) << 3) | CLOCKFD))
#define CLOCKID_TO_FD(clk)	((unsigned int) ~((clk) >> 3))

#ifndef HAVE_ONESTEP_SYNC
enum _missing_hwtstamp_tx_types {
	HWTSTAMP_TX_ONESTEP_SYNC = 2,
};
#endif

#ifndef HAVE_ONESTEP_P2P
enum {
	HWTSTAMP_TX_ONESTEP_P2P = 3,
};
#endif

#ifndef LIST_FOREACH_SAFE
#define	LIST_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = LIST_FIRST((head));				\
	    (var) && ((tvar) = LIST_NEXT((var), field), 1);		\
	    (var) = (tvar))
#endif

#ifndef SIOCGHWTSTAMP
#define SIOCGHWTSTAMP 0x89b1
#endif

#ifndef SO_SELECT_ERR_QUEUE
#define SO_SELECT_ERR_QUEUE 45
#endif

#ifndef PTP_PPS_OFFSET
#define PTP_PPS_OFFSET     _IOW('=', 9, int)
#endif

#ifndef HAVE_CLOCK_ADJTIME
static inline int clock_adjtime(clockid_t id, struct timex *tx)
{
	return syscall(__NR_clock_adjtime, id, tx);
}
#endif

#ifndef IFLA_BOND_MAX
enum {
	IFLA_BOND_UNSPEC,
	IFLA_BOND_MODE,
	IFLA_BOND_ACTIVE_SLAVE,
	IFLA_BOND_MIIMON,
	IFLA_BOND_UPDELAY,
	IFLA_BOND_DOWNDELAY,
	IFLA_BOND_USE_CARRIER,
	IFLA_BOND_ARP_INTERVAL,
	IFLA_BOND_ARP_IP_TARGET,
	IFLA_BOND_ARP_VALIDATE,
	IFLA_BOND_ARP_ALL_TARGETS,
	IFLA_BOND_PRIMARY,
	IFLA_BOND_PRIMARY_RESELECT,
	IFLA_BOND_FAIL_OVER_MAC,
	IFLA_BOND_XMIT_HASH_POLICY,
	IFLA_BOND_RESEND_IGMP,
	IFLA_BOND_NUM_PEER_NOTIF,
	IFLA_BOND_ALL_SLAVES_ACTIVE,
	IFLA_BOND_MIN_LINKS,
	IFLA_BOND_LP_INTERVAL,
	IFLA_BOND_PACKETS_PER_SLAVE,
	IFLA_BOND_AD_LACP_RATE,
	IFLA_BOND_AD_SELECT,
	IFLA_BOND_AD_INFO,
	IFLA_BOND_AD_ACTOR_SYS_PRIO,
	IFLA_BOND_AD_USER_PORT_KEY,
	IFLA_BOND_AD_ACTOR_SYSTEM,
	IFLA_BOND_TLB_DYNAMIC_LB,
	__IFLA_BOND_MAX,
};

#define IFLA_BOND_MAX   (__IFLA_BOND_MAX - 1)
#endif	/*IFLA_BOND_MAX*/

#ifdef __UCLIBC__

#if (_XOPEN_SOURCE >= 600 || _POSIX_C_SOURCE >= 200112L) && \
	defined __UCLIBC_HAS_THREADS_NATIVE__

#include <sys/timerfd.h>

#else

#define TFD_TIMER_ABSTIME (1 << 0)

static inline int clock_nanosleep(clockid_t clock_id, int flags,
				  const struct timespec *request,
				  struct timespec *remain)
{
	return syscall(__NR_clock_nanosleep, clock_id, flags, request, remain);
}

static inline int timerfd_create(int clockid, int flags)
{
	return syscall(__NR_timerfd_create, clockid, flags);
}

static inline int timerfd_settime(int fd, int flags,
				  const struct itimerspec *new_value,
				  struct itimerspec *old_value)
{
	return syscall(__NR_timerfd_settime, fd, flags, new_value, old_value);
}
#endif

#else /*__UCLIBC__*/

#include <sys/timerfd.h>

#define BIT(nr)              (1 << (nr))

#ifndef SO_REDUNDANT
#define SO_REDUNDANT         80
#define SCM_REDUNDANT        SO_REDUNDANT
#endif

#ifndef SO_RED_TIMESTAMPING
#define SO_RED_TIMESTAMPING  81
#define SCM_RED_TIMESTAMPING SO_RED_TIMESTAMPING
#endif

#define PTP_MSG_IN           (0x3 << 6)
#define PTP_EVT_OUT          (0x2 << 6)
#define DIRECTED_TX          (0x1 << 6)
#define RED_PORT_B           BIT(1)
#define RED_PORT_A           BIT(0)

#define MSG_REDINFO(m)       (&(m)->redinfo)
#define MSG_HWTS(m)          (&(m)->hwts.ts)
#define MSG_RED_HWTS(m)      (&(m)->red_hwts.ts)
#define MSG_REDINFO_B(m)     (&(m)->redinfo_b)
#define MSG_HWTS_B(m)        (&(m)->hwts_b.ts)
#define MSG_RED_HWTS_B(m)    (&(m)->red_hwts_b.ts)

#define REDINFO_T(r)         ((r)->io_port & (0x3 << 6))
#define REDINFO_PORTS(r)     ((r)->io_port & 0x3)
#define REDINFO_PATHID(r)    ((r)->pathid)
#define REDINFO_SEQNR(r)     ((r)->seqnr)

#define MSG_RED_PORTS(m)  (REDINFO_PORTS(MSG_REDINFO(m)))
#define MSG_RED_T(m)      (REDINFO_T(MSG_REDINFO(m)))
#define IS_PTP_MSG_IN(r)  (REDINFO_T(r) == PTP_MSG_IN)
#define IS_PTP_EVT_OUT(r) (REDINFO_T(r) == PTP_EVT_OUT)
#define IS_PTP_DIR_OUT(r) (REDINFO_T(r) == DIRECTED_TX)

#endif

#endif
