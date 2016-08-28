#UAPaint-NDS

An upload of my original code for a project I released back in 2008 prior to any formal education.

![Alt text](/screenshots/uamainmenu.png?raw=true "Zoomed grid overlay")
![Alt text](/screenshots/neoflash2009.png?raw=true "Neoflash 2009 screenshot")
![Alt text](/screenshots/filledcircle.png?raw=true "Filled circles with alpha")

##Original Readme:

-------UA Paint-------
  Uber Awesome Paint
	   By
     BassAceGold

Special thank's to:
-Salz` for the title animation 
-Sarra, Hitmanthe3rd, Bio_Hazard, Sho, C_H_R_I_S and others on IRC for collecting/creating stamps


A small paint program for pixel art.

Features:
-Saving and Loading of 24 bit bitmap images 256X192 or less
-zooming & grid when fully zoomed
-Multiple draw sizes
-colors
-fill tool
-line tool
-rectangle tool
-circle tool
-copy and paste with color transparency supported
-eraser
-Supports D-Pad Mode for plotting pixels  (When fully zoomed)
-10 pallets ready for customization 
-15 levels of undo!
-color editor: Supports all colors the hardware can support!
-Custom pallet save support (press Enter on the keyboard to save pictures and pallets)
-Guitar Hero On Tour controller support as tool shortcut buttons : Enable in Options
-typewriter mode for writing text
-stamp tool

---Installation---
-DLDI patch NDS file
-Copy NDS file to SD card
-Copy UAPaint Folder to anywhere on the card


It auto creates the folder for pictures when you save. If it doesn't work for some odd reason 
make a folder called "pics" on the root of your card.

---Controls---
Can can be swapped in options
-L: Swap pallet
-R:Switch Screens

Can't be changed
-Select:toggle grid, only when fully zoomed
-Y:undo
-A:plot pixel in D-pad mode
-X:Switch between D-pad and Stylus mode
-Start:Options menu
-Dpad: Scrolls background when in Stylus mode & zoomed in, Moves pixel cursor in D-pad mode
-B: if zoomed all the way and you want to veiw work unzoomed, hold B

--Guitar Hero Buttons--
Controller must be toggled in options menu
-Blue: pen
-Yellow: bucket, 
-Red: color picker
-Green: line tool

--Color Editor--
To enter this, switch to the top screen and tap the two pallet icon at the top right
Using the Settings you defined in options (L & R swap or not) you can switch screens and use the stylus to pick a color or use the slider bars,
once you have your color picked, you can add it to a custom pallet set by tap the desired sqaure on the slider screen. Once you go back, you will
find your colors in the pallet bar to the bottom left. You can tap the color you want from there.

--Copy and Paste Tool--
To use, select the tool at the top right of the top screen.Then, while holding the stylus, drag a box 
around what you want to copy. Two buttons popup, you can select copy to copy the object or cancel to exit the menu.
Once you select Copy, tap the screen and your object should appear, you can drag it around on screen. Once its in
position, press the "Set" button and it should be pasted! To paste again, tap the screen and select paste, or 
press cancel to go back to copying.

---Stamps----
!!!G6 Real users!!! Stamp Menu may load very slow due to very slow read/write speeds of internal memory.

If you choose to make stamps for UA Paint, the transparent color is 31,0,31 on the DS or 255,0,255 on PC and all 
stamps must be placed in X/UAPaint/stamps


Release History--------------------------------------------------------------------------------------------------
Version 1.20
-added no fill rectangle tool

Version 1.16
-fixed grid dissapearing on returning from full screen view in zoom
-fixed typewriter text dissapearing bug when moving it on screen
-fixed stamp and pasted image dissapearing bug when moving it in some areas of the screen
-fixed GH controller support
-more code cleanup

Version 1.15
-changed handling of directories (shouldn't freeze when saving on some cards now)
-fixed screen shake with the circle tool
-code clean up


Version 1.14
-optimised picture loading


Version 1.11
-fixed the character limit on the typewriter menu
-fixed saving bug
-fixed saved picture display glitching in load menu
-redid text scrolling in load menu
-disabled use of forbidden characters for file saving- prevents freezing if character was used


Version 1.10
-added GH controller to options menu to prevent problems
-fixed many menu related bugs
-slightly more optimised
-fixed large saving bug
-added transparency to copy and paste functions - define transparency color in the color creator default is white
-remembers draw sizes now when switching tools
-cursor on small color strip now updates according to color picked in the color editor
-added typewriter mode
-added spray can tool
-fixed D-pad plotting mode
-added title and loading screen
-fixed a color range bug on the color screen: now allows to make the full range of colors 0-31
-added number displays for color values in the color screen
-started stamp/tile placing tool -this isn't finished so more features will be added later to this
-can start and save stamps in app
-can now fine tune tool sizes by tapping + or - buttons
-G6/M3 real support


Version 1.02
-Added Guitar Hero Buttons as  tool shortcuts - Blue= Pen, Yellow=bucket, Red=color picker, Green=Line tool
-Fixed another grid bug

Version 1.01
-Fixed pallet saving bug
-Fixed Grid problems
-prevents user from opening unsupported file sizes

Version 1.00
-Changable canvas sizes!
-Now scans card for UAPaint Directory (directory can be placed anywhere)
-Fixed black BG bug when exited pallet menu
-zoom now zooms to the last place moved too.
-streamlined copy and paste & fixed copying bugs
-added new option to options menu
-added X flip and Y flip in copy and paste options
-fixed zoom bar position
-releasing the B button when zoomed now returns to correct zoom level


Version .99e
-fixed one or two small bugs
-added saving menu with keyboard
-can save and load pallets now 
(press Enter on the keyboard to save pictures and pallets)
-added copy and paste tool
-added unzoomed veiw to B if held 
-added scrolling to loading menus.. kind of, its not the greatest but it works i guess.

Version .99d
-fixed load menu cursor bug (no longer uses stylus, and still no menu scrolling)
-added options menu (press start)
-grid can be toggled with select button
-New Stats tool, keeps track of X and Y position (color counter not completed yet)
-add color editor (touch the double pallet indicator in the top right of top screen)
-Removed Color history and replaced with custom pallets (added pallet via color editor by touching one of the ten pallet slots with selected color)
-improved D-pad mode controls (hold down to continuously move)
-fixed major saving bug

Version .99c
-D-pad mode works while on top screen
-can now switch from D-pad mode to stylus mode with X button along with select
-Undo
-Massive speed up of some draw functions

Version .99b
-Press L to swap pallets
-Added fine color picking, Use A,B,X,Y to adjust color on the picker (when on top screen)
-Added D-pad Pixel Plotting Mode when fully zoomed, press Select to activate/deactivate and press A to plot pixel
-added color history list

Version .99:
-Initial Release

