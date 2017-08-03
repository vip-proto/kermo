#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>
#include <string.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20070509

#define YYEMPTY (-1)
#define yyclearin    (yychar = YYEMPTY)
#define yyerrok      (yyerrflag = 0)
#define YYRECOVERING (yyerrflag != 0)

extern int yyparse(void);

static int yygrowstack(void);
#define YYPREFIX "yy"
#line 2 "kermo.y"
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

#include <stdio.h>
#include <string.h>
#include "kermo.h"
/* #include "y.tab.h"*/

struct entry buffer[N];
struct luncher lunchers[L] = {
                                {
                                   .name = "native",
                                   .exe = "$EXE",
                                   .dir = "$GAME",
                                   .def = 1,
                                   .p1 = 0,
                                   .p2 = 0
                                }
                             };
struct gui_config config = {
                             .theme = 0,
                             .bg_color = 0,
                             .entry_bg_color = 0x0091bf,
                             .selected_entry_bg_color = 0xff0000,
                             .text_color = 0,
                             .selected_text_color = 0,
                             .starts_sort_factor = 0,
                             .time_sort_factor = 0,
                             .name_sort_factor = 0,
                             .position_sort_factor = 0,
                             .autoreload = 0,
                             .jpeg_logo_file = { 0 },
                           };

int n = 0;        /* Number of entries*/
int l = 1;        /* Number of lunchers*/
int err = 0;      /* Number of errors*/
int war = 0;      /* Number of warnings*/
map_t tt;         /* Temporary map*/
int t;            /* Current size of temporary map*/
char need_ussage_print = 0;

extern int yylex (void);

void _warn() {
   if (n) printf("[%-8s] Warning: ", buffer[n-1].dir);
      else printf("       Warning: ");
   war++;
}

void _warnl() {

   if (l) printf("[%-8s] Warning: ", lunchers[l-1].name);
      else printf("       Warning: ");
   war++;
}

static void fatal(char* s) {
   printf("Fatal error: %s\n", s);
   err++;
}

void yyerror(const char *str) {
   fatal((char*)str);
}

int yywrap() {
   return 1;
}

static void print_ussage() {
   printf("Buttons names:\n"
          "ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12\n"
          "CONSOLE, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, MINUS, EQUAL, BACKSPACE\n"
          "TAB, Q, W, E, R, T, Y, U, I, O, P, OPEN_SQB, CLOSE_SQB, ENTER\n"
          "CAPS_LOCK, A, S, D, F, G, H, J, K, L, SEMI, QUOTE, BACKSLASH\n"
          "L_SHIFT, Z, X, C, V, B, N, M, COMMA, POINT, SLASH, R_SHIFT\n"
          "L_CTRL, L_WIN, L_ALT, SPACE, R_ALT, R_WIN, MENU, R_CTRL\n"
          "INSERT, HOME, PGUP, DELETE, END, PGDOWN, UP, DOWN, LEFT, RIGHT\n"
          "NUM_LOCK, NUM_SLASH, NUM_MUL, NUM_MINUS\n"
          "NUM_7, NUM_8, NUM_9, NUM_PLUS\n"
          "NUM_4, NUM_5, NUM_6\n"
          "NUM_1, NUM_2, NUM_3, NUM_ENTER\n"
          "NUM_0, NUM_DEL\n"
          "MOUSE_UP, MOUSE_DOWN, MOUSE_LEFT, MOUSE_RIGHT, LMOUSE, RMOUSE\n"
          "MOUSE (interpreted as 4 mouse directions, must be the first)\n"
   );
}

static void add_entry_map(int x) {
   if ((buffer[n - 1].p1 && (x == 0)) || (buffer[n - 1].p2 && (x == 1))) {
      warnk("P%u is duplicated, newer applied!\n", x + 1);
   }

   if (t > 16) {
      warnk("P%u Too many buttons, last %u ignored!\n", x + 1, t - 16);
   }

   if (x == 0)
      memcpy(buffer[n - 1].map.p1, tt, sizeof(map_t));
   else
      memcpy(buffer[n - 1].map.p2, tt, sizeof(map_t));

   if (x == 0) {
      buffer[n - 1].p1 = 1;
      if (buffer[n - 1].p2 == 0) memcpy(buffer[n - 1].map.p2, tt, sizeof(map_t));
   } else {
      buffer[n - 1].p2 = 1;
      if (buffer[n - 1].p1 == 0) memcpy(buffer[n - 1].map.p1, tt, sizeof(map_t));
   }
}

