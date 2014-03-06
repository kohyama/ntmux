# ntmux (Network Terminal MUltipleXer)

Ntmux executes a command and sends keystrokes from the original terminal and bytes read from TCP client sockets to the command.

It lets any commands:

* interpreters of any language (ruby, python and etc.),
* vi and
* even a shell

can be controlled from anywhere:

* vim buffers,
* emacs buffers,
* other tmux windows,
* other terminals and
* other machines

using any TCP client:

* vim script,
* emacs lisp,
* telnet,
* nc and
* tcp test tools.

## Compile and install

    % cc -o ntmux ntmux.c
    % cp ntmux ~/bin/

## Usage

% ntmux *port* *command-and-args...*

## Demo

http://www.youtube.com/embed/dKNr3rk58YI

## License

Copyright (c) 2014 Yoshinori Kohyama.  Distributed under the BSD 3-Clause License.  See the file LICENSE.md.

