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

#ifndef CSS_H
#define CSS_H

/** @file css.h
 * @brief Header file for the default HTML styling used in the HTML output file.
 */

#include <string>

using std::string;

const string eol = "\n";

string cssText =
      "<style>" + eol
    + "body {" + eol
    + "    background: ivory;" + eol
    + "    color: maroon;" + eol
    + "}" + eol
    + eol
    + "table, th, td {" + eol
    + "    border: 1px solid bisque;" + eol
    + "}" + eol
    + eol
    + "table {" + eol
    + "    border-collapse: collapse;" + eol
    + "    background: beige;" + eol
    + "    font-family: \"courier new\",\"lucida console\",mono;" + eol
    + "    font-size: smaller;" + eol
    + eol
    + "    /* If \"fixed\" is not used, the widths are ignored. Sigh */" + eol
    + "    /* So the <table> is hard coded as <table style=\"width:95%\"> which I don't like. */" + eol
    + "    table-layout: fixed;" + eol
    + eol
    + "    /* Margin is half of 100-95% to accommodate a 95% wide table. */" + eol
    + "    margin-left: 2.5%;" + eol
    + "}" + eol
    + eol
    + "th {" + eol
    + "    background: burlywood;" + eol
    + "    color: maroon;" + eol
    + "}" + eol
    + eol
    + ".right {" + eol
    + "    text-align: right;" + eol
    + "    padding-right: 3px;" + eol
    + "    vertical-align: top;" + eol
    + "}" + eol
    + eol
    + ".left {" + eol
    + "    text-align: left;" + eol
    + "    padding-left: 3px;" + eol
    + "    vertical-align: top;" + eol
    + "}"
    + eol
    + ".middle {" + eol
    + "    text-align: center;" + eol
    + "    padding-right: 3px;" + eol
    + "    vertical-align: top;" + eol
    + "}" + eol
    + ".hidden {" + eol
    + "    background: ivory;" + eol
    + "    border-left: 1px solid ivory;" + eol
    + "    border-top: 1px solid ivory;" + eol
    + "    border-bottom: 1px solid ivory;" + eol
    + "}" + eol
    + "</style>" + eol;

#endif // CSS_H