static void add_luncher_map(int x) {
   if ((lunchers[l - 1].p1 && (x == 0)) || (lunchers[l - 1].p2 && (x == 1))) {
      warnl("P%u is duplicated, newer applied!\n", x + 1);
   }

   if (t > 16) {
      warnl("P%u Too many buttons, last %u ignored!\n", x + 1, t - 16);
   }

   if (x == 0)
      memcpy(lunchers[l - 1].map.p1, tt, sizeof(map_t));
   else
      memcpy(lunchers[l - 1].map.p2, tt, sizeof(map_t));

   if (x == 0) {
      lunchers[l-1].p1 = 1;
   } else {
      lunchers[l-1].p2 = 1;
   }
}

static void check_entry() {
   if (n) {

      if (buffer[n - 1].exe[0] == 0) {
         char tmp[128];
         sprintf(tmp, "[%s] EXE is not defined!\n", buffer[n - 1].dir);
         fatal(tmp);
      }
      if (buffer[n - 1].tit == 0) {
         warnk("TITLE is not defined, usging %s as TITLE!\n", buffer[n - 1].dir);
      }
      if (buffer[n - 1].lunc == NULL) {
         int i;
         for (i = 0; i < l; i++)
            if (lunchers[i].def) buffer[n - 1].lunc = lunchers + i;
      }
      if (!buffer[n - 1].p1 && buffer[n - 1].lunc->p1) {
         memcpy(buffer[n - 1].map.p1, buffer[n - 1].lunc->map.p1, sizeof(map_t));
         buffer[n - 1].p1 = 1;
      }
      if (!buffer[n - 1].p2 && buffer[n - 1].lunc->p2) {
         memcpy(buffer[n - 1].map.p2, buffer[n - 1].lunc->map.p2, sizeof(map_t));
         buffer[n - 1].p2 = 1;
      }
      if (!buffer[n - 1].p1 && !buffer[n - 1].p2) {
         warnk("No keymaps defined!\n");
      }
      if (buffer[n - 1].multip == -1) {
         if (buffer[n - 1].lunc->multip != -1)
            buffer[n - 1].multip = buffer[n - 1].lunc->multip;
         else
            buffer[n - 1].multip = 0;
      }
      if (buffer[n - 1].screen == -1) {
         if (buffer[n - 1].lunc->screen != -1)
            buffer[n - 1].screen = buffer[n - 1].lunc->screen;
         else
            buffer[n - 1].screen = 1;
      }
   }
}

static void add_entry(char* s) {

   if (n >= N) {
      char tmp[128];
      sprintf(tmp, "Too many entries, maximum is %u!", N);
      fatal(tmp);
   }

   if (strlen(s) > 8) {
      char tmp[128];
      snprintf(tmp, 128, "%16s EXE is too long, max is 8 characters!", s);
      fatal(tmp);
   }

   memset(&buffer[n], 0, sizeof(struct entry));
   strcpy(buffer[n].dir, s);
   strcpy(buffer[n].title, s);
   buffer[n].screen = -1;
   buffer[n].multip = -1;
   n++;
}

static void add_luncher(char* s) {
   int i;

   if (l >= L) {
      char tmp[128];
      sprintf(tmp, "Too many lunchers, maximum is %u!", L);
      fatal(tmp);
   }

   if (strlen(s) > 8) {
      char tmp[128];
      snprintf(tmp, 128, "%16s EXE is too long, max is 8 characters!", s);
      fatal(tmp);
   }

   for (i=0; i < l; i++) if (strcmp(lunchers[i].name, s) == 0) {
      char tmp[128];
      sprintf(tmp, "Luncher %s already exists!", s);
      fatal(tmp);
   }

   memset(&lunchers[l], 0, sizeof(struct luncher));
   strcpy(lunchers[l].name, s);
   lunchers[l].screen = -1;
   lunchers[l].multip = -1;
   l++;
}

