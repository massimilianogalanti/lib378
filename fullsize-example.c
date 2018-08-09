#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "378.h"

char mygetch ( void )
{
	int ch;
	struct termios oldt, newt;
	
	tcgetattr ( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
	
	return ch;
}

l378_DECLARE();

l378_KEYMAP = {
	['a'] = 'a', ['b'] = 'b', ['c'] = 'c',
	['d'] = 'd', ['e'] = 'e', ['f'] = 'f',
	['g'] = 'g', ['h'] = 'h', ['i'] = 'i',
	['j'] = 'j', ['k'] = 'k', ['l'] = 'l',
	['m'] = 'm', ['n'] = 'n', ['o'] = 'o',
	['p'] = 'p', ['q'] = 'q', ['r'] = 'r', ['s'] = 's',
	['t'] = 't', ['u'] = 'u', ['v'] = 'v',
	['w'] = 'w', ['x'] = 'x', ['y'] = 'y', ['z'] = 'z'
};

int main( const int argc, const char *argv[] )
{	
	char c;
	char* ns = NULL;

	while( '\n' != (c = mygetch()) )
	{
		switch( c )
		{
			case ' ':
				printf( " " );
				l378_confirm();
				ns = NULL;
				continue;
			case '\b':
				l378_popc();
				ns = l378_matche();
				break;
			case '\t':
				ns = l378_complete();
				break;
			default:
				l378_pushc( c );
				ns = l378_matche();
		}
		
		if( ns )
			printf( "\r%s", ns );
	}
	
	printf( "\n" );
	
	return 0;
}

// eof
