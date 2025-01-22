/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM wb_tracer

#if !defined(_TRACE_WB_TRACER_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_WB_TRACER_H

#include <linux/buffer_head.h>
#include <linux/fs.h>
#include <linux/tracepoint.h>
#include <linux/pagemap.h>
#include <linux/string.h>
#include <linux/limits.h>


#ifdef CONFIG_WB_TRACER

#ifndef __WB_TRACER_HELPERS
#define __WB_TRACER_HELPERS

#define __trace_wb_tracer_sync_start(id, type, reason, file, datasync, start, \
				     end)                                     \
do {                                                                          \
	char path_buf[256];                                                   \
	char *p = file_path((file), path_buf, sizeof(path_buf));              \
	trace_wb_tracer_sync_start(id, type, reason,                          \
				   IS_ERR(p) ? " (unknown)" : p,              \
				   datasync, start, end);                     \
} while (0)

#ifndef CONGIF_WB_TRACER_KPTR_RESTRICT
#define WB_TRACER_EXPORT_KPTR(__ptr)                            \
({                                                              \
	static_assert(sizeof(void *) == sizeof(unsigned long)); \
	unsigned long export;                                   \
	if (WARN_ON(ptr_to_hashval((__ptr), &export)))          \
		export = 0x0;                                   \
	(typeof(__ptr))export;                                  \
})
#else /* !CONGIF_WB_TRACER_KPTR_RESTRICT */
#define WB_TRACER_EXPORT_KPTR(__ptr) (__ptr)
#endif /* !CONGIF_WB_TRACER_KPTR_RESTRICT */

#endif /* __WB_TRACER_HELPERS */

TRACE_EVENT(wb_tracer_sync_start,
	TP_PROTO(
		wb_tracer_id_t id,
		wb_tracer_type_t type,
		wb_tracer_reason_t reason,
		const char *path,
		int datasync,
		loff_t start,
		loff_t end
	),

	TP_ARGS(id, type, reason, path, datasync, start, end),

	TP_STRUCT__entry(
		__field(wb_tracer_id_t,		id		)
		__field(wb_tracer_type_t,	type		)
		__field(wb_tracer_reason_t,	reason		)
		__field(loff_t,			start		)
		__field(loff_t,			end		)
		__field(int,			datasync	)
		__string(path,			path		)
	),

	TP_fast_assign(
		__entry->id		= id;
		__entry->type		= type;
		__entry->reason		= reason;
		__entry->datasync	= datasync;
		__entry->start		= start;
		__entry->end		= end;
		__assign_str(path);
		
	),

	TP_printk("id %lld type %d reason %d path %s datasync %d start %lld end %lld",
		  __entry->id, __entry->type, __entry->reason,
		  __get_str(path), __entry->datasync, 
		  __entry->start, __entry->end
	)
);


TRACE_EVENT(wb_tracer_sync_exit,
	TP_PROTO(wb_tracer_id_t id, int ret),

	TP_ARGS(id, ret),

	TP_STRUCT__entry(
		__field(wb_tracer_id_t,	id	)
		__field(int,		ret	)
	),

	TP_fast_assign(
		__entry->id		= id;
		__entry->ret		= ret;
	),

	TP_printk("id %lld ret %d", __entry->id, __entry->ret)
);


TRACE_EVENT(wb_tracer_sync_bio_submit,
	TP_PROTO(wb_tracer_id_t id, struct bio *bio),

	TP_ARGS(id, bio),

	TP_STRUCT__entry(
		__field(wb_tracer_id_t,	id	)
		__field(struct bio *,	bio	)
	),

	TP_fast_assign(
		__entry->id 		= id;
		__entry->bio		= WB_TRACER_EXPORT_KPTR(bio);
	),

	TP_printk("id %lld bio %p", __entry->id, __entry->bio)
);

TRACE_EVENT(wb_tracer_sync_bio_complete,
	TP_PROTO(struct bio *bio),

	TP_ARGS(bio),

	TP_STRUCT__entry(
		__field(struct bio*, bio)
	),

	TP_fast_assign(
		__entry->bio = WB_TRACER_EXPORT_KPTR(bio);
	),

	TP_printk("bio %p", __entry->bio)
);

TRACE_EVENT(wb_tracer_sync_bio_add_folio,
	TP_PROTO(
		wb_tracer_id_t id,
		struct bio *bio,
		struct folio *folio,
		struct buffer_head *bh
	),

	TP_ARGS(id, bio, folio, bh),

	TP_STRUCT__entry(
		__field(wb_tracer_id_t,	id)
		__field(struct bio *,	bio)
		__field(loff_t, 	start)
		__field(loff_t, 	len)
	),

	TP_fast_assign(
		__entry->id 	= id;
		__entry->bio	= WB_TRACER_EXPORT_KPTR(bio);
		__entry->start 	= ((folio_index(folio) << PAGE_SHIFT) + bh_offset(bh));
		__entry->len	= bh->b_size;
	),

	TP_printk("id %lld, bio %p, start %lld, len %lld",
		   __entry->id, __entry->bio, __entry->start, __entry->len
	)
);

#endif /* CONFIG_WB_TRACER */

#endif /* _TRACE_WB_TRACER_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
