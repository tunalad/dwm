/* See LICENSE file for copyright and license details. */
/*
 * taglist: while the mod key is held, pressing j/k steps focus exactly like
 * dwm normally does, with a small overlay listing the windows on the current
 * tag and the focused one highlighted.
 *
 * The overlay lives only while the mod key is held; releasing it (or any
 * mouse button/scroll) dismisses it. Pressing any other bound hotkey closes
 * the overlay and runs that hotkey normally. Pointer events are replayed so
 * mouse binds (mod+drag, bar clicks, ...) behave exactly as usual.
 */

static void taglistend(Drw *drawn, Window win);
void taglistdraw(Drw *drawn, Window win);

void
taglist(const Arg *arg)
{
	KeyCode modkey[] = { XKeysymToKeycode(dpy, XK_Super_L),
	                     XKeysymToKeycode(dpy, XK_Super_R) };
	XSetWindowAttributes wa = {0};
	Window win;
	Drw *drawn;
	Client *c;
	XEvent ev;
	KeySym keysym;
	unsigned int i;
	int n = 0;

	if (!selmon)
		return;
	for (c = selmon->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;
	if (!n)
		return;
	if (!selmon->sel)
		return;

	/* fire the window change first, like the original mod+j/k behavior */
	focusstack(arg);

	/* keyboard async: key repeat keeps working; pointer sync: needed to
	 * replay clicks to their real destination */
	if (XGrabKeyboard(dpy, root, True, GrabModeAsync, GrabModeAsync,
	                  CurrentTime) != GrabSuccess)
		return;
	if (XGrabPointer(dpy, root, False, ButtonPressMask | ButtonReleaseMask,
	                 GrabModeSync, GrabModeAsync, None,
	                 cursor[CurNormal]->cursor, CurrentTime) != GrabSuccess) {
		XUngrabKeyboard(dpy, CurrentTime);
		return;
	}

	wa.override_redirect = True;
	wa.event_mask = ExposureMask;
	wa.background_pixel = scheme[SchemeNorm][ColBg].pixel;
	win = XCreateWindow(dpy, root, 0, 0, 100, 100, 0,
	                    DefaultDepth(dpy, screen), CopyFromParent,
	                    DefaultVisual(dpy, screen),
	                    CWOverrideRedirect | CWEventMask | CWBackPixel, &wa);
	drawn = drw_create(dpy, screen, win, 100, 100);
	if (!drawn) {
		XDestroyWindow(dpy, win);
		XUngrabKeyboard(dpy, CurrentTime);
		XUngrabPointer(dpy, CurrentTime);
		return;
	}
	drw_setfontset(drawn, drw->fonts);
	drw_setscheme(drawn, scheme[SchemeNorm]);

	taglistdraw(drawn, win);
	XMapRaised(dpy, win);

	for (;;) {
		XNextEvent(dpy, &ev);
		switch (ev.type) {
		case KeyPress:
			keysym = XKeycodeToKeysym(dpy, ev.xkey.keycode, 0);
			if (IsModifierKey(keysym))
				break; /* ignore modifier presses while holding the mod */
			for (i = 0; i < LENGTH(keys); i++)
				if (keysym == keys[i].keysym
				&& CLEANMASK(keys[i].mod) == CLEANMASK(ev.xkey.state)
				&& keys[i].func)
					break;
			if (i < LENGTH(keys)) {
				if (keys[i].func == taglist) {
					/* j/k: keep cycling, move the highlight */
					focusstack(&keys[i].arg);
					taglistdraw(drawn, win);
				} else {
					/* any other hotkey: close overlay, run it */
					taglistend(drawn, win);
					keys[i].func(&keys[i].arg);
					return;
				}
			} else {
				/* unbound key combo: close the overlay */
				taglistend(drawn, win);
				return;
			}
			break;
		case KeyRelease:
			if (ev.xkey.keycode == modkey[0] || ev.xkey.keycode == modkey[1]) {
				taglistend(drawn, win);
				return;
			}
			break;
		case ButtonPress:
		case ButtonRelease:
			/* dismiss and let the click reach its normal target */
			XAllowEvents(dpy, ReplayPointer, CurrentTime);
			taglistend(drawn, win);
			return;
		case Expose:
			if (ev.xexpose.window == win)
				taglistdraw(drawn, win);
			break;
		}
	}
}

void
taglistdraw(Drw *drawn, Window win)
{
	Client *c;
	int w = 0, h, y, n = 0;

	for (c = selmon->clients; c; c = c->next)
		if (ISVISIBLE(c)) {
			n++;
			w = MAX(w, drw_fontset_getwidth(drw, c->name));
		}
	if (!n)
		return;

	w = MIN(MAX(w + lrpad, 160), selmon->ww - 20);
	h = n * bh;

	XMoveResizeWindow(dpy, win,
	                  selmon->wx + (selmon->ww - w) / 2,
	                  selmon->wy + (selmon->wh - h) / 2,
	                  w, h);
	drw_resize(drawn, w, h);
	XRaiseWindow(dpy, win);

	y = 0;
	for (c = selmon->clients; c; c = c->next) {
		if (!ISVISIBLE(c))
			continue;
		drw_setscheme(drawn,
		              (c == selmon->sel) ? scheme[SchemeSel] : scheme[SchemeNorm]);
		drw_text(drawn, 0, y, w, bh, lrpad / 2, c->name, 0);
		y += bh;
	}
	drw_setscheme(drawn, scheme[SchemeNorm]);
	drw_map(drawn, win, 0, 0, w, h);
}

static void
taglistend(Drw *drawn, Window win)
{
	drawn->fonts = NULL; /* fontset is shared with the global drw */
	drw_free(drawn);
	XUnmapWindow(dpy, win);
	XDestroyWindow(dpy, win);
	XUngrabKeyboard(dpy, CurrentTime);
	XUngrabPointer(dpy, CurrentTime);
}