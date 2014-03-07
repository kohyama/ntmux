/*
 *  Copyright (c) 2014 Yoshinori Kohyama.
 *  Distributed under the BSD 3-Clause License.
 *  See the file LICENSE.md.
 */
#include <sys/ioctl.h>  /* ioctl(2) */
#include <sys/select.h> /* select(2) */
#include <sys/socket.h> /* socket(2), getsockname(2), setsockopt(2),
                           bind(2), listen(2), accept(2) */
#include <sys/types.h>  /* read(2) */
#include <sys/uio.h>    /* read(2) */
#include <sys/wait.h>   /* waitpid(2) */
#include <arpa/inet.h>  /* htons(3) */
#include <err.h>        /* err(3) */
#include <errno.h>      /* EINITR */
#include <fcntl.h>      /* fcntl(2) */
#include <signal.h>     /* sigaction(2) */
#include <stdio.h>      /* BUFSIZ, perror(3) */
#include <stdlib.h>     /* system(3), atoi(3), exit(3), malloc(3), free(3) */
#include <string.h>     /* memcpy(3), strcpy(3) */
#include <termios.h>    /* tcgetattr(3) */
#include <unistd.h>     /* pipe(2), fork(2), dup2(2), close(2), execvp(3),
                           read(2), write(2)  */
#include <util.h>       /* forkpty(3) */

static void
reapchild(int signo)
{
  int status;

  while (waitpid(-1, &status, WNOHANG) > 0)
    ;
  
  if (WIFEXITED(status))
    _exit(WEXITSTATUS(status));
  _exit(3);
}

static void
server(int port, int pty)
{
  fd_set fds, tfds;
  int nc = 0; /* num of clients */
  int ss;     /* server socket */
  struct _cs {
    struct _cs *prev;
    struct _cs *next;
    int cs;
  } css = {NULL, NULL, -1}; /* client sockets */
  struct _cs *csp, *tcsp;
  int m;      /* max # of file descriters to be read */
  int i, n, yes = 1;
  struct sockaddr_in addr;
  char buf[BUFSIZ];
  socklen_t addrlen;

  if ((ss = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err(1, "socket");

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons((unsigned short)port);
  addrlen = sizeof(addr);

  if (setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
    err(1, "setsockopt");

  if (bind(ss, (struct sockaddr *)&addr, addrlen) < 0)
    err(1, "bind");

  if (getsockname(ss, (struct sockaddr *)&addr, &addrlen))
    err(1, "getsockname");

  listen(ss, 5);
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  FD_SET(pty, &fds);
  FD_SET(ss, &fds);
  for (; ; ) {
    memcpy(&tfds, &fds, sizeof(fd_set));
    m = ((pty > ss)?pty:ss);
    for (csp = css.next; csp != NULL; csp = csp->next)
      if (csp->cs > m)
        m = csp->cs;
    if (select(m + 1, &tfds, NULL, NULL, NULL) < 0)
      err(1, "select");

    /* inputs from terminal */
    if (FD_ISSET(0, &tfds)) {
      n = read(0, buf, BUFSIZ);
      write(pty, buf, n);
    }

    /* event from client sockets */
    for (csp = css.next; csp != NULL; csp = csp->next) {
      if (FD_ISSET(csp->cs, &tfds)) {
        /* some bytes */
        if ((n = read(csp->cs, buf, BUFSIZ)) > 0) {
          write(pty, buf, n);

        /* closed by client */
        } else {
          close(csp->cs);
          FD_CLR(csp->cs, &fds);
          csp->prev->next = csp->next;
          if (csp->next != NULL)
            csp->next->prev = csp->prev;
          tcsp = csp;
          csp = csp->prev;
          free(tcsp);
        }
      }
    }
 
    /* outputs from command */
    if (FD_ISSET(pty, &tfds)) {
      n = read(pty, buf, BUFSIZ);
      write(1, buf, n);
    }

    /* new client connection */
    if (FD_ISSET(ss, &tfds)) {
      csp = (struct _cs *)malloc(sizeof(struct _cs));
      if ((csp->cs = accept(ss, (struct sockaddr *)&addr, &addrlen)) < 0) {
        if (errno == EINTR)
          continue;
        err(1, "accept");
      }
      FD_SET(csp->cs, &fds);
      tcsp = css.next;
      css.next = csp;
      csp->prev = &css;
      csp->next = tcsp;
    }
  }
}

int
main(int argc, char *argv[])
{
  int port, master;
  struct sigaction sa;
  struct termios t;
  struct winsize w;

  if (argc < 3) {
    fprintf(stderr, "Usage: ntmux <port> command args ...");
    exit(1);
  }

  port = atoi(argv[1]);

  sa.sa_handler = reapchild;
  sigaction(SIGCHLD, &sa, NULL);

  tcgetattr(0, &t);
  ioctl(0, TIOCGWINSZ, &w);

  switch (forkpty(&master, NULL, &t, &w)) {
  case -1:
    err(1, "forkpty");
  case 0:
    execvp(argv[2], argv + 2);
  default:
    t.c_lflag &= ~(ECHO|ICANON);
    t.c_cc[VTIME] = 0;
    t.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &t);
    fcntl(0, F_SETFL, O_NONBLOCK);
    server(port, master);
  }
}
