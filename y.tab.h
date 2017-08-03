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
typedef union
{
   int n;
   char s[128];
   struct {
      short int r, s, t;
   } m;
} YYSTYPE;
extern YYSTYPE yylval;
