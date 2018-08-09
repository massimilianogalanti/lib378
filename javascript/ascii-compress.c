// ascii-compress.c
// NAME:
//  lib378.so
// DESCRIPTION:
//  text prediction library
// LICENSE:
//  LGPL
// COPYRIGHT & AUTHOR:
//  2007, <massi@users.sourceforge.net>

// MODULE: dictionary compression utility
// input must be sorted

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main( const int argc, const char *argv[] )
{
	char buff[128], obuff[128] = { '\0' };

	while( fgets( buff, sizeof( buff ), stdin ) )
	{	
		unsigned char i = 0;
		unsigned char k;
		unsigned char* p;

		buff[strlen( buff ) - 1] = '\0';

		for( i = 0; i < strlen( obuff ) && i < 0x0f; i++ )
			if( obuff[i] != buff[i] )
				break;
		
		k = strlen( &(buff[i]) );

		if( k > 0x0f )
		{
			fprintf( stderr, "k=%d (word: %s is too long compared to %s).\n", k, buff, obuff );
			continue;
		}
		
		if( argv[1] && 0 == i && (p = index( argv[1], buff[0] )) )
			fprintf( stdout, "\n%c=", *(p+1) );
		fprintf( stdout, "%02x%s", ((k & 0x0f) << 4) | (i & 0x0f), &(buff[i]) );

		strcpy( obuff, buff );
	}

	return 0;
}

// eof
