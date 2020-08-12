;===============================================================================
;Curse of the Lost Miner - Underground Madness Challenge
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
_keypadKey:
.byte $FF
_keypadDisable:
.byte $00
_secondFire:
.byte $00
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
	sta 53272-4096
	lda #12
	sta 53271-4096	;BRIGHT FONT
	lda #<_dliHandler2
	sta $0206
	lda #>_dliHandler2
	sta $0207
	pla
	rti
_dliHandler2:
	pha
	sta 54282	;Horiz retrace
	;Restore colors, swap after DLI
	lda _colorStore2	;BLACK BG
	sta 53272-4096
	lda _colorStore1
	sta 53271-4096
	lda #<_dliHandler
	sta $0206
	lda #>_dliHandler
	sta $0207
	pla
	rti
	
;===============================================================================
; Break key handler
;===============================================================================
.segment "CODE"
_breakHandler:
       pha
       lda #$FF
       sta _secondFire
       pla
       pla 
       rti
;===============================================================================
; Keypad interrupt continuation handler
;===============================================================================
.segment "CODE"
_keypadCont:

_kc1:  pha                 ;Save A
       lda _keypadDisable  ;Check if keypad disabled?
       beq _kc2            ;If enabled, register the key
       lda #$FF            ;If not, place no key
       sta _keypadKey
       pla                 ;Restore A
       jmp $FCB2           ;Continue with original handler

_kc2:  pla                 ;Restore A
       sta _keypadKey      ;Store last key pressed
_kc3:  jmp $FCB2           ;Continue with original handler

;===============================================================================
;VBI. Movement delay, Calling RMT
;===============================================================================
.segment "CODE"
_vbiRoutine:
	php
	pla
	;No attract
	lda #0
	sta 4
	;Movement delay
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
	jsr 8207
	lda #0
	sta _sfx
	;Music update - Call RMT	
_x:	jsr 8195	

	;Call original VBI routine
_x1:	jmp (_vbistorel)


;===============================================================================
; Set-up VBI routine
;===============================================================================
.segment "CODE"

.proc _rmtSetVBI: near
.segment "CODE"
	;Store original vbi address
	lda $204
	sta _vbistorel
	lda $205
	sta _vbistoreh

	;Set new 
        lda #<_vbiRoutine
	sta $204
	lda #>_vbiRoutine
	sta $205
	rts
.endproc

;===============================================================================
; Restore original VBI routine
;===============================================================================
.segment "CODE"
.proc _rmtRestoreVBI: near
.segment "CODE"
	;store original vbi address
	
	ldy _vbistorel
	ldx _vbistoreh
	sty $202
	stx $203
;	lda #7
;	jsr $e45c
	
	rts
.endproc

;===============================================================================
; MUSIC
;===============================================================================
.segment "CODE"

.proc _rmtPlaySongLine: near
.segment "CODE"
	;Music file is at page 36
	ldx #0
	ldy #36
	lda _songLine
	;Initialize the tracker
	jsr 8192
	;End of procedure
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
; Stop all RMT playback
;===============================================================================
.proc _rmtAllStop: near
.segment "CODE"
	jsr 8198
	rts
.endproc

;===============================================================================
; Suspend RMT playback
;===============================================================================
.segment "CODE"

.proc _rmtSuspend : near
.segment "CODE"
	lda #1
	sta _suspend
	rts
.endproc

;===============================================================================
; Resume RMT playback
;===============================================================================
.segment "CODE"

.proc _rmtResume : near
.segment "CODE"
	lda #0
	sta _suspend
	rts
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
.export _dliHandler
.export _colorStore1
.export _colorStore2

.export _keypadKey
.export _keypadDisable
.export _keypadCont

.export _secondFire
.export _breakHandler

.export _requestedSFXCode
.export _songLine
.export _rmtPlaySFX
.export _rmtPlaySongLine
