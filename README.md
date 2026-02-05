# brainfart
Simple brainf*ck interpreter in c

Repo is weird because I run on Windows, Mac, and Linux
If on a unix based os, you should be able to use Makefile_l and if you're on windows, use the main Makefile.
As its not compiled and I want to keep it small, you will need to go into the c file and search for `// Windows:`. Use either the `fopen_s` line for Windows or `fopen` for other (unix based) OS's.
Compile as usual and run the program with `<filePath>` and (optionally) `<input>` field
