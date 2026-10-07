/* See LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <X11/Xlib.h>

#include "dwm.h"

void
arrange(Monitor *m)
{
	if (m)
		showhide(m->stack);
	else for (m = mons; m; m = m->next)
		showhide(m->stack);
	if (m) {
		arrangemon(m);
		restack(m);
	} else for (m = mons; m; m = m->next)
		arrangemon(m);
}

void
arrangemon(Monitor *m)
{
	strncpy(m->ltsymbol, m->lt[m->sellt]->symbol, sizeof m->ltsymbol);
	m->ltsymbolw = TEXTW(m->ltsymbol);
	if (m->lt[m->sellt]->arrange)
		m->lt[m->sellt]->arrange(m);
}

void
monocle(Monitor *m)
{
	unsigned int n = 0;
	Client *c;

	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;
	if (n > 0) { /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
		m->ltsymbolw = TEXTW(m->ltsymbol);
	}
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
}

void
restack(Monitor *m)
{
	Client *c;
	XEvent ev;
	XWindowChanges wc;

	drawbar(m);
	if (!m->sel)
		return;
	if (m->sel->isfloating || !m->lt[m->sellt]->arrange)
		XRaiseWindow(dpy, m->sel->win);
	if (m->lt[m->sellt]->arrange) {
		wc.stack_mode = Below;
		wc.sibling = m->barwin;
		for (c = m->stack; c; c = c->snext)
			if (!c->isfloating && ISVISIBLE(c)) {
				XConfigureWindow(dpy, c->win, CWSibling|CWStackMode, &wc);
				wc.sibling = c->win;
			}
	}
	XSync(dpy, False);
	while (XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}

void
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc)
{
	unsigned int n, oe, ie;
	Client *c;

	oe = ie = enablegaps;
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (smartgaps && n == 1)
		oe = 0;   /* внешний отступ выключается для одного окна */

	*oh = gappx * oe;
	*ov = gappx * oe;
	*ih = gappx * ie;
	*iv = gappx * ie;
	*nc = n;
}

void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? (m->ww - 2 * ov - ih) * m->mfact : 0;
	else
		mw = m->ww - 2 * ov;

	for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my - 2 * ov - (MIN(n, m->nmaster) - i - 1) * iv) /
			    (MIN(n, m->nmaster) - i);
			resize(c, m->wx + ov, m->wy + ov + my,
			       mw - (2 * c->bw), h - (2 * c->bw), 0);
			my += HEIGHT(c) + iv;
		} else {
			h = (m->wh - ty - 2 * ov - (n - i - 1) * iv) / (n - i);
			resize(c, m->wx + ov + mw + ih, m->wy + ov + ty,
			       m->ww - 2 * ov - mw - ih - (2 * c->bw),
			       h - (2 * c->bw), 0);
			ty += HEIGHT(c) + iv;
		}
}
