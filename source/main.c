/*
 * Copyright (c) 2018 Devin Smith <devin@devinsmith.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <display.h>
#include <resource.h>

static void
run_title(void)
{
  unsigned char *tbytes;

  tbytes = resource_load(RESOURCE_TITLE);
  free(tbytes);
}

int
main(int argc, char *argv[])
{
  /* XXX: We need to do a pre-flight check and make sure all the files
   * that we are going to use are in place. Maybe RM can do it. */

  if (rm_init() != 0) {
    goto done;
  }

  if (display_start() != 0) {
    goto done;
  }

  run_title();

done:
  rm_exit();
  display_end();
  return 0;
}
