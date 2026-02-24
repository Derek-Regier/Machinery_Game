	xdef	_plot_pixel
	xdef	_clear_screen
	xdef	_clear_region
	xdef	_plot_horizontal_line
	xdef	_plot_vertical_line
	xdef	_plot_rectangle
	xdef	_plot_square
	xdef	_plot_line
	xdef	_plot_triangle
	xdef	_plot_bitmap_8
	xdef	_plot_bitmap_16
	xdef	_plot_bitmap_32


BASE    equ 8
ROW     equ 12
COL     equ 14

_plot_pixel:
    link    a6,#0
    movem.l d2-d6/a0,-(sp)

    move.w  ROW(a6),d2
    cmp.w   #400,d2
    bhs     pp_done

    move.w  COL(a6),d3
    cmp.w   #640,d3
    bhs     pp_done

    ; byte_index = row * 80 + col / 8
    move.w  d2,d4
    mulu.w  #80,d4              ; row * 80
    move.w  d3,d5
    lsr.w   #3,d5               ; col / 8
    add.w   d5,d4               ; byte_index

    ; bit_pos = 7 - (col % 8)
    move.w  #7,d6
    and.w   #7,d3               ; col % 8
    sub.w   d3,d6               ; bit_pos = 7 - (col % 8)

    ; mask = 1 << bit_pos
    move.b  #1,d5
    lsl.b   d6,d5

    ; base[byte_index] |= mask
    movea.l BASE(a6),a0
    adda.l  d4,a0
    or.b    d5,(a0)

pp_done:
    movem.l (sp)+,d2-d6/a0
    unlk    a6
    rts


base		equ		64		; offset from SP, not A6


_clear_screen:

	movem.l		d0-7/a0-6,-(sp)
	lea		zeros,a0
	movem.l		(a0)+,d1-7/a1-6
	movea.l		base(sp),a0
	adda.l		#32000,a0
	move.w		#614,d0

fill_loop:

	movem.l		d1-7/a1-6,-(a0)
	dbra		d0,fill_loop
	movem.l		d1-5,-(a0)
	movem.l		(sp)+,d0-7/a0-6
	rts

zeros:		ds.l		13

CLEARRE_BASE    equ 8
CLEAR_ROW       equ 12
CLEAR_COL       equ 14
CLEAR_LENGTH    equ 16
CLEAR_WIDTH     equ 18

_clear_region:
    link    a6,#0
    movem.l d0-7/a0-1,-(sp)

    movea.l CLEARRE_BASE(a6),a1     ; base pointer
    move.w  CLEAR_ROW(a6),d0       ; row
    move.w  CLEAR_COL(a6),d1       ; col
    move.w  CLEAR_LENGTH(a6),d2    ; length (number of rows to clear)
    move.w  CLEAR_WIDTH(a6),d3     ; width (number of cols to clear)

    ; if (row >= 400 || col >= 640) return
    cmp.w   #400,d0
    bhs     clearre_done
    cmp.w   #640,d1
    bhs     clearre_done

    ; clip length: if row + length > 400, length = 400 - row
    move.w  d0,d4
    add.w   d2,d4
    cmp.w   #400,d4
    bls     dont_clip_length
    move.w  #400,d2
    sub.w   d0,d2
dont_clip_length:

    ; clip width: if col + width > 640, width = 640 - col
    move.w  d1,d4
    add.w   d3,d4
    cmp.w   #640,d4
    bls     dont_clip_width
    move.w  #640,d3
    sub.w   d1,d3
dont_clip_width:

    move.w  #0,d5               ; r = 0

width_loop:
    move.w  #0,d4               ; c = 0

length_loop:
    ; cur_row = row + r
    move.w  CLEAR_ROW(a6),d0
    add.w   d5,d0               ; d0 = cur_row

    ; cur_col = col + c
    move.w  CLEAR_COL(a6),d1
    add.w   d4,d1               ; d1 = cur_col

    ; word_index = cur_row * 20 + cur_col / 32
    mulu.w  #20,d0              ; cur_row * 20 longs per row
    move.w  d1,d6
    lsr.w   #5,d6               ; cur_col / 32
    add.l   d6,d0               ; word_index in d0

    ; byte_offset = word_index * 4
    lsl.l   #2,d0               ; * 4 bytes per long

    ; bit_pos = 31 - (cur_col % 32)
    and.w   #31,d1              ; cur_col % 32
    move.w  #31,d7
    sub.w   d1,d7               ; bit_pos = 31 - (cur_col % 32)

    ; mask = ~(1UL << bit_pos)
    move.l  #1,d1
    lsl.l   d7,d1               ; 1UL << bit_pos
    not.l   d1                  ; ~(1UL << bit_pos)

    ; base[word_index] &= mask
    movea.l a1,a0
    adda.l  d0,a0               ; a0 = base + byte_offset
    and.l   d1,(a0)             ; clear the bit

    add.w   #1,d4               ; c++
    cmp.w   d3,d4
    blt     length_loop

    add.w   #1,d5               ; r++
    cmp.w   d2,d5
    blt     width_loop

