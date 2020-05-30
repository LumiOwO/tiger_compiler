
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"

#include "parser.h"
#include <iostream>

extern FILE* yyin;
extern int yylex();  
extern void yyerror(AST* ast, const char *msg);

std::set<std::string>   TigerParser::_typeset;
std::ostringstream      TigerParser::_str_real;
std::ostringstream      TigerParser::_str_label;



/* Line 189 of yacc.c  */
#line 88 "tiger.tab.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     K_NIL = 258,
     K_IF = 259,
     K_ELSE = 260,
     K_WHILE = 261,
     K_DO = 262,
     K_FOR = 263,
     K_TO = 264,
     K_BREAK = 265,
     K_THEN = 266,
     K_IN = 267,
     K_END = 268,
     K_OF = 269,
     K_TYPE = 270,
     K_ARRAY = 271,
     K_VAR = 272,
     K_FUNC = 273,
     K_LET = 274,
     O_ASSIGN = 275,
     O_COMPARE = 276,
     C_STRING = 277,
     C_INTEGER = 278,
     T_ID = 279,
     T_TYPE = 280
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 155 "tiger.tab.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  34
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   402

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  62
/* YYNRULES -- Number of states.  */
#define YYNSTATES  143

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    27,     2,
      35,    36,    33,    31,    42,    32,    43,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    44,    41,
      30,    28,    29,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    39,     2,    40,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,    26,    38,     2,     2,     2,     2,
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
      25
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    13,    17,    21,    25,
      29,    33,    37,    41,    45,    49,    51,    53,    55,    57,
      60,    62,    66,    70,    75,    78,    82,    86,    91,    98,
     103,   110,   115,   124,   126,   131,   137,   139,   143,   145,
     149,   153,   159,   161,   165,   170,   172,   175,   177,   179,
     181,   186,   188,   191,   195,   199,   203,   209,   214,   221,
     228,   236,   245
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      46,     0,    -1,    50,    -1,    24,    -1,    25,    -1,    50,
      21,    50,    -1,    50,    31,    50,    -1,    50,    32,    50,
      -1,    50,    33,    50,    -1,    50,    34,    50,    -1,    50,
      28,    50,    -1,    50,    27,    50,    -1,    50,    26,    50,
      -1,    50,    29,    50,    -1,    50,    30,    50,    -1,    22,
      -1,    23,    -1,     3,    -1,    54,    -1,    32,    50,    -1,
      49,    -1,    54,    20,    50,    -1,    47,    35,    36,    -1,
      47,    35,    52,    36,    -1,    35,    36,    -1,    35,    51,
      36,    -1,    48,    37,    38,    -1,    48,    37,    53,    38,
      -1,    48,    39,    50,    40,    14,    50,    -1,     4,    50,
      11,    50,    -1,     4,    50,    11,    50,     5,    50,    -1,
       6,    50,     7,    50,    -1,     8,    47,    20,    50,     9,
      50,     7,    50,    -1,    10,    -1,    19,    55,    12,    13,
      -1,    19,    55,    12,    51,    13,    -1,    50,    -1,    50,
      41,    51,    -1,    50,    -1,    50,    42,    52,    -1,    47,
      28,    50,    -1,    47,    28,    50,    42,    53,    -1,    47,
      -1,    54,    43,    47,    -1,    54,    39,    50,    40,    -1,
      56,    -1,    56,    55,    -1,    57,    -1,    60,    -1,    61,
      -1,    15,    47,    28,    58,    -1,    48,    -1,    37,    38,
      -1,    37,    59,    38,    -1,    16,    14,    48,    -1,    47,
      44,    48,    -1,    47,    44,    48,    42,    59,    -1,    17,
      47,    20,    50,    -1,    17,    47,    44,    48,    20,    50,
      -1,    18,    47,    35,    36,    28,    50,    -1,    18,    47,
      35,    59,    36,    28,    50,    -1,    18,    47,    35,    36,
      44,    48,    28,    50,    -1,    18,    47,    35,    59,    36,
      44,    48,    28,    50,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    42,    42,    49,    54,    61,    67,    73,    79,    85,
      91,    97,   103,   109,   115,   126,   130,   134,   138,   142,
     148,   152,   156,   162,   168,   172,   176,   182,   188,   194,
     198,   202,   206,   212,   216,   220,   226,   230,   236,   240,
     246,   252,   260,   266,   272,   281,   285,   290,   294,   298,
     304,   314,   320,   324,   328,   336,   344,   354,   360,   370,
     376,   382,   390
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "K_NIL", "K_IF", "K_ELSE", "K_WHILE",
  "K_DO", "K_FOR", "K_TO", "K_BREAK", "K_THEN", "K_IN", "K_END", "K_OF",
  "K_TYPE", "K_ARRAY", "K_VAR", "K_FUNC", "K_LET", "O_ASSIGN", "O_COMPARE",
  "C_STRING", "C_INTEGER", "T_ID", "T_TYPE", "'|'", "'&'", "'='", "'>'",
  "'<'", "'+'", "'-'", "'*'", "'/'", "'('", "')'", "'{'", "'}'", "'['",
  "']'", "';'", "','", "'.'", "':'", "$accept", "S", "id", "typename",
  "binary_operation", "expr", "expr_seq", "expr_list", "field_list",
  "lvalue", "declaration_list", "declaration", "type_declaration", "type",
  "type_fields", "variable_declaration", "function_declaration", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   124,    38,    61,    62,
      60,    43,    45,    42,    47,    40,    41,   123,   125,    91,
      93,    59,    44,    46,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    47,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    51,    51,    52,    52,
      53,    53,    54,    54,    54,    55,    55,    56,    56,    56,
      57,    58,    58,    58,    58,    59,    59,    60,    60,    61,
      61,    61,    61
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     2,
       1,     3,     3,     4,     2,     3,     3,     4,     6,     4,
       6,     4,     8,     1,     4,     5,     1,     3,     1,     3,
       3,     5,     1,     3,     4,     1,     2,     1,     1,     1,
       4,     1,     2,     3,     3,     3,     5,     4,     6,     6,
       7,     8,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    17,     0,     0,     0,    33,     0,    15,    16,     3,
       4,     0,     0,     0,    42,     0,    20,     2,    18,     0,
       0,     0,     0,     0,     0,     0,    45,    47,    48,    49,
      19,    24,    36,     0,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    46,     0,
      25,    22,    38,     0,    26,     0,     0,     0,     5,    12,
      11,    10,    13,    14,     6,     7,     8,     9,    21,     0,
      43,    29,    31,     0,     0,     0,     0,     0,    34,     0,
      37,     0,    23,     0,    27,     0,    44,     0,     0,     0,
       0,    51,    50,    57,     0,     0,     0,     0,    35,    39,
      40,     0,    30,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,    28,     0,    54,    53,    58,    59,     0,
      55,     0,     0,    41,    32,     0,     0,    60,     0,    61,
      56,     0,    62
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    32,    33,    63,    66,    18,
      25,    26,    27,   102,   107,    28,    29
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -87
static const yytype_int16 yypact[] =
{
     191,   -87,   191,   191,   -17,   -87,    -9,   -87,   -87,   -87,
     -87,   191,    80,    30,    -2,   -27,   -87,   340,    38,   246,
     217,    14,   -17,   -17,   -17,    49,    -9,   -87,   -87,   -87,
      45,   -87,   294,    29,   -87,   134,    35,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     -17,   191,   191,   191,    39,   -16,    34,   168,   -87,   191,
     -87,   -87,   260,    40,   -87,    42,    51,   310,   354,   118,
     368,   354,   354,   354,    45,    45,   -87,   -87,   340,   325,
     -87,   201,   340,    -8,    55,   191,    47,   -19,   -87,    74,
     -87,   191,   -87,   191,   -87,    81,   -87,   191,   191,    82,
      36,   -87,   -87,   340,    86,   -13,    50,    64,   -87,   -87,
     277,   191,   340,   232,    47,   -87,    63,   191,   191,    47,
      47,   -12,   -17,   340,   191,   -87,   -87,   340,   340,    79,
      66,   191,    47,   -87,   340,   191,   -17,   340,    85,   340,
     -87,   191,   340
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -87,   -87,    32,   -57,   -87,     0,     7,    18,    -1,   -87,
      84,   -87,   -87,   -87,   -86,   -87,   -87
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      17,    98,    19,    20,    85,     9,    22,     9,    23,    24,
      36,    30,    37,    38,   116,   118,   131,   105,    39,    40,
      41,    42,    43,    44,    45,    46,    47,   101,    86,   104,
      34,   119,   132,    35,    53,    62,    21,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
     140,    81,    82,    83,    54,    55,    56,   125,    48,     9,
       9,    57,   129,   130,    89,    60,    90,    84,    65,    87,
      93,    99,    10,    64,   115,   138,    92,    49,    46,    47,
      10,    50,    80,     1,     2,   103,     3,   108,     4,    94,
       5,    62,   100,   110,   120,   111,   114,   112,   113,     6,
     121,   126,     7,     8,     9,    10,   117,   135,   136,   109,
      58,   123,    11,   141,     0,    12,    31,   127,   128,   106,
       0,   133,     0,     0,   134,     0,     0,     0,     0,     0,
       0,   137,   106,     0,     0,   139,     0,     1,     2,    38,
       3,   142,     4,     0,     5,    40,    41,    42,    43,    44,
      45,    46,    47,     6,    65,     0,     7,     8,     9,    10,
       0,     0,     0,     0,     0,     0,    11,     0,   106,    12,
      61,     1,     2,     0,     3,     0,     4,     0,     5,     0,
       0,    88,     0,     0,     0,     0,     0,     6,     0,     0,
       7,     8,     9,    10,     1,     2,     0,     3,     0,     4,
      11,     5,     0,    12,     0,     0,    97,     0,     0,     0,
       6,     0,     0,     7,     8,     9,    10,     0,     0,     0,
       0,     0,    38,    11,    52,     0,    12,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,    38,   124,
       0,     0,     0,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     0,    38,     0,     0,     0,    51,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    38,     0,     0,
       0,     0,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    38,     0,     0,     0,     0,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    38,     0,
       0,     0,    91,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     0,     0,     0,    38,     0,     0,     0,   122,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     0,
       0,    38,     0,     0,     0,    59,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,    38,     0,     0,     0,
      95,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,    38,     0,     0,     0,    96,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,     0,     0,     0,     0,
       0,     0,    -1,    -1,    -1,    44,    45,    46,    47,    38,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    44,
      45,    46,    47
};

