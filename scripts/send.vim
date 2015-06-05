" Send selected text to OCAML
"
"   Usage:
"   
"   $ vim file.ml
"   Select the text in visual mode and then enter ":'<,'>call Send()
"
"   the selection will be sent to: $ ntmux ocaml
"
"   To Load This File: copy it to ~/.vimrc or :source send.vim
"
function! SendML() range
    let savedreg=@"
    silent execute a:firstline.",".a:lastline."yank"
    python import vim, subprocess
    python p=subprocess.Popen(["nc localhost 5000"], shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    python p.stdin.write(vim.eval('@"') + "\n;;\n")
    let @"=savedreg
    python p.stdin.close()
    python retstatus=p.poll()
    python print p.stdout.read()
endfunction
