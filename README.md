<p align="center">
  <img src="./icons/OAWP_logo_row_v2.svg" width="50%">
</p>

# Open Animated Wallpaper Player

Open Animated Wallpaper Player - An animated wallpaper player made to be easily configurable, be fast and work in compositors

OAWP is based on AlecsFerra's animated_wallpaper.c [github gist](https://gist.github.com/AlecsFerra/ef1cc008990319f3b676eb2d8aa89903).<br>
This is an alternative for [glouw/paperview](https://github.com/glouw/paperview) which does not rely on use of SDL2.

### Why using OAWP instead of paperview?

* It can work in compositors
* Configurable
* X11/Xorg support
* ~~Wayland support~~
* ~~Microsoft Windows support~~
* ~~Apple macOS support~~

### Why not just use AlecsFerra's animated_wallpaper.c?

Unlike AlecsFerra's animated_wallpaper.c, OAWP can be installed more easily, ~~supports more operating systems and display servers~~, can use CPU ~~or GPU~~ rendering instead of just CPU rendering and can be configured directly in ~/.config/OAWP/OAWP.conf instead of modifying the source code line by line.<br>
For example, instead of adding the path of the wallpaper files in the source code, the user can add the path of folder containing the animated wallpaper picture frames with the following options:

1. By writing the directory into the configuration file
2. By appending to the command an argument to the directory

## Dependencies:

* sh
* CMake
* GCC
* Make (GMake for BSD)
* Imlib2
* libX11
* libconfig
* lcat-rs/roflcat/lolcat (Optional)

## TODO:

* [x] Add arguments
* [x] Add configuration file
* [ ] Add fit
* [ ] Automate ImageMagick picture converting from gif to other formats with user prompt
* [ ] Add major desktop environments support
* [ ] Documentation and populate this README.md file and wiki

## Wiki

Get help from the [wiki](https://github.com/OAWP/OAWP/wiki)!

## Quick note!

* At the moment, OAWP is still buggy and it's advised to use the [stable version](https://github.com/OAWP/OAWP/tree/stable) if you are not developing for OAWP.
* All striked features from above are missing features that will be implemented into OAWP.
