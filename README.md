# dwm-scripts

Scripts to be used with my build of [dwm](https://github.com/dk949/dwm)

### List of Scripts and what they do

* layout-check - Takes a number as a parameter and spawns that many terminals.
  Prints a number to each one to distinguish them. Requires out from
  [shellutils](https://github.com/dk949/shellutils).
* makebg - requires feh. Sets the background with feh.
* picom-end - requires picom. Kills the picom process.
* picom-start - starts the picom process in the background.
* turnoff - just sudo poweroff. For dwm to call with a shortcut.
* update-variables - checks for new updates (with pacman) and caches the result
  in cache files which slstatus can access.

### Installation

``` sh
git clone https://github.com/dk949/dwm-scripts
cd dwm-scripts
make install
```

Note: This is the default method and it assumes that $HOME/.local/bin exists (it
will be created if not) and is on PATH. If you wish to install to a different
directory change `DESTDIR` and/or `PREFIX` variables in config.mk.
