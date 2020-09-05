# dwm-scripts
Scripts to be used with my build of [dwm](https://github.com/dk949/dwm)

### List of Scripts and what they do
* cputmp - returns cpu temperature. Is no longer used as part od the dwm build
* notify-lock - requires twmn (not really just that the folder ~/.config/twmn exists). Creates a lock file and deletes it after a certain time. While this file is created, no notifications can be sent. Prevents notifications from being spammed and the user having to wait for then to stop
* picom-end - requires picom. Kills the picom process
* picom-start - starts the picom process in the background
* refbar - refreshes the startusbar by killing the sleep process it spawns
* statusbar - pulls in and formats some system information. Then with a certain interval passes it to xsetroot (which is how the status bar is set in dwm).
* touchpad - sets some touchpad properties. The comented out properties were impossible to set for my touchpad.
* turnoff - just sudo poweroff. For dwm to call with a shortcut.
* wpa_supplicant_wrapper - On my system I have moved wpa_supplicant from /usr/bin to usr/local/bin. There I created this wrapper which sends stdout from wpa_supplicant to /dev/null. then the wrapper is symlinked back to /usr/bin/wpa_supplicant. If there is a better way to silence this thing on boot, i haven't found it yet.


### Installation
```
git clone https://github.com/dk949/dwm-scripts
cd dwm-scripts
./install
```
Note: This is the default method and it assumes that ~/bin exists, is on PATH and does not contain files named the same as files in this repo (if there are they will be removed). If you wish to install to a different drectory simply change the cd ~/bin portion of the install file to your desired directory.