void report() {
   int i;

   printf("\n\n");

   for (i = 0; i < l; i++) {
      printf("#%-8s %c %-22s %-23s %s %s\n", lunchers[i].name, lunchers[i].def ? '*':' ', lunchers[i].dir, lunchers[i].exe, lunchers[i].p1 ? "P1":"--", lunchers[i].p2?"P2":"--");
    }

   printf("\n\n");

   for (i = 0; i < n; i++) {
      printf("[%-8s] #%-8s %-37s %s %s\n", buffer[i].dir, buffer[i].lunc->name, buffer[i].title, buffer[i].p1 ? "P1":"--", buffer[i].p2?"P2":"--");
   }

   printf("\nNumber of entries: %u\nNumber of errors: %u\nNumber of warnings: %u\n\n", n, err, war);

   if (need_ussage_print) print_ussage();
}

#if defined(ALONE_TEST)

int main(int argc, char* argv[]) {

   FILE *f = NULL;

   printf("Kermo parser v2.0 - Udruzenje inzenjera i tehnicara ViP-PROTO (http://vip-proto.rs)\n\n");

   if (argc < 2) {
      printf("Warning: Input file is not specified, using \"mapper.conf\"\n");
      f = fopen("mapper.conf","rt");
   } else if (strcmp("--help", argv[1])==0) {
      printf("Ussage: kermo [file_mame]\n\n");
      print_ussage();
      return 0;
    } else f = fopen(argv[1],"rt");

   if (f) {
      yyin=f;
      yyparse();
      report();
   } else printf("Input file is not found!\n");

   return !!err;
}
#endif


