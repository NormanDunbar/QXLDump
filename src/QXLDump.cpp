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


#include "QXLDump.h"
#include "QXDOptions.h"
#include "QXDQxlWin.h"

// Version number.
const float version = 0.01;

// Various flags set according to the passed parameters.
QXDOptions options;

int main(int argc, char *argv[])
{
    // Sign on. We use cerr as we are keeping cout for the actual dump data.
    cerr << "QXLDump: version " << version << endl << endl;

    // Make sure that cout/cerr gets comma/dot separated thousands etc.
    cout.imbue(locale(cout.getloc(), new ThousandsSeparator<char>(',')));
    cerr.imbue(locale(cerr.getloc(), new ThousandsSeparator<char>(',')));

    // Parse command line args and bale if problems detected.
    bool allOk = options.parseArgs(argc, argv);
    if (!allOk) {
        return 1;
    }

    // Show help and exit requested?
    if (options.QXDhelp()) {
        return 0;
    }

    // Did we get any options? Default to --header if not.
    if (!(options.QXDheader() ||
          options.QXDmap() ||
          options.QXDData() ||
          options.QXDFile()))
    {
        cerr << "QXLDump: No dump options supplied." << endl
             << "Defaulting to '--header'." << endl;

        options.setHeader(true);
    }

    //Process the file.
    QXDQxlWin *qxl = new QXDQxlWin(&options);
    if (qxl == NULL) {
        cerr << "QXLDump: Unable to open QXL.WIN file requested." << endl;
        return 1;
    }

    // --Header requested?
    if (options.QXDheader()) {
        qxl->doHeader();
    }


    return allOk ? 0 : 1;
}