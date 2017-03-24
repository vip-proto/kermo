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

#include "kermo.h"
#include "y.tab.h"
#include <stdio.h>

%}
%%
[{]           return OB;
[}]           return CB;
TITLE         return TITLE;
EXE           return EXE;
COM           return EXE;
BAT           return EXE;
ELF           return EXE;
P1            return P1;
P2            return P2;
MOUSE         return MOUSE;
DEFAULT       return DEFAULT;
LUNCHER       return LUNCHER;
DIR           return DIR;
"$GUI"        return GUI;
BGCOLOR       return BGCOLOR;
ENTRYCOLOR    return ENTRYCOLOR;
SELCOLOR      return SELCOLOR;
TEXTCOLOR     return TEXTCOLOR;
SELTEXTCOLOR  return SELTEXTCOLOR;
LOGOJPEG      return LOGOJPEG;
VIPPROTO      { yylval.n = THEME_VIPPROTO; return THEME; }
CALLIDUSCLOUD { yylval.n = THEME_CALLIDUSCLOUD; return THEME; }
MULTIPLAYER   { yylval.n = 1; return MULTIPLAYER; }
SINGLEPLAYER  { yylval.n = 0; return MULTIPLAYER; }
AUTOPICTURE   { yylval.n = 1; return AUTOPICTURE; }
MANUALPICTURE { yylval.n = 0; return AUTOPICTURE; }
[\*]          { yylval.n = KEY_NONE; return KEY; }
MOUSE_UP      { yylval.n = KEY_MOUSE_UP; return KEY; }
MOUSE_DOWN    { yylval.n = KEY_MOUSE_DOWN; return KEY; }
MOUSE_LEFT    { yylval.n = KEY_MOUSE_LEFT; return KEY; }
MOUSE_RIGHT   { yylval.n = KEY_MOUSE_RIGHT; return KEY; }
LMOUSE        { yylval.n = KEY_LMOUSE; return KEY; }
MMOUSE        { yylval.n = KEY_MMOUSE; return KEY; }
RMOUSE        { yylval.n = KEY_RMOUSE; return KEY; }
ESC           { yylval.n = KEY_ESC; return KEY; }
F1            { yylval.n = KEY_F1; return KEY; }
F2            { yylval.n = KEY_F2; return KEY; }
F3            { yylval.n = KEY_F3; return KEY; }
F4            { yylval.n = KEY_F4; return KEY; }
F5            { yylval.n = KEY_F5; return KEY; }
F6            { yylval.n = KEY_F6; return KEY; }
F7            { yylval.n = KEY_F7; return KEY; }
F8            { yylval.n = KEY_F8; return KEY; }
F9            { yylval.n = KEY_F9; return KEY; }
F10           { yylval.n = KEY_F10; return KEY; }
F11           { yylval.n = KEY_F11; return KEY; }
F12           { yylval.n = KEY_F12; return KEY; }
CONSOLE       { yylval.n = KEY_CONSOLE; return KEY; }
1             { yylval.n = KEY_1; return KEY; }
2             { yylval.n = KEY_2; return KEY; }
3             { yylval.n = KEY_3; return KEY; }
4             { yylval.n = KEY_4; return KEY; }
5             { yylval.n = KEY_5; return KEY; }
6             { yylval.n = KEY_6; return KEY; }
7             { yylval.n = KEY_7; return KEY; }
8             { yylval.n = KEY_8; return KEY; }
9             { yylval.n = KEY_9; return KEY; }
0             { yylval.n = KEY_0; return KEY; }
MINUS         { yylval.n = KEY_MINUS; return KEY; }
EQUAL         { yylval.n = KEY_EQUAL; return KEY; }
BACKSPACE     { yylval.n = KEY_BACKSPACE; return KEY; }
TAB           { yylval.n = KEY_TAB; return KEY; }
Q             { yylval.n = KEY_Q; return KEY; }
W             { yylval.n = KEY_W; return KEY; }
E             { yylval.n = KEY_E; return KEY; }
R             { yylval.n = KEY_R; return KEY; }
T             { yylval.n = KEY_T; return KEY; }
Y             { yylval.n = KEY_Y; return KEY; }
U             { yylval.n = KEY_U; return KEY; }
I             { yylval.n = KEY_I; return KEY; }
O             { yylval.n = KEY_O; return KEY; }
P             { yylval.n = KEY_P; return KEY; }
OPEN_SQB      { yylval.n = KEY_OPEN_SQB; return KEY; }
CLOSE_SQB     { yylval.n = KEY_CLOSE_SQB; return KEY; }
ENTER         { yylval.n = KEY_ENTER; return KEY; }
CAPS_LOCK     { yylval.n = KEY_CAPS_LOCK; return KEY; }
A             { yylval.n = KEY_A; return KEY; }
S             { yylval.n = KEY_S; return KEY; }
D             { yylval.n = KEY_D; return KEY; }
F             { yylval.n = KEY_F; return KEY; }
G             { yylval.n = KEY_G; return KEY; }
H             { yylval.n = KEY_H; return KEY; }
J             { yylval.n = KEY_J; return KEY; }
K             { yylval.n = KEY_K; return KEY; }
L             { yylval.n = KEY_L; return KEY; }
SEMI          { yylval.n = KEY_SEMI; return KEY; }
QUOTE         { yylval.n = KEY_QUOTE; return KEY; }
L_SHIFT       { yylval.n = KEY_L_SHIFT; return KEY; }
R_SHIFT       { yylval.n = KEY_R_SHIFT; return KEY; }
Z             { yylval.n = KEY_Z; return KEY; }
X             { yylval.n = KEY_X; return KEY; }
C             { yylval.n = KEY_C; return KEY; }
V             { yylval.n = KEY_V; return KEY; }
B             { yylval.n = KEY_B; return KEY; }
N             { yylval.n = KEY_N; return KEY; }
M             { yylval.n = KEY_M; return KEY; }
COMMA         { yylval.n = KEY_COMMA; return KEY; }
POINT         { yylval.n = KEY_POINT; return KEY; }
SLASH         { yylval.n = KEY_SLASH; return KEY; }
BACKSLASH     { yylval.n = KEY_BACKSLASH; return KEY; }
L_CTRL        { yylval.n = KEY_L_CTRL; return KEY; }
L_WIN         { yylval.n = KEY_L_WIN; return KEY; }
L_ALT         { yylval.n = KEY_L_ALT; return KEY; }
SPACE         { yylval.n = KEY_SPACE; return KEY; }
R_ALT         { yylval.n = KEY_R_ALT; return KEY; }
R_WIN         { yylval.n = KEY_R_WIN; return KEY; }
MENU          { yylval.n = KEY_MENU; return KEY; }
R_CTRL        { yylval.n = KEY_R_CTRL; return KEY; }
INSERT        { yylval.n = KEY_INSERT; return KEY; }
HOME          { yylval.n = KEY_HOME; return KEY; }
PGUP          { yylval.n = KEY_PGUP; return KEY; }
DELETE        { yylval.n = KEY_DELETE; return KEY; }
END           { yylval.n = KEY_END; return KEY; }
PGDOWN        { yylval.n = KEY_PGDOWN; return KEY; }
UP            { yylval.n = KEY_UP; return KEY; }
DOWN          { yylval.n = KEY_DOWN; return KEY; }
LEFT          { yylval.n = KEY_LEFT; return KEY; }
RIGHT         { yylval.n = KEY_RIGHT; return KEY; }
NUM_LOCK      { yylval.n = KEY_NUM_LOCK; return KEY; }
NUM_SLASH     { yylval.n = KEY_NUM_SLASH; return KEY; }
NUM_MUL       { yylval.n = KEY_NUM_MUL; return KEY; }
NUM_MINUS     { yylval.n = KEY_NUM_MINUS; return KEY; }
NUM_7         { yylval.n = KEY_NUM_7; return KEY; }
NUM_8         { yylval.n = KEY_NUM_8; return KEY; }
NUM_9         { yylval.n = KEY_NUM_9; return KEY; }
NUM_PLUS      { yylval.n = KEY_NUM_PLUS; return KEY; }
NUM_4         { yylval.n = KEY_NUM_4; return KEY; }
NUM_5         { yylval.n = KEY_NUM_5; return KEY; }
NUM_6         { yylval.n = KEY_NUM_6; return KEY; }
NUM_1         { yylval.n = KEY_NUM_1; return KEY; }
NUM_2         { yylval.n = KEY_NUM_2; return KEY; }
NUM_3         { yylval.n = KEY_NUM_3; return KEY; }
NUM_ENTER     { yylval.n = KEY_NUM_ENTER; return KEY; }
NUM_0         { yylval.n = KEY_NUM_0; return KEY; }
NUM_DEL       { yylval.n = KEY_NUM_DEL; return KEY; }
"0x"[0-9abcdefABCDEF]+ { sscanf(yytext + 2, "%x", &yylval.n); return COLOR; }

