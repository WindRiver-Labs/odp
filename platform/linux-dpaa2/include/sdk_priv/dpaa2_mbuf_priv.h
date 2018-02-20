/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc. All rights reserved.
 */

/**
 * @file		dpaa2_mbuf_priv.h
 *
 * @brief		Buffer management library services for DPAA2 based applications.
 */

#ifndef _DPAA2_MBUF_PRIV_H_
#define _DPAA2_MBUF_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

/* DPAA2 header files */
#include <odp/api/plat/sdk/common/dpaa2_cfg.h>
#include <odp/api/plat/sdk/rts/dpaa2_mbuf.h>
#include <dpaa2_mbuf_priv_ldpaa.h>
#include <odp_config_internal.h>

/* Maximum release/acquire from QBMAN */
#define DPAA2_MBUF_MAX_ACQ_REL	7

#define MAX_BPID 256

/*Macros to define operations on FD*/
#define DPAA2_SET_FD_ADDR(fd, addr) qbman_fd_set_addr(fd, addr)
#define DPAA2_SET_FD_LEN(fd, length)	qbman_fd_set_len(fd, length)
#define DPAA2_SET_FD_BPID(fd, bpid)	qbman_fd_set_bpid(fd, bpid)
#define DPAA2_SET_FD_IVP(fd)   qbman_fd_set_bpid(fd, 0x4000)
#define DPAA2_SET_FD_OFFSET(fd, offset)	qbman_fd_set_offset(fd, offset)
#define DPAA2_SET_FD_INTERNAL_JD(fd, len) qbman_fd_set_frc(fd,(0x80000000 | len))
#define DPAA2_SET_FD_FRC(fd, frc)	qbman_fd_set_frc(fd, frc)
#define DPAA2_RESET_FD_CTRL(fd)		qbman_fd_set_ctrl(fd, 0)

#define	DPAA2_SET_FD_ASAL(fd, asal)	qbman_fd_set_ctrl(fd, (asal << 16))
#define DPAA2_SET_FD_FLC(fd, addr)	qbman_fd_set_flc(fd, addr)
#define DPAA2_SET_FLE_INTERNAL_JD(fle, len) dpaa2_fl_set_len(fle, (0x80000000 | (len)))
#define DPAA2_SET_FLE_OFFSET(fle, offset)	dpaa2_fl_set_offset(fle, offset)

#define DPAA2_GET_FLE_ADDR(fle)		dpaa2_fl_get_addr(fle)
#define DPAA2_SET_FLE_ADDR(fle, addr)	dpaa2_fl_set_addr(fle, addr)
#define DPAA2_SET_FLE_BPID(fle, bpid)	dpaa2_fl_set_bpid(fle, bpid)
#define DPAA2_GET_FLE_BPID(fle)		dpaa2_fl_get_bpid(fle)
#define DPAA2_SET_FLE_FIN(fle)		dpaa2_fl_set_final(fle, 1)
#define DPAA2_SET_FD_COMPOUND_FMT(fd)	qbman_fd_set_format(fd, 1)

#define DPAA2_GET_FD_ADDR(fd)	qbman_fd_get_addr(fd)
#define DPAA2_GET_FD_LEN(fd)	qbman_fd_get_len(fd)
#define DPAA2_GET_FD_BPID(fd)	qbman_fd_get_bpid(fd)
#define DPAA2_GET_FD_IVP(fd)	((qbman_fd_get_bpid(fd) & 0x4000) >> 14)
#define DPAA2_GET_FD_OFFSET(fd)	qbman_fd_get_offset(fd)
#define DPAA2_GET_FD_FRC(fd)	qbman_fd_get_frc(fd)
#define DPAA2_GET_FD_FLC(fd)	qbman_fd_get_flc(fd)

/* BE TODO */
#define DPAA2_SET_FLE_SG_EXT(fle)	fle->fin_bpid_offset |= (uint64_t)1<<29;
#define DPAA2_IS_SET_FLE_SG_EXT(fle)	\
	(fle->fin_bpid_offset & ((uint64_t)1<<29))? 1 : 0
#define DPAA2_SET_FLE_IVP(fle)   ((fle->fin_bpid_offset |= 0x4000))
#define DPAA2_SET_FLC_EWS(flc)  flc->word1_bits23_16 |= 0x1
#define DPAA2_SET_FLC_RSC(flc)  (flc->word1_bits31_24 |= 0x1)
#define DPAA2_SET_FLC_REUSE_BS(flc) flc->mode_bits |= 0x8000
#define DPAA2_SET_FLC_REUSE_FF(flc) flc->mode_bits |= 0x2000


