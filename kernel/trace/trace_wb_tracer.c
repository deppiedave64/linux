// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/kthread.h>

#define CREATE_TRACE_POINTS
#include <trace/events/wb_tracer.h>

// Export tracepoint symbols for use in modules (e. g. XFS on Fedora)
EXPORT_TRACEPOINT_SYMBOL_GPL(wb_tracer_sync_start);
EXPORT_TRACEPOINT_SYMBOL_GPL(wb_tracer_sync_exit);