#line 298 "kermo.y"
typedef union
{
   int n;
   char s[128];
   struct {
      short int r, s, t;
   } m;
} YYSTYPE;
#line 327 "y.tab.c"
#define OB 257
#define CB 258
#define TITLE 259
#define EXE 260
#define P1 261
#define P2 262
#define MOUSE 263
#define DEFAULT 264
#define LUNCHER 265
#define DIR 266
#define GUI 267
#define BGCOLOR 268
#define ENTRYCOLOR 269
#define SELCOLOR 270
#define TEXTCOLOR 271
#define SELTEXTCOLOR 272
#define LOGOJPEG 273
#define TIME_SORT_WEIGHT 274
#define STARTS_SORT_WEIGHT 275
#define TITLE_SORT_WEIGHT 276
#define POSITION_SORT_WEIGHT 277
#define AUTORELOAD 278
#define KEY 279
#define MULTIPLAYER 280
#define AUTOPICTURE 281
#define THEME 282
#define COLOR 283
#define NUMBER 284
#define STRING 285
#define PSTRING 286
#define ISTRING 287
#define LSTRING 288
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    3,    3,    6,    6,    9,    7,    8,    8,
   10,   10,   10,   10,   10,   10,   10,    4,    4,   14,
   12,   13,   13,   15,   15,   15,   15,   15,   15,   15,
   11,   17,   11,   16,   16,    1,    1,    1,    5,    5,
   18,   18,    2,    2,    2,   19,   19,   19,   19,   19,
   19,   19,   19,   19,   19,   19,
};
short yylen[] = {                                         2,
    2,    1,    2,    1,    1,    2,    0,    3,    1,    2,
    2,    2,    2,    1,    2,    1,    1,    1,    2,    0,
    3,    1,    2,    2,    2,    2,    2,    2,    1,    1,
    3,    0,    5,    1,    2,    1,    4,    5,    2,    3,
    1,    2,    1,    1,    1,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    1,
};
short yydefred[] = {                                      0,
    0,   20,    7,    0,    0,    0,    0,    0,    5,   18,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   56,    0,    0,   41,    0,    0,    0,   19,    0,    6,
   46,   47,   48,   49,   50,   51,   44,   45,   43,   52,
   53,   54,   55,    0,   42,    0,    0,    0,    0,    0,
   29,   30,    0,   22,    0,    0,    0,   14,    0,   16,
   17,    0,    9,   24,   25,    0,   26,   27,   28,   23,
   11,   12,   13,   15,   10,    0,   32,   36,   34,    0,
    0,    0,   31,   35,    0,    0,   37,    0,   33,   38,
};
short yydgoto[] = {                                       4,
   79,   40,    5,    6,    7,    8,    9,   62,   26,   63,
   67,   10,   53,   25,   54,   80,   82,   23,   24,
};
short yysindex[] = {                                   -267,
 -214,    0,    0,    0, -272, -272, -258, -258,    0,    0,
 -233, -232, -218, -217, -216, -205, -237, -237, -237, -237,
    0, -199, -199,    0, -257, -228, -272,    0, -258,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -199,    0, -204, -203, -174, -174, -201,
    0,    0, -257,    0, -200, -174, -174,    0, -198,    0,
    0, -228,    0,    0,    0, -251,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -193,    0,    0,    0, -248,
 -191, -250,    0,    0, -242, -244,    0, -169,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,   90,    0, -196,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -195,    0,    0,    0,   92,    0, -192,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -194,    0,    0,    0,    0,    0,    0,
    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,
    0, -243,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
  -69,    7,    0,   91,    0,   93,   10,    0,    0,   35,
   -8,   13,    0,    0,   45,   17,    0,   79,   -1,
};
#define YYTABLESIZE 288
short yytable[] = {                                       1,
   21,   46,   47,   48,   49,   76,   76,   50,   76,   83,
   84,   77,   76,   89,    2,   87,   84,   30,   28,    2,
    3,   45,   51,   52,   41,   42,   43,   78,   78,    3,
   78,   55,   56,   57,   78,   58,   88,   59,   30,   28,
   68,   37,   45,    8,    8,   38,   39,   72,   73,   31,
   32,   60,   61,   11,   12,   13,   14,   15,   16,   17,
   18,   19,   20,   21,   33,   34,   35,   22,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,   36,
   64,   65,   66,   69,   71,   81,   74,   85,   90,    2,
    4,    1,   39,   40,    3,   27,   75,   70,   86,   29,
   44,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   21,
};
short yycheck[] = {                                     267,
    0,  259,  260,  261,  262,  257,  257,  265,  257,  258,
   80,  263,  257,  258,  287,  258,   86,    8,    6,  287,
  288,   23,  280,  281,   18,   19,   20,  279,  279,  288,
  279,  260,  261,  262,  279,  264,  279,  266,   29,   27,
   49,  279,   44,  287,  288,  283,  284,   56,   57,  283,
  283,  280,  281,  268,  269,  270,  271,  272,  273,  274,
  275,  276,  277,  278,  283,  283,  283,  282,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  285,
  285,  285,  257,  285,  285,  279,  285,  279,  258,    0,
  287,    0,  288,  288,  287,    5,   62,   53,   82,    7,
   22,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  287,
};
#define YYFINAL 4
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 288
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"OB","CB","TITLE","EXE","P1","P2",
"MOUSE","DEFAULT","LUNCHER","DIR","GUI","BGCOLOR","ENTRYCOLOR","SELCOLOR",
"TEXTCOLOR","SELTEXTCOLOR","LOGOJPEG","TIME_SORT_WEIGHT","STARTS_SORT_WEIGHT",
"TITLE_SORT_WEIGHT","POSITION_SORT_WEIGHT","AUTORELOAD","KEY","MULTIPLAYER",
"AUTOPICTURE","THEME","COLOR","NUMBER","STRING","PSTRING","ISTRING","LSTRING",
};
char *yyrule[] = {
"$accept : program",
"program : header entries",
"program : entries",
"header : guiconfig lunchers",
"header : lunchers",
"lunchers : luncher",
"lunchers : lunchers luncher",
"$$1 :",
"luncher : LSTRING $$1 lbody",
"lbody : ldef",
"lbody : lbody ldef",
"ldef : EXE STRING",
"ldef : P1 mapa",
"ldef : P2 mapa",
"ldef : DEFAULT",
"ldef : DIR STRING",
"ldef : MULTIPLAYER",
"ldef : AUTOPICTURE",
"entries : entry",
"entries : entries entry",
"$$2 :",
"entry : ISTRING $$2 body",
"body : defin",
"body : body defin",
"defin : TITLE STRING",
"defin : EXE STRING",
"defin : P1 mapa",
"defin : P2 mapa",
"defin : LUNCHER STRING",
"defin : MULTIPLAYER",
"defin : AUTOPICTURE",
"mapa : OB array CB",
"$$3 :",
"mapa : OB MOUSE $$3 array CB",
"array : mapper",
"array : array mapper",
"mapper : KEY",
"mapper : OB KEY KEY CB",
"mapper : OB KEY KEY KEY CB",
"guiconfig : GUI cdefs",
"guiconfig : GUI THEME cdefs",
"cdefs : cdef",
"cdefs : cdefs cdef",
"number : NUMBER",
"number : KEY",
"number : COLOR",
"cdef : BGCOLOR COLOR",
"cdef : ENTRYCOLOR COLOR",
"cdef : SELCOLOR COLOR",
"cdef : TEXTCOLOR COLOR",
"cdef : SELTEXTCOLOR COLOR",
"cdef : LOGOJPEG STRING",
"cdef : TIME_SORT_WEIGHT number",
"cdef : STARTS_SORT_WEIGHT number",
"cdef : TITLE_SORT_WEIGHT number",
"cdef : POSITION_SORT_WEIGHT number",
"cdef : AUTORELOAD",
};
#endif
#if YYDEBUG
#include <stdio.h>
#endif

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 500

