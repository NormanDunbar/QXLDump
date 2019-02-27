/*
 * MIT License
 *
 * Copyright (c) 2017 Norman Dunbar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef QXLHEADER_H
#define QXLHEADER_H

#if defined (__GNUC__)
	#include <stdint.h>
#endif

const int QXLHeaderLength = 0x40;

typedef struct QXLHeader {

    char qwa_id[4];                 // 'QLWA' for disc/QXL.WIN. 0x01,'QWA' for partitions.
    uint16_t qwa_name_size;         // Size of the following name.
    char qwa_name[20];              // Space filled bytes of the name.
    uint16_t qwa_spr0;              // Spare - should be zero.
    uint16_t qwa_frmt;              // Format Random number.
    uint16_t qwa_uchk;              // Update counter.
    uint16_t qwa_intl;              // Interleave factor. 0 = SCSI/QXL.WIN.
    uint16_t qwa_sctg;              // Sectors per group.
    uint16_t qwa_sctt;              // Sectors per track. 0 = SCSI/QXL.WIN.
    uint16_t qwa_trkc;              // Tracks per cylinder. (Number of heads.) 0 = SCSI/QXL.WIN.
    uint16_t qwa_cyld;              // Cylinders per drive.
    uint16_t qwa_ngrp;              // Number of groups.
    uint16_t qwa_fgrp;              // Number of free groups.
    uint16_t qwa_sctm;              // Sectors (512Kb) per map.
    uint16_t qwa_nmap;              // Number of maps.
    uint16_t qwa_free;              // First Free group. (In map)
    uint16_t qwa_root;              // Root Directory Number. (FileId.)
    uint32_t qwa_rlen;              // Root directory length(plus 64 bytes!)

    union {
        uint16_t qwa_fcyl;          // First cylinder number. ST506 only. (Miracle Hard Drive?)
        uint32_t qwa_fsct;          // First Sector in this partition. 0 = SCSI/QXL.WIN.
    } qwa_first;

    uint16_t qwa_park;              // Park cylinder.
} QXLHeader;

#endif // QXLHEADER_H

