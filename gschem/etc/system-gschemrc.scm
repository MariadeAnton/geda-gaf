;-*-Scheme-*-
;
; Init file for gschem
;

;
;  WARNING: Make a backup copy of this file before making any changes!
;

;  ;'s are comments
;  keywords are case sensitive (guile feature)
;  mode strings are case sensitive
;  colors are not case sensitive
;
;  If an integer is out-side of the valid range, the corresponding limit
;  will be used and a warning issued.
;
; In case of difficulties, start gschem from a shell, as oppose to starting
; from a GUI. Then look for error messages and double check the parameter(s)
; listed in the message. If there are multiple messages, be sure to scroll
; back and look for the "first" error as this is often the only problem.
;
; A common symptom that appears when there is a problem with this file is
; "no menus" and a sensitivity error on  non-existent menu item. This is
; indicative of a syntax error in this file. Maybe one of the easiest mistake
; is to use a colon instead of a semi-colon and no useful information is
; provided for locating the problem. Try using an Editor like gedit or Kate
; with search high-lighting. There are not that many colons in this file and
; none of them should be the first character on a line.
;
; gschem-version string
;
; Specifies the version of this file.  This number is used to make sure
; that the rc file is compatible with the version of gschem that is
; being run. The end user should *not* change this value.
;

(gschem-version "@DATE_VERSION@")

;
; Start of color section
;

; Make the gschem color maps more user-friendly
(color-map-make-friendly display-color-map)
(color-map-make-friendly display-outline-color-map)

; Load up a color scheme which has a dark (black) background.
; Comment out the first line and comment in the second line for a
; light (almost white) background.  The dark background is the
; original look.
;
(load (build-path geda-rc-path "gschem-colormap-darkbg")) ; dark background
;(load (build-path geda-rc-path "gschem-colormap-lightbg")) ; light background
;(load (build-path geda-rc-path "gschem-colormap-bw")) ; light background, bw
;(load (build-path geda-rc-path "gschem-colormap-custom"))

; image-color string
;
; Controls if images (png, jpg, etc.) are color (enabled) or black/white
; (disabled). If disabled images will be generated in black and white.
; When enabled, colorable image types will be generated using the active
; color map scheme, (which could be black/white).
;
;(image-color "enabled")
;(image-color "disabled")

; invert-images string
;
; When image-color is disabled, invert-images will black and white so that
; images are black-on-white when enabled (default). When disabled, generated
; images will be the same as the black and white screen image; white-on-
; black (like the old gschem style).
;(invert-images "enabled")
(invert-images "disabled")

;
; End of color section
;

;
; Start of mode related keywords
;

;BEGIN =================> Display Configuration <===================
;
; draw-grips string
;
; Controls if the editing grips are drawn when selecting objects
;
;(draw-grips "enabled")
;(draw-grips "disabled")

;BEGIN ----------------------> Grid Setup <-------------------------
;
; grid-mode string
;
; Controls displaying of the grid and grid type.
;
;(grid-mode "none")
;(grid-mode "dots")
(grid-mode "mesh")

; dots-grid-dot-size integer
;
; The dots-grid-dot-size keyword controls the size of the grid dots in the
; dots grid display. The units are in pixels. The default (min) value of 1
; is the best performing as the grid dot size is rendered as a single pixel.
; Values of 2 and 3 are good values to try if the default grid dot size is
; too small for your tastes. Anything larger than 3 is probably too large.
;
;(dots-grid-dot-size 1)
(dots-grid-dot-size 2)
;(dots-grid-dot-size 3)

; dots-grid-fixed-threshold integer
;
; The dots-grid-fixed-threshold specifies the minimum number of pixels
; grid-spacing for the grid to be displayed. Using this parameter you can
; control the density of the displayed grid (smaller numbers will cause the
; grid to be drawn denser). This mode is only used when grid-mode is fixed.
;
(dots-grid-fixed-threshold 10)

; dots-grid-mode string
;
; The dots-grid-mode keyword controls the mode of the dotted grid, either
; variable or fixed. In the variable mode, the grid spacing changes
; depending on the zoom factor. In the fixed mode, the grid always
; represents the same number of units as the snap-spacing. You can
; control the density of the grid using the dots-grid-fixed-threshold.
(dots-grid-mode "variable")
;(dots-grid-mode "fixed")

; mesh-grid-threshold integer
;
; The mesh-grid-threshold specifies the minimum line pitch for the
; grid to be displayed. Using this parameter you can control maximum density
; of the displayed before the minor, then major grid-lines are switched off.
;
(mesh-grid-threshold 25)

;END ------------------------> Grid Setup <-------------------------

; object-clipping string
;
; Determines if the object clipping code is executed or not
; This code does speed up redraws a bit. Possible options are enabled
; or disabled. Clipping should not normally be turned off, since some
; X servers don't handle clipping correctly.
;
(object-clipping "enabled")
;(object-clipping "disabled")

; window-size integer integer
;
; window-size width height
;
; Specifies the size of the drawing area window.  The width and height
; are specified in pixels and do not include the three menu bars and
; scrollbars (so the window will be larger than the specified
; measurements). Try to keep an aspect ratio of 1.333333 if at all possible.
; These numbers are NOT the true size of the window, but of the drawing area.
;
;(window-size 650 487)  ; Good size for 800x600
(window-size 900 650)  ; Good size for 1024x768
;(window-size 950 712)  ; Good size for 1152x864
;(window-size 1100 825) ; Good size for 1280x1024

; world-size width height border
;
; Specifies the size of the world and a border (in world space units)
; Be sure all inputs are reals (floats/doubles) and don't try to reverse
; the values to get a portrait mode.  Code to support that needs to be added
; The code that implements this automatically transforms the dimensions into
; the proper aspect ratio.  All units are in inches.
; This is not the paper size.  That is specified elsewhere.  End users should
; not change this at all.
;
;(world-size 60.0 45.0 1.0)
(world-size 120.0 90.0 1.0)
;(world-size 180.0 135.0 1.0)

;BEGIN -------------------> Display Zoom Setup <--------------------

; warp-cursor string
;
; Controls if the cursor is warped (or moved) when you zoom in and out.
; Some people find this forced cursor movement annoying.
;
;(warp-cursor "enabled")
(warp-cursor "disabled")

; zoom-gain integer
;
; Controls the percentage size increase when zooming into the page.
; Un-zooming uses the inverse factor such that a zoom in / zoom out
; pair will return the schematic to the same size.
;  E.g.
;    20% increment => x 1.2 original size when zooming in
;                  => x 1 / 1.2 x original size when zooming out
;
(zoom-gain 20)
;;(zoom-gain 50) ; Hard-coded behaviour up to version 1.5.0.20080706

; zoom-with-pan string
;
; Sets the zoom in and zoom out functions to pan the display and then zoom.
; Basically zoom in / out where the mouse pointer sits on the display.
; Comment out if you want the default mode.
;
(zoom-with-pan "enabled")
;(zoom-with-pan "disabled")

;END ------------------->  Display Zoom Setup  <--------------------
;END ===================> Display Configuration <===================

;BEGIN ==================> Set Log Configuration <==================

; logging string or integer
;
; Determines if the logging mechanism is enabled or disabled
; Possible options are enabled or disabled
;
; See log-destiny keyword for control over where the messages go.
;
(logging "enabled")
;(logging "disabled")

; log-destiny string
;
; Specifies where log message go during run time.
; Possible options are;
;      console_window  The console window (if it's visible)
;      tty             The stdout of the terminal where gschem was run from
;      both            Both of the above locations
; Message are always written to the log file (unless logging is disabled)
; by the above keyword.
;
;
(log-destiny "console_window")
;(log-destiny "tty")
;(log-destiny "both")
;
; Controls if the console window is mapped when gschem is started up
; Possible options;
;       startup - opened up when gschem starts
;       later   - NOT opened up when gschem starts
;                 (can be opened by Options/Show console Window)
;
(console-window "enabled")
;(console-window "disabled")

; console-window-type string
;
; Controls if the console window is a transient or if it is decorated
; as a normal window (this is dependant on the window manager doing decoration
; right)
;
; Possible options;
;       decorated       - console window is a normal decorated window
;       transient       - console window is a transient dialog box, typically
;                         not decorated by the window manager
;
(console-window-type "decorated")
;(console-window-type "transient")

;END ====================> Log Configuration <======================

;BEGIN ================> Miscellaneous Options <====================

; action-feedback-mode string
;
; Set the default action feedback mode (for copy/move/component place).
; Set to outline to get an outline of the selection.
; Set to boundingbox to get a bounding box of the selection.
; For a fast machines with fast video use outline (it looks good).
; For a slow machine use boundingbox; it is much faster.
; Comment out if you want the default mode.
;
(action-feedback-mode "outline")
;(action-feedback-mode "boundingbox")

; add-attribute-offset integer
;
; This has not been implemented/debugged yet.
;
; Controls a offset which is added to the location of text items that are
; added to an object as an attribute. This offset is added when the following
; conditions occur;
;
;  1) Add/Attribute... has been invoked via the hotkey
;  2) It is the the "netname" attribute being added
;  3) It is being attached to a horizontal or vertical net segment
;  4) The initial mouse position is at or near the actual net (with one
;     grid unit).
;
; If these four conditions are not met, then this offset is not added.
(add-attribute-offset 50)