clearre_done:
    movem.l (sp)+,d0-7/a0-1
    unlk    a6
    rts

PLOTHL_LENGTH	equ	16
PLOTHL_COL	equ	14
PLOTHL_ROW	equ	12
PLOTHL_BASE	equ	8

_plot_horizontal_line:
    link    a6,#0
    movem.l d0-d7/a0,-(sp)

    move.w  PLOTHL_ROW(a6),d0
    move.w  PLOTHL_COL(a6),d1
    move.w  PLOTHL_LENGTH(a6),d2
    movea.l PLOTHL_BASE(a6),a0

    cmp.w   #400,d0
    bhs     plothl_done
    cmp.w   #640,d1
    bhs     plothl_done
    tst.w   d2
    beq     plothl_done

    ; clip length to screen right edge
    move.w  d1,d3
    add.w   d2,d3
    cmp.w   #640,d3
    bls     plothl_clipped
    move.w  #640,d2
    sub.w   d1,d2
plothl_clipped:

    ; advance a0 to start of row: row*80 = (row*64) + (row*16)
    move.w  d0,d3
    lsl.w   #6,d3           ; row * 64
    move.w  d0,d4
    lsl.w   #4,d4           ; row * 16
    add.w   d4,d3           ; row * 80
    adda.w  d3,a0

    ; start_byte = col >> 3,  start_bit = col & 7
    move.w  d1,d3
    lsr.w   #3,d3           ; d3 = start_byte
    move.w  d1,d4
    and.w   #7,d4           ; d4 = start_bit

    ; end_col = col + length - 1,  end_byte = end_col >> 3,  end_bit = end_col & 7
    move.w  d1,d5
    add.w   d2,d5
    subq.w  #1,d5           ; d5 = end_col
    move.w  d5,d6
    lsr.w   #3,d6           ; d6 = end_byte
    and.w   #7,d5           ; d5 = end_bit

    ; byte_span = end_byte - start_byte  (0 = same byte, 1 = adjacent, etc.)
    move.w  d6,d7
    sub.w   d3,d7           ; d7 = byte_span

    adda.w  d3,a0           ; a0 now points at start_byte in framebuffer

    ; left_mask  = 0xFF >> start_bit  (covers start_bit..7)
    ; e.g. start_bit=3 -> 0x1F = 0001_1111
    move.b  #$FF,d0
    lsr.b   d4,d0           ; d0.b = left_mask

    ; right_mask = 0xFF << (7 - end_bit)  (covers 0..end_bit)
    ; e.g. end_bit=4  -> 7-4=3 -> 0xFF<<3 = 0xF8 = 1111_1000
    move.b  #7,d1
    sub.b   d5,d1           ; d1.b = 7 - end_bit
    move.b  #$FF,d2
    lsl.b   d1,d2           ; d2.b = right_mask

    tst.w   d7
    bne     plothl_multi

    ; --- same byte: combine both masks and apply once ---
    and.b   d2,d0
    or.b    d0,(a0)
    bra     plothl_done

plothl_multi:
    ; left partial byte
    or.b    d0,(a0)+

    ; middle full bytes: count = byte_span - 1
    subq.w  #1,d7
    beq     plothl_right        ; byte_span was 1, no middle bytes

    subq.w  #1,d7               ; dbra iterates (n+1) times, so subtract 1 more
plothl_mid:
    move.b  #$FF,(a0)+
    dbra    d7,plothl_mid

plothl_right:
    ; right partial byte
    or.b    d2,(a0)

plothl_done:
    movem.l (sp)+,d0-d7/a0
    unlk    a6
    rts

PLOTVL_LENGTH	equ	16
PLOTVL_COL	equ	14
PLOTVL_ROW	equ	12
PLOTVL_BASE	equ	8

