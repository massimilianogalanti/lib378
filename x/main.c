#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

#include "378.h"

static l378_t my378;
static const char keymap[] = {
	['a'] = '2', ['b'] = '2', ['c'] = '2', ['d'] = '3', ['e'] = '3', ['f'] = '3', ['g'] = '4', ['h'] = '4', ['i'] = '4', ['j'] = '5', ['k'] = '5', ['l'] = '5', ['m'] = '6', ['n'] = '6', ['o'] = '6', ['p'] = '7', ['q'] = '7', ['r'] = '7', ['s'] = '7', ['t'] = '8', ['u'] = '8', ['v'] = '8', ['w'] = '9', ['x'] = '9', ['y'] = '9', ['z'] = '9'};

/* Send Fake Key Event */
static void SendKey(Display *disp, KeySym keysym, KeySym modsym)
{
	KeyCode keycode = 0, modcode = 0;

	keycode = XKeysymToKeycode(disp, keysym);
	if (keycode == 0)
		return;

	XTestGrabControl(disp, True);

	/* Generate modkey press */
	if (modsym != 0)
	{
		modcode = XKeysymToKeycode(disp, modsym);
		XTestFakeKeyEvent(disp, modcode, True, 0);
	}

	/* Generate regular key press and release */
	XTestFakeKeyEvent(disp, keycode, True, 0);
	XTestFakeKeyEvent(disp, keycode, False, 0);

	/* Generate modkey release */
	if (modsym != 0)
		XTestFakeKeyEvent(disp, modcode, False, 0);

	XSync(disp, False);
	XTestGrabControl(disp, False);
}

/* Main Function */
int main(const int argc, const char *argv[])
{
	XEvent report;
	GC gc;
	XGCValues gr_values;
	XFontStruct *fontinfo;
	int screen;
	char ascii;
	char *s = NULL;

	l378_dictionary_open(&my378, argv[1]);
	printf("Dictionary: %s\n", argv[1]);
	l378_keymap_set(&my378, keymap);

	Display *disp = XOpenDisplay(NULL);
	screen = DefaultScreen(disp);

	Window win = XCreateSimpleWindow(disp, RootWindow(disp, screen), 1, 1, 200, 42, 0, WhitePixel(disp, screen), WhitePixel(disp, screen));

	XSelectInput(disp, win, KeyPressMask);

	fontinfo = XLoadQueryFont(disp, "10x20");
	gr_values.font = fontinfo->fid;
	//gr_values.function = GXcopy;
	//gr_values.plane_mask = AllPlanes;
	gr_values.foreground = BlackPixel(disp, 0);
	gr_values.background = WhitePixel(disp, 0);

	gc = XCreateGC(disp, win, GCFont | GCForeground | GCBackground, &gr_values);

	XFillRectangle(disp, win, gc, 10, 20, 100, 100);

	XMapRaised(disp, win);

	while (1)
	{
		XDrawImageString(disp, win, gc, 0, 20, "Match: ", 7);
		XDrawImageString(disp, win, gc, 0, 40, "Code : ", 7);
		//XFlush(disp);
		XNextEvent(disp, &report);
		switch (report.type)
		{
		case KeyPress:
			XLookupString(&report.xkey, &ascii, 1, NULL, NULL);
			switch (ascii)
			{
			case 'q':
				goto out;
			case ' ':
				SendKey(disp, XK_Tab, XK_Alt_L);
				sleep(1);
				{
					int i;
					char c[] = {0, 0};

					for (i = 0; i < strlen(s); i++)
					{
						c[0] = s[i];
						SendKey(disp, XStringToKeysym(c), 0);
					}
					SendKey(disp, XK_space, 0);
				}
				sleep(1);
				SendKey(disp, XK_Tab, XK_Alt_L);
				l378_clear(&my378);
				continue;
			case '-':
				break;
			case '1':
				l378_popc(&my378);
				break;
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				l378_pushc(&my378, ascii);
				break;
			default:
				continue;
			}

			if ((s = l378_matche(&my378)))
				XDrawImageString(disp, win, gc, 80, 20, s, strlen(s));
			else if ((s = l378_matche(&my378)))
				XDrawImageString(disp, win, gc, 80, 20, s, strlen(s));

			XDrawImageString(disp, win, gc, 80, 40, my378.pattern, strlen(my378.pattern));

			XFlush(disp);
			break;
		default:
			break;
		}
	}
out:
	l378_dictionary_close(&my378);
	XDestroyWindow(disp, win);
	XCloseDisplay(disp);
	return 0;
}

/* EOF */
