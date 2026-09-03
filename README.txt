=IndexPSPArchiveEditor v1.0=

AA means Asset Archive (GRP, EVT etc)

Available encoding versions are 2.1
Higher versions are more effecient at encoding data.

=INFO=

-When replacing an image, use a BMP/PNG file, images will reduced to 256 colours.
-When replacing other file types, such as SSAD or PSC6, use a binary file
-You must source your own copy of the asset archives! They must be copied into the "OriginalArchives" folder for the program to find them.
-You can also use this program though cmd, type "ipae.exe ?" to get help with.

=WARNINGS=

-Currently, the program only supports PNG and 24-bit BMP files.
-Might not correctly modify ADV & EVT AA, it hasn't been tested.

--HOW TO USE--

Currently this program will only work through cmd. Type "ipae.exe ?" to get information on the commands this program has.

-LIST SYNTAX-

!Lists must be added to the "Lists" directory.
!Files to be replaced must be added to the ".NewFiles" directory.

Using lists and the Batch Replace (BR) command, you can... batch replace files.

Lines starting with '!' are used to change directory. This can help you organise files into subfolders.
Lines starting with '#' are comments, they don't do anything. The program will output them but that is all.
Lines starting with '=', usually at the start of a file, is used to change the number of files contained within the AA.

The lines which will actually replace files are formatted as so:
FILE_INDEX:FILE_NAME

"FILE_INDEX" is the index of the file to be replaced. if it is larger than the number of files contained within the AA,
final index = FILE_INDEX mod FILE_INDEX_MAX so the program will not crash.

"FILE_NAME" is the name of the file to replace the file contained within the AA.
.png/.bmp files will automatically be reduced to 256 colours as that is the maximum amount of colours the game supports.

if you add another ':' more data can be added however the program will not use it.