"\""[^"\""\n\t]*"\"" {
                        int i;
                        yylval.s[0] = 0;
                        for (i = 0; yytext[i]; yylval.s[++i] = 0)
                           if (i > 0) yylval.s[i - 1] = yytext[i];
                        if (i > 1) yylval.s[i - 2] = 0;
                        return STRING;
                     }

"'"[^"'"\n\t]*"'"    {
                        int i;
                        yylval.s[0] = 0;
                        for (i = 0;yytext[i]; yylval.s[++i] = 0)
                           if (i > 0) yylval.s[i - 1]=yytext[i];
                        if (i > 1) yylval.s[i - 2] = 0;
                        return PSTRING;
                     }

"\["[^"\""\n\t]*"\]" {
                        int i;
                        yylval.s[0] = 0;
                        for (i = 0; yytext[i]; yylval.s[++i] = 0)
                           if (i > 0) yylval.s[i - 1] = yytext[i];
                        if (i > 1) yylval.s[i - 2] = 0;
                        return ISTRING;
                     }

"#"[^"\""\n\t]*      {
                        int i;
                        yylval.s[0] = 0;
                        for (i = 0; yytext[i]; yylval.s[++i] = 0)
                           if (i > 0) yylval.s[i - 1] = yytext[i];
                        if (i > 0) yylval.s[i - 1] = 0;
                        return LSTRING;
                     }


[^\n\t\r ]+          {
                        warnk("Unkwown identifier %s, will be ignored!\n", yytext);
                        need_ussage_print = 1;
                     }

"/*"([^"*"]|("*"[^"/"]))*"*/" { }
"//"[^\n]*\n { }
[ \t\n\r]

%%
