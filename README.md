# dwm-scripts
Scripts to be used with my build of [dwm](https://github.com/dk949/dwm)

### List of Scripts and what they do
* cputmp - returns cpu temperature. Is used by the statusbar
* layout-check - Takes a number as a parameter and spawns that many terminals the prints a number to each one to distinguish them. Requires out from [shellutils](https://github.com/dk949/shellutils)
* notify-lock - requires twmn (not really just that the folder ~/.config/twmn exists). Creates a lock file and deletes it after a certain time. While this file is created, no notifications can be sent. Prevents notifications from being spammed and the user having to wait for then to stop.
* picom-end - requires picom. Kills the picom process
* picom-start - starts the picom process in the background
* refbar - refreshes the startusbar by killing the sleep process it spawns
* statusbar - pulls in and formats some system information. Then with a certain interval passes it to xsetroot (which is how the status bar is set in dwm).
* touchpad - sets some touchpad properties. The comented out properties were impossible to set for my touchpad.
* turnoff - just sudo poweroff. For dwm to call with a shortcut.


### Installation
```
git clone https://github.com/dk949/dwm-scripts
cd dwm-scripts
./install
```
Note: This is the default method and it assumes that ~/bin exists, is on PATH and does not contain files named the same as files in this repo (if there are they will be removed). If you wish to install to a different drectory simply change the cd ~/bin portion of the install file to your desired directory.
