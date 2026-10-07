# macOS feel on top of this dwm

Nothing here is compiled into dwm. Every item is a separate, optional process,
so dwm itself stays small and fast. If a tweak costs CPU while idle, skip it.

## Key map shipped in `config.def.h` / `config.h`

`Super` plays the role of `Cmd`. `Ctrl` is **not** remapped: Linux apps expect
`Ctrl+C/V/X`. If you want `Cmd+C/V/X`, do that in your terminal emulator's
key bindings, not globally.

| Keys | Action |
|---|---|
| `Super+Space` | launcher (`dmenu_run`) |
| `Super+Tab` / `Super+Shift+Tab` | next / previous client (list order, not most-recently-used) |
| `Super+Q`, `Super+W` | close focused client |
| `Super+Ctrl+Q` | lock screen (`slock`) |
| `Super+Shift+S` | region screenshot (`maim -us`) |
| `Super+Ctrl+Shift+S` | full-screen screenshot (`maim -u`) |
| `Super+Ctrl+Space` | emoji picker (`rofi -show emoji`, needs the rofi-emoji plugin) |
| `Super+Shift+V` | clipboard history (`clipmenu`, needs `clipmenud` running) |
| `Super+1..9` etc. | tags, unchanged (these are dwm's "Spaces") |
| `` Super+` `` | back to the previous tag (this was `Super+Tab` before) |

`Super+Shift+3/4` are **not** screenshots: `TAGKEYS` already uses
`Super+Shift+<digit>` to move a window to a tag, and dwm runs every matching
binding, so both would fire.

## Spotlight: launcher

dmenu is the default and the cheapest. On a capable machine,
`rofi -show drun` gives icons and fuzzy search. On a weak machine stay with
dmenu: rofi is a GTK-free but still much heavier process to start.

## Mission Control: window overview

- `rofi -show window` is a plain list of windows, cheap and good enough.
- `skippy-xd` draws a thumbnail overview. It needs a compositor-style
  capture of every window, so expect a noticeable pause on old GPUs.

Bind either from `config.h` with, for example,
`{ MODKEY, XK_e, spawn, SHCMD("rofi -show window") },`.

## Notifications

`dunst` is a single small daemon. Style it in `~/.config/dunst/dunstrc`
(`font`, `frame_width`, `corner_radius`, `origin`, `width`; see `dunst(5)`).
Keep the timeouts short and animations off.

## Clipboard history

Start the daemon from your session (`~/.xinitrc`): `clipmenud &`.
`clipmenu` then pops up the history. It uses dmenu by default; set
`CM_LAUNCHER=rofi` in the environment if you prefer rofi.

## Screenshots

`maim` takes the picture and `slop` provides the region selection. Files go
to `/tmp` and disappear on reboot; change the paths in the `screenshot_*`
arrays in `config.h` if you want to keep them. To copy to the clipboard
instead, pipe maim into `xclip -selection clipboard -t image/png`.

## Trackpad

- Natural scrolling is a libinput option, no daemon needed. In
  `/etc/X11/xorg.conf.d/40-libinput.conf`, inside the touchpad
  `InputClass`: `Option "NaturalScrolling" "true"`.
- `libinput-gestures` can turn three-finger swipes into key presses, e.g.
  `gesture swipe left 3 xdotool key super+Right` in
  `~/.config/libinput-gestures.conf` (your user must be in the `input`
  group). **Caveat:** stock dwm has no "next/previous tag" action, so
  `Super+Left/Right` do nothing until you add one. The suckless `shiftview`
  patch is the usual way. It is not applied in this tree on purpose.

## Compositor (only if you must)

Skip it if you do not see tearing. If you do, use picom with the cheapest
settings:

    picom --backend xrender --vsync

and in `picom.conf`: `shadow = false;`, `fading = false;`, and no `blur-*`
options. **Blur and animations destroy performance on weak hardware**; they
are the reason people leave heavier desktops.

## Look and feel

- `WhiteSur` GTK theme and icon theme for consistent widgets.
- `Inter` (SIL OFL) is a good UI font. SF Pro is Apple's and its license
  restricts use and redistribution, so only use it where you are allowed to.

## Do not do this

- **Global menu** via `vala-panel-appmenu`: D-Bus heavy and fragile with
  non-GTK apps.
- **Auto-hiding bar** driven by repeated `XMoveWindow` calls at 60 Hz: it
  keeps the CPU busy while the bar is moving and fights dwm's own layout.
- **rofi on very weak machines**: use dmenu.
- Remapping `Ctrl` globally (e.g. with xmodmap) to imitate `Cmd`: it breaks
  every terminal and many apps.
