/******************************************************************************
  This file is part of Arcade cabinet project.

  Copyright (C) 2014 ViP-PROTO Association, http://vip-proto.com
  Aleksandar Rikalo <aleksandar.rikalo@vip-proto.rs>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307, USA.

  The GNU General Public License is contained in the file COPYING.
******************************************************************************/

#if !defined(KERMO_H)
#define KERMO_H

#include "scan_codes.h"

#define N 1024
#define L 10

typedef unsigned char map_t[48];

struct full_map {
  map_t p1;
  map_t p2;
};

struct luncher {
   char name[16];
   char exe[256];
   char dir[256];
   struct full_map map;
   char p1;
   char p2;
   char def;
   char screen;
   char multip;
};

struct entry {
   char dir[16];
   char title[128];
   char exe[32];
   struct full_map map;
   char p1;
   char p2;
   char tit;
   char screen;
   char multip;
   struct luncher* lunc;
   void* user;
};

struct gui_config {
   int theme;
   unsigned int bg_color;
   unsigned int entry_bg_color;
   unsigned int selected_entry_bg_color;
   unsigned int text_color;
   char jpeg_logo_file[128];
};

struct picture_data {
   int width;
   int height;
   int bytes_per_pixel;
   unsigned char data[];
};

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
/*** PARSER ***/
extern struct entry buffer[N];
extern struct gui_config config;
extern int n; // Number of entries
extern FILE *yyin;
extern char need_ussage_print;
extern void report();
extern void _warn();
extern int yyparse(void);

/*** LOGIC & UTILITY ***/
extern void send_reset();
extern void send_map(const struct full_map *map);
extern void run_and_wait(const struct entry *ig, int pic_width, int pic_height, int num_ss);
extern struct picture_data* read_and_resize_jpeg(const char* filename, int width, int height);
#ifdef __cplusplus
}
#endif

#define warnk _warn();printf
#define warnl _warnl();printf

#endif
