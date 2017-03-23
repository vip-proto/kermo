/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "kermo.y" /* yacc.c:339  */

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
}
#endif



#line 355 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 291 "kermo.y" /* yacc.c:355  */

   int n;
   char s[128];
   struct {
      short int r, s, t;
   } m;

#line 457 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 472 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   74

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  78

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   282

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   307,   307,   308,   311,   312,   315,   316,   319,   319,
     322,   323,   326,   332,   333,   334,   340,   346,   352,   360,
     361,   364,   364,   367,   368,   371,   378,   384,   385,   386,
     401,   407,   415,   416,   416,   426,   440,   450,   455,   460,
     466,   467,   470,   471,   474,   475,   476,   477,   478
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OB", "CB", "TITLE", "EXE", "P1", "P2",
  "MOUSE", "DEFAULT", "LUNCHER", "DIR", "GUI", "BGCOLOR", "ENTRYCOLOR",
  "SELCOLOR", "TEXTCOLOR", "LOGOJPEG", "KEY", "MULTIPLAYER", "AUTOPICTURE",
  "THEME", "COLOR", "STRING", "PSTRING", "ISTRING", "LSTRING", "$accept",
  "program", "header", "lunchers", "luncher", "$@1", "lbody", "ldef",
  "entries", "entry", "$@2", "body", "defin", "mapa", "$@3", "array",
  "mapper", "guiconfig", "cdefs", "cdef", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282
};
# endif

#define YYPACT_NINF -21

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-21)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       7,    27,   -21,   -21,    14,   -16,     8,   -21,   -16,   -21,
       8,    13,    15,    24,    28,    26,    43,    43,   -21,    -4,
      19,   -21,   -16,   -21,   -21,     8,   -21,   -21,   -21,   -21,
     -21,    43,   -21,    30,    38,    49,    49,    39,   -21,   -21,
      -4,   -21,    40,    49,    49,   -21,    41,   -21,   -21,    19,
     -21,   -21,   -21,     2,   -21,   -21,   -21,   -21,   -21,   -21,
     -21,   -21,   -21,    47,   -21,   -21,     5,   -21,    48,     3,
     -21,   -21,    11,     9,   -21,    64,   -21,   -21
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    21,     8,     0,     0,     5,     6,     3,    19,
       0,     0,     0,     0,     0,     0,     0,    40,    42,     0,
       0,     1,     2,     7,    20,     4,    44,    45,    46,    47,
      48,    41,    43,     0,     0,     0,     0,     0,    30,    31,
      22,    23,     0,     0,     0,    15,     0,    17,    18,     9,
      10,    25,    26,     0,    27,    28,    29,    24,    12,    13,
      14,    16,    11,     0,    33,    37,     0,    35,     0,     0,
      32,    36,     0,     0,    38,     0,    34,    39
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -21,   -21,   -21,    59,    -6,   -21,   -21,    21,    66,    10,
     -21,   -21,    32,    12,   -21,     4,   -20,   -21,    58,     6
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    20,    49,    50,     8,     9,
      19,    40,    41,    54,    69,    66,    67,    10,    17,    18
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      23,    33,    34,    35,    36,    63,    63,    37,    63,    70,
       2,    64,    63,    76,    21,    74,    38,    39,    24,    23,
       1,    65,    65,    32,    65,    42,    43,    44,    65,    45,
      75,    46,    24,     2,     3,     3,    26,    32,    27,    47,
      48,    11,    12,    13,    14,    15,    71,    28,    55,    16,
      30,    29,    53,    71,    51,    59,    60,    11,    12,    13,
      14,    15,    52,    56,    58,    61,    68,    72,    77,    25,
      62,    22,    57,    73,    31
};

