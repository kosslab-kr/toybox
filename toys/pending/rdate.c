/* rdate.c - Get time from remote host or set its time if allowed
 *
 * Copyright 2016 Backsun Sim <zuidrijk@gmail.com>
 *

USE_RDATE(NEWTOY(rdate, "ps", TOYFLAG_USR|TOYFLAG_BIN))

config RDATE
  bool "rdate"
  default n
  help
    usage: rdate -p HOSTNAME
    -p HOSTNAME: Get and print time from host
    -s HOSTNAME: Set current system time to server's time (if allowed) [TBD]
*/

#define FOR_rdate
#include "toys.h"

time_t get_date_remote(const char* hostname)
{
  int fd;
  char *host, *port;
  uint32_t rtime;

  host = strtok(hostname, ":");
  port = strtok(NULL, ":");

  if (port == NULL)
    port = "37";

  fd = xsocket(AF_INET, SOCK_STREAM, 0);
  xconnect(host, port, AF_INET, SOCK_STREAM, 0, 0);
  read(fd, &rtime, 4);
  // TODO: Timeout should be implemented
  close(fd);

  return ntohl(rtime) - 2208988800UL;
}

void rdate_main(void)
{
  time_t rtime_buf;

  if (toys.optflags & FLAG_s)
     help_exit("this feature is still under development");
  else if (!(toys.optflags & FLAG_p))
     help_exit("no hostname");

  rtime_buf = get_date_remote(toys.optargs[0]);
  xprintf("%s", ctime(&rtime_buf));
}

