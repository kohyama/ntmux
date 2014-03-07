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

## Disclaimer

Currently tested only under

    % uname -pv
    Darwin Kernel Version 13.1.0: Thu Jan 16 19:40:37 PST 2014; root:xnu-2422.90.20~2/RELEASE_X86_64 i386
    % cc --version
    Apple LLVM version 5.0 (clang-500.2.79) (based on LLVM 3.3svn)
    Target: x86_64-apple-darwin13.1.0
    Thread model: posix

## Demo

<a href="https://www.youtube.com/watch?v=dKNr3rk58YI">![ss 2014-03-06 21.38.02.png](https://qiita-image-store.s3.amazonaws.com/0/12593/20ebc62f-a085-d708-143b-c49fde705802.png "ss 2014-03-06 21.38.02.png")</a>

## License

Copyright (c) 2014 Yoshinori Kohyama.  Distributed under the BSD 3-Clause License.  See the file LICENSE.md.

