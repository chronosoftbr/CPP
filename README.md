C++17_Filesystem Demo Program for Win10

First version

Usage:

-Using Microsoft Visual Studio Community 2019.
-Create a empty console project.
-Add the files...
-Compile...
-Do not forget: activate option c++17 in language project property.

-Create a directory preferable in C:\ (i.e. C:\Filesystem).
-Put the C++17_Filesystem.exe in the created directory.
-Open a Console and change to directory you create.
-Run it... You will see:

Enter folder name to be analized (i.e. c:\dir_x):

after...

Enter TXT result file name (i.e. c:\\Filesystem\\result.txt):

after...

Enter one Parameter:
        -d = search for duplicated files
        -p = create a filename-hash file
        -a = search for altered files

------- Example for -d option:

************* Console output *************
Searching for duplicated files in folder "c:\\duplicator" and subfolders.


                53 Files in 5 Folders. 1 Duplicated files in 2.851 seconds.
                0 Failed to open files and 0 not identified files type.

************* Result File *************
		22/5/2020	7:22

	Duplicated files in folder "c:\\duplicator" and subfolders.
"c:\\duplicator\\Tst1\\p5.sound.js"
"c:\\duplicator\\Tst2\\p5.sound.js"

------- Example for -p option:

************* Console output *************
Creating a filename-hash file...
                53 Files in 34.168 seconds.
                
************* Result File *************
		22/5/2020	7:29

"c:\\duplicator\\MP\\0080.txt" = 1b75ee0fde0e63526fde9cae1462df06f20f061da63f542e6f0fe57f28232974
"c:\\duplicator\\MP\\0081.txt" = b5ecd71c77f4fb0e41afe01ed2244b404b58a4c10885bd0f2736321fb77bfb6e
...
"c:\\duplicator\\Tst3\\abc448bits.txt" = 248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1
"c:\\duplicator\\Tst3\\abc896bits.txt" = e19c7bfa13d2d7c30cc07ce74e5e651801aacf0e8cacd037fd0e3615abfeb30a

Note: the hash algorithm used is SHA-256.

------- Example for -a option:

************* Console output *************
Enter TXT hash file to be searched (i.e. c:\myHash.txt):

Note: this file will be created by the user from result of -p option.
      The idea here is to check if selected files do not has altered by a virus, for example.
      
Example:
a0c5a7ec8fce5388d75b459e625d89ff76811c891bb41da015d91ad2de8172cf
ddf3147264288d2bda28aa901558118b09163f305c8a925326a767a7ad994657

************* Result File *************
		22/5/2020	7:43

Found 
"c:\\duplicator\\MP\\1116699131.pdf"

Found 
"c:\\duplicator\\MP\\0165.txt"


