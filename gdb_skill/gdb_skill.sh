#!/bin/sh

# Do not print the introductory and copyright messages
gdb -q
# We can set an alias in .bashrc for gdb -q as
alias gdb="gdb -q"

# Turn off the confirm message when quit the gdb
# Or add this command in .gdbinit instead
(gdb) set confirm off

# When there is lots of output and you don't want to print
# "---type <return> to continue, or q<return> to quit ---",
# you can set as
(gdb) set pagination off
# or
(gdb) set height 0
# This also can be added to .gdbinit

# We can use s(tep) command go into the function or n(ext) command to just get pass the function.
(gdb) s
(gdb) n

# We can go into the function that has no debug message.

