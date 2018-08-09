// 378.c

#include "378.h"

int l378_dictionary_open( l378_t* pdata, const char* fname )
{
	if( !(pdata->dictionary = fopen( fname, "r" )) )
		return -1;
#ifdef USE_BZ2
	pdata->bdictionary = BZ2_bzReadOpen( &(pdata->bzerror), pdata->dictionary, 0, 1, NULL, 0 );
#endif

	return 0;
}

int l378_dictionary_close( l378_t* pdata )
{
	if( !pdata->dictionary )
		return -1;

#ifdef USE_BZ2
	BZ2_bzReadClose( &(pdata->bzerror), pdata->bdictionary );
#endif

	fclose( pdata->dictionary );
	return 0;
}

int l378_keymap_set( l378_t* pdata, const char* kmap)
{
	pdata->keymap = kmap;

	return 0;
}

// dictionary element
// c[0]...c[n]
//
// c[0] bitstring: LLLLPPPP
//   LLLL (upper nibble) lenght of this string
//   PPPP (lower nibble) number of chars identical to the previous word
//
// c[1]..c[n] string

// get next compressed word
static char* get_next_word( l378_t* pdata, char* buff )
{
	if( !pdata->dictionary )
		return NULL;
#ifndef USE_BZ2
	if( feof( pdata->dictionary ) )
		rewind( pdata->dictionary );

	buff[0] = (char) fgetc( pdata->dictionary );
	fread( &(buff[1]), (buff[0] >> 4) & 0x0f, 1, pdata->dictionary );
#else
	if( BZ_STREAM_END == pdata->bzerror )
	{
		//BZ2_bzReadGetUnused( &(pdata->bzerror), pdata->bdictionary, NULL, NULL );
		BZ2_bzReadClose( &(pdata->bzerror), pdata->bdictionary );
		rewind( pdata->dictionary );
		pdata->bdictionary = BZ2_bzReadOpen( &(pdata->bzerror), pdata->dictionary, 0, 1, NULL, 0 );
	}

	if( 1 == BZ2_bzRead( &(pdata->bzerror), pdata->bdictionary, buff, 1 ) )
		BZ2_bzRead( &(pdata->bzerror), pdata->bdictionary, &(buff[1]), (buff[0] >> 4) & 0x0f );

	if( pdata->bzerror )
		return NULL;
	//	printf("ouch!: %d\n", pdata->bzerror);
#endif
	buff[1 + ((buff[0] >> 4) & 0x0f)] = '\0';

	return buff;
}

// just like strcmp(s1, s2), but with additional translation of s2
static int compare_translated( const char* s1, const char* s2, const char* tr )
{
	int i = 0;
	
	while( i < strlen( s1 ) )
	{
		if( s1[i] != tr[(int) s2[i]] )
			return -1;
		i++;
	}
	return 0;
}

char* predict_match_next( l378_t* pdata, const int exact )
{
	char word[128] = { '\0' };
	
	do
	{
		if( (get_next_word( pdata, word )) )
		{
			// expand in buffer
			strcpy( &(pdata->buff[(int) (word[0] & 0x0f)]), &(word[1]) );

			// compare
			if( !compare_translated( pdata->pattern, pdata->buff, pdata->keymap ))
				if( !exact || (strlen( pdata->pattern ) == strlen( pdata->buff )) )
					return pdata->buff;
		}
#ifndef USE_BZ2		
	} while( !feof( pdata->dictionary ) );
#else
	} while( BZ_STREAM_END != pdata->bzerror );
#endif
	return NULL;
}

// eof
