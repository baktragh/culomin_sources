;===============================================================================
;Curse of the lost miner
;===============================================================================

;Supplementary variables
.segment "DATA"
_vbistorel:
.byte 0
_vbistoreh:
.byte 0
_sfx:
.byte 0
_suspend:
.byte 0
_mvDelay:
.byte 0
_colorStore1:
.byte 0
_colorStore2:
.byte 0
_requestedSFXCode:
.byte 0
_songLine:
.byte 0

;==============================================================================
; DLI Different colors and character set for status bar
;==============================================================================

.segment "CODE"
_dliHandler:
	pha
	sta 54282	;Horiz retrace
	lda #48		;DARK BG
	sta 53272
	lda #12
	sta 53271	;BRIGHT FONT
	lda #<_dliHandler2
	sta 512
	lda #>_dliHandler2
	sta 513
	pla
	rti
_dliHandler2:
	pha
	sta 54282	;Horiz retrace
	;Restore colors, swap after DLI
	lda _colorStore2	;BLACK BG
	sta 53272
	lda _colorStore1
	sta 53271
	lda #<_dliHandler
	sta 512
	lda #>_dliHandler
	sta 513
	pla
	rti

;===============================================================================
;VBI. Movement delay, Calling RMT.
;===============================================================================

; Some definitions

vRMT_BASE = 28672+1024
vRMT_SFX  = vRMT_BASE+15
vRMT_UPDATE = vRMT_BASE+3 
vRMT_STOP = vRMT_BASE+6

aRMT_MUSIC = 32768



.segment "CODE"
_vbiRoutine:
	php
	pla
	;movement delay
	lda _mvDelay
	cmp #0
	beq _n
	dec _mvDelay
	
	;if audio is suspended, do not call RMT routines
_n:	lda _suspend
	cmp #0
	bne _x1
	;if SFX not requested, continue to music update
	lda _sfx
	cmp #0
	beq _x
	;SFX
	ldx #3
	lda #30
	ldy _sfx
	jsr vRMT_SFX
	lda #0
	sta _sfx
	;Music update - Call RMT	
_x:	jsr vRMT_UPDATE	

	;Call original VBI routine
_x1:	jmp (_vbistorel)


;===============================================================================
; Set-up VBI routine
;===============================================================================
.segment "CODE"

.proc _rmtSetVBI: near
.segment "CODE"
	;store original vbi address
	lda 548
	sta _vbistorel
	lda 549
	sta _vbistoreh
	;Set new 
	ldx #>_vbiRoutine
	ldy #<_vbiRoutine
	lda #7
	jsr $e45c
	
	rts
.endproc

;Restore VBI original routine
.segment "CODE"

;===============================================================================
; Restore original VBI routine
;===============================================================================
.proc _rmtRestoreVBI: near
.segment "CODE"
	;store original vbi address
	
	ldy _vbistorel
	ldx _vbistoreh
	lda #7
	jsr $e45c
	
	rts
.endproc

;===============================================================================
; Music
;===============================================================================
.segment "CODE"

.proc _rmtPlaySongLine: near
.segment "CODE"
	;Music file
	ldx #<aRMT_MUSIC
	ldy #>aRMT_MUSIC
	lda _songLine
	;Initialize the tracker
	jsr vRMT_BASE
	;End of procedure
	rts
.endproc


;-Stop RMT routine
.segment "CODE"

;===============================================================================
; Stop all RMT playback
;===============================================================================
.proc _rmtAllStop: near
.segment "CODE"
	jsr vRMT_STOP
	rts
.endproc

;===============================================================================
; Sound effects
;===============================================================================
.segment "CODE"
.proc _rmtPlaySFX: near
.segment "CODE"
        lda _requestedSFXCode
        sta _sfx
        rts
.endproc

;===============================================================================
; Suspend/Resume routines
;===============================================================================
;-Suspend RMT routine--------
.segment "CODE"

.proc _rmtSuspend: near
.segment "CODE"
	lda #1
	sta _suspend
	rts
.endproc

;-Resume RMT routine--------
.segment "CODE"

.proc _rmtResume: near
.segment "CODE"
	lda #0
	sta _suspend
	rts
.endproc

;===============================================================================
; Auxiliarey routines
;===============================================================================

;-OS call - cold start--------
.segment "CODE"

.proc _asmReboot: near
.segment "CODE"
	jmp 58487
.endproc


;===============================================================================
; Exports
;===============================================================================
.export _mvDelay
.export _rmtSuspend
.export _rmtResume
.export _rmtSetVBI
.export _rmtAllStop
.export _rmtRestoreVBI
.export _asmReboot
.export _dliHandler
.export _colorStore1
.export _colorStore2
.export _requestedSFXCode
.export _rmtPlaySFX
.export _rmtPlaySongLine
.export _songLine