/* Copyright (C) 2000 Free Software Foundation, Inc.  */

/* { dg-do preprocess } */
/* { dg-options -pedantic } */

/* Tests macro syntax, for both definition and invocation, including:-

   o Full range of macro definition semantics.
   o No. of arguments supplied to function-like macros.
   o Odd GNU rest args behaviour.
   o Macro arguments do not flow into the rest of the file.  */


/* Test basic macro definition syntax.  The macros are all called
   "foo" deliberately to provoke an (excess) redefinition warning in
   case the macros succeed in being entered in the macro hash table
   despite being an error.

   Split a couple of the lines to check that the errors appear on the
   right line (i.e. are associated with the correct token).  */

#define ;			/* { dg-error "identifier" } */
#define SEMI;			/* { dg-warning "space" } */
#define defined			/* { dg-error "defined" } */
#define foo(X			/* { dg-error "missing" } */
#define foo\
(X,)				/* { dg-error "parameter name" } */
#define foo(, X)		/* { dg-error "parameter name" } */
#define foo(X, X)		/* { dg-error "duplicate" } */
#define foo(X Y)		/* { dg-error "comma" } */
#define foo(()			/* { dg-error "token may not appear" } */
#define foo(..., X)		/* { dg-error "missing" } */
#define foo \
__VA_ARGS__			/* { dg-warning "__VA_ARGS__" } */
#define foo(__VA_ARGS__)	/* { dg-warning "__VA_ARGS__" } */
#define foo(...) __VA_ARGS__	/* OK.  */
#define __VA_ARGS__		/* { dg-warning "__VA_ARGS__" } */

/* test # of supplied arguments.  */
#define none()
#define one(x)
#define two(x, y)
#define var0(...)
#define var1(x, ...)
none()				/* OK.  */
none(ichi)			/* { dg-error "too many" } */
one()				/* OK.  */
one(ichi)			/* OK.  */
one(ichi\
, ni)				/* { dg-error "too many" } */
two(ichi)			/* { dg-error "not enough" } */
var0()				/* OK.  */
var0(ichi)			/* OK.  */
var1()				/* { dg-warning "rest arguments to be used" } */
var1(ichi)			/* { dg-warning "rest arguments to be used" } */
var1(ichi, ni)			/* OK.  */

/* This tests two oddities of GNU rest args - omitting a comma is OK,
   and backtracking a token on pasting an empty rest args.  */
#define rest(x, y...) x ## y	/* { dg-warning "ISO C" } */
rest(ichi,)			/* OK.  */
rest(ichi)			/* { dg-warning "rest arguments to be used" } */
#if 23 != rest(2, 3)		/* OK, no warning.  */
#error 23 != 23 !!
#endif

/* Test that we don't allow arguments to flow into the rest of the
   file.  */
#define half_invocation do_nowt(2
#define do_nowt(x) x
half_invocation )		/* OK.  */
do_nowt (half_invocation))	/* { dg-error "unterminated argument" } */
