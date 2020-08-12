;===============================================================================
; Curse of the Lost Miner
; Game data
; Game data will be included in separate segments
;===============================================================================


;-------------------------------------------------------------------------------
; Export symbols, so the symbols are visible for the main program
;-------------------------------------------------------------------------------
.export  _CLM_DATA_CAVES
.export  _CLM_DATA_CHARSET1
.export  _CLM_DATA_CHARSET2
.export  _CLM_DATA_CAVEDL
.export  _CLM_DATA_PMGAREA
.export  _CLM_DATA_CAVESCREEN
.export  _CLM_DATA_STATUSBARSCREEN

;-------------------------------------------------------------------------------
; Caves (layout of the elements)
; It doesn't matter at what address this data is
;-------------------------------------------------------------------------------
.segment "CLM_SEG_CAVES"
_CLM_DATA_CAVES:
.incbin "data/levels.dat"
_CLM_DATA_CAVES_END:

;-------------------------------------------------------------------------------
; Character sets for displaying cave elements
; Character sets must be aligned at page boudnary 
;-------------------------------------------------------------------------------
; Character set 1
.segment "CLM_SEG_CHARSET1"
_CLM_DATA_CHARSET1:
.incbin "data/clmfont1.fnt"
_CLM_DATA_CHARSET1_END:

;Character set 2
.segment "CLM_SEG_CHARSET2" 
_CLM_DATA_CHARSET2:
.incbin "data/clmfont2.fnt"
_CLM_DATA_CHARSET2_END:

;-------------------------------------------------------------------------------
; Display list for displaying caves and status bar
; Display list should not cross the 1 KB boudnary
;-------------------------------------------------------------------------------
.segment "CLM_SEG_CAVEDL"
_CLM_DATA_CAVEDL:

.byte 112 ,112 ,112           ; Blank
.byte 068                     ; LMS for GR.13
.word _CLM_DATA_CAVESCREEN    ; Screen memory
.byte 004 ,004 ,004 ,004 ,004 ,004 ,004 ,004 ,004 ,004 ,004 ,004 ,004 ,004 ,004
.byte 004 ,004 ,004 ,004 ,004 ,004
.byte 240                     ; Blank + DLI

.byte 066                         ; LMS for GR.0
.word _CLM_DATA_STATUSBARSCREEN   ; Status bar screen memory
.byte 128                         ; DLI
.byte 065                         ; JVB
.byte <_CLM_DATA_CAVEDL,>_CLM_DATA_CAVEDL  ;DL Jump
_CLM_DATA_CAVEDL_END:

;-------------------------------------------------------------------------------
; Raster music tracker song - The song is a binary file
; It will be included with section headers
;-------------------------------------------------------------------------------
.segment "CLM_SEG_RMTSONG"
_CLM_DATA_RMTSONG:
.incbin "data/music.rmt"

;-------------------------------------------------------------------------------
; Raster music tracker player - The player is a binary file
; It will be included with section headers
;-------------------------------------------------------------------------------
.segment "CLM_SEG_RMTPLAYER"
.incbin "data/rmtplayer.obx"

;-------------------------------------------------------------------------------
; PMG Area (dummy segment, not written to the binary file)
;-------------------------------------------------------------------------------
.segment "CLM_SEG_PMGAREA"
_CLM_DATA_PMGAREA:

;-------------------------------------------------------------------------------
; Cave display memory and status bar (dummy segment, not written to the binary)
;-------------------------------------------------------------------------------
.segment "CLM_SEG_CAVESCREEN"
_CLM_DATA_CAVESCREEN:

.segment "CLM_SEG_STATUSBARSCREEN"
_CLM_DATA_STATUSBARSCREEN: