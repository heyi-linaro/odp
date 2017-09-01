/* Copyright (c) 2017, ARM Limited. All rights reserved.
 *
 * Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <odp_debug_internal.h>
#include <odp_packet_io_internal.h>

#define SUBSYSTEM_VERSION 0x00010000UL
ODP_SUBSYSTEM_DEFINE(pktio_ops, "packet IO operations", SUBSYSTEM_VERSION);

/* Instantiate init and term functions */
ODP_SUBSYSTEM_FOREACH_TEMPLATE(pktio_ops, init_global, ODP_ERR)
ODP_SUBSYSTEM_FOREACH_TEMPLATE(pktio_ops, init_local, ODP_ERR)
ODP_SUBSYSTEM_FOREACH_TEMPLATE(pktio_ops, term_global, ODP_ABORT)

ODP_SUBSYSTEM_CONSTRUCTOR(pktio_ops)
{
	odp_subsystem_constructor(pktio_ops);

	/* Further initialization per subsystem */
}
