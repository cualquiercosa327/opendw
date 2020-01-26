/*
 * Copyright (c) 2020 Devin Smith <devin@devinsmith.net>
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

#include "display.h"
#include "offsets.h"
#include "tables.h"
#include "viewport.h"
#include "utils.h"

struct viewport_data {
  int xpos;
  int ypos;
  int runlength;
  int numruns;
  int unknown1;
  int unknown2;
  unsigned char *data;
};

unsigned char viewport_tl[] = {
  0xAA, 0xAA, 0xAA, 0x06, // 0x00 (0x675C-0x675F)
  0xAA, 0xAA, 0xAA, 0x06, // 0x01 (0x6760-0x6763)
  0xAA, 0xAE, 0xE2, 0x06, // 0x02 (0x6764-0x6767)
  0x00, 0x00, 0x00, 0x66, // 0x03 (0x6768-0x676B)
  0x22, 0x22, 0x20, 0x66, // 0x04 (0x676C-0x676F)
  0x0E, 0xAA, 0xA0, 0x66, // 0x05 (0x6770-0x6773)
  0x0E, 0xAE, 0x06, 0x66, // 0x06 (0x6774-0x6777)
  0x0E, 0xE0, 0x66, 0x66, // 0x07 (0x6778-0x677B)
  0x0A, 0x06, 0x66, 0x66, // 0x08 (0x677C-0x677F)
  0x06, 0x66, 0x66, 0x66  // 0x09 (0x6780-0x6783)
};

unsigned char viewport_tr[] = {
  0x60, 0xAA, 0xAE, 0xEA, // 0x00 (0x6788-0x678B)
  0x60, 0xAE, 0xEE, 0xAE, // 0x01 (0x678C-0x678F)
  0x60, 0x2E, 0xEE, 0xE2, // 0x02 (0x6790-0x6793)
  0x66, 0x00, 0x00, 0x00, // 0x03 (0x6794-0x6797)
  0x66, 0x02, 0x22, 0x22, // 0x04 (0x6798-0x679B)
  0x66, 0x02, 0xEE, 0xE0, // 0x05 (0x679C-0x679F)
  0x66, 0x60, 0x2E, 0xE0, // 0x06 (0x67A0-0x67A3)
  0x66, 0x66, 0x62, 0xE0, // 0x07 (0x67A4-0x67A7)
  0x66, 0x66, 0x66, 0xA0, // 0x08 (0x67A8-0x67AB)
  0x66, 0x66, 0x66, 0x60  // 0x09 (0x67AC-0x67AF)
};

unsigned char viewport_bl[] = {
  0x00, 0x06, 0x66, 0x66, // 0x00 (0x67B4-0x67B7)
  0xAE, 0xE0, 0x06, 0x66, // 0x01 (0x67B8-0x67BB)
  0xAA, 0xEA, 0xE0, 0x66, // 0x02 (0x67BC-0x67BF)
  0xAE, 0xAE, 0x2E, 0x06, // 0x03 (0x67C0-0x67C3)
  0xAE, 0xE2, 0x2A, 0x06, // 0x04 (0x67C4-0x67C7)
  0x22, 0x22, 0x2A, 0x06, // 0x05 (0x67C8-0x67CB)
  0x22, 0x22, 0x2A, 0x06, // 0x06 (0x67CC-0x67CF)
  0x22, 0x22, 0x2A, 0x06, // 0x07 (0x67D0-0x67D3)
  0x22, 0x22, 0x2A, 0x06, // 0x08 (0x67D4-0x67D7)
  0x22, 0x22, 0x2A, 0x06, // 0x09 (0x67D8-0x67DB)
  0x22, 0x22, 0x2A, 0x06, // 0x0A (0x67DC-0x67DF)
  0x22, 0x22, 0x20, 0x06, // 0x0B (0x67E0-0x67E3)
  0x22, 0x22, 0x00, 0x96  // 0x0C (0x67E4-0x67E7)
};

unsigned char viewport_br[] = {
  0x66, 0x66, 0x60, 0x00, // 0x00 (0x67EC-0x67EF)
  0x66, 0x60, 0x0E, 0xEA, // 0x01 (0x67F0-0x67F3)
  0x66, 0x0E, 0xAE, 0xAA, // 0x02 (0x67F4-0x67F7)
  0x60, 0xE2, 0xEA, 0xEA, // 0x03 (0x67F8-0x67FB)
  0x60, 0xA2, 0x2E, 0xEA, // 0x04 (0x67FC-0x67FF)
  0x60, 0xA2, 0x22, 0x22, // 0x05 (0x6800-0x6803)
  0x60, 0xA2, 0x22, 0x22, // 0x06 (0x6804-0x6807)
  0x60, 0xA2, 0x22, 0x2A, // 0x07 (0x6808-0x680B)
  0x60, 0xA2, 0x22, 0x2A, // 0x08 (0x680C-0x680F)
  0x60, 0xA2, 0x22, 0x2A, // 0x09 (0x6810-0x6813)
  0x60, 0xA2, 0x22, 0x2A, // 0x0A (0x6814-0x6817)
  0x00, 0x02, 0x22, 0x2A, // 0x0B (0x6818-0x681B)
  0x91, 0x00, 0x02, 0x2A, // 0x0C (0x681C-0x681F)
};

struct viewport_data viewports[] = {
  {
    0x00, 0x00, 0x04, 0x0A, 0x00, 0x00,
    viewport_tl
  },
  {
    0x98, 0x00, 0x04, 0x0A, 0x00, 0x00,
    viewport_tr
  },
  {
    0x00, 0x7B, 0x04, 0x0D, 0x00, 0x00,
    viewport_bl
  },
  {
    0x98, 0x7B, 0x04, 0x0D, 0x00, 0x00,
    viewport_br
  }
};

/* D88 */
static void process_quadrant(const struct viewport_data *d, unsigned char *data)
{
  int newx, newy;
  uint16_t offset;

  newx = d->xpos >> 1;
  newy = d->ypos << 1;
  printf("%02x %02x, %02x, %02x\n", d->xpos, d->ypos, newx, newy);
  offset = get_offset(d->ypos);
  offset += newx;
  printf("Offset: %04x\n", offset);
  unsigned char *p = data + offset;
  unsigned char *q = d->data;
  for (int i = 0; i < d->numruns; i++) {
    for (int j = 0; j < d->runlength; j++) {
      unsigned char val = *q;
      unsigned char dval = *p;

      dval = dval & get_and_table(val);
      dval = dval | get_or_table(val);

      printf("(%02x %02x) ", dval, val);
      *p = dval;
      p++;
      q++;
    }
    offset += 0x50;
    p = data + offset;
    printf("\n");
  }
}

void draw_viewport()
{
  unsigned char *data = calloc(sizeof(unsigned char), 0x50 * 0x88);

  /* Iterate backwards like DW does */
  int vidx = 3;
  while (vidx >= 0) {
    const struct viewport_data *p = &viewports[vidx];
    process_quadrant(p, data);
    vidx--;
  }

#if 0
  // 0x88 x 0x50
  dump_hex(data, 0x2a80);
  uint8_t al;
  unsigned char *src = data;
  int i, counter = 0x2a80 + 0xA10;
  int hi, lo;

  for (i = 0xA10; i < counter; i += 2) {

    al = *src++;

    /* Each nibble represents a color */
    /* for example 0x82 represents color 8 then color 2. */
    hi = (al >> 4) & 0xf;
    lo = al & 0xf;

    framebuffer[i] = vga_palette[hi];
    framebuffer[i + 1] = vga_palette[lo];
  }

  display_update();
#endif

  free(data);
}
