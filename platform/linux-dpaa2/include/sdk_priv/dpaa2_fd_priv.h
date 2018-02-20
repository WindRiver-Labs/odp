/*
 * Copyright (c) 2016 Freescale Semiconductor, Inc. All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Freescale Semiconductor nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY Freescale Semiconductor ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Freescale Semiconductor BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __FSL_qbman_fd_H
#define __FSL_qbman_fd_H


/**
 * DOC: DPAA2 FD - Frame Descriptor APIs for DPAA2
 *
 * Frame Descriptors (FDs) are used to describe frame data in the DPAA2.
 * Frames can be enqueued and dequeued to Frame Queues which are consumed
 * by the various DPAA accelerators (WRIOP, SEC, PME, DCE)
 *
 * There are three types of frames: Single, Scatter Gather and Frame Lists.
 *
 * The set of APIs in this file must be used to create, manipulate and
 * query Frame Descriptor.
 *
 */

/**
 * struct dpaa2_dq - the qman result structure
 * @dont_manipulate_directly: the 16 32bit data to represent the whole
 * possible qman dequeue result.
 *
 * When frames are dequeued, the FDs show up inside "dequeue" result structures
 * (if at all, not all dequeue results contain valid FDs). This structure type
 * is intentionally defined without internal detail, and the only reason it
 * isn't declared opaquely (without size) is to allow the user to provide
 * suitably-sized (and aligned) memory for these entries.
 */
struct dpaa2_dq {
	uint32_t dont_manipulate_directly[16];
};

/* Parsing frame dequeue results */
/* FQ empty */
#define DPAA2_DQ_STAT_FQEMPTY       0x80
/* FQ held active */
#define DPAA2_DQ_STAT_HELDACTIVE    0x40
/* FQ force eligible */
#define DPAA2_DQ_STAT_FORCEELIGIBLE 0x20
/* Valid frame */
#define DPAA2_DQ_STAT_VALIDFRAME    0x10
/* FQ ODP enable */
#define DPAA2_DQ_STAT_ODPVALID      0x04
/* Volatile dequeue */
#define DPAA2_DQ_STAT_VOLATILE      0x02
/* volatile dequeue command is expired */
#define DPAA2_DQ_STAT_EXPIRED       0x01

/**
 * dpaa2_dq_flags() - Get the stat field of dequeue response
 * @dq: the dequeue result.
 */
uint32_t dpaa2_dq_flags(const struct dpaa2_dq *dq);

/**
 * dpaa2_dq_is_pull() - Check whether the dq response is from a pull
 * command.
 * @dq: the dequeue result.
 *
 * Return 1 for volatile(pull) dequeue, 0 for static dequeue.
 */
static inline int dpaa2_dq_is_pull(const struct dpaa2_dq *dq)
{
	return (int)(dpaa2_dq_flags(dq) & DPAA2_DQ_STAT_VOLATILE);
}

/**
 * dpaa2_dq_is_pull_complete() - Check whether the pull command is completed.
 * @dq: the dequeue result.
 *
 * Return boolean.
 */
static inline int dpaa2_dq_is_pull_complete(
					const struct dpaa2_dq *dq)
{
	return (int)(dpaa2_dq_flags(dq) & DPAA2_DQ_STAT_EXPIRED);
}

/**
 * dpaa2_dq_seqnum() - Get the seqnum field in dequeue response
 * seqnum is valid only if VALIDFRAME flag is TRUE
 * @dq: the dequeue result.
 *
 * Return seqnum.
 */
uint16_t dpaa2_dq_seqnum(const struct dpaa2_dq *dq);

/**
 * dpaa2_dq_odpid() - Get the seqnum field in dequeue response
 * odpid is valid only if ODPVAILD flag is TRUE.
 * @dq: the dequeue result.
 *
 * Return odpid.
 */
uint16_t dpaa2_dq_odpid(const struct dpaa2_dq *dq);

/**
 * dpaa2_dq_fqid() - Get the fqid in dequeue response
 * @dq: the dequeue result.
 *
 * Return fqid.
 */
uint32_t dpaa2_dq_fqid(const struct dpaa2_dq *dq);

/**
 * dpaa2_dq_byte_count() - Get the byte count in dequeue response
 * @dq: the dequeue result.
 *
 * Return the byte count remaining in the FQ.
 */
uint32_t dpaa2_dq_byte_count(const struct dpaa2_dq *dq);

/**
 * dpaa2_dq_frame_count() - Get the frame count in dequeue response
 * @dq: the dequeue result.
 *
 * Return the frame count remaining in the FQ.
 */
uint32_t dpaa2_dq_frame_count(const struct dpaa2_dq *dq);

/**
 * dpaa2_dq_fd_ctx() - Get the frame queue context in dequeue response
 * @dq: the dequeue result.
 *
 * Return the frame queue context.
 */
uint64_t dpaa2_dq_fqd_ctx(const struct dpaa2_dq *dq);

/**
 * dpaa2_dq_fd() - Get the frame descriptor in dequeue response
 * @dq: the dequeue result.
 *
 * Return the frame descriptor.
 */
const struct qbman_fd *dpaa2_dq_fd(const struct dpaa2_dq *dq);
#endif /* __FSL_qbman_fd_H */
