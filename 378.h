// 378.h
// NAME:
//  lib378.so
// DESCRIPTION:
//  text prediction library
// LICENSE:
//  LGPL
// COPYRIGHT & AUTHOR:
//  2007, <massi@users.sourceforge.net>

#ifndef _378_H_
#define _378_H_

#include <string.h>
#include <stdio.h>
#ifdef USE_BZ2
#include <bzlib.h>
#endif
/* Private API */

typedef struct
{
	FILE* dictionary;
#ifdef USE_BZ2
	BZFILE* bdictionary;
	int bzerror;
#endif
	char pattern[128];
	char buff[128];
	const char* keymap;
} l378_t;

// low level macros
#define predict_clear( s ) { s[0] = '\0'; }
#define predict_pushc( s, c ) { int l = strlen( s ); s[l++] = (char) c; s[l] = '\0'; }
#define predict_popc( s ) { int l = strlen( s ); if( l > 0 ) s[--l] = '\0'; }

/* Public API */

// dictionary ops
int l378_dictionary_open( l378_t*, const char* );
int l378_dictionary_close( l378_t* );

// set keymap
int l378_keymap_set( l378_t*, const char* );

// matches given string against dictionary (as substring or as whole word)
char* predict_match_next( l378_t*, const int);

#define l378_pushc( p, c )	predict_pushc( (p)->pattern, c )
#define l378_popc( p )		predict_popc( (p)->pattern )
#define l378_match( p )		predict_match_next( (p), 0 )
#define l378_matche( p )	predict_match_next( (p), 1 )
//#define l378_complete( p )	l378_match( (p) )
//#define l378_confirm( p )	predict_clear( (p)->pattern )
#define l378_clear( p )		predict_clear( (p)->pattern )

#endif /*_378_H_*/
