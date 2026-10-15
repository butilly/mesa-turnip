/*
 * Copyright © 2016 Red Hat.
 * Copyright © 2016 Bas Nieuwenhuizen
 * SPDX-License-Identifier: MIT
 *
 * based in part on anv driver which is:
 * Copyright © 2015 Intel Corporation
 */

#ifndef TU_QUEUE_H
#define TU_QUEUE_H

#include "tu_common.h"

enum tu_queue_type
{
   TU_QUEUE_GFX,
   TU_QUEUE_SPARSE,
};

struct tu_kgsl_submit_record
{
   uint32_t timestamp;
   uint32_t submit_id;
   uint32_t command_buffers;
   uint32_t ibs;
   uint32_t waits;
   uint32_t signals;
};

struct tu_queue
{
   struct vk_queue vk;

   struct tu_device *device;

   enum tu_queue_type type;
   uint32_t msm_queue_id;
   uint32_t priority;

   uint32_t sparse_queue_id;

   uint32_t sparse_syncobj, gfx_syncobj;
   uint64_t sparse_timepoint, gfx_timepoint;

   unsigned render_pass_idx;

   int fence;           /* timestamp/fence of the last queue submission */

   /* Diagnostic snapshot: status queries may run outside submit_mutex. */
   uint32_t diagnostic_submit_id;
   uint32_t diagnostic_command_buffer_count;
   uint32_t diagnostic_ib_count;

   /* KGSL-only bounded history, protected independently of submit_mutex:
    * timestamp waits/status queries can run on other threads.
    */
   pthread_mutex_t diagnostic_mutex;
   struct tu_kgsl_submit_record diagnostic_history[64];
   uint32_t diagnostic_next;
   uint32_t diagnostic_count;
   uint32_t diagnostic_dumped;
};
VK_DEFINE_HANDLE_CASTS(tu_queue, vk.base, VkQueue, VK_OBJECT_TYPE_QUEUE)

VkResult
tu_queue_init(struct tu_device *device,
              struct tu_queue *queue,
              enum tu_queue_type type,
              const VkQueueGlobalPriorityKHR global_priority,
              int idx,
              const VkDeviceQueueCreateInfo *create_info,
              struct tu_queue *shared_queue);

void
tu_queue_finish(struct tu_queue *queue);

#endif