; auto-load-last string
;
; Determines if the newly placed components are embedded in the schematic
; or if only the filename is specified and the component is searched for
; instead.  If it is enabled, then all new components will be embedded
; othewise they are not embedded.  This can be controlled on the fly during
; runtime with the "Embed Component" checkbox on the select component dialog
; box
;
(auto-load-last "enabled")
;(auto-load-last "disabled")

; autosave interval
;
; Controls if a backup copy is made every "interval" seconds.
; Note that the backup copy is made when you make some change to the schematic,
; and there were more than "interval" seconds from the last autosave.
; Autosaving will not be allowed if setting it to zero.
(auto-save-interval 180)

; Attribute autoplacement grid
(define autoplace-attributes-grid 50)

; component-dialog-attributes stringlist
;
; Sets a list of attributs that are visible in the component select dialog.
; The attributes are sorted in the same order as they appear in the list.
; If the first list element is an asterisk "*", all attributes will be
; displayed in alphabetical order.
;
; Note 1. An empty list will disable the attribute view in the dialog.
; Note 2. To enable the list ALL of the semi-colons must be removed
;         from the first option!
;
(component-dialog-attributes '("device" "description" "footprint" "comment"
                               "net" "model" "model-name" "file" "value"
                               "numslots" "slotdef" "slot" "documentation"
                               "symversion" "author" "use-license="
                               "dist-license=" ))
