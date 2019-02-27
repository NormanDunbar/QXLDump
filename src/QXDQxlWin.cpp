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

#include <sstream>
#include <cctype>

#include "Thousands.h"

using std::stringstream;
using std::isprint;

#include "QXDQxlWin.h"
#include "css.h"
#include "rootDir.h"

QXDQxlWin::QXDQxlWin(QXDOptions *opt)
{
    mOptions = opt;
    mIfs = NULL;
    mQXLMap = NULL;
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

    // If we allocated a map, delete it.
    if (mQXLMap) {
        delete [] mQXLMap;
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

    // Read in the map blocks.
    readMap();

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


    // Write out the HTML headings etc.
    if (mOptions->QXDHtml()) {
        // HTML required.
        cout << "<html>" << endl << "<head>" << endl << "<title>QXLDump</title>" << endl
             << cssText << "</head>" << endl << "<body>" << endl;
        cout << "<h1>QXLDump</H1>" << endl;
        cout << "<p><strong>Qxl.win File Name: </strong>" << mOptions->QXDQxlFile() << "</p>" << endl << endl;

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
    cout << "The header of the qxl.win file starts at offset zero in the physical file. It takes up the first "
         << "64 ($0040) bytes of the file.</p>" << endl;

    cout << "<table style=\"width:95%\">" << endl
         << "<tr><th style=\"width:5%\";>Offset Hex</th>"
         << "<th style=\"width:5%\";>Offset Dec</th>"
         << "<th style=\"width:10%\";>Mnemonic</th>"
         << "<th style=\"width:10%\";>Value</th>"
         << "<th>Description</th></tr>" << endl;

    // Header data. ID bytes.
    displayOffset(offset, 4, "qwa_id");
     cout << "<td class=\"right\">" << Header.qwa_id[0]
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

    cout << "<td class=\"right\">";
    for (unsigned x = 0; x < 20; x++) {
        cout << Header.qwa_name[x];
    }
    cout <<  "</td><td class=\"text\">Bytes of QXL.WIN file name.</td></tr>" << endl;


    // Header data. Spare 0.
    offset += 20;
    displayData(offset, 4, Header.qwa_spr0, "qwa_spr0", "Spare. Should be zero.");

    // Header data. Update Check.
    offset += 2;
    displayData(offset, 4, Header.qwa_frmt, "qwa_uchk(HI)", "Format random number.");

    // Header data. Update Check.
    offset += 2;
    displayData(offset, 4, Header.qwa_uchk, "qwa_uchk(LO)", "Update counter.");

    // Header data. Interleave factor.
    offset += 2;
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
    displayData(offset, 4, Header.qwa_ngrp, "qwa_ngrp", "Total number of groups.");

    // Header data. Number of free groups.
    offset += 2;
    displayData(offset, 4, Header.qwa_fgrp, "qwa_fgrp", "Number of free or unusable groups.");

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

    cout << "<tr><td>&nbsp;</td><td class=\"right\" colspan=\"2\"><strong>Total Disc Size:<strong></td><td class=\"right\">"
         << (float)(Header.qwa_ngrp * Header.qwa_sctg * 512) / 1024 << "</td><td>"
         << "Disc size in Kb.</td></tr>" << endl;

    cout << "<tr><td>&nbsp;</td><td class=\"right\" colspan=\"2\"><strong>Total Free Space:<strong></td><td class=\"right\">"
         << (float)(Header.qwa_fgrp * Header.qwa_sctg * 512) / 1024 << "</td><td>"
         << "Free space in Kb.</td></tr>" << endl;

    // Close the table.
    cout << "</table>" << endl << "<hr>" << endl << endl;;
}


void QXDQxlWin::displayData(uint16_t &offset, const uint16_t width, const uint32_t value, const string name, const string description)
{
    // Display details about the fields in the header. Each
    // is added as a complete row in the table.
    displayOffset(offset, width, name);
    cout << "<td class=\"right\">" << value << "</td>"
         << "<td class=\"left\">" << description << "</td></tr>" << endl;
}

void QXDQxlWin::displayOffset(uint16_t &offset, const uint16_t width, const string name)
{
    // Display details about the offsets etc for the header data
    // but excludes the value and description.
    // We have the following:
    //
    // OFFSET(hex) OFFSET(dec) NAME.
    cout << "<tr><td class=\"right\">$"
         << setbase(16) << setw(4) << setfill('0') << offset << "</td>"
         << "<td class=\"right\">"
         << setbase(10) << setw(0) << setfill(' ') << offset << "</td>"
         << "<td class=\"left\">" << name << "</td>";
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

void QXDQxlWin::readMap()
{
    // Allocate some space for the maximum map entries.
    // Used + free.
    uint16_t maxBlocks = Header.qwa_ngrp;

    mQXLMap = new uint16_t[maxBlocks];

    mIfs->seekg(0x40);      // Start of map in file.

    // Read in each and every entry as a 16 bit unsigned word.
    for (int mapBlock = 0; mapBlock < maxBlocks; mapBlock++) {
        mQXLMap[mapBlock] = getWord();
    }
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

bool QXDQxlWin::getMapBlockData(const uint16_t blockId, uint16_t &nextBlock, uint32_t &blockFileAddress)
{
    // Given a blockId, return the block address in the map, and the
    // Offset into the whole file where this block's data starts.
    //
    // The BlockID can also be a FileID as a FileId is simply the
    // first block's BlockId in the map.
    //
    // The BlockId is < Header.qwa_ngrp.
    // The blockMapAddress is 0x40 + (BlockId * 2);
    //
    // The blockFileAddress is BlockId * Header.wqa_sgrp * 512.
    //

    if (blockId < Header.qwa_ngrp) {
        // We are good to go!
        nextBlock = mQXLMap[blockId];
        blockFileAddress = blockId * Header.qwa_sctg * 512;
        return true;
    } else {
        // Oops, blockID out of range.
        cerr << "QXLDump: getMapBlockData(): BlockID (" << blockId
             << ") out of range. Maximum allowed is: " << Header.qwa_ngrp << "." << endl;
        return false;
    }
}

string QXDQxlWin::getBlockChainTable(const uint16_t blockId)
{
    // Return a string, containing the full definition of
    // an HTML table, for the given block chain.

    stringstream s;
    s << "<table style=\"width:95%\">" << endl
      << "<tr><th class=\"middle\" style=\"width:5%\">First Block</th>"
      << "<th class=\"middle\">Map Block Chain</th></tr>"
      << endl
      << "<tr><td class=\"middle\">$" << setbase(16) << setw(4) << setfill('0')
      << blockId
      << setbase(10) << setw(0) << setfill(' ') << "</td><td class=\"left\">"
      << getBlockChain(blockId)
      << endl << "</td></tr>" << endl
      << "</table>" << endl;

    return s.str();
}

string QXDQxlWin::getBlockChain(const uint16_t blockId)
{
    // Return a string of all the blocks in a given chain.
    // We do not return the starting block though.

    // Walk the free chain blocks from the map.
    uint32_t ignore = 0;
    uint16_t nextBlock;
    stringstream s;

    nextBlock = blockId;

    do
    {
        // Get the block that comes next. Bale out on error.
        if (!getMapBlockData(nextBlock, nextBlock, ignore)) {
            break;
        };

        if (!nextBlock) {
            s << setbase(10) << "(END).";
        } else {
            s << "$" << setbase(16) << setw(4) << setfill('0') << nextBlock
              << setw(0) << setfill(' ') << " -> ";
        }

        // Next block is already in nextFreeBlock.
    } while (nextBlock);

    return s.str();
}


void QXDQxlWin::doMap()
{
    // Dump out the map.
    // Similar to doFree() as we dump the raw map blocks
    // first, then attempt to analyse.

    cout << "<h2>Map</h2>" << endl;

    // Show the chain of map blocks for the map itself.
    cout << "<h3>Map Location Data</h3>" << endl;
    cout << "<p>The following table shows the locations, in the map, of the blocks used for the map itself. "
         << "The map always starts at location $0040 on disc, and this corresponds to the first block of "
         << "the map itself. </p>" << endl;

    cout << getBlockChainTable(0) << endl;

    cout << "<h3>Map Raw Data</h3>" << endl;

    cout << "<p>The following table shows the dump of all " << Header.qwa_ngrp << " blocks in the map. "
         << "Each block in the map represents a group of " << Header.qwa_sctg << " sectors of 512 bytes "
         << "giving a minimum size of any file on this disc of " << Header.qwa_sctg * 512 << " bytes. </p>"
         << endl
         << "<p>To use the table below, scan down the left side till you find the top 3 digits of the "
         << "required block number. For example, $010. Now look across the top for the lowest digit of "
         << "the block, for example, $7. The intersection of the row and column holds the value of the "
         << "block that comes next in this block chain. If that value is $0000, then this will be the final "
         << "block in the chain.</p>" << endl;

    cout << "<table style=\"width:95%\">" << endl
         << "<tr><td class=\"hidden\">&nbsp;</td>"
         << "<th class=\"middle\" colspan=\"16\">Least Significant Nibble</th></tr>" << endl
         << "<tr><th class=\"hidden\" style=\"width:3%\">&nbsp;</th>";

    // Least significant nibble across the top. ($---x)
    for (int lowNibble = 0; lowNibble < 16; lowNibble++) {
        cout << "<th style=\"width:2%\">$" << setbase(16) << setw(1) << lowNibble
             << setw(0) << setfill(' ') << "</td>";
    }
    cout << "</tr>" << endl;

    // Most significant byte plus the high nibble down the side. ($xxx_)
    // This gives 16 columns per row. Could get messy! Max blocks is 65536
    // which will give 4096 rows. Hmmm.
    // Qwa_ngrp will always divide exactly by 256. In case you were wondering!
    for (int highByte = 0; highByte < Header.qwa_ngrp/256; highByte++) {
        for (int highNibble = 0; highNibble < 16; highNibble++) {
            // thisWord is $xxx - everything but the lowest nibble.
            // Use it down the left most column of the table.
            uint16_t thisWord = (highByte * 256) + (highNibble * 16);
            cout << "<tr><th style=\"width:2%\">$" << setbase(16) << setw(3) << setfill('0')
                 << thisWord
                 << setw(0) << setfill(' ') << "</td>";

            // Now the (next) 16 map entries.
            for (int lowNibble = 0; lowNibble < 16; lowNibble++) {
                uint16_t blockId = thisWord + lowNibble;
                cout << "<td class=\"middle\">$" << setbase(16) << setw(4) << setfill('0')
                     << mQXLMap[blockId]
                     << setw(0) << setfill(' ') << setbase(10) << "</td>";
            }

            cout << "</tr>" << endl;
        }
    }
    cout << "</table>" << endl;
    cout << "<hr>" << endl << endl;
}

void QXDQxlWin::doRoot()
{
    // Dump out the root directory.
    cout << "<h2>Root Directory</h2>" << endl;
    cout << "<h3>Map Location Data</h3>" << endl;

    cout << "<p>The following table shows the locations, in the map, of the blocks used for the root directory. "
         << "The root directory is the directory of the qxl.win file itself. This directory is so important "
         << "that it's location - in the map - is stored in the file header. This allows the physical location "
         << "within the disc itself, to be determined easily. </p>"
         << endl
         << "<p>The file id for the root directory in this file is $"
         << setbase(16) << setw(4) << setfill('0') << Header.qwa_root << setbase(10) << setw(0) << setfill(' ')
         << ".</p>" << endl;

    uint16_t rootBlock = Header.qwa_root;

    if (mOptions->QXDVerbose()) {
        // Show the chain of map blocks for the root directory.
        cout << getBlockChainTable(rootBlock);

        // Show the entire root directory, as a hex dump.
        cout << "<h3>Root Sectors</h3>" << endl;
        cout << "<p>The following table is a hex dump of the entire directory. Please note that the first "
             << "64 bytes (4 rows) of the dump are not part of the directory itself. They are <em>supposed</em> "
             << "to be a copy of the directory entry for the file, but appear, simply, unused.</p>" << endl;

        cout << getHexDumpTable(rootBlock) << endl;
    }

    // Show the directory as a table of entries.
    cout << "<h3>Root Directory</h3>" << endl;
    cout << "The following table shows the entries in the root directory. Unused entries, such as for "
         << "deleted files, are not listed here. You can see those in the raw data above in '--verbose' mode.</p>" << endl;

    cout << getDirectoryTable(rootBlock, Header.qwa_rlen) << endl;

    cout << "<hr>" << endl << endl;

}

void QXDQxlWin::doData(uint16_t blockId)
{
    // Dump out a single data block.
    cout << "<h2>Data Block</h2>" << endl;
    cout << "<p>Note that when dumping a single data block that the first 64 bytes <em>may not</em> "
         << "be part of the actual data in the block, if and only if, the block is the <em>first</em> block "
         << "in the appropriate data file.</p>" << endl
         << "<p>Similarly, if a block is the <em>final</em> one in a file or directory, then some of the "
         << "displayed data below, may be random garbage left over from previous use of the sectors which "
         << "make up the final block of a file or directory.</p>" << endl;

    cout << "<h3>Block Data</h3>" << endl;
    cout << "<p>The following table is a hex dump of data block " << blockId << "."
         << endl;

    cout << getHexDumpTable(blockId) << endl;

    cout << "<hr>" << endl << endl;
}

void QXDQxlWin::doFree()
{
    // Dump out the entire free space area.
    // There's no analysis can be done here.
    cout << "<h2>Free Space</h2>" << endl;
    cout << "<h3>Map Location Data</h3>" << endl;

    cout << "<p>The following table shows the complete chain of free blocks in the map for this qxl.win file. "
         << "The first block id in the chain, $"
         << setbase(16) << setw(4) << setfill('0') << Header.qwa_free << setbase(10) << setw(0) << setfill(' ')
         << ", in this case, will be used as the file id for the next new file or directory to be created. "
         << "When a file or directory is deleted, the chain of blocks, making up that file or "
         << "directory, are added at the <em>front</em> of the free space list. This means that, in essence, "
         << "trying to undelete a file on a qxl.win, will be rather difficult. </p>"
         << endl
         << "<p>This file's free space contains " << Header.qwa_fgrp << " ($"
         << setbase(16) << setw(4) << setfill('0') << Header.qwa_free << setbase(10) << setw(0) << setfill(' ')
         << ") blocks.</p>" << endl;

    uint16_t nextFreeBlock = Header.qwa_free;
    cout << getBlockChainTable(nextFreeBlock);

    cout << "<hr>" << endl << endl;
}

void QXDQxlWin::doFile(uint16_t fileId)
{
    // Dump out a given file, from it's fileid.
    // Which is just the first block in the map.
    // Which we can use to find the file on disc too.

    cout << "<h2>File: " << fileId << "</h2>" << endl;
    cout << "<h3>Map Location Data</h3>" << endl;

    cout << "<p>The following table shows the complete chain of blocks in the map for the data file which "
         << "has file id = $"
         << setbase(16) << setw(4) << setfill('0') << fileId << setbase(10) << setw(0) << setfill(' ')
         << " (" << fileId << ").</p>" << endl;

    uint16_t nextBlock = fileId;
    cout << getBlockChainTable(nextBlock);

    // We don't need --verbose mode to dump a file as there's nothing
    // to show if we only dump out in verbose mode!
    cout << "<h3>File data</h3>" << endl;
    cout << "<p>The following table is a hex dump of the entire file. Please note that the <strong>first "
         << "64 bytes (4 rows)</strong> of the file are <strong>not part of the file</strong> itself. They are <em>supposed</em> "
         << "to be a copy of the directory entry for the file, but appear, simply, unused.</p>" << endl
         << endl;

    cout << getHexDumpTable(fileId) << endl;
    cout << "<hr>" << endl << endl;
}

void QXDQxlWin::doDirectory(uint16_t blockId)
{
    // Dump out the requested directory.

    uint16_t dirBlock = blockId;

    cout << "<h2>Directory Dump</h2>" << endl;

    if (mOptions->QXDVerbose()) {
        // Show the chain of map blocks for the root directory.
        cout << "<h3>Map Location Data</h3>" << endl;

        cout << "<p>The following table shows the locations, in the map, of the blocks used for the requested directory.</p>"
             << endl;

        cout << getBlockChainTable(dirBlock);

        // Show the entire root directory, as a hex dump.
        cout << "<h3>Directory Sectors</h3>" << endl;
        cout << "<p>The following table is a hex dump of the entire directory. Please note that the first "
             << "64 bytes (4 rows) of the dump are not part of the directory itself. They are <em>supposed</em> "
             << "to be a copy of the directory entry for the file, but appear, simply, unused.</p>" << endl;

        cout << getHexDumpTable(dirBlock) << endl;
    }

    // Show the directory as a table of entries.
    cout << "<h3>Directory</h3>" << endl;
    cout << "The following table shows the entries in the directory. Unused entries, such as for "
         << "deleted files, are not listed here. You can see those in the raw data in '--verbose' mode.</p>" << endl;

    cout << getDirectoryTable(dirBlock, mOptions->QXDDirLength()) << endl;
    cout << "<hr>" << endl << endl;

}

string QXDQxlWin::getHexDumpTable(uint16_t blockId)
{
    // Dump out a hex listing of a file as a table.
    stringstream s;

    uint16_t nextBlock = blockId;
    uint32_t ignore;

    do {
        s << "<p><Strong>Block: $</strong>"
          << setbase(16) << setw(4) << setfill('0') << nextBlock
          << " (" << setbase(10) << setw(0) << setfill(' ') << nextBlock
          << ")</p>" << endl
          << "<table style=\"width:70%\">" << endl
          << "<tr><th class=\"middle\" style=\"width:10%\">Offset</th>"
          << "<th class=\"middle\" style=\"width:45%\">Hex Codes</th>"
          << "<th class=\"middle\" style=\"width:15%\">ASCII Codes</th></tr>"
          << endl;

        s << getHexDumpRows(nextBlock)
          << "</table>" << endl;

        getMapBlockData(nextBlock, nextBlock, ignore);
    } while (nextBlock);

    return s.str();
}


string QXDQxlWin::getHexDumpRows(uint16_t blockId)
{
    // Return one block's worth of file as a 16 byte
    // per row table. As many rows as required are returned.
    uint32_t fileOffset = 0;
    uint16_t nextBlock;
    stringstream s;
    string ascii;

    // Each block is a given number of sectors.
    uint8_t sector[512 * Header.qwa_sctg];

    // Convert the blockID to a file offset.
    nextBlock = blockId;
    fileOffset = nextBlock * Header.qwa_sctg * 512;

    // I suppose we better read the file?
    mIfs->seekg(fileOffset);
    mIfs->read((char *)&sector[0], 512 * Header.qwa_sctg);

    // 16 bytes per row = 16 rows per sector.
    const uint16_t numRows = 16;
    for (uint16_t x=0; x < (512 * Header.qwa_sctg)/numRows; x++) {
        // Do the offset.
        s << "<tr><td class=\"middle\">$"
          << setbase(16) << setw(8) << setfill('0') << fileOffset + (x * numRows)
          << setfill(' ') << setw(0) << "</td><td class=\"middle\">";

        // Do 16 bytes per row.
        uint16_t byteOffset = (x * 16);
        ascii.clear();

        for (uint16_t byte = 0; byte < 16; byte++) {
            uint8_t c = sector[byteOffset + byte];

            s << setw(2) << setfill('0') << (c & 0xff)
              << setw(0) << setfill(' ') << ' ';

            if (isprint(c)) {
                ascii.push_back(sector[byteOffset + byte]);
            } else {
                ascii.push_back('.');
            }
        }
        s << "</td><td class=\"middle\">" << ascii << "</td></tr>" << endl;
    }

    return s.str();
}


string QXDQxlWin::getDirectoryTable(uint16_t blockId, const uint32_t dirLength)
{
    // Dump out a directory listing as a table. However,
    // we are only interested in the valid entries and not
    // in any potential "crap" (technical term) after the
    // official end of the directory.
    // As each directory entry is a fixed 64 bytes,
    // We therefore get 2048/64 entries in the first n blocks
    // And file_size mod 64 in the last block.
    // Easy!
    stringstream s;
    uint16_t thisBlock = blockId;
    uint16_t nextBlock;
    uint32_t ignore;
    uint16_t numDirEntries;
    uint16_t fullBlocksDone = 0;
    bool firstBlock=true;

    // Calculate the number of entries in a full block.
    numDirEntries = (512 * Header.qwa_sctg) / 64;

    do {
        s << "<p><Strong>Block: $</strong>"
          << setbase(16) << setw(4) << setfill('0') << thisBlock
          << " (" << setbase(10) << setw(0) << setfill(' ') << thisBlock
          << ")</p>" << endl;

        s << "<table style=\"width:95%\">" << endl << "<tr>"
          << "<th class=\"middle\" style=\"width:10%\">File<br>Length</th>"
          << "<th class=\"middle\" style=\"width:5%\">Acc<br>-ess</th>"
          << "<th class=\"middle\" style=\"width:5%\">File<br>Type</th>"
          << "<th class=\"middle\" style=\"width:10%\">Data<br>Space</th>"
          << "<th class=\"middle\" style=\"width:10%\">File<br>Extra</th>"
          << "<th class=\"middle\" style=\"width:10%\">Name<br>Size</th>"
          << "<th class=\"middle\" style=\"width:20%\">File<br>Name</th>"
          << "<th class=\"middle\" style=\"width:10%\">Update<br>Date</th>"
          << "<th class=\"middle\" style=\"width:5%\">Vers<br>-ion</th>"
          << "<th class=\"middle\" style=\"width:5%\">File<br>Id</th>"
          << "<th class=\"middle\" style=\"width:10%\">Backup<br>date</th>"
          << "</tr>" << endl;

        // Get the following block number, in case we are done.
        getMapBlockData(thisBlock, nextBlock, ignore);

        // Are we on a full block of 2048/64 entries?
        // Or on the last one, which may be smaller?
        if (!nextBlock) {
            // Last block, possibly short.
            numDirEntries = (dirLength / 64) - (fullBlocksDone * numDirEntries);
        }

        s << getDirectoryRows(thisBlock, numDirEntries, firstBlock)
          << "</table>" << endl;

        // Process the correct block!
        thisBlock = nextBlock;
        fullBlocksDone++;
        firstBlock = false;

    } while (thisBlock);

    return s.str();
}

string QXDQxlWin::getDirectoryRows(uint16_t blockId, uint16_t numDirEntries, const bool firstBlock)
{
    // Returns rows of data for a directory block.
    // As many rows as required are returned.
    uint32_t fileOffset = 0;
    uint16_t thisBlock = blockId;
    stringstream s;

    // Make sure that S gets comma/dot separated thousands etc.
    s.imbue(locale(s.getloc(), new ThousandsSeparator<char>(',')));

    rootDir directory[numDirEntries];

    // Convert the blockID to a file offset.
    fileOffset = thisBlock * Header.qwa_sctg * 512;

    // I suppose we better read the file?
    // Bit by bloody bit because endian-ness! Sigh.
    mIfs->seekg(fileOffset);
    for (uint16_t x=0; x < numDirEntries; x++) {
        directory[x].hdr_flen = getLong();
        directory[x].hdr_accs = getByte();
        directory[x].hdr_type = getByte();
        directory[x].hdr_data = getLong();
        directory[x].hdr_xtra = getLong();
        directory[x].hdr_name_size = getWord();
        mIfs->read(directory[x].hdr_name, 36);
        directory[x].hdr_date = getLong();
        directory[x].hdr_vers = getWord();
        directory[x].hdr_flid = getWord();
        directory[x].hdr_bkup = getLong();
    }

    // The first entry in the first block of a directory is
    // always empty, scrap, unused, etc.
    uint16_t startEntry = 0;
    if (firstBlock) {
        startEntry++;
    }

    for (uint16_t x=startEntry; x < numDirEntries; x++) {

        // Don't do deleted entries.
        if ((!directory[x].hdr_flen &&
             !directory[x].hdr_flid) ||
            (!directory[x].hdr_name_size)) {
            continue;
        }

        // Do the file length.
        s << setbase(10) << setfill(' ') << setw(0)
          << "<tr>"
          << "<td class=\"right\">"
          << directory[x].hdr_flen
          << "</td>";

        // Do the Access byte.
        s << "<td class=\"right\">"
          << (directory[x].hdr_accs & 0xff)
          << "</td>";

        // Do the File type.
        s << "<td class=\"right\">"
          << (directory[x].hdr_type & 0xff)
          << "</td>";

        // Do the File data space. EXECable files only.
        s << "<td class=\"right\">"
          << directory[x].hdr_data
          << "</td>";

        // Do the Extra Information.
        s << "<td class=\"right\">$"
          << setbase(16) << setw(8) << setfill('0') << directory[x].hdr_xtra
          << setbase(10) << setfill(' ') << setw(0) << "</td>";

        // Do the Name Size.
        s << "<td class=\"right\">"
          << directory[x].hdr_name_size
          << "</td>";

        // Do the File name. Up to 36 bytes.
        s << "<td class=\"left\">";
        for (uint16_t y = 0; y < directory[x].hdr_name_size; y++) {
            s << directory[x].hdr_name[y];
        }
        s << "</td>";

        // Do the File update date.
        s << "<td class=\"right\">$"
          << setbase(16) << setw(8) << setfill('0') << directory[x].hdr_flen
          << setbase(10) << setfill(' ') << setw(0) << "</td>";

        // Do the File version number.
        s << "<td class=\"right\">"
          << directory[x].hdr_vers
          << "</td>";

        // Do the File ID. Used in the map. Zero = deleted.
        s << "<td class=\"right\">"
          << directory[x].hdr_flid
          << "</td>";

        // Do the Backup Date.
        s << "<td class=\"right\">$"
          << setbase(16) << setw(8) << setfill('0') << directory[x].hdr_flen
          << setbase(10) << setfill(' ') << setw(0) << "</td>";

        s << "</tr>" << endl;
    }

    return s.str();
}
