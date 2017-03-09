# QXLDump
QXLDump is a small utility to dump out, in hex, the contents of a Sinclair QL formatted QXL.WIN file - in a meaningful context, not just a pile of hex numbers.

The QXL.WIN format and many details of how it works can be found at <http://qdosmsq.dunbar-it.co.uk/doku.php?id=qdosmsq:fs:qlwa>.

The utility was developed in the Code-Blocks IDE using Embarcadero C++ version 10.1 (32 bit). Both of these are free. If you need to build the source then feel free to make changes, just remember to pass them back to me for others to use too. Thanks.

If you have a different C++ compiler, then the following might work:

```
cd SourceCode\QXLDump
CPP src\*.cpp -I include -o bin\release\QXLDump.exe
```

Where `CPP` is the command line version of your compiler.

QXLDump is a utility I needed in order to assist me check changes to qxl.win files in a forthcoming mega utility! (I've been told a million times about exaggerating!) - I need to see changes within a qxl.win file as I do different things to it in an abusive manner!

It is offered here as a potentially useful tool.

**QXLDump will never open your qxl.win file in write mode. Only read.** so your data are always safe with this utility.


# Using QXLDump.

QXLDump always outputs its results to 'stdout' - aka the screen. This can be redirected to a file if desired. Output is in HTML format, and cannot be changed.

```
QXLDump c:\qxl.win --header >qxl.win.html
```

The file ``qxl.win.html`` can now be opened in your favourite browser for inspection. Alternatively, you can use Internet Explorer!

Error messages are written to `stderr` and these too can be redirected to a separate file in the normal manner. Error output is always in plain text format.

```
QXLDump c:\qxl.win --header >qxl.win.html 2>qxl.win.errors.txt
```

As above, the output can be viewed in your browser. Errors, in file ``qxl.win.errors.txt`` can be reviewed with any text editor.

## Command line options:

If a qxl.win filename is passed, with no other parameters or options, then the equivalent of ``--root`` and ``--header`` is assumed. See below for details on these two options.

### Help

The application help messages are displayed, and the application then exits. No processing is done.

```
QXLDump --help
```

The command will display the following:

```
QXLDump: version 0.01

USAGE:

QXLDump [options] QXLWIN_file

'QXLWIN_file' is the file name of the qxl.win file to be dumped.

OPTIONS:

'--help' Displays this help, and exits.
'--header' Displays the file header.
'--map' Displays the file map.
'--root' Displays the root directory.
'--block=n' Displays a single data block.
'--free' Displays the file's free space.
'--all' Sets '--header', '--map', '--root' and '--free'.
'--dir=n --size=s' Displays the directory with FileId 'n', and file size of 's' bytes. Both are required.
'--file=n' Displays the file with FileId 'n'.

'--verbose' Displays lots more information, hex dumps etc. Best avoided!

OUTPUT:

Output is written to 'stdout' and so, can be easily redirected to a file.
Errors are written to 'stderr' and won't interfere with the data on 'stdout'.
```

### Header

The qxl.win file header is displayed. This is always the first 64 bytes of the file.

```
QXLDump c:\qxl.win --header
```

### Map

The list of blocks making up the entire qxl.win map area are dumped out. The map is used to determine where on disc data files and directories are located. The address of a block on disc is computed as the ``blockId * sectors per block * 512``.

```
QXLDump c:\qxl.win --map
```

### Root

The root directory is dumped out in readable format. Only those entries which point to valid and existing (not deleted) files and other directories are listed. Deleted entries are not shown.

If ``--verbose`` is specified, each block in the root directory is dumped out in hex and a list of all the blocks used by the root directory, in the map, are displayed.

```
QXLDump c:\qxl.win --root
```

or

```
QXLDump c:\qxl.win --root --verbose
```

### Block

A single block is dumped out in hex. The block number must be specified on the command line in decimal. ``--verbose`` has no effect here.

```
QXLDump c:\qxl.win --block=1234
```

### Free

The free space list is very similar to the map in that it dumps out the map blocks making up the free space chain. ``--verbose`` has no effect here.

```
QXLDump c:\qxl.win --free
```

### Directory

A dump of the required directory blocks is carried out for the passed directory. The directory required must be specified by it's block number, not a name. ``--verbose`` will dump out the directory blocks in hex, if specified.

The directory contents are displayed in a readable format.

The size of the directory *must* be supplied. This is to stop the utility trying to read past the logical EOF and into the uncharted and shark infested waters of random garbage which can cause no end of trouble to an innocent dump utility! (Ask me how I know!)

You obtain the directory id number and size from a listing of its parent directory, which could be the root.

```
QXLDump c:\qxl.win --directory=61 --size=1024
```

or

```
QXLDump c:\qxl.win --directory=61 --size=1024 --verbose
```

### File

A dump of the required file's blocks is carried out for the passed fine. The file required must be specified by it's block number, not a name. ``--verbose`` will dump out the file's data  blocks in hex, if specified.

You obtain the file id number from a listing of its parent directory, which could be the root.

```
QXLDump c:\qxl.win --file=27
```

### All

Using this option turns on ``--header``, ``--map``, ``--root`` and ``--free``. Using ``--verbose`` will have the effect described for each individual option, as appropriate.

### Verbose

This option modifies the output of the others. Normally it will cause a dump of the block numbers, from the map, that make up a directory or file's structure on disc. In addition, it will cause a hex dump of the blocks making up a file or directory, in addition to whatever the normal option outputs.
