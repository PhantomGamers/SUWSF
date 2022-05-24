# SUWSF

Somewhat Universal Widescreen Fix is intended to enable widescreen aspect ratios (e.g. 21:9, 32:9, 48:9) in games where it is unsupported.

## WARNING IT IS NOT RECOMMENDED TO USE THIS FIX IN GAMES THAT CONTAIN ANTICHEAT

## Installation

- **Ensure you have the latest [64bit VC Redist](https://aka.ms/vs/17/release/vc_redist.x64.exe) or [32bit VC Redist](https://aka.ms/vs/17/release/vc_redist.x86.exe) installed.**
- Download [the latest release](https://github.com/phantomgamers/suwsf/releases/latest)
- For 32bit (x86) games download SUWSF-x86.zip, for 64bit (x86_64) games download SUWSF-x64.zip.
- Extract it so that `SUWSF.asi`, `SUWSF.ini`, and `dsound.dll` are in the same folder as the game executable that you are trying to fix.
- If `dsound.dll` would collide with a file included in the game try a different name on [this list](https://github.com/ThirteenAG/Ultimate-ASI-Loader#description) which is marked as supporting the architecture (x86 or x64) of the game you are trying to fix.

Note that it must be alongside the actual game executable, it will not work if it is only placed alongside a game's launcher.  

## Uninstall

To uninstall, delete the `SUWSF.asi`, `SUWSF.ini`, and `dsound.dll` files that you extracted to the game executable location.  

## Configuration

In `SUWSF.ini` you can configure the following settings:  

### **[UserSettings]**

A group of settings specific to the user.

#### **Resolution**

The user's screen resolution that should be used in patch calculations.  

Valid options: A screen resolution (e.g. "3440x1440"), "auto" (automatically detect from primary screen, may be affected by DPI scaling)  
Default: "auto"  

### **[Patch]**

A group of settings related to a patch. Patches can optionally have names (e.g. `[Patch:AspectRatio]` or `[Patch:FOV]`). Names do not matter as long as they contain the word `Patch`.  
As many patch groups as needed can be used.  

#### **Enabled**

Whether a patch should be enabled. Use this for optional patches that users can choose to enable or disable.

Valid options: "true", "false"  
Default: "true"  

#### **Pattern**

This should be set to a pattern that matches the bytes you would like to patch.  

Commonly this will be 16/9 in bytes, which can either be `39 8E E3 3F` or `3B 8E E3 3F` depending on the game.  

Wildcards are supported for bytes that can vary between game versions (e.g. `39 8E ?? 3F`).  

#### **Offset**

The offset from the pattern match that should be written. Use this if you need to include bytes before the thing you are trying to patch.  
e.g. to get to `39` in `90 90 39 8E E3 3F` set to 2  

Default: 0  

#### **Value**

The value to write to the matched bytes.  

If `ValueType` is `float` then this can contain numbers or included variables. This can also contain an expression!  
If `ValueType` is `byte` then this should contain an array of bytes! e.g. `90 90 90 90`  

Included variables:  
`aspectratio` - This will be replaced with the user's screen's aspect ratio.  
`width` - This will be replaced with the user's screen's width.  
`height` - This will be replaced with the user's screen's height.  

Example:  
`Value="width/height"`  

#### **ValueType**

What type the Value setting contains.  

Valid options: "float", "double", "int", "byte"  
Default: `float`  

#### **Match**

Which match should be targeted. Useful for cases where a pattern can have multiple matches but only one should be changed.  

Valid options: number of match (starting from 1), "last", "all"  
**Note that `last` might match different bytes than the last match in a hex editor.** This is because SUWSF works from memory while a hex editor works on the file on disk.  
Default: "all"  


#### **Module**

Which file should be targeted by patch. Useful for cases where the patch needs to be applied to a file other than the main game executable. (e.g. "UnityPlayer.dll")  

Valid options: "auto" (detects main game executable name), any string.  
Default: "auto"  

## Troubleshooting

If you get an error such as "Unable to load SUWSF.asi. Error:126", ensure you have the latest [64bit VC Redist](https://aka.ms/vs/17/release/vc_redist.x64.exe) or [32bit VC Redist](https://aka.ms/vs/17/release/vc_redist.x86.exe) installed depending on which version of SUWSF you are trying to use.

If this does not work for you, try renaming `dsound.dll` to `dinput8.dll` or any other name on [this list](https://github.com/ThirteenAG/Ultimate-ASI-Loader#description) and see if the problem remains.

`version.dll` is a good alternative to try.

## Credits

- [@ThirteenAG](https://github.com/ThirteenAG) for [Hooking.Patterns](https://github.com/ThirteenAG/Hooking.Patterns), [IniReader](https://github.com/ThirteenAG/IniReader) and [UltimateASILoader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)
- [@sergey-shandar](https://github.com/sergey-shandar) for [getboost](https://github.com/sergey-shandar/getboost)
- [@codeplea](https://github.com/codeplea) for [tinyexpr](https://github.com/codeplea/tinyexpr)
