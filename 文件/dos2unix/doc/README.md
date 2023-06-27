# dos2unix
`dos2unix` is a Win32 tool which can:

* Convert files from Windows (CRLF) to Unix (LF) (with backup option).
* Convert files from Unix (LF) to Windows (CRLF) (with backup option).
* Check the line endings of a file.
* It can perform the operations mentioned above recursively on directories.
* Integration with the Windows shell (so it is possible to right click on a file or directory and perform one of the operations).


## Installation

To install the program, open as administrator a MS-DOS window and execute:
```
dos2unix install <filename>
```

where `<filename>` is a file containing file extensions, e.g.
```
C:\> type extensions.txt
h
c
cpp
```


The installer makes the following changes in the registry:

1. For each already registered file extension:
  * Add: `HKEY_CLASSES_ROOT\<filetype>\shell\LineEndings\`
2. For each non-registered file extension:
  * Add: `HKEY_CLASSES_ROOT\.<extension>\`
  * Add: `HKEY_CLASSES_ROOT\<extension>lineendings\`
* Add `HKEY_CLASSES_ROOT\Folder\shell\LineEndings\`.
* Add `HKEY_LOCAL_MACHINE\SOFTWARE\dos2unix`.


Only the registered file extensions will be taken into account.

Once installed, you can right-click on a directory or a file with one of the chosen file extensions and you will get the cascade menu `Line endings`.



## Uninstallation

To uninstall the program, open as administrator a MS-DOS window and execute:
```
dos2unix uninstall
```