_plot_vertical_line:
    link    a6,#0
    movem.l d0-d5/a0,-(sp)

    move.w  PLOTVL_ROW(a6),d0
    move.w  PLOTVL_COL(a6),d1
    move.w  PLOTVL_LENGTH(a6),d2
    movea.l PLOTVL_BASE(a6),a0

    ; bounds check
    cmp.w   #400,d0
    bhs     plotvl_done
    cmp.w   #640,d1
    bhs     plotvl_done
    tst.w   d2
    beq     plotvl_done

    ; clip: row + length must not exceed 400
    move.w  d0,d3
    add.w   d2,d3
    cmp.w   #400,d3
    bls     plotvl_clipped
    move.w  #400,d2
    sub.w   d0,d2
plotvl_clipped:

    ; bit_mask = 0x80 >> (col & 7) -- constant for all rows, compute once
    move.w  d1,d3
    and.w   #7,d3
    move.b  #$80,d4
    lsr.b   d3,d4           ; d4.b = bit_mask

    ; advance a0 to first byte: base + row*80 + col/8
    ; row*80 = (row<<6) + (row<<4), no mulu needed
    move.w  d0,d3
    lsl.w   #6,d3           ; row * 64
    move.w  d0,d5
    lsl.w   #4,d5           ; row * 16
    add.w   d5,d3           ; d3 = row * 80
    move.w  d1,d5
    lsr.w   #3,d5           ; col / 8
    add.w   d5,d3
    adda.w  d3,a0           ; a0 = &base[row*80 + col/8]

    ; loop: set bit, step down one row (80 bytes) each iteration
    subq.w  #1,d2
plotvl_loop:
    or.b    d4,(a0)
    adda.w  #80,a0
    dbra    d2,plotvl_loop

plotvl_done:
    movem.l (sp)+,d0-d5/a0
    unlk    a6
    rts

PLOTR_BASE	equ	8
PLOTR_ROW	equ	12
PLOTR_COL	equ	14
PLOTR_LENGTH	equ	16
PLOTR_WIDTH	equ	18
	
_plot_rectangle:

	link	a6,#0
	movem.l	d0-4/a0,-(sp)

	movea.l	PLOTR_BASE(a6),a0
	move.w	PLOTR_ROW(a6),d0
	move.w	PLOTR_COL(a6),d1
	move.w	PLOTR_LENGTH(a6),d2
	move.w	PLOTR_WIDTH(a6),d3

	cmp.w	#0,d0
	blo	plotr_done

	cmp.w	#0,d1
	blo	plotr_done

	move.w 	d3,-(sp) 	;UINT16 width
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_horizontal_line ;top

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear width from stack

	move.w	d0,d4		;still need row for later
	add.w	d2,d4		;row + length
	sub.w	#1,d4		;(row + length) - 1
	move.w 	d3,-(sp) 	;UINT16 width
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d4,-(sp)	;UINT16 (row + length) - 1
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_horizontal_line ;bottom
	
	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear (row + length) - 1 from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear width from stack

	move.w 	d2,-(sp) 	;UINT16 length
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_vertical_line ;left

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear length from stack


	add.w	d3,d1		;col + width
	sub.w	#1,d1		;(col + width) - 1
	move.w 	d2,-(sp) 	;UINT16 length
	move.w 	d1,-(sp) 	;UINT16 (col + width) - 1
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_vertical_line ;right

	addq.l #4,sp		;clear base from stack
	addq.l #2,sp 		;clear row from stack
	addq.l #2,sp 		;clear (col + width) - 1 from stack
	addq.l #2,sp		;clear length from stack

plotr_done:	
	
	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts	

PLOTQ_BASE	equ	8
PLOTQ_ROW	equ	12
PLOTQ_COL	equ	14
PLOTQ_SIDE	equ	16
	
_plot_square:

	link	a6,#0
	movem.l	d0-4/a0,-(sp)

	movea.l	PLOTQ_BASE(a6),a0
	move.w	PLOTQ_ROW(a6),d0
	move.w	PLOTQ_COL(a6),d1
	move.w	PLOTQ_SIDE(a6),d2

	cmp.w	#0,d2
	blo	plotq_done

	move.w 	d2,-(sp) 	;UINT16 side
	move.w 	d2,-(sp) 	;UINT16 side
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_rectangle 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear side from stack
	addq.l 	#2,sp		;clear side from stack

plotq_done:

	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts

END_COL		equ	18	
END_ROW		equ	16
START_COL	equ	14
START_ROW	equ	12
PLOTL_BASE	equ	8
	

