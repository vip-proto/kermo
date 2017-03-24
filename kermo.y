%{
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
#include "y.tab.h"

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
                             .jpeg_logo_file = { 0 },
                           };

int n = 0;        // Number of entries
int l = 1;        // Number of lunchers
int err = 0;      // Number of errors
int war = 0;      // Number of warnings
map_t tt;         // Temporary map
int t;            // Current size of temporary map
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


%}

%union
{
   int n;
   char s[128];
   struct {
      short int r, s, t;
   } m;
}

%token OB CB TITLE EXE P1 P2 MOUSE DEFAULT LUNCHER DIR GUI BGCOLOR ENTRYCOLOR SELCOLOR TEXTCOLOR SELTEXTCOLOR LOGOJPEG
%token <n> KEY MULTIPLAYER AUTOPICTURE THEME COLOR
%token <s> STRING PSTRING ISTRING LSTRING
%type <m> mapper

%%

program:
   header entries
   | entries

header:
   guiconfig lunchers
   | lunchers;

lunchers:
   luncher
   | lunchers luncher;

luncher:
   LSTRING { add_luncher($1); } lbody;

lbody:
   ldef
   | lbody ldef;

ldef:
EXE STRING {
              if (lunchers[l - 1].exe[0]) {
                 warnk("Luncher's EXE is duplicated, newer is applied!\n");
              }
              strcpy(lunchers[l - 1].exe, $2);
            }
   | P1 mapa { add_luncher_map(0); }
   | P2 mapa { add_luncher_map(1); }
   | DEFAULT {
                 int i;
                 for (i = 0; i < l - 1; i++)
                    lunchers[i].def = 0;
                  lunchers[l-1].def = 1;
              }
   | DIR STRING {
                   if (lunchers[l - 1].dir[0]) {
                      warnk("Luncher's DIR is duplicated, newer is applied!\n");
                   }
                   strcpy(lunchers[l - 1].dir, $2);
                }
   | MULTIPLAYER {
                    if (lunchers[l - 1].multip != -1) {
                       warnk("Luncher's MULTIPLAYER is duplicated, newer is applied!\n");
                    }
                    lunchers[n - 1].multip = $1;
                 }
   | AUTOPICTURE {
                    if (lunchers[l - 1].screen != -1) {
                       warnk("Luncher's AUTOPICTURE is duplicated, newer is applied!\n");
                    }
                    lunchers[n - 1].screen = $1;
                 };

entries:
   entry
   | entries entry;

entry:
   ISTRING { add_entry($1); } body { check_entry(); };

body:
   defin
   | body defin;

defin:
   TITLE STRING {
                   if (buffer[n-1].tit) {
                      warnk("TITLE is duplicated, newer is applied!\n");
                   }
                   strcpy(buffer[n - 1].title, $2);
                   buffer[n - 1].tit = 1;
                }
   | EXE STRING {
                   if (buffer[n - 1].exe[0]) {
                      warnk("EXE is duplicated, newer is applied!\n");
                   }
                   strcpy(buffer[n - 1].exe, $2);
                }
   | P1 mapa { add_entry_map(0); }
   | P2 mapa { add_entry_map(1); }
   | LUNCHER STRING {
                       int i, b;
                       b = 1;
                       if (buffer[n - 1].lunc) {
                          warnk("LUNCHER is duplicated, newer is applied!\n");
                       }
                       for (i = 0; i < l; i++)
                          if (strcmp(lunchers[i].name, $2) == 0) {
                             b = 0;
                             buffer[n - 1].lunc = lunchers + i;
                          }
                       if (b) {
                          warnk("No LUNCHER %s defined, entry is ignored!\n", $2);
                       }
                    }
   | MULTIPLAYER {
                    if (buffer[n - 1].multip != -1) {
                       warnk("MULTIPLAYER is duplicated, newer is applied!\n");
                    }
                    buffer[n - 1].multip = $1;
                 }
   | AUTOPICTURE {
                    if (buffer[n - 1].screen != -1) {
                       warnk("AUTOPICTURE is duplicated, newer is applied!\n");
                    }
                    buffer[n - 1].screen = $1;
                 };

mapa:
   OB array CB
   | OB MOUSE {
                 t = -4;
                 memset(tt, 0, sizeof(map_t));
                 tt[0] = KEY_MOUSE_UP;
                 tt[1] = KEY_MOUSE_DOWN;
                 tt[2] = KEY_MOUSE_LEFT;
                 tt[3] = KEY_MOUSE_RIGHT;
              } array CB;

array:
   mapper {
             if (t < 0) {
                t = -t;
             } else {
                t = 0;
                memset(tt, 0, sizeof(map_t));
             }
             if (t < 16) {
                tt[t] = $1.r;
                tt[16 + t] = $1.s;
                tt[32 + t] = $1.t;
             }
             t++;
          }
   | array mapper {
                     if (t < 16) {
                        tt[t] = $2.r;
                        tt[16 + t] = $2.s;
                        tt[32 + t] = $2.t;
                     }
                     t++;
                  };

mapper:
   KEY {
          $$.r = $1;
          $$.s = 0;
          $$.t = 0;
        }
   | OB KEY KEY CB {
                             $$.r = $2;
                             $$.s = $3;
                             $$.t = 0;
                         }
   | OB KEY KEY KEY CB {
                                   $$.r = $2;
                                   $$.s = $3;
                                   $$.t = $4;
                                };
guiconfig:
   GUI cdefs { config.theme = THEME_VIPPROTO; }
   | GUI THEME cdefs { config.theme = $2; };

cdefs:
   cdef
   | cdefs cdef;

cdef:
   BGCOLOR COLOR { config.bg_color = $2; }
   | ENTRYCOLOR COLOR { config.entry_bg_color = $2;}
   | SELCOLOR COLOR { config.selected_entry_bg_color = $2; }
   | TEXTCOLOR COLOR { config.text_color = $2; }
   | SELTEXTCOLOR COLOR { config.selected_text_color = $2; }
   | LOGOJPEG STRING { strncpy(config.jpeg_logo_file, $2, 128); };
%%
