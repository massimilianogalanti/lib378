#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "378.h"

char mygetch( void )
{
	int ch;
	struct termios oldt, newt;
	
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	
	return ch;
}

static l378_t my378;
static const char keymap[] =  {
	['a'] = '2', ['b'] = '2', ['c'] = '2',
	['d'] = '3', ['e'] = '3', ['f'] = '3',
	['g'] = '4', ['h'] = '4', ['i'] = '4',
	['j'] = '5', ['k'] = '5', ['l'] = '5',
	['m'] = '6', ['n'] = '6', ['o'] = '6',
	['p'] = '7', ['q'] = '7', ['r'] = '7', ['s'] = '7',
	['t'] = '8', ['u'] = '8', ['v'] = '8',
	['w'] = '9', ['x'] = '9', ['y'] = '9', ['z'] = '9'
};

int main( const int argc, const char *argv[] )
{	
	char c;
	char* s = NULL;

	l378_dictionary_open( &my378, "dictionary.bin.bz2" );
	l378_keymap_set( &my378, keymap );
	
	printf( "     1     2     3\n" );
	printf( "     C    abc   def\n\n" );
	printf( "     4     5     6\n" );
	printf( "    ghi   jkl   mno\n\n" );
	printf( "     7     8     9\n" );
	printf( "    pqrs  tuv  wxyz\n\n" );
	printf( "    TAB    0     - \n" );
	printf( "    auto   _   next\n\n" );

	while( '\n' != (c = mygetch()) )
	{
		switch( c )
		{
			//case '0':
			//	break;
			//case '\t':
			//	if( (s = l378_match( &my378 )) )
			//		printf( "%s\r", s );
			//	continue;
			case '-':
				break;
			case '1':
				printf( "\t\t\t\t\r" );
				l378_popc( &my378 );
				break;
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				l378_pushc( &my378, c );
				break;
			default:
				continue;	
		}
		
		//printf( "[%s]:", my378.pattern );

		if( (s = l378_matche( &my378 )) )
			printf( "%s_", s );
		else if( (s = l378_match( &my378 )) )
		{
			unsigned char j;

			for( j = 0; j < strlen( my378.pattern ); j++ )
				printf( "%c", s[j] );
			printf( "_%s", &(s[j]) );
		}
		printf( "\t\t\r");
	}
	
	printf( "\n" );
	
	l378_dictionary_close( &my378 );
	return 0;
}

// eof
