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

#ifndef QXDQXLWIN_H
#define QXDQXLWIN_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "QXDOptions.h"
#include "QXLHeader.h"


using std::ifstream;
using std::cout;
using std::cerr;
using std::setw;
using std::setfill;
using std::setbase;
using std::string;

class QXDQxlWin
{
    public:
        QXDQxlWin(QXDOptions *opt);
        ~QXDQxlWin();

        void doHeader();
        void doMap();
        void doRoot();
        void doData();
        void doFree();
        void doFile(uint16_t fileId);

    protected:

    private:
        QXDOptions *mOptions;
        ifstream *mIfs;
        QXLHeader Header;
        uint32_t *mQXLMap;
        bool QXDOpenFile();
        bool getMapBlockData(const uint16_t blockId, uint16_t &nextBlock, uint32_t &blockFileAddress);
        string getBlockChain(const uint16_t blockId);
        string getBlockChainTable(const uint16_t blockId);
        void displayData(uint16_t &offset, const uint16_t width, const uint32_t value, const string name, const string description);
        void displayOffset(uint16_t &offset, const uint16_t width, const string name);
        uint8_t getByte();
        uint16_t getWord();
        uint32_t getLong();
        void readHeader();
        void readMap();
};

#endif // QXDQXLWIN_H
