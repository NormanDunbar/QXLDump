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
#include "css.h"

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

    // Read in the file header. Field by bloody field.
    // Because the QL is intelligent and has numbers stored big endian
    // but the PC is arse about face aka little endian.
    readHeader();

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


    // Write out the HTML or TEXT headings etc.
    if (mOptions->QXDHtml()) {
        // HTML required.
        cout << "<html>" << endl << "<head>" << endl << "<title>QXLDump</title>" << endl
             << cssText << "</head>" << endl << "<body>" << endl;
        cout << "<h1>QXLDump</H1>" << endl;
        cout << "<p><strong>" << mOptions->QXDQxlFile() << "</strong></p>" << endl << endl;

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
    cout << "<h2>File Header</h2>" << endl;
    cout << "<table style=\"width:95%\">" << endl
         << "<tr><th style=\"width:5%\";>Offset Hex</th>"
         << "<th style=\"width:5%\";>Offset Dec</th>"
         << "<th style=\"width:10%\";>Mnemonic</th>"
         << "<th style=\"width:10%\";>Value</th>"
         << "<th>Description</th></tr>" << endl;

    // Header data. ID bytes.
    displayOffset(offset, 4, "qwa_id");
     cout << "<td class=\"number\">" << Header.qwa_id[0]
                   << Header.qwa_id[1]
                   << Header.qwa_id[2]
                   << Header.qwa_id[3] << "</td>"
         << "<td class=\"text\">QLWA Identification bytes.</td></tr>" << endl;

    // Header data. Name Size.
    offset += 4;
    displayData(offset, 4, Header.qwa_name_size, "qwa_name_size", "Size of QXL.WIN file name.");

    // Header data. Name.
    offset += 2;
    displayOffset(offset, 4, "qwa_name");

    cout << "<td class=\"number\">";
    for (unsigned x = 0; x < 20; x++) {
        cout << Header.qwa_name[x];
    }
    cout <<  "</td><td class=\"text\">Bytes of QXL.WIN file name.</td></tr>" << endl;


    // Header data. Spare 0.
    offset += 20;
    displayData(offset, 4, Header.qwa_spr0, "qwa_spr0", "Spare. Should be zero.");

    // Header data. Update Check.
    offset += 2;
    displayData(offset, 8, Header.qwa_uchk, "qwa_uchk", "Update counter. First word is random. Second is counter.");

    // Header data. Interleave factor.
    offset += 4;
    displayData(offset, 4, Header.qwa_intl, "qwa_intl", "Interleave factor.");

    // Header data. Sectors per group.
    offset += 2;
    displayData(offset, 4, Header.qwa_sctg, "qwa_sctg", "Sectors per group.");

    // Header data. Sectors per track.
    offset += 2;
    displayData(offset, 4, Header.qwa_sctt, "qwa_sctt", "Sectors per track.");

    // Header data. Tracks per cylinder.
    offset += 2;
    displayData(offset, 4, Header.qwa_trkc, "qwa_trkc", "Tracks per cylinder.");

    // Header data. Cylinders per drive.
    offset += 2;
    displayData(offset, 4, Header.qwa_cyld, "qwa_cyld", "Cylinders per drive.");

    // Header data. Number of groups.
    offset += 2;
    displayData(offset, 4, Header.qwa_ngrp, "qwa_ngrp", "Number of groups.");

    // Header data. Number of free groups.
    offset += 2;
    displayData(offset, 4, Header.qwa_fgrp, "qwa_fgrp", "Number of free groups.");

    // Header data. Sectors per map.
    offset += 2;
    displayData(offset, 4, Header.qwa_sctm, "qwa_sctm", "Sectors per map.");

    // Header data. Number of maps.
    offset += 2;
    displayData(offset, 4, Header.qwa_nmap, "qwa_nmap", "Number of maps.");

    // Header data. First Free Group.
    offset += 2;
    displayData(offset, 4, Header.qwa_free, "qwa_free", "First free group.");

    // Header data. Root Directory FileID.
    offset += 2;
    displayData(offset, 4, Header.qwa_root, "qwa_root", "Root directory file number.");

    // Header data. Root directory length.
    offset += 2;
    displayData(offset, 8, Header.qwa_rlen, "qwa_rlen", "Root directory length.");

    // Header data. First cylinder/sector.
    offset += 4;
    displayData(offset, 8, Header.qwa_first.qwa_fcyl, "qwa_fcyl", "First cylinder (word) *** OR ***");
    displayData(offset, 4, Header.qwa_first.qwa_fsct, "qwa_fsct", "First sector (long word).");

    // Header data. .
    offset += 4;
    displayData(offset, 4, Header.qwa_park, "qwa_park", "Parking cylinder.");


    // Close the table.
    cout << "</table>" << endl << "<hr>" << endl << endl;
}


void QXDQxlWin::displayData(uint16_t &offset, const uint16_t width, const uint32_t value, const string name, const string description)
{
    // Display details about the fields in the header. Each
    // is added as a complete row in the table.
    displayOffset(offset, width, name);
    cout << "<td class=\"number\">" << value << "</td>"
         << "<td class=\"text\">" << description << "</td></tr>" << endl;
}

void QXDQxlWin::displayOffset(uint16_t &offset, const uint16_t width, const string name)
{
    // Display details about the offsets etc for the header data
    // but excludes the value and description.
    // We have the following:
    //
    // OFFSET(hex) OFFSET(dec) NAME.
    cout << "<tr><td class=\"number\">$"
         << setbase(16) << setw(4) << setfill('0') << offset << "</td>"
         << "<td class=\"number\">"
         << setbase(10) << setw(0) << setfill(' ') << offset << "</td>"
         << "<td class=\"text\">" << name << "</td>";
}

void QXDQxlWin::readHeader()
{
    // Convert from QL big endian format to PC/Linux little endian.
    mIfs->read(&Header.qwa_id[0], 4);
    Header.qwa_name_size = getWord();
    mIfs->read(&Header.qwa_name[0], 20);
    Header.qwa_spr0 = getWord();
    Header.qwa_uchk = getLong();
    Header.qwa_intl = getWord();
    Header.qwa_sctg = getWord();
    Header.qwa_sctt = getWord();
    Header.qwa_trkc = getWord();
    Header.qwa_cyld = getWord();
    Header.qwa_ngrp = getWord();
    Header.qwa_fgrp = getWord();
    Header.qwa_sctm = getWord();
    Header.qwa_nmap = getWord();
    Header.qwa_free = getWord();
    Header.qwa_root = getWord();
    Header.qwa_rlen = getLong();
    Header.qwa_first.qwa_fsct = getLong();
    Header.qwa_park = getWord();
}

uint8_t QXDQxlWin::getByte() {
    // Read an unsigned byte from the qxl.win file.
    char c;
    uint8_t result;

    mIfs->get(c);    // Signed.
    result = c;     // Unsigned. But -2 becomes 2.
    if (c < 0)
        result += 256;

    return result;
}


uint16_t QXDQxlWin::getWord() {
    // Read a big endian word from a qxl.win file.
    uint16_t result = 0;

    result = getByte();
    result *= 256;
    result += getByte();

    return result;
}


uint32_t QXDQxlWin::getLong() {
    // Read  big endian long word from a qxl.win file.
    uint32_t result = 0;

    result = getWord();
    result *= 65536;
    result += getWord();

    return result;
}


