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

#include "QXDQxlWin.h"

QXDQxlWin::QXDQxlWin(QXDOptions *opt)
{
    mOptions = opt;
    mIfs = NULL;
    QXDOpenFile();
}

QXDQxlWin::~QXDQxlWin()
{
    // If we opened the file, close it.
    if (mIfs) {
        mIfs->close();
        delete mIfs;
        cerr << mOptions->QXDQxlFile() << " closed." << endl;
    }

    // Close the output file.
    if (mOptions->QXDHtml()) {
        // HTML required.
        cout << endl << "</body></html>" << endl << endl;

    } else {
        // TEXT required.
        cout << endl << endl;
    }
}

bool QXDQxlWin::QXDOpenFile()
{
    // Open the QXL.WIN file we were passed on the command line.
    // We know the file exists by the time we get here.
    mIfs = new ifstream(mOptions->QXDQxlFile(), std::ios_base::binary|std::ios_base::in);
    cerr << mOptions->QXDQxlFile() << " opened." << endl;

    // Read in the file header.
    mIfs->read((char *)&Header, QXLHeaderLength);

    // Do we have a QXL.WIN?
    if (!(Header.qwa_id[0] == 'Q' &&
          Header.qwa_id[1] == 'L' &&
          Header.qwa_id[2] == 'W' &&
          Header.qwa_id[3] == 'A'))
    {
        cerr << "QXLDump: Invalid file. " << mOptions->QXDQxlFile()
             << " is not a QXL.WIN file. Header <> 'QLWA'." << endl;
        return false;
    }

    reverseHeader();

    // Write out the HTML or TEXT headings etc.
    if (mOptions->QXDHtml()) {
        // HTML required.
        cout << "<html><head><title>QXLDump</title></head>" << endl << "<body>" << endl;
        cout << "<h1>QXLDump</H1>" << endl;
        cout << "<p>" << mOptions->QXDQxlFile() << "</p>" << endl << endl;

    } else {
        // TEXT required.
        cout << "QXLDump: <" << mOptions->QXDQxlFile() << ">." << endl << endl;
    }

    // Indicate good or bad with the file.
    return mIfs->good();

}

void QXDQxlWin::doHeader()
{
    uint16_t offset = 0;
    // Dump the QXL.WIN header.
    cout << "<table>" << endl
         << "<tr><th>Offset Hex</th>"
         << "<th>Offset Dec</th>"
         << "<th>Mnemonic</th>"
         << "<th>Value</th>"
         << "<th>Description</th></tr>" << endl;

    // Header data. ID bytes.
    cout << "<tr><td>$"
         << setbase(16) << setw(4) << setfill('0') << offset << "</td>"
         << "<td>"
         << setbase(10) << setw(0) << setfill(' ') << offset << "</td>"
         << "<td>qwa_id</td>"
         << "<td>" << Header.qwa_id[0]
                   << Header.qwa_id[1]
                   << Header.qwa_id[2]
                   << Header.qwa_id[3] << "</td>"
         << "<td>QLWA Identification bytes.</td></tr>" << endl;

    // Header data. Name Size.
    offset += 4;
    cout << "<tr><td>$"
         << setbase(16) << setw(4) << setfill('0') << offset << "</td>"
         << "<td>"
         << setbase(10) << setw(0) << setfill(' ') << offset << "</td>"
         << "<td>qwa_name_size</td>"
         << "<td>" << Header.qwa_name_size << "</td>"
         << "<td>Size of QXL.WIN file name.</td></tr>" << endl;

    // Header data. Name.
    offset += 2;
    cout << "<tr><td>$"
         << setbase(16) << setw(4) << setfill('0') << offset << "</td>"
         << "<td>"
         << setbase(10) << setw(0) << setfill(' ') << offset << "</td>"
         << "<td>qwa_name</td>"
         << "<td>";

    for (unsigned x = 0; x < 20; x++) {
        cout << Header.qwa_name[x];
    }

    cout <<  "</td>"
         << "<td>Bytes of QXL.WIN file name.</td></tr>" << endl;



    // Close the table.
    cout << "</table>" << endl << "<hr>" << endl << endl;
}


void QXDQxlWin::reverseHeader()
{
    // Convert from QL big endian format to PC/Linux little endian.

    cerr << "Name size " << Header.qwa_name_size << endl;
    Header.qwa_name_size = reverseWord(Header.qwa_name_size);
    cerr << "Name size " << Header.qwa_name_size << endl;
    Header.qwa_spr0 = reverseWord(Header.qwa_spr0);
    Header.qwa_uchk = reverseLong(Header.qwa_uchk);
    Header.qwa_intl = reverseWord(Header.qwa_intl);
    Header.qwa_sctg = reverseWord(Header.qwa_sctg);
    Header.qwa_sctt = reverseWord(Header.qwa_sctt);
    Header.qwa_trkc = reverseWord(Header.qwa_trkc);
    Header.qwa_cyld = reverseWord(Header.qwa_cyld);
    Header.qwa_ngrp = reverseWord(Header.qwa_ngrp);
    Header.qwa_fgrp = reverseWord(Header.qwa_fgrp);
    Header.qwa_sctm = reverseWord(Header.qwa_sctm);
    Header.qwa_nmap = reverseWord(Header.qwa_nmap);
    Header.qwa_free = reverseWord(Header.qwa_free);
    Header.qwa_root = reverseWord(Header.qwa_root);
    Header.qwa_rlen = reverseLong(Header.qwa_rlen);
    Header.qwa_first.qwa_fsct = reverseLong(Header.qwa_first.qwa_fsct);
    Header.qwa_park = reverseWord(Header.qwa_park);
}

uint16_t QXDQxlWin::reverseWord(uint16_t word)
{
    // Reverse big endian word to little endian.
   union {
       char bytes[2];
       uint16_t word;
   } temp;

   temp.word = word;
   return temp.bytes[0]  << 8 | temp.bytes[1];

   // $0004 = $04 $00 <--> $00 $04
}

uint32_t QXDQxlWin::reverseLong(uint32_t longWord)
{
    // Reverse big endian longword to little endian.
    union {
        uint32_t longWord;
        uint16_t words[2];
    } temp;

    temp.longWord = longWord;
    temp.words[0] = reverseWord(temp.words[0]);
    temp.words[1] = reverseWord(temp.words[1]);

    return temp.words[0] << 16 | temp.words[1];
}