static const yytype_int16 yycheck[] =
{
       0,     9,     2,     3,    20,    24,    15,    24,    17,    18,
      37,    11,    39,    21,   100,    28,    28,    36,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    84,    44,    86,
       0,    44,    44,    35,    20,    35,     4,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
     136,    51,    52,    53,    22,    23,    24,   114,    20,    24,
      24,    12,   119,   120,    57,    36,    59,    28,    36,    35,
      28,    16,    25,    38,    38,   132,    36,    39,    33,    34,
      25,    43,    50,     3,     4,    85,     6,    13,     8,    38,
      10,    91,    37,    93,    44,    14,    14,    97,    98,    19,
      36,    38,    22,    23,    24,    25,    20,    28,    42,    91,
      26,   111,    32,    28,    -1,    35,    36,   117,   118,    87,
      -1,   122,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,   131,   100,    -1,    -1,   135,    -1,     3,     4,    21,
       6,   141,     8,    -1,    10,    27,    28,    29,    30,    31,
      32,    33,    34,    19,   122,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   136,    35,
      36,     3,     4,    -1,     6,    -1,     8,    -1,    10,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      22,    23,    24,    25,     3,     4,    -1,     6,    -1,     8,
      32,    10,    -1,    35,    -1,    -1,     5,    -1,    -1,    -1,
      19,    -1,    -1,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    21,    32,     7,    -1,    35,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    21,     7,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    21,    -1,    -1,    -1,    11,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    21,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    21,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    42,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    21,    -1,    -1,    -1,    42,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    21,    -1,    -1,    -1,    41,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    21,    -1,    -1,    -1,
      40,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    21,    -1,    -1,    -1,    40,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,    31,
      32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     6,     8,    10,    19,    22,    23,    24,
      25,    32,    35,    46,    47,    48,    49,    50,    54,    50,
      50,    47,    15,    17,    18,    55,    56,    57,    60,    61,
      50,    36,    50,    51,     0,    35,    37,    39,    21,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    20,    39,
      43,    11,     7,    20,    47,    47,    47,    12,    55,    41,
      36,    36,    50,    52,    38,    47,    53,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      47,    50,    50,    50,    28,    20,    44,    35,    13,    51,
      51,    42,    36,    28,    38,    40,    40,     5,     9,    16,
      37,    48,    58,    50,    48,    36,    47,    59,    13,    52,
      50,    14,    50,    50,    14,    38,    59,    20,    28,    44,
      44,    36,    42,    50,     7,    48,    38,    50,    50,    48,
      48,    28,    44,    53,    50,    28,    42,    50,    48,    50,
      59,    28,    50
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (res, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, res); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, AST* &res)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, res)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    AST* &res;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (res);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, AST* &res)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, res)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    AST* &res;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, res);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, AST* &res)
#else
static void
yy_reduce_print (yyvsp, yyrule, res)
    YYSTYPE *yyvsp;
    int yyrule;
    AST* &res;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , res);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, res); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, AST* &res)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, res)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    AST* &res;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (res);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (AST* &res);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (AST* &res)
