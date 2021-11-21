# SUWSF

Somewhat Universal Widescreen Fix is intended to enable widescreen aspect ratios (e.g. 21:9, 32:9, 48:9) in games where it is unsupported.

## WARNING IT IS NOT RECOMMENDED TO USE THIS FIX IN GAMES THAT CONTAIN ANTICHEAT

# Installation

Download [the latest release](https://github.com/phantomgamers/suwsf/releases/latest) and extract it so that `SUWSF.asi`, `SUWSF.ini`, and `wininet.dll` are in the same folder as the game executable that you are trying to fix.

Note that it must be alongside the actual game executable, it will not work if it is only placed alongside a game's launcher.

# Uninstall

To uninstall, delete the `SUWSF.asi`, `SUWSF.ini`, and `wininet.dll` files that you extracted to the game executable location.

# Configuration

In `SUWSF.ini` you can configure the following settings:

## [AspectRatio]

Contains settings related to setting different aspect ratios.

### TargetBytes

This should be set to the bytes of the aspect ratio that the game is in by default.

Commonly this will be 16/9 in bytes, which can either be `39 8E E3 3F` or `3B 8E E3 3F` depending on the game.

### DesiredResolution

This should be set to your screen resolution, or the resolution you have set in-game if it is different.

You can also set it to an aspect ratio in resolution format (e.g. "16x9" or "21x9")

# Troubleshooting

If this does not work for you, verify that a file named SUWSF*.log (where * is a date) is being created when you launch the game.  
If it is not, try renaming `wininet.dll` to `dsound.dll` or any other name on [this list](https://github.com/ThirteenAG/Ultimate-ASI-Loader#description) and see if the problem remains.

If you get an error such as "Unable to load SUWSF.asi. Error:126", ensure you have the latest [64bit VC Redist installed](https://aka.ms/vs/17/release/vc_redist.x64.exe), alternatively use the [VisualCppRedist AIO](https://github.com/abbodi1406/vcredist) package which includes all versions of VC redist.

# Credits

- [@DarthTon](https://github.com/DarthTon) for the Windows memory hacking library [Blackbone](https://github.com/DarthTon/Blackbone)
- [@ThirteenAG](https://github.com/ThirteenAG) for [IniReader](https://github.com/ThirteenAG/IniReader) and [UltimateASILoader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)
