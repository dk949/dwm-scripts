# dwm-scripts
Scripts to be used with my build of [dwm](https://github.com/dk949/dwm)

### List of Scripts and what they do
* layout-check - Takes a number as a parameter and spawns that many terminals the prints a number to each one to distinguish them. Requires out from [shellutils](https://github.com/dk949/shellutils)
* makebg - requires feh. Sets the backgorund with feh.
* notify-lock - requires twmn (not really just that the folder ~/.config/twmn exists). Creates a lock file and deletes it after a certain time. While this file is created, no notifications can be sent. Prevents notifications from being spammed and the user having to wait for then to stop.
* picom-end - requires picom. Kills the picom process
* picom-start - starts the picom process in the background
* turnoff - just sudo poweroff. For dwm to call with a shortcut.
* update-variables - checks for new updates (with pacman) and caches the result in cache files which slstatus can access.


### Installation
```
git clone https://github.com/dk949/dwm-scripts
cd dwm-scripts
make install
```
Note: This is the default method and it assumes that $HOME/.local/bin  exists (it will be created if not) and is on PATH. If you wish to install to a different directory change `DESTDIR` and/or `PREFIX` variables in config.mk.
