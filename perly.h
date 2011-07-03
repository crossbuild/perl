/* -*- buffer-read-only: t -*-
   !!!!!!!   DO NOT EDIT THIS FILE   !!!!!!!
   This file is built by regen_perly.pl from perly.y.
   Any changes made here will be lost!
 */

#ifdef PERL_CORE

/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     GRAMPROG = 258,
     GRAMEXPR = 259,
     GRAMBLOCK = 260,
     GRAMBARESTMT = 261,
     GRAMFULLSTMT = 262,
     GRAMSTMTSEQ = 263,
     WORD = 264,
     METHOD = 265,
     FUNCMETH = 266,
     THING = 267,
     PMFUNC = 268,
     PRIVATEREF = 269,
     QWLIST = 270,
     FUNC0SUB = 271,
     UNIOPSUB = 272,
     LSTOPSUB = 273,
     PLUGEXPR = 274,
     PLUGSTMT = 275,
     LABEL = 276,
     FORMAT = 277,
     SUB = 278,
     ANONSUB = 279,
     PACKAGE = 280,
     USE = 281,
     WHILE = 282,
     UNTIL = 283,
     IF = 284,
     UNLESS = 285,
     ELSE = 286,
     ELSIF = 287,
     CONTINUE = 288,
     FOR = 289,
     GIVEN = 290,
     WHEN = 291,
     DEFAULT = 292,
     LOOPEX = 293,
     DOTDOT = 294,
     YADAYADA = 295,
     FUNC0 = 296,
     FUNC1 = 297,
     FUNC = 298,
     UNIOP = 299,
     LSTOP = 300,
     RELOP = 301,
     EQOP = 302,
     MULOP = 303,
     ADDOP = 304,
     DOLSHARP = 305,
     DO = 306,
     HASHBRACK = 307,
     NOAMP = 308,
     LOCAL = 309,
     MY = 310,
     MYSUB = 311,
     REQUIRE = 312,
     COLONATTR = 313,
     PREC_LOW = 314,
     DOROP = 315,
     OROP = 316,
     ANDOP = 317,
     NOTOP = 318,
     BINDOP = 319,
     ASSIGNOP = 320,
     DORDOR = 321,
     OROR = 322,
     ANDAND = 323,
     BITOROP = 324,
     BITANDOP = 325,
     SHIFTOP = 326,
     MATCHOP = 327,
     REFGEN = 328,
     UMINUS = 329,
     POWOP = 330,
     POSTDEC = 331,
     POSTINC = 332,
     PREDEC = 333,
     PREINC = 334,
     ARROW = 335,
     PEG = 336
   };
#endif

/* Tokens.  */
#define GRAMPROG 258
#define GRAMEXPR 259
#define GRAMBLOCK 260
#define GRAMBARESTMT 261
#define GRAMFULLSTMT 262
#define GRAMSTMTSEQ 263
#define WORD 264
#define METHOD 265
#define FUNCMETH 266
#define THING 267
#define PMFUNC 268
#define PRIVATEREF 269
#define QWLIST 270
#define FUNC0SUB 271
#define UNIOPSUB 272
#define LSTOPSUB 273
#define PLUGEXPR 274
#define PLUGSTMT 275
#define LABEL 276
#define FORMAT 277
#define SUB 278
#define ANONSUB 279
#define PACKAGE 280
#define USE 281
#define WHILE 282
#define UNTIL 283
#define IF 284
#define UNLESS 285
#define ELSE 286
#define ELSIF 287
#define CONTINUE 288
#define FOR 289
#define GIVEN 290
#define WHEN 291
#define DEFAULT 292
#define LOOPEX 293
#define DOTDOT 294
#define YADAYADA 295
#define FUNC0 296
#define FUNC1 297
#define FUNC 298
#define UNIOP 299
#define LSTOP 300
#define RELOP 301
#define EQOP 302
#define MULOP 303
#define ADDOP 304
#define DOLSHARP 305
#define DO 306
#define HASHBRACK 307
#define NOAMP 308
#define LOCAL 309
#define MY 310
#define MYSUB 311
#define REQUIRE 312
#define COLONATTR 313
#define PREC_LOW 314
#define DOROP 315
#define OROP 316
#define ANDOP 317
#define NOTOP 318
#define BINDOP 319
#define ASSIGNOP 320
#define DORDOR 321
#define OROR 322
#define ANDAND 323
#define BITOROP 324
#define BITANDOP 325
#define SHIFTOP 326
#define MATCHOP 327
#define REFGEN 328
#define UMINUS 329
#define POWOP 330
#define POSTDEC 331
#define POSTINC 332
#define PREDEC 333
#define PREINC 334
#define ARROW 335
#define PEG 336



#endif /* PERL_CORE */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */

    I32	ival; /* __DEFAULT__ (marker for regen_perly.pl;
				must always be 1st union member) */
    char *pval;
    OP *opval;
    GV *gvval;
#ifdef PERL_IN_MADLY_C
    TOKEN* p_tkval;
    TOKEN* i_tkval;
#else
    char *p_tkval;
    I32	i_tkval;
#endif
#ifdef PERL_MAD
    TOKEN* tkval;
#endif



/* Line 1676 of yacc.c  */
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif





/* Generated from:
 * 9ad92a083cdd8f9edbf9d7020aa844e0c662e7588c7375300c75354fac23e0cc perly.y
 * 738ca60a0b4cb075902435e976a2f393d438e8e6e32ba81e037dd773b75c87b5 regen_perly.pl
 * ex: set ro: */
