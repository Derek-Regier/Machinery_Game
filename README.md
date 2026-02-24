# Machinery_Game

Instructions on how to use .mak files

There are three test makefiles.

tst_rast.mak

tst_bit.mak

tst_font.mak

---- 

In order to make these tests one at a time in the same folder use this command in GULAM:

make -f *filename*

e.g.

make -f tst_rast.mak

make -f tst_bit.mak

make -f tst_font.mak

-----

the outputs that can be run are:

tst_rast.prg

tst_bit.prg

tst_font.prg 

-----

A fast way to delete output and program files is to use del.sh

Open git_bash within this directory 

then run

./del.sh

This will remove all ".prg" and ".o"
