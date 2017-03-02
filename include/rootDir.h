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

#ifndef ROOTDIR_H
#define ROOTDIR_H

typedef struct rootDir {
    uint32_t hdr_flen;              // File length plus 64. Zero = deleted.
    uint8_t  hdr_accs;              // Access byte.
    uint8_t  hdr_type;              // File type.
    uint32_t hdr_data;              // File dataspace. EXECable files only.
    uint32_t hdr_xtra;              // Extra Information.
    uint32_t hdr_name_size;         // Size of filename.
    char     hdr_name[36];          // File name. Up to 36 bytes.
    uint32_t hdr_date;              // File update date.
    uint16_t hdr_vers;              // File version number.
    uint16_t hdr_flid;              // File ID. Used in the map. Zero = deleted.
    uint32_t hdt_bkup;              // File backup date.
} rootDir;

#endif // ROOTDIR_H
