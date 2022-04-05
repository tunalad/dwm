# tunalad's EPIC dwm fork
"dwm is an extremely fast, small, and dynamic window manager for X". Until I patched the hell out of it! It's still pretty fast tho, just not as small I guess.

## Requirements
- set `$TERMINAL` environment variable
- monospaced font (`adobe-source-code-pro-fonts`)
- `libx11`
- `libxft`
- `libxinerama`
- `xorg-xkbcomp`

## Installaion
```
git clone https://github.com/tunalad/dwm.git
cd dwm
sudo make clean install
```

## Patches applied
- [actualfullscreen](https://dwm.suckless.org/patches/actualfullscreen/)
- [alwayscenter](https://dwm.suckless.org/patches/alwayscenter/)
- [alwaysontop](https://dwm.suckless.org/patches/alwaysontop/)
- [attachbottom](https://dwm.suckless.org/patches/attachbottom/)
- [autostart](https://dwm.suckless.org/patches/autostart/)
- [environmentvars](https://dwm.suckless.org/patches/environmentvars/)
- [focusonnetactive](https://dwm.suckless.org/patches/focusonnetactive/)
- [fullgaps](https://dwm.suckless.org/patches/fullgaps/)
- [moveresize](https://dwm.suckless.org/patches/moveresize/)
- [movestack](https://dwm.suckless.org/patches/movestack/)
- [pertag](https://dwm.suckless.org/patches/pertag/)
- [shiftview](https://lists.suckless.org/dev/1104/7590.html)
- [systray](https://dwm.suckless.org/patches/systray/)
