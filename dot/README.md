* `.profile`
  Sets environment settings, has special config for console, etc
* `.interactive`
  Sets interactive shell settings (a manual process)
* `.remote_profile`
  A special version of .profile for copying to frequently accessed
  remote systems.

* `.lynxrc`
  Lynx doesn't handle as many sites as other text mode browsers, but I
  like it's keybindings better than (say) elinks.

* `.tmux.conf`
  `screen` is okay, `tmux` is a bit nicer

* `.vimrc`
  I can type the most important bits from memory quickly but this has
  a few other nice-to-haves.

* `.xinitrc`
  A script to start initial X11 clients.

* `.sxhkdrc`
  Simple X hotkey daemon, sometimes started in .xinitrc

* `.xbindkeysrc`
  Another X hotkey daemon, sometimes started in .xinitrc

* `.screw-numlock.xmodmap`
  `xmodmap` config to remove capslock/numlock as modifiers, make those
  keys escape and tab respectively. Then it cleans up keypad keys to
  always act as if numlock was on.

* `.Xdefaults`
  X11 resource settings, mostly for `xterm`

* `.gitconfig`
  Credential caching and no color diffs

* `config/gtk-3.0/bookmarks`
  File chooser bookmarked directories

* `config/user-dirs.dirs`
  Customize the "well known" user directories of the Freedesktop standard.
