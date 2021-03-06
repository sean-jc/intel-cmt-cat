/*
 *   BSD LICENSE
 *
 *   Copyright(c) 2016 Intel Corporation. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>

#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RDT_MAX_SOCKETS 8

#ifndef MIN
/**
 * Macro to return the minimum of two numbers
 */
#define MIN(a, b) ({ \
	typeof(a) _a = (a); \
	typeof(b) _b = (b); \
	_a < _b ? _a : _b; \
})
#endif /* !MIN */

#ifndef MAX
/**
 * Macro to return the maximum of two numbers
 */
#define MAX(a, b) ({ \
	typeof(a) _a = (a); \
	typeof(b) _b = (b); \
	_a > _b ? _a : _b; \
})
#endif /* !MAX */

/* L3 cache allocation class of service data structure */
struct cat_config {
	cpu_set_t cpumask;		/**< CPUs bitmask */
	int cdp;			/**< data & code masks used if true */
	union {
		uint64_t mask;		/**< capacity bitmask (CBM) */
		struct {
			uint64_t data_mask; /**< data capacity bitmask (CBM) */
			uint64_t code_mask; /**< code capacity bitmask (CBM) */
		};
	};
};

/* rdtset command line configuration structure */
struct rdtset {
	pid_t pid;			/**< process PID */
	struct cat_config l3_config[CPU_SETSIZE]; /**< L3 configuration */
	unsigned l3_config_count;	/**< Num of L3 config entries */
	cpu_set_t cpu_aff_cpuset;	/**< CPU affinity configuration */
	cpu_set_t reset_cpuset;		/**< List of CPUs to reset COS assoc */
	unsigned int	sudo_keep:1,	/**< don't drop elevated privileges */
			verbose:1,	/**< be verobose */
			command:1;	/**< command to be executed detected */
};

struct rdtset g_cfg;

/**
 * @brief Parse CPU set string
 *
 * @note Parse elem, the elem could be single number/range or group
 *       1) A single number elem, it's just a simple digit. e.g. 9
 *       2) A single range elem, two digits with a '-' between. e.g. 2-6
 *       3) A group elem, combines multiple 1) or 2) with e.g 0,2-4,6
 *       Within group elem, '-' used for a range separator;
 *       ',' used for a single number.
 *
 * @param cpustr set as a string
 * @param cpustr_len set as a string len
 * @param cpuset parsed cpuset
 *
 * @return number of parsed characters on success
 * @retval -ERRNO on error
 */
int str_to_cpuset(const char *cpustr, const unsigned cpustr_len,
		cpu_set_t *cpuset);

/**
 * @brief Converts CPU set (cpu_set_t) to string
 *
 * @param cpustr output string
 * @param cpustr_len max output string len
 * @param cpumask input cpuset
 */
void cpuset_to_str(char *cpustr, const unsigned cpustr_len,
		const cpu_set_t *cpumask);

#ifdef __cplusplus
}
#endif

#endif /* _COMMON_H */
