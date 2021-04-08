/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     COMMA = 258,
     NUMBER = 259,
     STRING = 260,
     TK_WALL = 261,
     TK_GATE = 262,
     TK_EARTH = 263,
     TK_WEB = 264,
     TK_BOARD = 265,
     TK_STEP = 266,
     TK_HOST = 267,
     TK_DOC = 268,
     TK_END = 269,
     TK_BOX_SIZE = 270,
     TK_SPHER_SIZE = 271,
     TK_TOR_SIZE = 272,
     TK_TOR_SIZE2 = 273,
     TK_DIFFUSE = 274,
     TK_AMBIENT = 275,
     TK_SPECULAR = 276,
     TK_EMISSION = 277,
     TK_SHININESS = 278,
     TEX_XP = 279,
     TEX_YP = 280,
     TEX_ZP = 281,
     TEX_XN = 282,
     TEX_YN = 283,
     TEX_ZN = 284,
     SPHER_TEX = 285,
     TLNT = 286
   };
#endif
/* Tokens.  */
#define COMMA 258
#define NUMBER 259
#define STRING 260
#define TK_WALL 261
#define TK_GATE 262
#define TK_EARTH 263
#define TK_WEB 264
#define TK_BOARD 265
#define TK_STEP 266
#define TK_HOST 267
#define TK_DOC 268
#define TK_END 269
#define TK_BOX_SIZE 270
#define TK_SPHER_SIZE 271
#define TK_TOR_SIZE 272
#define TK_TOR_SIZE2 273
#define TK_DIFFUSE 274
#define TK_AMBIENT 275
#define TK_SPECULAR 276
#define TK_EMISSION 277
#define TK_SHININESS 278
#define TEX_XP 279
#define TEX_YP 280
#define TEX_ZP 281
#define TEX_XN 282
#define TEX_YN 283
#define TEX_ZN 284
#define SPHER_TEX 285
#define TLNT 286




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 59 "vred.y"
{
  double dval;
  char*  sval;
  double* dpval;
  Vect* vectval;
  Tex*  texval;
  struct structBoxProps* bpval;
  struct structSpherProps* spval;
  App* appval;
}
/* Line 1529 of yacc.c.  */
#line 122 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