_plot_line:
    link    a6,#-6
    movem.l d0-d7/a0,-(sp)

    movea.l PLOTL_BASE(a6),a0
    move.w  START_ROW(a6),d0
    move.w  START_COL(a6),d1
    move.w  END_ROW(a6),d2
    move.w  END_COL(a6),d3

    ; --- step_r and abs_dr ---
    move.w  d2,d7
    sub.w   d0,d7              ; d7 = dr = end_row - start_row
    move.w  #1,d4              ; step_r = 1 (default)
    tst.w   d7
    bge     dr_pos
    move.w  #-1,d4             ; step_r = -1
    neg.w   d7                 ; abs_dr = |dr|
dr_pos:
    move.w  d7,-2(a6)

    ; --- step_c and abs_dc ---
    move.w  d3,d7
    sub.w   d1,d7              ; d7 = dc = end_col - start_col
    move.w  #1,d5              ; step_c = 1 (default)
    tst.w   d7
    bge     dc_pos
    move.w  #-1,d5             ; step_c = -1
    neg.w   d7                 ; abs_dc = |dc|
dc_pos:
    move.w  d7,-4(a6)

    ; --- initial err = abs_dc - abs_dr ---
    move.w  -4(a6),d6
    sub.w   -2(a6),d6

plotl_loop:
    ; plot_pixel(base, row, col) -- right-to-left push
    move.w  d1,-(sp)           ; col
    move.w  d0,-(sp)           ; row
    move.l  a0,-(sp)           ; base
    jsr     _plot_pixel
    addq.l  #8,sp              ; 4 + 2 + 2 = 8 bytes

    ; exit if endpoint just plotted
    cmp.w   d2,d0
    bne     plotl_calc
    cmp.w   d3,d1
    beq     plotl_done

plotl_calc:
    ; e2 = 2 * err
    move.w  d6,d7
    add.w   d7,d7              ; d7 = e2
    move.w  d7,-6(a6)          ; save e2 -- both branches below read it

    ; if e2 > -abs_dr  <=>  e2 + abs_dr > 0
    add.w   -2(a6),d7          ; d7 = e2 + abs_dr, sets flags
    ble     skip_col           ; e2 + abs_dr <= 0 means e2 <= -abs_dr, skip
    sub.w   -2(a6),d6          ; err -= abs_dr
    add.w   d5,d1              ; col += step_c
skip_col:

    ; if e2 < abs_dc  <=>  e2 - abs_dc < 0
    move.w  -6(a6),d7          ; reload original e2
    sub.w   -4(a6),d7          ; d7 = e2 - abs_dc, sets flags
    bge     skip_row           ; e2 - abs_dc >= 0 means e2 >= abs_dc, skip
    add.w   -4(a6),d6          ; err += abs_dc
    add.w   d4,d0              ; row += step_r
skip_row:

    bra     plotl_loop

plotl_done:
    movem.l (sp)+,d0-d7/a0
    unlk    a6
    rts

PLOTT_BASE	equ	8
PLOTT_ROW	equ	12
PLOTT_COL	equ	14
BASE_LEN	equ	16
PLOTT_HEIGHT	equ	18
DIRECTION	equ	20
	
_plot_triangle:	

	link	a6,#0
	movem.l	d0-4/a0,-(sp)

	movea.l	PLOTT_BASE(a6),a0
	move.w	PLOTT_ROW(a6),d0	
	move.w	PLOTT_COL(a6),d1	
	move.w	BASE_LEN(a6),d2
	move.w	PLOTT_HEIGHT(a6),d3
	move.w	DIRECTION(a6),d4

	cmp.w	#0,d4		
	beq	top_left

	cmp.w	#1,d4
	beq	top_right

	cmp.w	#2,d4
	beq	bottom_left

	cmp.w	#3,d4
	beq	bottom_right
	
top_left:

	move.w 	d2,-(sp) 	;UINT16 base_len 
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_horizontal_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear bas_len from stack
	
	move.w 	d3,-(sp) 	;UINT16 height
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_vertical_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear height from stack

	add.w	d0,d3		;row+height
	sub.w	#1,d3		;row+height-1

	add.w	d1,d2		;col+base_len
	sub.w	#1,d3		;col+base_len-1

	move.w	d2,-(sp)	;UINT16 col+base_len-1
	move.w 	d0,-(sp) 	;UINT16 row
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d3,-(sp)	;UINT16 row+height-1
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row+height-1 from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp		;clear col+base_len-1 from stack
	
	bra	plott_done
	
