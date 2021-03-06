/* -*- buffer-read-only: t -*- vi: set ro: */
/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
#line 1
/* Test changing to a directory named by a file descriptor.
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

#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  char *cwd = getcwd (NULL, 0);
  int fd = open (".", O_RDONLY);
  int i;

  ASSERT (cwd);
  ASSERT (0 <= fd);

  /* Check for failure cases.  */
  {
    int bad_fd = open ("/dev/null", O_RDONLY);
    ASSERT (0 <= bad_fd);
    errno = 0;
    ASSERT (fchdir (bad_fd) == -1);
    ASSERT (errno == ENOTDIR);
    ASSERT (close (bad_fd) == 0);
    errno = 0;
    ASSERT (fchdir (-1) == -1);
    ASSERT (errno == EBADF);
  }

  /* Repeat test twice, once in '.' and once in '..'.  */
  for (i = 0; i < 2; i++)
    {
      ASSERT (chdir (".." + 1 - i) == 0);
      ASSERT (fchdir (fd) == 0);
      {
	size_t len = strlen (cwd) + 1;
	char *new_dir = malloc (len);
	ASSERT (new_dir);
	ASSERT (getcwd (new_dir, len) == new_dir);
	ASSERT (strcmp (cwd, new_dir) == 0);
	free (new_dir);
      }

      /* For second iteration, use a cloned fd, to ensure that dup
	 remembers whether an fd was associated with a directory.  */
      if (!i)
	{
	  int new_fd = dup (fd);
	  ASSERT (0 <= new_fd);
	  ASSERT (close (fd) == 0);
	  fd = new_fd;
	}
    }

  free (cwd);
  return 0;
}
