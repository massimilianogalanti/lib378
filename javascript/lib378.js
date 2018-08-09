/*
	lib378.js
	
	VERSION: CVS

	LICENSE: LGPL
	AUTHOR: Massimiliano Galanti <massi@users.sourceforge.net>
	WEB: http://lib378.sourceforge.net
*/

var my378 = "";

var dict_idx = 0;
var dict_buffer = "";
var match = "";

var keymap = new Array();

keymap ['a'] = 2; keymap ['b'] = 2; keymap ['c'] = 2;
keymap ['d'] = 3; keymap ['e'] = 3; keymap ['f'] = 3;
keymap ['g'] = 4; keymap ['h'] = 4; keymap ['i'] = 4;
keymap ['j'] = 5; keymap ['k'] = 5; keymap ['l'] = 5;
keymap ['m'] = 6; keymap ['n'] = 6; keymap ['o'] = 6;
keymap ['p'] = 7; keymap ['q'] = 7; keymap ['r'] = 7; keymap ['s'] = 7;
keymap ['t'] = 8; keymap ['u'] = 8; keymap ['v'] = 8;
keymap ['w'] = 9; keymap ['x'] = 9; keymap ['y'] = 9; keymap ['z'] = 9;

var nibble = "0123456789abcdef";

function get_next_word()
{
	if( dict_idx == dictionary[my378.charAt(0)].length )
		dict_idx = 0;
	
	var len = nibble.indexOf( dictionary[my378.charAt(0)].charAt( dict_idx++ ) );
	var same = nibble.indexOf( dictionary[my378.charAt(0)].charAt( dict_idx++ ) );
	
	var word = dict_buffer.substring( 0, same ) + dictionary[my378.charAt(0)].substring( dict_idx, dict_idx + len);
	
	dict_buffer = word;
	dict_idx += len;
	
	return word;
}

function compare_translated( s1, s2 )
{
	var i = 0;
	
	while( i < s1.length )
	{
		if( s1.charAt(i) != keymap[s2.charAt(i)] )
			return -1;
		i++;
	}
	return 0;
}

function predict_match_next( s, exact )
{
	do
	{
		var w = get_next_word();
		if( w && !compare_translated( s, w ) )
			if( !exact || s.length == w.length )
				return w;
	} while( dict_idx < dictionary[my378.charAt(0)].length );

	return "";
}

function trapkey(e)
{
	var keynum

	if( window.event ) // IE
	{
		keynum = e.keyCode
	}
	else if( e.which ) // Netscape/Firefox/Opera
	{
		keynum = e.which
	}

	switch( String.fromCharCode( keynum ).charAt( 0 ) )
	{
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			my378 += String.fromCharCode( keynum );
		case '#':
			break;
		case '*':
			my378 = my378.substr(0, my378.length - 1);
			break;
		case '0':
			if( document.getElementById( "lib378" ).style.visibility != "hidden" )
				insertAtCursor( e.target, match );
			else
				insertAtCursor( e.target, ' ' );
			document.getElementById( "lib378" ).style.visibility = "hidden";
			my378 = "";
			dict_idx = 0;
			return false;
		default:
			return true;
	}

	match = predict_match_next( my378, true );

	if( match )
		document.getElementById( "lib378" ).style.background = "#00ff00";
	else
	{
		match = predict_match_next( my378, false );

		if( match )
			document.getElementById( "lib378" ).style.background = "yellow";
		else
			document.getElementById( "lib378" ).style.background = "red";
	}
	document.getElementById( "lib378" ).innerHTML = "<b>" + match.substr( 0, my378.length ) + "</b>" + match.substr( my378.length );
	document.getElementById( "lib378" ).style.visibility = "visible";
	//document.getElementById( "lib378" ).style.left = findPos(e.target)[0];
	document.getElementById( "lib378" ).style.top = findPos(e.target)[1];

	return false;
}

function lib378_init()
{
	document.write('<div id="lib378" style="background: yellow; color: black; border: 2px solid black; font: 16px arial,serif; position: fixed; bottom: 0px; left: 0px; width: 100%; height: 1em; visibility: hidden; text-align: center;"></div>');
}

document.onload = lib378_init();
document.onkeypress = trapkey;

/* 3rd party functions */

// http://www.quirksmode.org/js/findpos.html
function findPos(obj) {
	var curleft = curtop = 0;
	if (obj.offsetParent) {
		curleft = obj.offsetLeft
		curtop = obj.offsetTop
		while (obj = obj.offsetParent) {
			curleft += obj.offsetLeft
			curtop += obj.offsetTop
		}
	}
	return [curleft,curtop];
}

// adapted from: http://www.laviska.com/view.php?id=138
function insertAtCursor(obj, text) {

	var pos = obj.selectionStart; // Try to get cursor position
	var scrollPos = obj.scrollTop // Save scroll position
	
	if( document.selection ) {
		// IE Method
		// Force focus on the intended field
		if( !obj.focus() ) obj.focus();
		document.selection.createRange().text=text;
	} else {
		// Standards Compliant Method (Firefox, Opera, ...)
		obj.value = obj.value.substr(0, pos) + text + obj.value.substr(pos);
		obj.selectionStart = pos + text.length;
		obj.selectionEnd = pos + text.length;
	}
	
	// Reset scroll position
	obj.scrollTop = scrollPos;
	
}