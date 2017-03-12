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

#ifndef QXDOPTIONS_H
#define QXDOPTIONS_H

#include <string>
#include <iostream>
#include <fstream>

#if defined (__GNUC__)
	#include <stdint.h>
#endif

using std::string;
using std::cerr;
using std::endl;
using std::ifstream;

class QXDOptions
{
    public:
        QXDOptions();
        ~QXDOptions();

        // Getters.
        // Return HTML mode.
        bool QXDHtml() { return mHtml; }

        // Return HELP mode.
        bool QXDhelp() { return mHelp; }

        // Are we dumping the header?
        bool QXDheader() { return mHeader; }

        // Are we dumping the map?
        bool QXDmap() { return mMap; }

        // Are we dumping the root directory?
        bool QXDRoot() { return mRoot; }

        // Are we dumping the data blocks?
        bool QXDDataBlock() { return mDataBlock; }

        // Are we dumping the free space?
        bool QXDFree() { return mFree; }

        // Are we dumping a single file or directory?
        bool QXDFile() { return mFile; }
        bool QXDDirectory() { return mDirectory; }

        // Are we in verbose mode?
        bool QXDVerbose() { return mVerbose; }

        // Which file are we dumping?
        uint16_t QXDFileId() { return mFileId; }

        // Which block are we dumping?
        uint16_t QXDBlockId() { return mBlockId; }

        // Which directory are we dumping?
        uint16_t QXDDirId() { return mDirId; }
        uint32_t QXDDirLength() { return mDirLength; }

        // What is the name of the QXL.WIN file?
        string QXDQxlFile() { return mQXLFile; }

        // Setters.
        void setHeader( bool val) { mHeader = val; mRoot = val; }

        // Others.
        // How to call the utility.
        void usage();

        // Parse the command line arguments.
        bool parseArgs(int argc, char *argv[]);

    protected:

    private:

        bool mHtml;
        bool mHelp;
        bool mHeader;
        bool mMap;
        bool mRoot;
        bool mDataBlock;
        bool mFree;
        bool mFile;
        bool mDirectory;
        uint16_t mFileId;
        uint16_t mDirId;
        uint16_t mBlockId;
        uint32_t mDirLength;
        bool mSizeOk;
        bool mVerbose;

        uint32_t getDigits(const string &lookHere, const unsigned pos, bool *ok);
        bool fileExists(string fullPath);

        string mQXLFile;

};

#endif // QXDOPTIONS_H