#else
int
yyparse (res)
    AST* &res;
#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 42 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("S");
        res = (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 49 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("ID");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 54 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("typename");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 61 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 67 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 73 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 79 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 85 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 91 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 97 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 103 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 109 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        debug_print("expr" << token->text << "expr");
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 115 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("expr" << (yyvsp[(2) - (3)])->label() << "expr");
        LexAST* token = (LexAST*)(yyvsp[(2) - (3)]);
        (yyval) = new BinaryExprAST((ExprAST*)(yyvsp[(1) - (3)]), token->text, (ExprAST*)(yyvsp[(3) - (3)]));
        delete (yyvsp[(2) - (3)]);
    ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 126 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("string-constant");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 130 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("integer-constant");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 134 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("nil");
        (yyval) = new NilExprAST();
    ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 138 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("lvalue");
        (yyval) = new VarExprAST((VarAST*)(yyvsp[(1) - (1)]));
    ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 142 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("- expr");
        LexAST* token = (LexAST*)(yyvsp[(1) - (2)]);
        (yyval) = new UnaryExprAST(token->text, (ExprAST*)(yyvsp[(2) - (2)]));
        delete (yyvsp[(1) - (2)]);
    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 148 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("binary_operation");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 152 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("lvalue := expr");
        (yyval) = new AssignExprAST((VarAST*)(yyvsp[(1) - (3)]), (ExprAST*)(yyvsp[(3) - (3)]));
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 156 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("id ( )");
        LexAST* token = (LexAST*)(yyvsp[(1) - (3)]);
        (yyval) = new FuncExprAST(token->text, nullptr);
        delete (yyvsp[(1) - (3)]);
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 162 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("id ( expr-list )");
        LexAST* token = (LexAST*)(yyvsp[(1) - (4)]);
        (yyval) = new FuncExprAST(token->text, (ExprListAST*)(yyvsp[(3) - (4)]));
        delete (yyvsp[(1) - (4)]);
    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 168 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("( )");
        (yyval) = new SequenceExprAST(nullptr);
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 172 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("( expr-seq )");
        (yyval) = new SequenceExprAST((ExprListAST*)(yyvsp[(2) - (3)]));
    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 176 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("type-id { }");
        LexAST* token = (LexAST*)(yyvsp[(1) - (3)]);
        (yyval) = new RecordDefineExprAST(token->text, nullptr);
        delete (yyvsp[(1) - (3)]);
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 182 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("type-id { field-list }");
        LexAST* token = (LexAST*)(yyvsp[(1) - (4)]);
        (yyval) = new RecordDefineExprAST(token->text, (FieldListAST*)(yyvsp[(3) - (4)]));
        delete (yyvsp[(1) - (4)]);
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 188 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("type-id [ expr ] of expr");
        LexAST* token = (LexAST*)(yyvsp[(1) - (6)]);
        (yyval) = new ArrayDefineExprAST(token->text, (ExprAST*)(yyvsp[(3) - (6)]), (ExprAST*)(yyvsp[(6) - (6)]));
        delete (yyvsp[(1) - (6)]);
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 194 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("if expr then expr");
        (yyval) = new IfExprAST((ExprAST*)(yyvsp[(2) - (4)]), (ExprAST*)(yyvsp[(4) - (4)]), nullptr);
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 198 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("if expr then expr else expr");
        (yyval) = new IfExprAST((ExprAST*)(yyvsp[(2) - (6)]), (ExprAST*)(yyvsp[(4) - (6)]), (ExprAST*)(yyvsp[(6) - (6)]));
    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 202 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("while expr do expr");
        (yyval) = new WhileExprAST((ExprAST*)(yyvsp[(2) - (4)]), (ExprAST*)(yyvsp[(4) - (4)]));
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 206 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("for id := expr to expr do expr");
        LexAST* token = (LexAST*)(yyvsp[(2) - (8)]);
        (yyval) = new ForExprAST(token->text, (ExprAST*)(yyvsp[(4) - (8)]), (ExprAST*)(yyvsp[(6) - (8)]), (ExprAST*)(yyvsp[(8) - (8)]));
        delete (yyvsp[(2) - (8)]);
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 212 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("break");
        (yyval) = new BreakExprAST();
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 216 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("let declaration-list in end");
        (yyval) = new LetExprAST((DeclListAST*)(yyvsp[(2) - (4)]), nullptr);
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 220 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("let declaration-list in expr-seq end");
        (yyval) = new LetExprAST((DeclListAST*)(yyvsp[(2) - (5)]), (ExprListAST*)(yyvsp[(4) - (5)]));
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 226 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("expr");
        (yyval) = new ExprListAST((ExprAST*)(yyvsp[(1) - (1)]), nullptr);
    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 230 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("expr ; expr-seq");
        (yyval) = new ExprListAST((ExprAST*)(yyvsp[(1) - (3)]), (ExprListAST*)(yyvsp[(3) - (3)]));
    ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 236 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("expr");
        (yyval) = new ExprListAST((ExprAST*)(yyvsp[(1) - (1)]), nullptr);
    ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 240 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("expr , expr-list");
        (yyval) = new ExprListAST((ExprAST*)(yyvsp[(1) - (3)]), (ExprListAST*)(yyvsp[(3) - (3)]));
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 246 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("id = expr");
        LexAST* token = (LexAST*)(yyvsp[(1) - (3)]);
        (yyval) = new FieldListAST(token->text, (ExprAST*)(yyvsp[(3) - (3)]), nullptr);
        delete (yyvsp[(1) - (3)]);
    ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 252 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("id = expr , field-list");
        LexAST* token = (LexAST*)(yyvsp[(1) - (5)]);
        (yyval) = new FieldListAST(token->text, (ExprAST*)(yyvsp[(3) - (5)]), (FieldListAST*)(yyvsp[(5) - (5)]));
        delete (yyvsp[(1) - (5)]);
    ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 260 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("id");
        LexAST* token = (LexAST*)(yyvsp[(1) - (1)]);
        (yyval) = new SimpleVarAST(token->text);
        delete (yyvsp[(1) - (1)]);
    ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 266 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("lvalue . id");
        LexAST* token = (LexAST*)(yyvsp[(3) - (3)]);
        (yyval) = new FieldVarAST((VarAST*)(yyvsp[(1) - (3)]), token->text);
        delete (yyvsp[(3) - (3)]);
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 272 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("lvalue [ expr ]");
        (yyval) = new IndexVarAST((VarAST*)(yyvsp[(1) - (4)]), (ExprAST*)(yyvsp[(3) - (4)]));
    ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 281 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("declaration");
        (yyval) = new DeclListAST((DeclAST*)(yyvsp[(1) - (1)]), nullptr);
    ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 285 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("declaration-list declaration");
        (yyval) = new DeclListAST((DeclAST*)(yyvsp[(1) - (2)]), (DeclListAST*)(yyvsp[(2) - (2)]));
    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 290 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("type-declaration");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 294 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("variable-declaration");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 298 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("function-declaration");
        (yyval) = (yyvsp[(1) - (1)]);
    ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 304 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("type type-id = type");
        LexAST* token = (LexAST*)(yyvsp[(2) - (4)]);
        (yyval) = new TypeDeclAST(token->text, (TypeAST*)(yyvsp[(4) - (4)]));
        
        TigerParser::insert(token->text);
        delete (yyvsp[(2) - (4)]);
    ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 314 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("type-id");
        LexAST* token = (LexAST*)(yyvsp[(1) - (1)]);
        (yyval) = new SimpleTypeAST(token->text);
        delete (yyvsp[(1) - (1)]);
    ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 320 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("{ }");
        (yyval) = new RecordTypeAST(nullptr);
    ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 324 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("{ type-fields }");
        (yyval) = new RecordTypeAST((TypeFieldsAST*)(yyvsp[(2) - (3)]));
    ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 328 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("array of type-id");
        LexAST* token = (LexAST*)(yyvsp[(3) - (3)]);
        (yyval) = new ArrayTypeAST(token->text);
        delete (yyvsp[(3) - (3)]);
    ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 336 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("id : typename");
        LexAST* token1 = (LexAST*)(yyvsp[(1) - (3)]);
        LexAST* token3 = (LexAST*)(yyvsp[(3) - (3)]);
        (yyval) = new TypeFieldsAST(token1->text, token3->text, nullptr);
        delete (yyvsp[(1) - (3)]);
        delete (yyvsp[(3) - (3)]);
    ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 344 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("id : typename , type-fields");
        LexAST* token1 = (LexAST*)(yyvsp[(1) - (5)]);
        LexAST* token3 = (LexAST*)(yyvsp[(3) - (5)]);
        (yyval) = new TypeFieldsAST(token1->text, token3->text, (TypeFieldsAST*)(yyvsp[(5) - (5)]));
        delete (yyvsp[(1) - (5)]);
        delete (yyvsp[(3) - (5)]);
    ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 354 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("var id := expr");
        LexAST* token = (LexAST*)(yyvsp[(2) - (4)]);
        (yyval) = new VarDeclAST(token->text, nullptr, (ExprAST*)(yyvsp[(4) - (4)]));
        delete (yyvsp[(2) - (4)]);
    ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 360 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("var id : type-id := expr");
        LexAST* token2 = (LexAST*)(yyvsp[(2) - (6)]);
        LexAST* token4 = (LexAST*)(yyvsp[(4) - (6)]);
        (yyval) = new VarDeclAST(token2->text, new SimpleTypeAST(token4->text), (ExprAST*)(yyvsp[(6) - (6)]));
        delete (yyvsp[(2) - (6)]);
        delete (yyvsp[(4) - (6)]);
    ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 370 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("function id ( ) = expr");
        LexAST* token = (LexAST*)(yyvsp[(2) - (6)]);
        (yyval) = new FuncDeclAST(token->text, nullptr, nullptr, (ExprAST*)(yyvsp[(6) - (6)]));
        delete (yyvsp[(2) - (6)]);
    ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 376 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("function id ( type-fields ) = expr");
        LexAST* token = (LexAST*)(yyvsp[(2) - (7)]);
        (yyval) = new FuncDeclAST(token->text, (TypeFieldsAST*)(yyvsp[(4) - (7)]), nullptr, (ExprAST*)(yyvsp[(7) - (7)]));
        delete (yyvsp[(2) - (7)]);
    ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 382 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("function id ( ) : type-id = expr");
        LexAST* token2 = (LexAST*)(yyvsp[(2) - (8)]);
        LexAST* token6 = (LexAST*)(yyvsp[(6) - (8)]);
        (yyval) = new FuncDeclAST(token2->text, nullptr, new SimpleTypeAST(token6->text), (ExprAST*)(yyvsp[(8) - (8)]));
        delete (yyvsp[(2) - (8)]);
        delete (yyvsp[(6) - (8)]);
    ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 390 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"
    {
        debug_print("function id ( type-fields ) : type-id = expr");
        LexAST* token2 = (LexAST*)(yyvsp[(2) - (9)]);
        LexAST* token7 = (LexAST*)(yyvsp[(7) - (9)]);
        (yyval) = new FuncDeclAST(token2->text, (TypeFieldsAST*)(yyvsp[(4) - (9)]), new SimpleTypeAST(token7->text), (ExprAST*)(yyvsp[(9) - (9)]));
        delete (yyvsp[(2) - (9)]);
        delete (yyvsp[(7) - (9)]);
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 2211 "tiger.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (res, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (res, yymsg);
	  }
	else
	  {
	    yyerror (res, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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
		      yytoken, &yylval, res);
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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
		  yystos[yystate], yyvsp, res);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (res, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, res);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, res);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 398 "E:\\_LLVM\\My_LLVM_Pro\\parser\\src\\parser\\tiger.ypp"


AST* TigerParser::parse(const char *filepath)
{
    std::cout << "Begin Parsing \"" << filepath << "\"...\n";
    if(!(yyin = fopen(filepath, "r"))) {
        std::cerr << "Cannot open tiger source file: \"" << filepath << "\"" << std::endl;
        exit(1);
        return nullptr;
    }
    TigerParser::reset();

    AST *res;
    yyparse(res);
    std::cout << "End Parsing." << std::endl;
    return res;
}

void yyerror(AST* ast, const char *msg)
{
    std::cout << "Error encountered: " << msg << std::endl;
}