;(component-dialog-attributes '("*"))
;(component-dialog-attributes '())

; continue-component-place string
;
; If this enabled then multiple instances of the same component can be placed
; immediately without having to click on the name or Apply in the Component
; Place... dialog box.  If this is disabled then only one component can be
; placed (the user must then press Apply in the dialog box to place multiple
; instances of the same component)
;
(continue-component-place "enabled")
;(continue-component-place "disabled")

; embed-components string
;
; Determines if the newly placed components are embedded in the schematic
; or if only the filename is specified and the component is searched for
; instead.  If it is enabled, then all new components will be embedded
; othewise they are not embedded.  This can be controlled on the fly during
; runtime with the "Embed Component" checkbox on the select component dialog
; box
;
;(embed-components "enabled")
(embed-components "disabled")

;  enforce-hierarchy string
;
;  Controls if the movement between hierarchy levels (of the same underlying
;  schematics) is allowed or not.
;  If this is enabled, then the user cannot (without using the page manager)
;  move between hierarchy levels otherwise, if enabled, the user sees all
;  the hierarchy levels as being flat.
;
(enforce-hierarchy "enabled")
;(enforce-hierarchy "disabled")

; force-boundingbox string
;
; Controls if the entire bounding box of a symbol is used when figuring out
; whichend of the pin is considered the active port.  Enable this when
; gschem is guessing incorrectly.
;
;(force-boundingbox "enabled")
(force-boundingbox "disabled")

; keyboardpan-gain integer
;
; Controls how much the display pans when using the keyboard cursor keys.
; A larger value provides greater pan distance when pressing the cursor
; keys, while a smaller value provides a smoother, but smaller pan
; distance when moving the cursor keys.
(keyboardpan-gain 20)
;;(keyboardpan-gain 10)
;;(keyboardpan-gain 1)
;;(keyboardpan-gain 5)

; magnetic-net-mode string
;
; Controls the initial setting of the magnetic net mode. The magnetic
; net mode marks a possible connection that is close to the current
; cursor position
(magnetic-net-mode "enabled")
;(magnetic-net-mode "disabled")

; netconn-rubberband string
;
; Controls if net connections are maintained when you move a connecting
; component or net.
;
(netconn-rubberband "enabled")
;(netconn-rubberband "disabled")

; select-slack-pixels integer
;
; Controls how many pixels around an object can still be clicked as part of
; that object.
; A larger value gives greater ease in selecting small, or narrow objects.
(select-slack-pixels 10)
;;(select-slack-pixels 4)
;;(select-slack-pixels 0)
;;(select-slack-pixels 1)

; snap-size number
;
; Sets the default snap spacing at start-up of gschem.
;
(snap-size 100)

; sort-components-library string
;
; If this is enabled, then the component library will be sorted in
; alphanumeric order.  Bear in mind that this option is totally
; cosmetic, and will not alter the component search order (latest
; added gets scanned first).
;
;(sort-component-library "enabled")
(sort-component-library "disabled")

;END ==================> Miscellaneous Options <====================

;BEGIN =================> Nets and Routing Setup <==================

; net-consolidate string
;
;  Controls if the net consolidation code is used when schematics are read
;  in, written to disk, and when nets are being drawn (does not consolidate
;  when things are being copied or moved yet).  Net consolidation is the
;  connection of nets which can be combined into one.
;  Comment out if you want the default mode
;
(net-consolidate "enabled")
;(net-consolidate "disabled")

; net-endpoint-mode string
;
; Not fully implemented.
;(net-endpoint-mode "none")
;(net-endpoint-mode "empty")
(net-endpoint-mode "filledbox")

;  net-midpoint-mode string
;
; Not fully implemented.
;(net-midpoint-mode "none")
;(net-midpoint-mode "empty")
(net-midpoint-mode "filledbox")

;  net-direction-mode string
;
;  Controls if the net direction mode is used. This mode tries to guess
;  the best continuation direction of a L-shape net when adding a net.
;
(net-direction-mode "enabled")
;(net-direction-mode "disabled")

;  net-selection-mode string
;
; Controls how many net segments are selected when you click at a net
; If one of the enabled items is used, the selection state will toggle
; through all selection states. The mode defines the maximum search depth
; for the net selection mode
;
;(net-selection-mode "disabled")
(net-selection-mode "net")
;(net-selection-mode "all")

;BEGIN --------------------->  Net Ripper  <------------------------

; Bus ripper controls
; The following keywords control the auto bus ripper addition code
;
; bus-ripper-size  => Sets the size of the auto bus rippers.
; bus-ripper-type  => Sets the bus ripper type either a "component" or
;                     plain "net"
; bus-ripper-syname  => If above is set to component, specify the symbol name.
;                       The symbol must exist in a component library
; bus-ripper-rotation => Either "symmetric" or "non-symmetric".  This deals
;                        with how the bus ripper symbol is rotated when it
;                        is auto added to a schematic.
;

; The default bus ripper setup
(bus-ripper-size 200)
(bus-ripper-type "component")
(bus-ripper-symname "busripper-1.sym")
(bus-ripper-rotation "non-symmetric")

;END ------------------------>  Net Ripper  <-----------------------

;END ==================> Nets and Routing Setup <===================

;BEGIN ------------------------>  Styles  <-------------------------
;  bus-style string
;
;  Set to thin if you want thin buses.
;  Set to thick if you want thick buses.
;  This mode also determines what bus style gets printed
;
;(bus-style "none")
;(bus-style "thin")
(bus-style "thick")

; line-style string
;
; Set to none disable using line width styles.
; Set to thin if you want thin lines
; Set to thick if you want thick lines.
; This mode also determines what line style gets printed
;
(line-style "none")
;(line-style "thin")
;(line-style "thick")

;  net-style string
;
;  Set to thin if you want thin nets.
;  Set to thick if you want thick nets.
;  This mode also determines what net style gets printed
;
;(net-style "none")
(net-style "thin")
;(net-style "thick")

; pin-style string
;
; Set to none disable using line width styles, pins width will be
; based on the pin type, either net or bus.
; Set to thin if you want thin pins
; Set to thick if you want thick pins.
; This mode also determines what pin style gets printed
;
;(pin-style "none")
;(pin-style "thin")
(pin-style "thick")

; Line widths can be set for a given style using the appropriate
; keywords. While defaults are reasonable values, some installations
; may require different values for optimization. One example might
; be a large display operating in a high resolution mode, where
; 5 pixels may appear minute.
;
; Note: If the corresponding xxx-style is NOT used then the internal
; defaults are assigned, in which case: Nets and Pin will be the old
; "narrow" values, Lines will be an over-ridable THIN default, and
; Buses will be over-ridable THICK default. In situtations where
; "tweeking" is NOT required, all the keywords for Bus, Line, Net,
; and Pin styles and widths would be better left commented out.
;
; Or, if you feel a compelling urge, you could uncomment them all,
; along with the styles and just see what you get!
;
(thin-bus-width 15)
(thick-bus-width 30)

(thin-line-width 15)
(thick-line-width 30)

(thin-net-width 15)
(thick-net-width 25)

(thin-pin-width 15)
(thick-pin-width 30)

;END ------------------------->  Styles  <--------------------------

;BEGIN ==============> Pointer Device  Preferences <================

; fast-mousepan string
;
; Controls if text is drawn properly or if a simplified version (a line which
; represents the text string) is drawn during mouse pan. Drawing a simple
; line speeds up mousepan a lot for big schematics
;(fast-mousepan "enabled")
;(fast-mousepan "disabled")

; drag-can-move string
;
; If enabled, the drag movement over selected objects can move the objects.
;(drag-can-move "enabled")
;(drag-can-move "disabled")

; middle-button string
;
; Controls if the middle mouse button draws strokes, repeats the last
; command, does an action (move and copy (holding down the ALT key)
; are supported) on a single objects, or if it does the mouse panning.
;
;(middle-button "Stroke")
;(middle-button "Repeat")
;(middle-button "Action")
;(middle-button "Pan")

; mousepan-gain integer
;
; Controls how much the display pans when using mousepan.  A larger value
; provides greater pan distance when moving the mouse, while a smaller value
; provides a smoother, but smaller pan distance when moving the mouse.
;(mousepan-gain 1)
;;(mousepan-gain 5)
;;(mousepan-gain 10)

; scrollpan-steps non-zero integer
;
; Controls the number of scroll-wheel pan events required to traverse the
; viewed schematic area. Larger numbers mean more scroll steps are required
; to pan across the viewed area and giving finer control over positioning.
; scrollpan-steps can not be 0.
;
;(scrollpan-steps 8)
;;(scrollpan-steps 4) ; Hard-coded behaviour up to version 1.5.0.20080706

; scroll-wheel string
;
; Controls the binding of the mouse scroll wheel.
; "classic" style is the gschem default, where scrolling with no modifier
; key is mapped to zoom, + CTRL -> x-axis pan, + SHIFT -> y-axis pan.
; "gtk" style changes this behaviour to be like other GTK appliactions,
; which is awkward for normal people, if you are adnormal and want Gschem
; use no modifier-> y-axis pan, + CTRL -> zoom, + SHIFT -> x-axis pan.
; set the scroll-wheel value to "gtk", otherwise use the "classic"
;
;(scroll-wheel "gtk")
;;(scroll-wheel "classic")

; pointer-hscroll string
;
; Controls if horizontal scroll events are enabled for the pointing device.
; Pointers with a horizontal scroll feature integrated with a middle button
; and scroll wheel can have problems with generating a horizontal scroll
; events when attempting to press the middle button. This option allows dis-
; abling of pointer horizontal scroll events so that the integrated middle
; button can be utilized without interference. If the screen always seems to
; pan by itself when attempting to mouse-pan with the scroll-wheel button,
; then try setting this to "disabled".
;
; Enable/disable mouse horizontal scroll events:
;(pointer-hscroll "enabled")
;(pointer-hscroll "disabled")

; third-button string
;
; Controls if the third mouse button performs the popup ("popup") or
; if it does the mouse panning ("mousepan")
;
;(third-button "Popup")
;(third-button "Pan")

;END ================> Pointer Device  Preferences <================

;BEGIN ==================>  Printer Related  <======================

; page-size width height
;
; Specifies the size of the default paper size
; Be sure all inputs are reals (floats/doubles) and don't try to reverse
; the values to get a portrait mode.  Code to support that needs to be added
; The code that implements this automatically transforms the dimensions into
; the proper aspect ratio.  All units are in inches. (use output-orientation
; to get portrait mode)
;
(paper-size 11.0 8.5) ; letter
;(paper-size 14.0 8.5) ; legal
;(paper-size 8.5 5.5) ; statement
;(paper-size 17.0 11.0) ; tabloid
;(paper-size 11.0 17.0) ; ledger
;(paper-size 13.0 8.5) ; folio
;(paper-size 10.833 8.472) ; quarto
;(paper-size 14 10) ; 10x14
;(paper-size 10.0 7.5) ; executive
;(paper-size 11.0 8.5) ; A
;(paper-size 17.0 11.0) ; B
;(paper-size 22.0 17.0) ; C
;(paper-size 34.0 22.0) ; D
;(paper-size 44.0 34.0) ; E
;(paper-size 46.81 33.11) ; A0
;(paper-size 33.11 23.39) ; A1
;(paper-size 23.39 16.54) ; A2
;(paper-size 16.54 11.69) ; A3
;(paper-size 11.69 8.27) ; A4
;(paper-size 8.27 5.83) ; A5
;(paper-size 5.83 4.13) ; A6
;(paper-size 4.13 2.91) ; A7
;(paper-size 2.91 2.05) ; A8
;(paper-size 2.05 1.46) ; A9
;(paper-size 1.46 1.02) ; A10
;(paper-size 1.02 0.71) ; A11
;(paper-size 0.71 0.51) ; A12

; paper-sizes string width height
;
; Specifies which paper sizes are available for printing.
; The width and height parameters are in the order for landscape printing,
; so reversing them for portrait won't work just yet. (and that will be
; implemented differently.
; The default paper size is set above.  All units are in inches (forgiveness
; please).
;
; You MUST change this list in this file (gschem-systemrc) if you want
; any reordering changes to take effect.
;
(paper-sizes "Letter : 8.5 in x 11 in" 11.0 8.5)
(paper-sizes "Legal : 8.5 in x 14 in" 14.0 8.5)
(paper-sizes "Statement : 5.5 in x 8.5 in" 8.5 5.5)
(paper-sizes "Tabloid : 11 in x 17 in" 17.0 11.0)
(paper-sizes "Ledger : 17 in x 11 in" 11.0 17.0)
(paper-sizes "Folio : 8.5 in x 13 in" 13.0 8.5)
(paper-sizes "Quarto : 8.472 in x 10.833 in" 10.833 8.472)
(paper-sizes "10x14 : 10 in x 14 in " 14.0 10.0)
(paper-sizes "Executive : 7.5 x 10" 10.0 7.5)
(paper-sizes "A : 8.5 in x 11 in" 11.0 8.5)
(paper-sizes "B : 11 in x 17 in" 17.0 11.0)
(paper-sizes "C : 17 in x 22 in" 22.0 17.0)
(paper-sizes "D : 22 in x 34 in" 34.0 22.0)
(paper-sizes "E : 34 in x 44 in" 44.0 34.0)
(paper-sizes "A0  84.10 cm x 118.90 cm" 46.81 33.11)
(paper-sizes "A1  59.40 cm x 84.10 cm" 33.11 23.39)
(paper-sizes "A2  42.00 cm x 59.40 cm" 23.39 16.54)
(paper-sizes "A3  29.70 cm x 42.00 cm" 16.54 11.69)
(paper-sizes "A4  21.00 cm x 29.70 cm" 11.69 8.27)
(paper-sizes "A5  14.80 cm x 21.00 cm" 8.27 5.83)
(paper-sizes "A6  10.50 cm x 14.80 cm" 5.83 4.13)
(paper-sizes "A7  7.40 cm x 10.50 cm" 4.13 2.91)
(paper-sizes "A8  5.20 cm x 7.40 cm" 2.91 2.05)
(paper-sizes "A9  3.70 cm x 5.20 cm" 2.05 1.46)
(paper-sizes "A10 2.60 cm x 3.70 cm" 1.46 1.02)
(paper-sizes "A11 1.80 cm x 2.60 cm" 1.02 0.71)
(paper-sizes "A12 1.30 cm x 1.80 cm" 0.71 0.51)

; print-command string
;
; The command to send data to in order to print to a printer.  On most
; systems, this will be "lpr".
;
(print-command "lpr")

; output-type string
;
; Controls what is actually printed
;       string is either "extents" or "extents no margins" or
;       "current window"
;
(output-type "extents")
;(output-type "extents no margins")
;(output-type "current window")
;;; (output-type "limits")  "limits" is considered deprecated and should
;;;                         not be used.

; output-orientation string
;
; Controls which way the output page is layed out (landscape or portrait)
;
;(output-orientation "portrait")
(output-orientation "landscape")

; output-color string
;
; Controls if output (postscript) is color (enabled) or black/white (disabled)
;
(output-color "disabled")
;(output-color "enabled")

; output-capstyle string
;
; Controls the capstyle at the end of lines in the postscript output
;
(output-capstyle "square")
;(output-capstyle "round")
;(output-capstyle "butt")

; setpagedevice-orientation string
;
; If enabled, puts a << /Orientation x >> setpagedevice into the postscript
; output.  x is either 1 for landscape or 0 for portrait.
;
(setpagedevice-orientation "disabled")
;(setpagedevice-orientation "enabled")

; setpagedevice-pagesize string
;
; If enabled, puts a << /PageSize XxY >> setpagedevice into the postscript
; output.  XxY is the size of the paper in points.
;
(setpagedevice-pagesize "disabled")
;(setpagedevice-pagesize "enabled")

;END ====================>  Printer Related  <======================

;BEGIN =================> General System Options <==================

;  file-preview string
;
;  Controls if the preview area in the File Open/Save As and Component
;  dialog boxes is enabled by default or not
;
;(file-preview "enabled")
(file-preview "disabled")

; handleboxes string
;
; Controls if the handleboxes (which contain the menu and toolbar) are
; visible or not.
;
(handleboxes "enabled")
;(handleboxes "disabled")

; raise-dialog-boxes-on-expose
;
; Controls if dialog boxes are raised whenever an expose event happens
; Default is disabled since gtk2 supposedly handles the raising of these
; dialogs correctly now.
;
;(raise-dialog-boxes-on-expose "enabled")
(raise-dialog-boxes-on-expose "disabled")

; save-ui-settings string
;
; Controls if settings are retained between sessions. Note this does not
; control saving of configurations variables, which can be saved in the
; Preferences Dialog. This feature saves the Main Window size and
; Position, the state of all tool bars, and menu settings.
;
(save-ui-settings "enabled")
;(save-ui-settings "disabled")

; toolbars string
;
; Controls if the toolbars are visible or not. If toolbars are not enabled
; here then toolbars can not enabled without restarting.
;
(toolbars "enabled")
;(toolbars "disabled")

; toolbars-mode string
;
; Controls if Icons, Text, or both are displayed on toolbars. This keyword
; will over-ride the settings in gtkrc's. This may be required in certain
; enviroments. gschem does not default this value so the setting will be
; determined by gtk if this keyword is not set. The last mode, "retention"
; will cause each toolbar to be restored with values preserved from the
; last shutdown when save-ui-settings was "enabled".
;
;(toolbars-mode "only-icons")
;(toolbars-mode "only-text")
;(toolbars-mode "show-both")
;(toolbars-mode "show-both-horiz")
(toolbars-mode "retention")

; untitled-name string
;
; Specify the default untitled basename (usually only used a startup time)
; And typically not changed at runtime
;
(untitled-name "untitled")

;BEGIN ===================> Scrollbar Options <=====================

; scrollbars string
;
; Controls if the scrollbars are enabled or disabled. If you disable
; the scrollbars, you will not be able to use the scroll wheel on your
; mouse. See scrollbars-visible.
;
;(scrollbars "enabled")
;(scrollbars "disabled")

; scrollbar-update string
;
; Specifies the behavior of the scrollbars in the main window.
;    continuous - display is redrawn as you move the scrollbar
;    delayed - display is redrawn once you stop moving the scrollbar
; Default is continuous
;
;(scrollbar-update "continuous")
;(scrollbar-update "delayed")

; scrollbars-visible string
;
; Controls if the scrollbars are displayed (enabled) or not (disabled)
; If scrollbars-visible is disabled, scrollbars will not be displade
; scroll wheel on the pointer will still be functional.
;
;(scrollbars-visible "enabled")
;(scrollbars-visible "disabled")


;END =====================> Scrollbar Options <=====================
;END =======================> System Options <======================

;BEGIN =================> Set Text Configuration <==================
;
; Sets the default caps style used for the input of text
; lower specifies that all inputed text is in lowercase
; upper specifies that all inputed text is in uppercase
; both specifies that all inputed text is used as is (no case conversion)
;
;(text-case "lower")
;(text-case "upper")
;(text-case "both")

; text-display-zoomfactor integer
;
; Sets the zoom factor at which text is drawn as a box. When the current
; Zoom is at or above this value, text will be drawn as boxes if text-feed
; back is set to "only-when-readable". This parameter is not valid when
; text-feedback is set to "always".
;
;(text-display-zoomfactor 21)

; text-feedback string
;
; Controls if text is rendered when doing an xor action (like copy/move)
; or when the zoom is above the zoom threashold specified by text-display
; -zoomfactor. Comment out if you want the default mode.
;
;(text-feedback "only-when-readable")
;(text-feedback "always")

; text-origin-marker string
;
; Controls if the text origin markers are displayed (or not)
;
;(text-origin-marker "enabled")
;(text-origin-marker "disabled")

; text-marker-size integer
;
; Sets the default size of the text marker, this value is ignored
; when the text-origin-marker is disabled.
;
;(text-marker-size 15)

; text-size integer
;
; Sets the default text size.
;
;(text-size 10)

;END ====================> Text Configuration <=====================

;BEGIN ==================>  Undo Sub-System  <======================

; undo-control string
;
; Controls if the undo is enabled or not
;
;(undo-control "enabled")
;(undo-control "disabled")

; undo-levels number
;
; Determines the number of levels of undo.  Basically this number decides
; how many backup schematics are saved on disk.
;
;(undo-levels 10)

; undo-type string
;
; Controls which kind of undo is used.  The default is to use the disk as
; the storing medium (ie after every action the undo information is stored
; to disk).  The other mechanism uses only memory.  The disk mechanism is
; nice because you get undo-level number of backups of the schematic written
; to disk as backups so you should never lose a schematic due to a crash.
;
;(undo-type "disk")
;(undo-type "memory")

; undo-panzoom string
;
; Controls if pan or zoom commands are saved in the undo list.  If this
; is enabled then a pan or zoom command will be considered a command and
; can be undone.  If this is false, then panning and zooming is not saved
; in the undo list and cannot be undone.  Note, the current viewport
; information is saved for every command, so the display will change to the
; viewport before a command is executed.
;
;(undo-panzoom "enabled")
;(undo-panzoom "disabled")

;END ====================>  Undo Sub-System  <======================

; reset-componet-library
;
; When reset-component-library is executed, then all known component library
; paths are erased.  This is useful if the user wants to override all the
; system provided paths and provide his/her own set.  Normally this is not
; commented in.
;
; (reset-component-library)

;----------------------------------------------------------------------------
;
; End of mode related keywords
;

;
; Start of hooks
;
(use-modules (gschem deprecated))

;; Uncomment this scheme code if you want automatic numbering when
;; placing new component and copying components.
;
;(load-from-path "auto-uref.scm")
;(add-hook! add-component-hook auto-uref)
;(add-hook! copy-component-hook auto-uref)

;; Define value of page-offset for auto number on insert.
;; Refdeses will be numbered from integer multiples of page-offset,
;; depending on the lowest refdes value found on the page.
;; If lowest value is 323 and page offset is 100, then next refdes
;; will be 301.
;; Setting to 0 disables the feature.
;
;(auto-uref-set-page-offset 100)

; Define default pin attributes
; Attributes:
;   - Attribute name.
;   - Value of the attribute.
;   - Visibility: #t (visible) or #f (hidden).
;   - Show_list:  a list containing what to show, using
;                 elements like "name" or "value", or an empty list.
(define default-pin-attributes
       '(("pintype"   "unknown" #f ())
         ("pinlabel"  "unknown" #t ("value"))
         ("pinnumber" "0"       #t ("value"))
         ("pinseq"    "0"       #f ())))

; Convert a character into a string
(define char2str
  (lambda (char)
    (list->string (list char))))

; Load the default position of attributes, for attribute autoplacing
; functions.
(load-from-path "default-attrib-positions.scm")

; Adds the default pin attributes to each newly placed pin.
(define (add-default-pin-attributes object)
  (for-each
    (lambda (a)
      (apply add-attribute-to-object object a)) default-pin-attributes))

; Comment in this hook to automatically add the default attributes to
; each newly placed pin
(add-hook! add-pin-hook add-default-pin-attributes)

; Comment in this to load the functions to place attributes automatically.
(load-from-path "auto-place-attribs.scm")

; Autoplace pin text attributes hook.
; Comment in these if you want the pin attributes to be automatically placed.
; There are different hooks for situations like adding a new pin and rotating
; or mirroring an existing one.
; The #t at the end means that function is appended to the end of the hook.
(add-hook! add-pin-hook    (lambda (pin) (autoplace-pin-attributes pin )) #t)
(add-hook! rotate-pin-hook (lambda (pin) (autoplace-pin-attributes pin )) #t)
(add-hook! mirror-pin-hook (lambda (pin) (autoplace-pin-attributes pin )) #t)

; Autoplace component/net/buses text attributes hook.
; Comment in these if you want the component attributes to be
; automatically placed.
; There are different hooks for situations like adding a new pin, rotating
; or mirroring an existing one, adding a new attribute or a new component.
; The #t at the end means that function is appended to the end of the hook.
;(add-hook! add-component-object-hook (lambda (object)
;       (autoplace-object-attributes object)) #t)
;(add-hook! rotate-component-object-hook (lambda (object)
;       (autoplace-object-attributes object)) #t)
;(add-hook! mirror-component-object-hook (lambda (object)
;       (autoplace-object-attributes object)) #t)
;(add-hook! add-attribute-hook (lambda (object)
;       (autoplace-object-attributes object)) #t)
;(add-hook! complex-place-list-changed-hook (lambda (object)
;         (autoplace-object-attributes object)) #t)

; Autoplace netname= attribute hook.  This autoplaces netname
; attribute at the time that it's added.
(load-from-path "auto-place-netname.scm")
(add-hook! add-objects-hook place-netname-attribute-handler)

;; Automatically place a titleblock (or other components) when creating
;; a new page.
;; Comment in these lines if you want gschem to automatically place a titleblock
;; when you create a new _empty_ page.
;; Users can customize the default titleblock by adding the following line
;; (without the semi-colons at the beginning) to the gschemrc file;
;; (define default-titleblock "title-A4.sym")
;; Change "title-A4.sym" by the name of your prefered titleblock!
;
;; If you don't want a titleblock to be added automatically, then add one of
;; the following lines to your gschemrc file (without the semicolon).
;; There are several ways, so just choose one;
;;   (define default-titleblock "")
;;   (define default-titleblock '())
;;   (define default-titleblock #f)
;
;(define default-titleblock "title-B.sym")

; Load the regular expressions module
(if (provided? 'regex)
    (use-modules (ice-9 regex))
    (display "Your Guile installation doesn't provide the regex module.\n"))

;(add-hook! (@ (gschem hook) new-page-hook) (lambda (page)
;   ; Only place the titleblock if there are no objects in the page
;   ; and the page filename ends in ".sym".
;   (if (and (null? (get-objects-in-page page))
;            ; If the guile installation doesn't provide the regex module,
;            ; don't care about the page filename.
;            (if (provided? 'regex)
;                (not (string-match ".*\\.[sS][yY][mM]"
;                                   (get-page-filename page)))
;                #t))
;;      Syntax             Symbol name        X   Y    angle selectable mirrored
;      (add-component-at-xy page default-titleblock 40000 40000   0       #f       #f))

   ;; After adding titleblock, reset page to mark as unchanged.
;   ((@ (geda page) set-page-dirty!) (active-page) #f))
;           #t)

; Evaluate an expression entered in the magic-colon text box.
; In 20 years this might dispatch to an interpreter for some other language.
(define (invoke-macro s-expr)
  (gschem-log (format #f "~s\n" (eval-string-protected s-expr))))

;
; End of hooks
;

;
; Start of path related keywords
;

; attribute-name string
;
; Specifies the default attributes which are presented to the user in the
; "Add Attribute" dialog box.
; The main purpose of this keyword is to allow the user to add any attributes
; which should be in this dialog box.
; Some of these names are specific for symbols while others are for general
; components or nets.  The attribute names are case sensitive. (change this?)
;
; The order of the attribute-name keywords determines the order they
; are displayed.
;
(attribute-name "netname")
(attribute-name "footprint")
(attribute-name "value")
(attribute-name "refdes")
(attribute-name "source")
(attribute-name "model-name")
(attribute-name "model")
(attribute-name "net")
(attribute-name "device")
(attribute-name "pinnumber")
(attribute-name "pinseq")
(attribute-name "pintype")
(attribute-name "pinlabel")
(attribute-name "numslots")
(attribute-name "slot")
(attribute-name "slotdef")
(attribute-name "graphical")
(attribute-name "description")
(attribute-name "documentation")
(attribute-name "symversion")
(attribute-name "comment")
(attribute-name "author")
(attribute-name "dist-license")
(attribute-name "use-license")
(attribute-name "file")

;
; End of path related keywords
;

;
; Start of stroke related keywords
;

;
; This section defines associations between a stroke sequence and a
; guile function which is executed when the stroke is drawn in the
; gschem window
;
; Strokes are defined as follows;
;
; 1  2  3
;
; 4  5  6
;
; 7  8  9
;
; The sequence of number such as "852" specify how the stroke is drawn.
; Sequence "852" happens to be a vertical line drawn from the bottom going
; up.
;
; Please see the libstroke documentation for further information on the
; stroke description.
;
; For the most part I went a little overboard on the stroke defs, you
; probably can get away with many less stroke defs, but I'm a very
; sloppy stroke drawing person.
;
; Be careful here, strokes is a rather large list, and make sure you
; maintain proper ( and )'s.
;

(define strokes
; Letter L for line
  '(("14789" . add-line)

; line up for zoom out
("852" . view-zoom-out)
; line down for zoom in
("258" . view-zoom-in)

; Letter C for copy
("3214789" . edit-copy)
("214789" . edit-copy)
("21489" . edit-copy)
("32478" . edit-copy)

; Letter B for Box
("7417654456987" . add-box)

; Letter E for edit
("563214789" . edit-attributes)
("53214789" . edit-attributes)
("5321478" . edit-attributes)
("5214789" . edit-attributes)
("521478" . edit-attributes)
("453214789" . edit-attributes)
("45321478" . edit-attributes)
("456321478" . edit-attributes)
("456214789" . edit-attributes)
("45621478" . edit-attributes)

; Letter I for Insert component
("123789258" . add-component)

; Letter N for net
("415963" . add-net)
("7414863" . add-net)
("74148963" . add-net)
("74158963" . add-net)
("7415963" .  add-net)

; Letter M for move
("741236963" . edit-move)
("7412572369" . edit-move)
("7412575369" . edit-move)
("741258369" . edit-move)
("74125852369" . edit-move)
("7412585369" . edit-move)
("74125863" . edit-move)
("74126963" . edit-move)
("741475369" . edit-move)
("7414785369" . edit-move)
("74148369" . edit-move)
("7414852369" . edit-move)
("741485369" . edit-move)
("74148669" . edit-move)
("741552369" . edit-move)
("741575369" . edit-move)
("7415852369" . edit-move)
("741585369" . edit-move)
("74185369" . edit-move)
("74255369" . edit-move)
("7425852369" . edit-move)
("742585369" . edit-move)
("7426963" . edit-move)
("74585369" . edit-move)

; Capital Letter O for Open
("12369874" . file-open)
("74123698" . file-open)

; Letter D for delete
("14786321" . edit-delete)
("14789621" . edit-delete)
("147896321" . edit-delete)
("15896321" . edit-delete)
("257896321" . edit-delete)
("25896321" . edit-delete)
("4789621" . edit-delete)

; Letter S for save
("2145987" . edit-select )
("215987" . edit-select )
("2156987" . edit-select )
("21256987" . edit-select )
("3215987" . edit-select )
("32156987" . edit-select )
("32148987" . edit-select )
("32145987" . edit-select )

; Letter U for undo
("1478963" . edit-undo )

; Letter Z for zoom window
("125789" . view-zoom-box)
("1254789" . view-zoom-box)
("1235789" . view-zoom-box)
("2354789" . view-zoom-box)
("2324789" . view-zoom-box)
("12354789" . view-zoom-box)
("12324789" . view-zoom-box)
("12365789" . view-zoom-box)
("1232789" . view-zoom-box)

))

;
; End of stroke related keywords
;

;
; Start of keymapping related keywords
;

;;;; Keymapping
;;
;; Everything is case-sensitive.  Any number of keys may be bound in
;; sequence, and each keystroke consists of a non-modifier key with
;; some number of modifiers applied.  Examples:
;;
;;  *(map-keys "F N" 'file-new-window)
;;
;;    The "New Window" command will be run when an <F> is typed,
;;    followed by an <A>.
;;
;;  *(map-keys "<Control><Shift>A" 'edit-deselect)
;;
;;    The "Deselect All" command will be run when the <Ctrl> and
;;    <Shift> keys are held down, and the <A> key is pressed.
;;
;;  *(map-keys "O <Shift>S" 'options-snapsize)
;;
;;    The "Snap Size" dialog box will be shown when an <O> is typed,
;;    followed by an <S> typed with the <Shift> key held down.
;;
;; Key names can be found in /usr/include/gtk-2.0/gdk/gdkkeysyms.h on
;; most Linux systems.  For other systems, please see your platform
;; documentation.
;;
;; Later keybindings override earlier ones.

(map-keys "A C" "add-component")
(map-keys "A A" "add-attribute")
(map-keys "A N" "add-net")
(map-keys "A U" "add-bus")
(map-keys "A T" "add-text")
(map-keys "A L" "add-line")
(map-keys "A H" "add-path")
(map-keys "A B" "add-box")
(map-keys "A I" "add-circle")
(map-keys "A R" "add-arc")
(map-keys "A P" "add-pin")
(map-keys "A G" "add-picture")

(map-keys "<Control>A"        "edit-select-all")
(map-keys "<Control><Shift>A" "edit-deselect")
(map-keys "<Control><Shift>I" "edit-select-invert")

;;(map-keys "B"          "add-box")
(map-keys "<Shift>B"   "add-bus")
(map-keys "C"          "edit-copy")
(map-keys "<Control>C" "clipboard-copy")
(map-keys "D"          "edit-delete")

(map-keys "E <Shift>U" "edit-undo")
(map-keys "E <Shift>R" "edit-redo")
(map-keys "E S"        "edit-select")
(map-keys "E C"        "edit-copy")
(map-keys "E A"        "edit-attributes")
(map-keys "E Y"        "edit-mcopy")
(map-keys "E X"        "edit-text")
(map-keys "E M"        "edit-move")
(map-keys "E D"        "edit-delete")
(map-keys "E R"        "edit-rotate")
(map-keys "E I"        "edit-mirror")
(map-keys "E <Shift>S" "edit-slot")
(map-keys "E O"        "edit-color")
(map-keys "E L"        "edit-lock")
(map-keys "E <Shift>L" "edit-unlock")
(map-keys "E N"        "edit-pintype")
(map-keys "E W"        "edit-linetype")
(map-keys "E F"        "edit-filltype")
(map-keys "E T"        "edit-translate")
(map-keys "E <Shift>colon" "edit-invoke-macro")
(map-keys "E B"        "edit-embed")
(map-keys "E U"        "edit-unembed")
(map-keys "E P"        "edit-update")

(map-keys "F N"        "file-new")
(map-keys "F W"        "file-new-window")
(map-keys "F O"        "file-open")
(map-keys "F S"        "file-save")
(map-keys "F E"        "page-close")
(map-keys "F A"        "file-save-as")
(map-keys "F L"        "file-save-all")
(map-keys "F P"        "file-print")
(map-keys "F R"        "page-revert")
(map-keys "F I"        "file-write-image")
(map-keys "F T"        "file-run-script")
(map-keys "F C"        "file-close")
(map-keys "F Q"        "file-quit")

(map-keys "H A"        "help-show-about")
(map-keys "H M"        "help-show-manual")
(map-keys "H F"        "help-show-faq")
(map-keys "H W"        "help-show-wiki")
(map-keys "H H"        "help-shows")
(map-keys "H C"        "hierarchy-documentation")

(map-keys "<Shift>H D" "hierarchy-down-schematic")
(map-keys "<Shift>H S" "hierarchy-down-symbol")
(map-keys "<Shift>H U" "hierarchy-up")
(map-keys "<Shift>H O" "hierarchy-documentation")

(map-keys "I"          "add-component")
(map-keys "L"          "add-line")
(map-keys "M"          "edit-move")
(map-keys "N"          "add-net")

(map-keys "O A"        "options-action-feedback")
(map-keys "O G"        "options-cycle-grid")
(map-keys "O S"        "options-cycle-snap")
(map-keys "O R"        "options-rubberband")
(map-keys "O M"        "options-magneticnet")
(map-keys "O <Shift>S" "options-snap-size")
(map-keys "O T"        "options-show-text-size")
(map-keys "O L"        "options-show-console")
(map-keys "O C"        "options-show-coordinates")

(map-keys "P M"        "page-manager")
(map-keys "P N"        "page-next")
(map-keys "P P"        "page-prev")
(map-keys "P E"        "page-new")
(map-keys "P R"        "page-revert")
(map-keys "P C"        "page-close")
(map-keys "P D"        "page-discard")
(map-keys "P <Shift>P" "page-print")

(map-keys "<Alt>Q"     "file-quit")
(map-keys "R"          "view-redraw")
(map-keys "<Shift>R"   "edit-redo")
(map-keys "S"          "edit-select")

(map-keys "T A"        "attributes-attach")
(map-keys "T D"        "attributes-detach")
(map-keys "T N"        "attributes-show-name")
(map-keys "T V"        "attributes-show-value")
(map-keys "T B"        "attributes-show-both")
(map-keys "T T"        "attributes-visibility-toggle")
(map-keys "T <Shift>F" "attributes-find-text")
(map-keys "T H"        "attributes-hide-text")
(map-keys "T <Shift>H" "attributes-show-text")
(map-keys "T E"        "attributes-editor")
(map-keys "T U"        "attributes-autonumber")

(map-keys "U"          "edit-undo")
(map-keys "<Shift>U"   "edit-undo")

(map-keys "V A"        "view-zoom-all")
(map-keys "V B"        "view-zoom-box")
(map-keys "V E"        "view-zoom-extents")
(map-keys "V S"        "view-zoom-selected")
(map-keys "V I"        "view-zoom-in")
(map-keys "V O"        "view-zoom-out")
(map-keys "V P"        "view-pan")
(map-keys "V R"        "view-redraw")
(map-keys "V D"        "view-documentation")
(map-keys "V H"        "view-show-hidden")
(map-keys "V N"        "view-show-nets")
(map-keys "V K"        "view-dark-colors")
(map-keys "V L"        "view-light-colors")
(map-keys "V W"        "view-bw-colors")

(map-keys "<Control>V" "clipboard-paste")
(map-keys "W"          "view-zoom-box")
(map-keys "X"          "view-pan")
(map-keys "<Control>X" "clipboard-cut")

(map-keys "Y C 1"         "buffer-copy1")
(map-keys "Y U 1"         "buffer-cut1")
(map-keys "Y P 1"         "buffer-paste1")
(map-keys "Y C 2"         "buffer-copy2")
(map-keys "Y U 2"         "buffer-cut2")
(map-keys "Y P 2"         "buffer-paste2")
(map-keys "Y C 3"         "buffer-copy3")
(map-keys "Y U 3"         "buffer-cut3")
(map-keys "Y P 3"         "buffer-paste3")
(map-keys "Y C 4"         "buffer-copy4")
(map-keys "Y U 4"         "buffer-cut4")
(map-keys "Y P 4"         "buffer-paste4")
(map-keys "Y C 5"         "buffer-copy5")
(map-keys "Y U 5"         "buffer-cut5")
(map-keys "Y P 5"         "buffer-paste5")

(map-keys "<Control>Y"    "edit-redo")
(map-keys "Z"             "view-zoom-in")
(map-keys "<Shift>Z"      "view-zoom-out")
(map-keys "<Control>Z"    "edit-undo")

(map-keys "Escape"        "cancel")
(map-keys "bracketright"  "scale-up-snap-size")
(map-keys "bracketleft"   "scale-down-snap-size")
(map-keys "Left"          "view-pan-left")
(map-keys "Right"         "view-pan-right")
(map-keys "Up"            "view-pan-up")
(map-keys "Down"          "view-pan-down")
(map-keys "period"        "repeat-last")
(map-keys "colon"         "edit-invoke-macro")

(map-keys "minus"         "view-zoom-out")
(map-keys "plus"          "view-zoom-in")

(map-keys "Delete"        "edit-delete")
(map-keys "greater"       "page-next")
(map-keys "Page_Down"     "page-next")
(map-keys "less"          "page-prev")
(map-keys "Page_Up"       "page-prev")

; Definitions for the top pull down menu bar
;
; The "menu item name" is the name of the item as it will appear in the menu
; The "menu action" is the scheme function which is executed when the item
; is selected off of the menu.  And "menu hotkey function" is the scheme
; function which is executed when the hotkey is pressed.
; "menu hotkey function" has no real functionality in the menuing scheme other
; than providing the information as to what hotkey to display in the menu.
; "menu hotkey function" is not executed when you select an item off of the
; list.  The hotkeys which are displayed are defined by the global-keymap.
; Actions can have several hotkeys, but the displayed keys are the last
; ones found.
;
; The SEPARATOR keyword is case sensitive and puts a seperator into the menu.
;

(or (defined? 'define-syntax)
    (use-modules (ice-9 syncase)))

;; Define a no-op macro for flagging strings as translatable.
(define-syntax N_
  (syntax-rules ()
    ((N_ expr) expr)))

(define file-menu-items
;;
;;      menu item name           menu action            menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "_New")              file-new             "gtk-new"             ,(N_ "Create a new empty file" ))
        (,(N_ "_Open...")          file-open            "gtk-open"            ,(N_ "Open an existing schematic or symbol file"))
        (,(N_ "Open Recen_t")      #f                   "geda-open-recent"    ,(N_ "Open recently accessed schematic or symbol files"))

        ("SEPARATOR"               #f                   #f)
        (,(N_ "_Save")             file-save            "gtk-save"            ,(N_ "Save the current document"))
        (,(N_ "Save _As...")       file-save-as         "gtk-save-as"         ,(N_ "Save the current document to a new name or location"))
        (,(N_ "Save All")          file-save-all        "gtk-save"            ,(N_ "Save all open documents"))
        (,(N_ "_Revert")           page-revert          "gtk-revert-to-saved" ,(N_ "Discard changes and reload the current document"))

        ("SEPARATOR"               #f                   #f            #f)
        (,(N_ "_Print...")         file-print           "gtk-print"           ,(N_ "Print the current document"))
        (,(N_ "Write _image...")   file-write-image     "gtk-print"           ,(N_ "This is a dummy tip"))

        ("SEPARATOR"               #f                   #f            #f)
        (,(N_ "Execute Script...") file-run-script      "gtk-execute"         ,(N_ "Execute a script file"))

        ("SEPARATOR"               #f                   #f            #f)
        (,(N_ "_Close")            file-close           "gtk-close"           ,(N_ "Close the current document"))
        (,(N_ "_Quit")             file-quit            "gtk-quit"            ,(N_ "Quit gschem and exit"))
      )
)

(define edit-menu-items
;;
;;      menu item name              menu action            menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "_Undo")              edit-undo              "gtk-undo"         ,(N_ "Undo the last action"))
        (,(N_ "_Redo")              edit-redo              "gtk-redo"         ,(N_ "redo the last un-done action"))

        ("SEPARATOR"               #f                     #f                 #f)
        (,(N_ "Cu_t")               clipboard-cut          "gtk-cut"          ,(N_ "Cut the current selection to the system clipboard"))
        (,(N_ "_Copy")              clipboard-copy         "gtk-copy"         ,(N_ "Copy the current selection to the system clipboard"))
        (,(N_ "_Paste")             clipboard-paste        "gtk-paste"        ,(N_ "Paste the contents of the system clipboard"))
        (,(N_ "_Delete")            edit-delete            "gtk-delete"       ,(N_ "Delete the current selection" ))

        ("SEPARATOR"               #f                     #f)
        (,(N_ "Copy Mode")          edit-copy              "geda-copy"        ,(N_ "Copy selection"))
        (,(N_ "Multiple Copy Mode") edit-mcopy             "geda-multi"       ,(N_ "Make Multible Copies of selection"))
        (,(N_ "Move Mode")          edit-move              "geda-move"        ,(N_ "Move Objects"))
        (,(N_ "Rotate 90 Mode")     edit-rotate            "geda-rotate"      ,(N_ "Rotate the current selection about a point"))
        (,(N_ "Mirror Mode")        edit-mirror            "geda-mirror"      ,(N_ "Mirror an object about a point"))

        ("SEPARATOR"               #f                     #f                       #f)
        (,(N_ "Edit...")            edit-attributes        "gtk-indent"       ,(N_ "Edit Properties"))
        (,(N_ "Edit Text...")       edit-text              "gtk-edit"         ,(N_ "Open the Text Editor Dialog"))
        (,(N_ "Slot...")            edit-slot              "geda-slot"        ,(N_ "Open the Slot Editor Dialog"))
        (,(N_ "Color...")           edit-color             "gtk-select-color" ,(N_ "Open the Color Editor Dialog"))
        (,(N_ "Edit Pin...")        edit-pintype           "geda-pin-type"    ,(N_ "Open the Pin Type Dialog"))
        (,(N_ "Line Width & Type...") edit-linetype        "geda-line-type"   ,(N_ "Open the Line Editor Dialog"))
        (,(N_ "Fill Type...")         edit-filltype        "geda-mesh"        ,(N_ "Open the Fill Editor Dialog"))
        (,(N_ "Symbol Translate...")  edit-translate       "gtk-convert"      ,(N_ "Reset the X-Y Zero point"))
        (,(N_ "Lock")               edit-lock              "geda-lock"        ,(N_ "Lock selected objects"))
        (,(N_ "Unlock")             edit-unlock            "geda-unlock"      ,(N_ "Unlock selected objects"))

        ("SEPARATOR"               #f                     #f)
        (,(N_ "Invoke Macro")       edit-invoke-macro      "gtk-execute"      ,(N_ "Invoke a macro"))
        (,(N_ "Embed Component/Picture")    edit-embed    #f                  ,(N_ "Embed a component or image object"))
        (,(N_ "Unembed Component/Picture")  edit-unembed   "gtk-clear"        ,(N_ "Unembed a component or image object"))
        (,(N_ "Update Component")   edit-update            "gtk-refresh"      ,(N_ "Reload definition of selected component"))
      )
)

(define select-menu-items
     `( (,(N_ "Select Mode")        edit-select            "gschem-select"       ,(N_ "Activate Select mode"))
        (,(N_ "Deselect Mode")      edit-deselect          "gschem-unselect"     ,(N_ "Activate Deselect mode"))
        (,(N_ "Select All")         edit-select-all        "gschem-select-all"   ,(N_ "Select all objects"))
        (,(N_ "Deselect All")       edit-deselect-all      "geda-unselect-all"   ,(N_ "Unselect everything"))
        (,(N_ "_Invert Selection")  edit-select-invert     "gschem-invert"       ,(N_ "Invert the current selection set"))
        ("SEPARATOR"               #f                     #f)
      )
)

(define buffer-menu-items
;;
;;      menu item name                    menu action             menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "Copy into 1")    buffer-copy1            "gtk-copy"  ,(N_ "Copy selection to first auxiliary buffer"))
        (,(N_ "Copy into 2")    buffer-copy2            "gtk-copy"  ,(N_ "Copy selection to second auxiliary buffer"))
        (,(N_ "Copy into 3")    buffer-copy3            "gtk-copy"  ,(N_ "Copy selection to third auxiliary buffer"))
        (,(N_ "Copy into 4")    buffer-copy4            "gtk-copy"  ,(N_ "Copy selection to forth auxiliary buffer"))
        (,(N_ "Copy into 5")    buffer-copy5            "gtk-copy"  ,(N_ "Copy selection to fifth auxiliary buffer"))
        (,(N_ "Cut into 1")     buffer-cut1             "gtk-cut"   ,(N_ "Cut selection to first auxiliary buffer"))
        (,(N_ "Cut into 2")     buffer-cut2             "gtk-cut"   ,(N_ "Cut selection to second auxiliary buffer"))
        (,(N_ "Cut into 3")     buffer-cut3             "gtk-cut"   ,(N_ "Cut selection to third auxiliary buffer"))
        (,(N_ "Cut into 4")     buffer-cut4             "gtk-cut"   ,(N_ "Cut selection to forth auxiliary buffer"))
        (,(N_ "Cut into 5")     buffer-cut5             "gtk-cut"   ,(N_ "Cut selection to fifth auxiliary buffer"))
        (,(N_ "Paste from 1")   buffer-paste1           "gtk-paste" ,(N_ "Insert contents of the first auxiliary buffer"))
        (,(N_ "Paste from 2")   buffer-paste2           "gtk-paste" ,(N_ "Insert contents of the second auxiliary buffer"))
        (,(N_ "Paste from 3")   buffer-paste3           "gtk-paste" ,(N_ "Insert contents of the third auxiliary buffer"))
        (,(N_ "Paste from 4")   buffer-paste4           "gtk-paste" ,(N_ "Insert contents of the forth auxiliary buffer"))
        (,(N_ "Paste from 5")   buffer-paste5           "gtk-paste" ,(N_ "Insert contents of the fifth auxiliary buffer"))
      )
)

(define view-menu-items
;;
;;      menu item name                    menu action             menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "_Redraw")             view-redraw             "gtk-refresh"          ,(N_ "redraw the current window"))
        (,(N_ "_Pan")                view-pan                "geda-zoom-pan"        ,(N_ "Activate Panning"))
        (,(N_ "Zoom _Box")           view-zoom-box           "geda-zoom-box"        ,(N_ "Zoom to a Windowed region"))
        (,(N_ "Zoom _Selection")     view-zoom-selected      "geda-zoom-selection"  ,(N_ "Zoom to selected objects"))
        (,(N_ "Zoom _Extents")       view-zoom-extents       "gtk-zoom-100"         ,(N_ "Zoom to the extents of the drawing"))
        (,(N_ "Zoom _In")            view-zoom-in            "gtk-zoom-in"          ,(N_ "Increase the Zoom magnification"))
        (,(N_ "Zoom _Out")           view-zoom-out           "gtk-zoom-out"         ,(N_ "Decrease the Zoom magnification"))
        (,(N_ "Zoom _All")           view-zoom-all           "gtk-fullscreen"       ,(N_ "Zoom to the limits of the drawing area"))

        ("SEPARATOR"                #f                      #f                       #f)
        (,(N_ "D_ocumentation...")   view-documentation      "gtk-index")
        (,(N_ "Show/Hide Inv Text")  view-show-hidden        "gtk-find-and-replace" ,(N_ "Toggle hidden text attributes"))
        (,(N_ "Show/Hide Net Names") view-show-nets          "geda-show-nets" ,      (N_ "Toggle hidden net name attributes"))

        ("SEPARATOR"                #f                       #f)
        (,(N_ "_Dark color scheme")  view-dark-colors        #f                   ,(N_ "Set the color map to the Dark set"))
        (,(N_ "_Light color scheme") view-light-colors       #f                   ,(N_ "Set the color map to the Light set"))
        (,(N_ "B_W color scheme")    view-bw-colors          #f                   ,(N_ "Set the color map to the Black and White"))
      )
)

(define page-menu-items
;;
;;      menu item name                    menu action             menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "_Manager...")       page-manager            "gtk-properties"       ,(N_ "Open the Page Manager"))
        (,(N_ "_Previous")         page-prev               "gtk-go-back"          ,(N_ "Switch to the previous page"))
        (,(N_ "_Next")             page-next               "gtk-go-forward"       ,(N_ "Switch to the next page"))
        (,(N_ "Ne_w")              page-new                "gtk-new"              ,(N_ "Create a new Page"))
        (,(N_ "Prin_t")            page-print              "gtk-print"            ,(N_ "Print the current page"))
        (,(N_ "_Revert")           page-revert             "gtk-revert-to-saved"  ,(N_ "Discard changes and reload the current documents"))
        (,(N_ "_Close")            page-close              "gtk-close"            ,(N_ "Close the current page"))

        ("SEPARATOR"              #f                      #f)
        (,(N_ "_Discard")          page-discard            "gtk-discard"          ,(N_ "Close the current page without saving"))
      )
)

(define add-menu-items
;;
;;      menu item name                    menu action             menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "_Component...")     add-component           "geda-component"   ,(N_ "Insert a symbol from the component library"))
        (,(N_ "_Net")              add-net                 "gschem-net"       ,(N_ "Add net"))
        (,(N_ "B_us")              add-bus                 "gschem-bus"       ,(N_ "Add bus"))
        (,(N_ "_Attribute...")     add-attribute           "insert-attribute" ,(N_ "Add attribute"))
        (,(N_ "_Text...")          add-text                "gtk-bold"         ,(N_ "Add text"))

        ("SEPARATOR"              #f                      #f)
        (,(N_ "A_rc")              add-arc                 "geda-arc"         ,(N_ "Create arc"))
        (,(N_ "_Box")              add-box                 "geda-box"         ,(N_ "Add box"))
        (,(N_ "C_ircle")           add-circle              "geda-circle"      ,(N_ "Add circle"))
        (,(N_ "_Line")             add-line                "geda-line"        ,(N_ "Add line"))
        (,(N_ "Pat_h")             add-path                "geda-path"        ,(N_ "Add Path"))
        (,(N_ "Pictu_re...")       add-picture             "gtk-orientation-portrait" ,(N_ "Insert an image into the current document"))
        (,(N_ "_Pin")              add-pin                 "geda-pin"                 ,(N_ "Add pin"))
      )
)

(define hierarchy-menu-items
;;
;;      menu item name                    menu action             menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "_Down Schematic")   hierarchy-down-schematic  "gtk-go-down"       ,(N_ "Descend down in the schematic hierarchy"))
        (,(N_ "Down _Symbol")      hierarchy-down-symbol     "gtk-goto-bottom"   ,(N_ "Descend down in the symbol hierarchy"))
        (,(N_ "_Up")               hierarchy-up              "gtk-go-up"         ,(N_ "ascend up in the schematic hierarchy"))
        (,(N_ "D_ocumentation...") hierarchy-documentation   "gtk-info"          ,(N_ "Find component documentation"))
      )
)

(define attributes-menu-items
;;
;;      menu item name                    menu action             menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "_Attach")           attributes-attach             "gtk-go-up"            ,(N_ "Attach selected attributes to symbol"))
        (,(N_ "_Detach")           attributes-detach             "gtk-go-down"          ,(N_ "Dettach selected attributes from a symbol"))
        (,(N_ "Show _Value")       attributes-show-value         "attribute-show-value" ,(N_ "Set selected value visible"))
        (,(N_ "Show _Name")        attributes-show-name          "attribute-show-name"  ,(N_ "Set selected name visible"))
        (,(N_ "Show _Both")        attributes-show-both          "attribute-show-both"  ,(N_ "Set selected name and value visible"))
        ("SEPARATOR"                    #f                       #f)
        (,(N_ "_Toggle Visibility")      attributes-visibility   "attribute-visibility" ,(N_ "Toggle attribute visibilty"))
        (,(N_ "_Find Specific Text...")  attributes-find-text    "gtk-find-and-replace" ,(N_ "Find an attribute"))
        (,(N_ "_Hide Specific Text...")  attributes-hide-text    #f                     ,(N_ "Hide selected attribute"))
        (,(N_ "_Show Specific Text...")  attributes-show-text    #f                     ,(N_ "Show a specific attribute value"))
        ("SEPARATOR"                    #f                       #f)
        (,(N_ "Attrbute _Editor...")     attributes-editor       "gtk-indent"           ,(N_ "Open the Attibutes Editor Dialog"))
        (,(N_ "A_utonumber Text...")     attributes-autonumber   #f                     ,(N_ "Open Auto Number dialog"))
      )
)

(define options-menu-items
;;
;;      menu item name                    menu action             menu stock icon menu       Menu Item Tooltip
;;
     `( (,(N_ "Cycle _grid styles")       options-cycle-grid       "gtk-jump-to"        ,(N_ "Toggle grid between Dot, Mesh and Off"))
        (,(N_ "Scale _up Grid Spacing")   scale-up-snap-size       "gtk-media-previous" ,(N_ "Increase the snap size"))
        (,(N_ "Scale _down Grid Spacing") scale-down-snap-size     "gtk-media-next"     ,(N_ "Increase the snap size"))
        (,(N_ "S_nap Grid Spacing...")    options-snap-size        "gtk-media-rewind"   ,(N_ "Adjust snap size"))
        ("SEPARATOR"                     #f                        #f)
        (,(N_ "Toggle _Snap On-Off")      options-cycle-snap       #f ,(N_ "Toggle the object snap mode"))
        (,(N_ "Toggle _Outline-Box")      options-action-feedback  #f ,(N_ "Toggle action feedback"))
        (,(N_ "Toggle _Rubberband")       options-rubberband       #f ,(N_ "Toggle rubberband mode"))
        (,(N_ "Toggle _Magnetic Net")     options-magneticnet      #f ,(N_ "Toggle magnetic net mode"))
        ("SEPARATOR"                     #f                        #f)
        (,(N_ "Show _Console Window...")  options-show-console     #f ,(N_ "Display the console"))
        (,(N_ "Show Coord _Window...")    options-show-coordinates #f ,(N_ "Display coordinates"))
        ("SEPARATOR"                     #f                        #f)
        (,(N_ "_Text Size...")            options-show-text-size   #f                ,(N_ "Open the Text Size settings"))
        (,(N_ "_Preferences...")          options-show-settings    "gtk-preferences" ,(N_ "Open the Preferences dialog"))
      )
)

(define help-menu-items
;;
;;      menu item name                menu action               menu hotkey action        menu stock icon
;;
     `(
        (,(N_ "Gschem Guide")           help-show-manual          "gtk-help")
        (,(N_ "_Hotkeys...")            help-show-hotkeys         "preferences-desktop-keyboard-shortcuts")
        (,(N_ "gschem _FAQ...")         help-show-faq             "help-faq")
        ("SEPARATOR"                   #f                         #f)
        (,(N_ "Component D_ocumentation...") hierarchy-documentation   "gtk-indent")
        ("SEPARATOR"                   #f                         #f)
        (,(N_ "gEDA Docu_mentation...") help-show-geda            "symbol-datasheet")
        (,(N_ "gEDA _Wiki...")          help-show-wiki            "web-browser")
        (,(N_ "_About...")              help-show-about           "gtk-about")
      )
)

;
; Now actually add the menus.  The order here defines the order in which
; the menus appear in the top menu bar.
;
(add-menu (N_ "_File")       file-menu-items)
(add-menu (N_ "_Edit")       edit-menu-items)
(add-menu (N_ "_Select")     select-menu-items)
(add-menu (N_ "_Buffer")     buffer-menu-items)
(add-menu (N_ "_View")       view-menu-items)
(add-menu (N_ "_Page")       page-menu-items)
(add-menu (N_ "_Add")        add-menu-items)
(add-menu (N_ "Hie_rarchy")  hierarchy-menu-items)
(add-menu (N_ "A_ttributes") attributes-menu-items)

;; Add Paul Tan's tools utility menu
;;(load-from-path "gschem-tools-menu.scm")

(add-menu (N_ "_Options") options-menu-items)
(add-menu (N_ "_Help") help-menu-items)

;
; End of keymapping related keywords
;

;;
;; Major modes
;;

;; Comment in this scheme code if you want to link with pcb
;;
;; Please note that the code in pcb.scm is still highly experimental
;; and there are known (and easy) ways to crash pcb and/or gschem with this code.
;; The short answer is neither program likes a pipe to break.
;;
; (load-from-path "pcb.scm")
