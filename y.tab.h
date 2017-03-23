/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OB = 258,
    CB = 259,
    TITLE = 260,
    EXE = 261,
    P1 = 262,
    P2 = 263,
    MOUSE = 264,
    DEFAULT = 265,
    LUNCHER = 266,
    DIR = 267,
    GUI = 268,
    BGCOLOR = 269,
    ENTRYCOLOR = 270,
    SELCOLOR = 271,
    TEXTCOLOR = 272,
    LOGOJPEG = 273,
    KEY = 274,
    MULTIPLAYER = 275,
    AUTOPICTURE = 276,
    THEME = 277,
    COLOR = 278,
    STRING = 279,
    PSTRING = 280,
    ISTRING = 281,
    LSTRING = 282
  };
#endif
/* Tokens.  */
#define OB 258
#define CB 259
#define TITLE 260
#define EXE 261
#define P1 262
#define P2 263
#define MOUSE 264
#define DEFAULT 265
#define LUNCHER 266
#define DIR 267
#define GUI 268
#define BGCOLOR 269
#define ENTRYCOLOR 270
#define SELCOLOR 271
#define TEXTCOLOR 272
#define LOGOJPEG 273
#define KEY 274
#define MULTIPLAYER 275
#define AUTOPICTURE 276
#define THEME 277
#define COLOR 278
#define STRING 279
#define PSTRING 280
#define ISTRING 281
#define LSTRING 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 291 "kermo.y" /* yacc.c:1909  */

   int n;
   char s[128];
   struct {
      short int r, s, t;
   } m;

#line 116 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
