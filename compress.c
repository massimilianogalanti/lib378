// compress.c
// NAME:
//  lib378.so
// DESCRIPTION:
//  text prediction library
// LICENSE:
//  LGPL
// COPYRIGHT & AUTHOR:
//  2007, <massi@users.sourceforge.net>

// MODULE: dictionary compression utility
// input file must be sorted

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(const int argc, const char *argv[])
{
	FILE *fdin;
	FILE *fdout;

	char buff[128], obuff[128] = {'\0'};

	if (!(fdin = fopen(argv[1], "r")))
	{
		printf("dictionary file not found!\n");
		return -1;
	}

	if (!(fdout = fopen("dictionary.bin", "w")))
	{
		printf("unable to create dictionary file!\n");
		return -1;
	}

	while (fgets(buff, sizeof(buff), fdin))
	{
		unsigned char i = 0;
		unsigned char k;

	again:
		buff[strlen(buff) - 1] = '\0';

		for (i = 0; i < strlen(obuff) && i < 0x0f; i++)
			if (obuff[i] != buff[i])
				break;

		k = strlen(&(buff[i]));

		if (k > 0x0f)
		{
			fprintf(stderr, "(word: %s is too long for %s)...", buff, obuff);
			k = 0x0f;
#if 0
			fprintf( stderr, "truncating!\n" );
#endif
#if 1
			fprintf(stderr, "splitting!\n");
			fprintf(fdout, "%c%s", ((k & 0x0f) << 4) | (i & 0x0f), &(buff[i]));
			strcpy(obuff, buff);
			goto again;
#endif
#if 0
			fprintf( stderr, "discarding!\n" );
			continue;
#endif
		}
#if 0
		if( i > 0x0f )
		{
			fprintf( stderr, "i=%d (word: %s is too long compared to %s).\n", k, buff, obuff );
			//continue;
			i = 0x0f;
		}
		//buff[i + k] = '\0';

		//printf( "\\%03o%s", i, &(buff[i]) );
		/* LLLLPPPP */
		//printf( "(L=%d:P=%d)%s o=%s n=%s\n", (k & 0x0f), (i & 0x0f), &(buff[i]), obuff, buff );
#endif
		fprintf(fdout, "%c%s", ((k & 0x0f) << 4) | (i & 0x0f), &(buff[i]));

		strcpy(obuff, buff);
	}

	fclose(fdout);
	fclose(fdin);
	return 0;
}

// eof
