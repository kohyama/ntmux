#!/usr/bin/env sh
#
# Send *.ml file to ocaml REPL.
#
# 
#  Example:
#
#   1. Start the ocaml with ntmux
#   
#       $ ./ntmux 5000 rlwrap ocaml
#
#   2. Open another terminal
#       $ ./send_ocaml.sh  commands.ml
#
#  So it will run in the REPL.

echo "#use \"$(readlink -f $1)\" ;;" |  nc localhost 5000
echo "Sent OK."