int      yydebug;
int      yynerrs;
int      yyerrflag;
int      yychar;
short   *yyssp;
YYSTYPE *yyvsp;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* variables for the parser stack */
static short   *yyss;
static short   *yysslim;
static YYSTYPE *yyvs;
static int      yystacksize;
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(void)
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = yyssp - yyss;
    newss = (yyss != 0)
          ? (short *)realloc(yyss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    yyss  = newss;
    yyssp = newss + i;
    newvs = (yyvs != 0)
          ? (YYSTYPE *)realloc(yyvs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse(void)
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

#ifdef lint
    goto yyerrlab;
#endif

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 7:
#line 326 "kermo.y"
{ add_luncher(yyvsp[0].s); }
break;
case 11:
#line 333 "kermo.y"
{
              if (lunchers[l - 1].exe[0]) {
                 warnk("Luncher's EXE is duplicated, newer is applied!\n");
              }
              strcpy(lunchers[l - 1].exe, yyvsp[0].s);
            }
break;
case 12:
#line 339 "kermo.y"
{ add_luncher_map(0); }
break;
case 13:
#line 340 "kermo.y"
{ add_luncher_map(1); }
break;
case 14:
#line 341 "kermo.y"
{
                 int i;
                 for (i = 0; i < l - 1; i++)
                    lunchers[i].def = 0;
                  lunchers[l-1].def = 1;
              }
break;
case 15:
#line 347 "kermo.y"
{
                   if (lunchers[l - 1].dir[0]) {
                      warnk("Luncher's DIR is duplicated, newer is applied!\n");
                   }
                   strcpy(lunchers[l - 1].dir, yyvsp[0].s);
                }
break;
case 16:
#line 353 "kermo.y"
{
                    if (lunchers[l - 1].multip != -1) {
                       warnk("Luncher's MULTIPLAYER is duplicated, newer is applied!\n");
                    }
                    lunchers[n - 1].multip = yyvsp[0].n;
                 }
break;
case 17:
#line 359 "kermo.y"
{
                    if (lunchers[l - 1].screen != -1) {
                       warnk("Luncher's AUTOPICTURE is duplicated, newer is applied!\n");
                    }
                    lunchers[n - 1].screen = yyvsp[0].n;
                 }
break;
case 20:
#line 371 "kermo.y"
{ add_entry(yyvsp[0].s); }
break;
case 21:
#line 371 "kermo.y"
{ check_entry(); }
break;
case 24:
#line 378 "kermo.y"
{
                   if (buffer[n-1].tit) {
                      warnk("TITLE is duplicated, newer is applied!\n");
                   }
                   strcpy(buffer[n - 1].title, yyvsp[0].s);
                   buffer[n - 1].tit = 1;
                }
break;
case 25:
#line 385 "kermo.y"
{
                   if (buffer[n - 1].exe[0]) {
                      warnk("EXE is duplicated, newer is applied!\n");
                   }
                   strcpy(buffer[n - 1].exe, yyvsp[0].s);
                }
break;
case 26:
#line 391 "kermo.y"
{ add_entry_map(0); }
break;
case 27:
#line 392 "kermo.y"
{ add_entry_map(1); }
break;
case 28:
#line 393 "kermo.y"
{
                       int i, b;
                       b = 1;
                       if (buffer[n - 1].lunc) {
                          warnk("LUNCHER is duplicated, newer is applied!\n");
                       }
                       for (i = 0; i < l; i++)
                          if (strcmp(lunchers[i].name, yyvsp[0].s) == 0) {
                             b = 0;
                             buffer[n - 1].lunc = lunchers + i;
                          }
                       if (b) {
                          warnk("No LUNCHER %s defined, entry is ignored!\n", yyvsp[0].s);
                       }
                    }
break;
case 29:
#line 408 "kermo.y"
{
                    if (buffer[n - 1].multip != -1) {
                       warnk("MULTIPLAYER is duplicated, newer is applied!\n");
                    }
                    buffer[n - 1].multip = yyvsp[0].n;
                 }
break;
case 30:
#line 414 "kermo.y"
{
                    if (buffer[n - 1].screen != -1) {
                       warnk("AUTOPICTURE is duplicated, newer is applied!\n");
                    }
                    buffer[n - 1].screen = yyvsp[0].n;
                 }
break;
case 32:
#line 423 "kermo.y"
{
                 t = -4;
                 memset(tt, 0, sizeof(map_t));
                 tt[0] = KEY_MOUSE_UP;
                 tt[1] = KEY_MOUSE_DOWN;
                 tt[2] = KEY_MOUSE_LEFT;
                 tt[3] = KEY_MOUSE_RIGHT;
              }
break;
case 34:
#line 433 "kermo.y"
{
             if (t < 0) {
                t = -t;
             } else {
                t = 0;
                memset(tt, 0, sizeof(map_t));
             }
             if (t < 16) {
                tt[t] = yyvsp[0].m.r;
                tt[16 + t] = yyvsp[0].m.s;
                tt[32 + t] = yyvsp[0].m.t;
             }
             t++;
          }
break;
case 35:
#line 447 "kermo.y"
{
                     if (t < 16) {
                        tt[t] = yyvsp[0].m.r;
                        tt[16 + t] = yyvsp[0].m.s;
                        tt[32 + t] = yyvsp[0].m.t;
                     }
                     t++;
                  }
break;
case 36:
#line 457 "kermo.y"
{
          yyval.m.r = yyvsp[0].n;
          yyval.m.s = 0;
          yyval.m.t = 0;
        }
break;
case 37:
#line 462 "kermo.y"
{
                             yyval.m.r = yyvsp[-2].n;
                             yyval.m.s = yyvsp[-1].n;
                             yyval.m.t = 0;
                         }
break;
case 38:
#line 467 "kermo.y"
{
                                   yyval.m.r = yyvsp[-3].n;
                                   yyval.m.s = yyvsp[-2].n;
                                   yyval.m.t = yyvsp[-1].n;
                                }
break;
case 39:
#line 473 "kermo.y"
{ config.theme = THEME_VIPPROTO; }
break;
case 40:
#line 474 "kermo.y"
{ config.theme = yyvsp[-1].n; }
break;
case 43:
#line 481 "kermo.y"
{ yyval.n = yyvsp[0].n; }
break;
case 44:
#line 482 "kermo.y"
{ yyval.n = yyvsp[0].n; }
break;
case 45:
#line 483 "kermo.y"
{ yyval.n = yyvsp[0].n; }
break;
case 46:
#line 486 "kermo.y"
{ config.bg_color = yyvsp[0].n; }
break;
case 47:
#line 487 "kermo.y"
{ config.entry_bg_color = yyvsp[0].n;}
break;
case 48:
#line 488 "kermo.y"
{ config.selected_entry_bg_color = yyvsp[0].n; }
break;
case 49:
#line 489 "kermo.y"
{ config.text_color = yyvsp[0].n; }
break;
case 50:
#line 490 "kermo.y"
{ config.selected_text_color = yyvsp[0].n; }
break;
case 51:
#line 491 "kermo.y"
{ strncpy(config.jpeg_logo_file, yyvsp[0].s, 128); }
break;
case 52:
#line 492 "kermo.y"
{ config.time_sort_factor = yyvsp[0].n; }
break;
case 53:
#line 493 "kermo.y"
{ config.starts_sort_factor = yyvsp[0].n; }
break;
case 54:
#line 494 "kermo.y"
{ config.name_sort_factor = yyvsp[0].n; }
break;
case 55:
#line 495 "kermo.y"
{ config.position_sort_factor = yyvsp[0].n; }
break;
case 56:
#line 496 "kermo.y"
{ config.autoreload = 1; }
break;
#line 1028 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    return (1);

yyaccept:
    return (0);
}