static const yytype_uint8 yycheck[] =
{
       6,     5,     6,     7,     8,     3,     3,    11,     3,     4,
      26,     9,     3,     4,     0,     4,    20,    21,     8,    25,
      13,    19,    19,    17,    19,     6,     7,     8,    19,    10,
      19,    12,    22,    26,    27,    27,    23,    31,    23,    20,
      21,    14,    15,    16,    17,    18,    66,    23,    36,    22,
      24,    23,     3,    73,    24,    43,    44,    14,    15,    16,
      17,    18,    24,    24,    24,    24,    19,    19,     4,    10,
      49,     5,    40,    69,    16
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    13,    26,    27,    29,    30,    31,    32,    36,    37,
      45,    14,    15,    16,    17,    18,    22,    46,    47,    38,
      33,     0,    36,    32,    37,    31,    23,    23,    23,    23,
      24,    46,    47,     5,     6,     7,     8,    11,    20,    21,
      39,    40,     6,     7,     8,    10,    12,    20,    21,    34,
      35,    24,    24,     3,    41,    41,    24,    40,    24,    41,
      41,    24,    35,     3,     9,    19,    43,    44,    19,    42,
       4,    44,    19,    43,     4,    19,     4,     4
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    28,    29,    29,    30,    30,    31,    31,    33,    32,
      34,    34,    35,    35,    35,    35,    35,    35,    35,    36,
      36,    38,    37,    39,    39,    40,    40,    40,    40,    40,
      40,    40,    41,    42,    41,    43,    43,    44,    44,    44,
      45,    45,    46,    46,    47,    47,    47,    47,    47
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     2,     1,     1,     2,     0,     3,
       1,     2,     2,     2,     2,     1,     2,     1,     1,     1,
       2,     0,     3,     1,     2,     2,     2,     2,     2,     2,
       1,     1,     3,     0,     5,     1,     2,     1,     4,     5,
       2,     3,     1,     2,     2,     2,     2,     2,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 8:
#line 319 "kermo.y" /* yacc.c:1646  */
    { add_luncher((yyvsp[0].s)); }
#line 1596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 326 "kermo.y" /* yacc.c:1646  */
    {
              if (lunchers[l - 1].exe[0]) {
                 warnk("Luncher's EXE is duplicated, newer is applied!\n");
              }
              strcpy(lunchers[l - 1].exe, (yyvsp[0].s));
            }
#line 1607 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 332 "kermo.y" /* yacc.c:1646  */
    { add_luncher_map(0); }
#line 1613 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 333 "kermo.y" /* yacc.c:1646  */
    { add_luncher_map(1); }
#line 1619 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 334 "kermo.y" /* yacc.c:1646  */
    {
                 int i;
                 for (i = 0; i < l - 1; i++)
                    lunchers[i].def = 0;
                  lunchers[l-1].def = 1;
              }
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 340 "kermo.y" /* yacc.c:1646  */
    {
                   if (lunchers[l - 1].dir[0]) {
                      warnk("Luncher's DIR is duplicated, newer is applied!\n");
                   }
                   strcpy(lunchers[l - 1].dir, (yyvsp[0].s));
                }
#line 1641 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 346 "kermo.y" /* yacc.c:1646  */
    {
                    if (lunchers[l - 1].multip != -1) {
                       warnk("Luncher's MULTIPLAYER is duplicated, newer is applied!\n");
                    }
                    lunchers[n - 1].multip = (yyvsp[0].n);
                 }
#line 1652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 352 "kermo.y" /* yacc.c:1646  */
    {
                    if (lunchers[l - 1].screen != -1) {
                       warnk("Luncher's AUTOPICTURE is duplicated, newer is applied!\n");
                    }
                    lunchers[n - 1].screen = (yyvsp[0].n);
                 }
#line 1663 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 364 "kermo.y" /* yacc.c:1646  */
    { add_entry((yyvsp[0].s)); }
#line 1669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 364 "kermo.y" /* yacc.c:1646  */
    { check_entry(); }
#line 1675 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 371 "kermo.y" /* yacc.c:1646  */
    {
                   if (buffer[n-1].tit) {
                      warnk("TITLE is duplicated, newer is applied!\n");
                   }
                   strcpy(buffer[n - 1].title, (yyvsp[0].s));
                   buffer[n - 1].tit = 1;
                }
#line 1687 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 378 "kermo.y" /* yacc.c:1646  */
    {
                   if (buffer[n - 1].exe[0]) {
                      warnk("EXE is duplicated, newer is applied!\n");
                   }
                   strcpy(buffer[n - 1].exe, (yyvsp[0].s));
                }
#line 1698 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 384 "kermo.y" /* yacc.c:1646  */
    { add_entry_map(0); }
#line 1704 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 385 "kermo.y" /* yacc.c:1646  */
    { add_entry_map(1); }
#line 1710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 386 "kermo.y" /* yacc.c:1646  */
    {
                       int i, b;
                       b = 1;
                       if (buffer[n - 1].lunc) {
                          warnk("LUNCHER is duplicated, newer is applied!\n");
                       }
                       for (i = 0; i < l; i++)
                          if (strcmp(lunchers[i].name, (yyvsp[0].s)) == 0) {
                             b = 0;
                             buffer[n - 1].lunc = lunchers + i;
                          }
                       if (b) {
                          warnk("No LUNCHER %s defined, entry is ignored!\n", (yyvsp[0].s));
                       }
                    }
#line 1730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 401 "kermo.y" /* yacc.c:1646  */
    {
                    if (buffer[n - 1].multip != -1) {
                       warnk("MULTIPLAYER is duplicated, newer is applied!\n");
                    }
                    buffer[n - 1].multip = (yyvsp[0].n);
                 }
#line 1741 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 407 "kermo.y" /* yacc.c:1646  */
    {
                    if (buffer[n - 1].screen != -1) {
                       warnk("AUTOPICTURE is duplicated, newer is applied!\n");
                    }
                    buffer[n - 1].screen = (yyvsp[0].n);
                 }
#line 1752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 416 "kermo.y" /* yacc.c:1646  */
    {
                 t = -4;
                 memset(tt, 0, sizeof(map_t));
                 tt[0] = KEY_MOUSE_UP;
                 tt[1] = KEY_MOUSE_DOWN;
                 tt[2] = KEY_MOUSE_LEFT;
                 tt[3] = KEY_MOUSE_RIGHT;
              }
#line 1765 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 426 "kermo.y" /* yacc.c:1646  */
    {
             if (t < 0) {
                t = -t;
             } else {
                t = 0;
                memset(tt, 0, sizeof(map_t));
             }
             if (t < 16) {
                tt[t] = (yyvsp[0].m).r;
                tt[16 + t] = (yyvsp[0].m).s;
                tt[32 + t] = (yyvsp[0].m).t;
             }
             t++;
          }
#line 1784 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 440 "kermo.y" /* yacc.c:1646  */
    {
                     if (t < 16) {
                        tt[t] = (yyvsp[0].m).r;
                        tt[16 + t] = (yyvsp[0].m).s;
                        tt[32 + t] = (yyvsp[0].m).t;
                     }
                     t++;
                  }
#line 1797 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 450 "kermo.y" /* yacc.c:1646  */
    {
          (yyval.m).r = (yyvsp[0].n);
          (yyval.m).s = 0;
          (yyval.m).t = 0;
        }
#line 1807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 455 "kermo.y" /* yacc.c:1646  */
    {
                             (yyval.m).r = (yyvsp[-2].n);
                             (yyval.m).s = (yyvsp[-1].n);
                             (yyval.m).t = 0;
                         }
#line 1817 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 460 "kermo.y" /* yacc.c:1646  */
    {
                                   (yyval.m).r = (yyvsp[-3].n);
                                   (yyval.m).s = (yyvsp[-2].n);
                                   (yyval.m).t = (yyvsp[-1].n);
                                }
#line 1827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 466 "kermo.y" /* yacc.c:1646  */
    { config.theme = THEME_VIPPROTO; }
#line 1833 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 467 "kermo.y" /* yacc.c:1646  */
    { config.theme = (yyvsp[-1].n); }
#line 1839 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 474 "kermo.y" /* yacc.c:1646  */
    { config.bg_color = (yyvsp[0].n); }
#line 1845 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 475 "kermo.y" /* yacc.c:1646  */
    { config.entry_bg_color = (yyvsp[0].n);}
#line 1851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 476 "kermo.y" /* yacc.c:1646  */
    { config.selected_entry_bg_color = (yyvsp[0].n); }
#line 1857 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 477 "kermo.y" /* yacc.c:1646  */
    { config.text_color = (yyvsp[0].n); }
#line 1863 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 478 "kermo.y" /* yacc.c:1646  */
    { strncpy(config.jpeg_logo_file, (yyvsp[0].s), 128); }
#line 1869 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1873 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 479 "kermo.y" /* yacc.c:1906  */

