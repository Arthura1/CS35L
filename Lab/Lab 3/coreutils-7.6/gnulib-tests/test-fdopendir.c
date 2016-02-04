/* -*- buffer-read-only: t -*- vi: set ro: */
/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
#line 1
/* Test opening a directory stream from a file descriptor.
   Copyright (C) 2009 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Eric Blake <ebb9@byu.net>, 2009.  */

#include <config.h>

#include <dirent.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ASSERT(expr) \
  do									     \
    {									     \
      if (!(expr))							     \
        {								     \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          fflush (stderr);						     \
          abort ();							     \
        }								     \
    }									     \
  while (0)

int
main ()
{
  DIR *d;
  int fd;

  /* A non-directory cannot be turned into a directory stream.  */
  fd = open ("/dev/null", O_RDONLY);
  ASSERT (0 <= fd);
  errno = 0;
  ASSERT (fdopendir (fd) == NULL);
  ASSERT (errno == ENOTDIR);
  ASSERT (close (fd) == 0);

  /* A bad fd cannot be turned into a stream.  */
  errno = 0;
  ASSERT (fdopendir (-1) == NULL);
  ASSERT (errno == EBADF);

  /* This should work.  */
  fd = open (".", O_RDONLY);
  ASSERT (0 <= fd);
  d = fdopendir (fd);
  /* We know that fd is now out of our reach, but it is not specified
     whether it is closed now or at the closedir.  We also can't
     guarantee whether dirfd returns fd, some other descriptor, or
     -1.  */
  ASSERT (d);
  ASSERT (closedir (d) == 0);
  /* Now we can guarantee that fd must be closed.  */
  errno = 0;
  ASSERT (dup2 (fd, fd) == -1);
  ASSERT (errno == EBADF);

  return 0;
}