#define GET_VIRT_ADDR_FROM_ZONE(addr, bz) ((addr - bz->phys_addr) + dpaa2_memzone_virt(bz))
#define GET_PHY_ADDR_FROM_ZONE(addr, bz) (bz->phys_addr + ((uintptr_t)addr - dpaa2_memzone_virt(bz)))

#define DPAA2_INLINE_MBUF_FROM_BUF(buf, meta_data_size) \
	((struct dpaa2_mbuf *)((uint64_t)buf -  meta_data_size))
#define DPAA2_BUF_FROM_INLINE_MBUF(mbuf, meta_data_size) \
	((uint8_t *)((uint64_t)mbuf + meta_data_size))

/* Refer to Table 7-3 in SEC BG */
struct qbman_fle {
	uint32_t addr_lo;
	uint32_t addr_hi;
	uint32_t length;
	/* FMT must be 00, MSB is final bit  */
	uint32_t fin_bpid_offset;
	uint32_t frc;
	uint32_t reserved[3]; /* Not used currently */
};

/*!
 * Structure representing private buffer pool list. This buffer pool list may
 * have several buffer pools
 */
struct dpaa2_bp_list {
	struct dpaa2_bp_list *next;
	uint8_t num_buf_pools;
	struct buf_pool buf_pool[DPAA2_MAX_BUF_POOLS];
};

/*!
 * @details	Initializes the buffer pool list. Within this API
 *		memory required will be allocated, dpbp will get initialized,
 *		buffer pool list will be configured
 *
 * @param[in]	bp_list_cfg - buffer pool list configuration
 *
 * @returns	buffer pool list pointer in case of success; NULL otherwise
 *
 */
struct dpaa2_bp_list *dpaa2_mbuf_create_bp_list(
		struct dpaa2_bp_list_cfg *bp_list_cfg);


/*!
 * @details	Add the buffer pool list to the global list. Also the buffer
 *		pools in the list are stored in sorted order of size.
 *
 * @param[in]	bp_list - buffer pool list
 *
 * @returns	none
 *
 */
void dpaa2_add_bp_list(
		struct dpaa2_bp_list *bp_list);


/*!
 * @details	Allocate a SG DPAA2 buffer of given size from given 'dev'.
 *
 * @param[in]	dev - DPAA2 device. Buffer will be allcoated from the pool
 *		affined to this 'dev'
 *
 * @param[in]	size - the DPAA2 buffer size required.
 *
 * @returns	dpaa2 buffer on success; NULL of failure .
 *
 */
dpaa2_mbuf_pt dpaa2_mbuf_alloc_sg(
		struct dpaa2_dev *dev,
		uint32_t size);


/*!
 * @details	Allocate SG DPAA2 buffer from given buffer pool.
 *
 * @param[in]	bpid - buffer pool id (which was filled in by DPAA2 at
 *		'dpaa2_mbuf_create_bp_list'
 *
 * @param[in]	length - if single buffer length is greater than the buffer size
 *		it may allocate SG list.
 *
 * @returns	dpaa2 buffer on success; NULL on failure.
 *
 */
dpaa2_mbuf_pt dpaa2_mbuf_alloc_sg_from_bpid(
		uint16_t bpid,
		int length);


/*!
 * @details	Make a complete copy of given DPAA2 buffer in case of SG
 *
 * @param[out]	to_buf - DPAA2 buffer on which the 'from_buffer' is copied.
 *		'to_buf' should already have the buffer frames in it, and thus
 *		no new frame from any buffer pool will be allocated inside
 *		the function.
 *
 * @param[in]	from_buf - DPAA2 buffer which needs to be copied.
 *
 * @returns	DPAA2_SUCCESS on success; DPAA2_FAILURE otherwise
 *
 */
int dpaa2_mbuf_sg_copy(
		dpaa2_mbuf_pt to_buf,
		dpaa2_mbuf_pt from_buf);

/* Extern declarations */
extern struct dpaa2_bp_list *g_bp_list;

#ifdef __cplusplus
}
#endif

#endif	/* _DPAA2BUF_PRIV_H_ */
