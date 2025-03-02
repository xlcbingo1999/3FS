/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_COMPAT_H
#define LIBURING_COMPAT_H

#if __has_include(<linux/time_types.h>)
#include <linux/time_types.h>
#else

#ifndef __kernel_rwf_t
#include <linux/types.h>
typedef int __bitwise __kernel_rwf_t;
#endif

#ifndef __kernel_timespec
struct __kernel_timespec {
	long long tv_sec;
	long long tv_nsec;
};
#endif
/* <linux/time_types.h> is included above and not needed again */
#define UAPI_LINUX_IO_URING_H_SKIP_LINUX_TIME_TYPES_H 1
#endif

#include <inttypes.h>

struct open_how {
	uint64_t	flags;
	uint64_t	mode;
	uint64_t	resolve;
};

#endif