top_right:

	sub.w	d2,d1		;col-base_len
	add.w	#1,d1		;col-base_len+1
	
	move.w 	d2,-(sp) 	;UINT16 base_len 
	move.w 	d1,-(sp) 	;UINT16 col-base_len+1
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_horizontal_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col-base_len+1 from stack
	addq.l 	#2,sp		;clear bas_len from stack

	move.w	PLOTT_COL(a6),d1 ;restoring col register
	
	move.w 	d3,-(sp) 	;UINT16 height
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_vertical_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear height from stack

	add.w	d0,d3		;row+height
	add.w	#1,d3		;row+height+1

	sub.w	d1,d2		;col-base_len
	add.w	#1,d3		;col-base_len+1

	move.w	d2,-(sp)	;UINT16 col-base_len+1
	move.w 	d0,-(sp) 	;UINT16 row
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d3,-(sp)	;UINT16 row+height+1
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row+height+1 from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp		;clear col-base_len+1 from stack

	bra	plott_done
	
bottom_left:

	move.w 	d2,-(sp) 	;UINT16 base_len 
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_horizontal_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear bas_len from stack
	
	move.w 	d3,-(sp) 	;UINT16 height
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_vertical_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear height from stack

	add.w	d0,d3		;row+height
	add.w	#1,d3		;row+height+1

	sub.w	d1,d2		;col-base_len
	add.w	#1,d3		;col-base_len+1

	move.w	d2,-(sp)	;UINT16 col-base_len+1
	move.w 	d0,-(sp) 	;UINT16 row
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d3,-(sp)	;UINT16 row+height+1
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row+height+1 from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp		;clear col-base_len+1 from stack
	
	bra	plott_done
	
bottom_right:

	sub.w	d2,d1		;col-base_len
	add.w	#1,d1		;col-base_len+1
	
	move.w 	d2,-(sp) 	;UINT16 base_len 
	move.w 	d1,-(sp) 	;UINT16 col-base_len+1
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_horizontal_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col-base_len+1 from stack
	addq.l 	#2,sp		;clear bas_len from stack

	move.w	PLOTT_COL(a6),d1 ;restoring col register
	sub.w	d3,d0		;row-height
	add.w	#1,d0		;row-height+1
	
	move.w 	d3,-(sp) 	;UINT16 height
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row-height+1
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_vertical_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row-height+1 from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear height from stack
	
	move.w	PLOTT_ROW(a6),d0 ;restoring col register
	sub.w	d0,d3		;row-height
	add.w	#1,d3		;row-height+1

	add.w	d1,d2		;col+base_len
	add.w	#1,d3		;col+base_len+1

	move.w	d2,-(sp)	;UINT16 col+base_len+1
	move.w 	d0,-(sp) 	;UINT16 row
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d3,-(sp)	;UINT16 row-height+1
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_line 

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row-height+1 from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp		;clear col+base_len+1 from stack
	
plott_done:
	
	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts


PLOT8_HEIGHT	equ	8
PLOT8_COL	equ	10
PLOT8_ROW	equ	12
PLOT8_BASE	equ	14
	
_plot_bitmap_8:

	link	a6,#0
	movem.l	d0-4/a0,-(sp)

	movea.l	PLOT8_BASE(a6),a0
	move.w	PLOT8_ROW(a6),d0
	move.w	PLOT8_COL(a6),d1
	move.w	PLOT8_HEIGHT(a6),d2
	
plot8_done:
	
	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts

PLOT16_HEIGHT	equ	8
PLOT16_COL	equ	10
PLOT16_ROW	equ	12
PLOT16_BASE	equ	14
	
_plot_bitmap_16:

	link	a6,#0
	movem.l	d0-4/a0,-(sp)

	movea.l	PLOT16_BASE(a6),a0
	move.w	PLOT16_ROW(a6),d0
	move.w	PLOT16_COL(a6),d1
	move.w	PLOT16_HEIGHT(a6),d2
	
plot16_done:
	
	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts

PLOT32_HEIGHT	equ	8
PLOT32_COL	equ	10
PLOT32_ROW	equ	12
PLOT32_BASE	equ	14
	
_plot_bitmap_32:

	link	a6,#0
	movem.l	d0-4/a0,-(sp)

	movea.l	PLOT32_BASE(a6),a0
	move.w	PLOT32_ROW(a6),d0
	move.w	PLOT32_COL(a6),d1
	move.w	PLOT32_HEIGHT(a6),d2
	
plot32_done:
	
	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts
