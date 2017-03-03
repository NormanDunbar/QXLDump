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

#include "QXDOptions.h"


QXDOptions::QXDOptions()
{
    mHelp = false;
    mHtml = true;
    mHeader = false;
    mMap = false;
    mRoot = false;
    mData = false;
    mFree = false;
    mFile = false;
    mFileId = 0;
    mQXLFile = "";
}

QXDOptions::~QXDOptions()
{
    // Nothing to do here!
}


bool QXDOptions::parseArgs(int argc, char *argv[]) {

    bool invalidArgs = false;
    bool gotFileAlready = false;

    if (argc < 2) {
        // Insufficient args.
        cerr << "QXLDump: No arguments supplied." << endl;
        return false;
    }

    for (int arg = 1; arg < argc; arg++) {
        // Convert args to lower case and to strings.
        // Not really safe with Unicode though!
        string thisArg = string(argv[arg]);
        for (int c = 0; c < thisArg.length(); c++) {
            thisArg[c] = tolower(thisArg[c]);
        }

        // Try ALL ...
        if ((thisArg == "--all") ||
            (thisArg == "-a")) {
            mHeader = true;
            mMap = true;
            mData = true;
            mRoot = true;
            mFree = true;
            continue;
        }

        // Try HEADER ...
        if ((thisArg == "--header") ||
            (thisArg == "-h")) {
            mHeader = true;
            continue;
        }

        // Try MAP ...
        if ((thisArg == "--map") ||
            (thisArg == "-m")) {
            mMap = true;
            continue;
        }

        // Try ROOT ...
        if ((thisArg == "--root") ||
            (thisArg == "-r")) {
            mRoot = true;
            continue;
        }

        // Try DATA ...
        if ((thisArg == "--data") ||
            (thisArg == "-d")) {
            mData = true;
            continue;
        }

        // Try FREE ...
        if ((thisArg == "--free") ||
            (thisArg == "-e")) {
            mData = true;
            continue;
        }

        // Try FILE=n ...
        if ((thisArg.substr(0, 6) == "--file") ||
            (thisArg.substr(0, 2) == "-f")) {
            bool fileOK = true;
            unsigned temp = getDigits(thisArg, 7, &fileOK);
            if (fileOK) {
                mFileId = temp;
                mFile = true;
            }

            continue;
        }

        // Try F=n ...
        if (thisArg.substr(0, 2) == "-f") {
            bool fileOK = true;
            unsigned temp = getDigits(thisArg, 3, &fileOK);
            if (fileOK) {
                mFileId = temp;
                mFile = true;
            }

            continue;
        }

        // Nope? Try help then ...
        if ((thisArg == "--help") ||
            (thisArg == "-?")) {
            mHelp = true;
            continue;
        }

        // Either a filename or an error.
        // Try for an error ...
        if (thisArg[0] == '-') {
            cerr << "QXLDump: Invalid argument '";
            cerr << string(argv[arg]) << "'." << endl;
            invalidArgs = true;
            continue;
        }

        // Must (!) be a filename.
        // Do not lowercase it as we are possibly on Linux via WINE!
        // And we only want a single QXLWIN file.
        if (!gotFileAlready) {
            mQXLFile = string(argv[arg]);
            if (fileExists(mQXLFile)) {
                gotFileAlready = true;
            } else {
                cerr << "QXLDump: File '" << mQXLFile << "' not found." << endl;
                invalidArgs = true;
            }
        } else {
            // Too many QXLWIN files? Spaces around '=' in --file=/-f=?
            cerr << "QXLDump: Parameter error. (" << argv[arg] << ")." << endl;
            invalidArgs = true;
        }

    }

    // Did we just want help?
    // Do it and bale out.
    if (mHelp) {
        usage();
        return true;
    }

    // We need at least a trace file.
    if (mQXLFile.empty()) {
        cerr << "QXLDump: No trace file supplied." << endl;
        invalidArgs = true;
    }

    // Did we barf?
    if (invalidArgs) {
        usage();
        return false;
    }

    return true;
}

uint32_t QXDOptions::getDigits(const string &lookHere, const unsigned pos, bool *ok) {

    *ok = true;     // Assume all ok.

    try {
        return (uint32_t)stoul(lookHere.substr(pos), NULL, 10);
    } catch (...) {
        *ok = false;
        return 0;
    }
}

bool QXDOptions::fileExists(const string &fullPath) {

    ifstream *testFile = new ifstream(fullPath);
    if (testFile->good()) {
        // File exists. Hooray!
        testFile->close();
        delete testFile;
        return true;
    } else {
        // File is missing.
        return false;
    }
}
void QXDOptions::usage() {

    cerr << endl << "USAGE:" << endl << endl;
    cerr << "QXLDump [options] QXLWIN_file" << endl << endl;
    cerr << "'QXLWIN_file' is the file name of the qxl.win file to be dumped." << endl << endl;

    cerr << "OPTIONS:" << endl << endl;
    cerr << "'-?' or '--help' Displays this help, and exits." << endl;
    cerr << "'-h' or '--header' Displays the file header." << endl;
    cerr << "'-m' or '--map' Displays the file map." << endl;
    cerr << "'-r' or '--root' Displays the root directory." << endl;
    cerr << "'-d' or '--data' Displays the file's (used) data." << endl;
    cerr << "'-e' or '--free' Displays the file's free space." << endl;
    cerr << "'-a' or '--all' Sets '--header', '--map', '--root', '--data' amd '--free'." << endl;
    cerr << "'-f=n' or '--file=n' Displays the file with FileId 'n'." << endl << endl;

    cerr << "OUTPUT:" << endl << endl;
    cerr << "Output is written to 'stdout' and so, can be easily redirected to a file." << endl;
    cerr << "Errors etc are written to 'stderr' and won't interfere with the data on 'stdout'." << endl;
}




