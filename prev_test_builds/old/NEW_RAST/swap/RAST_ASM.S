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

PLOTHL_LENGTH	equ	8
PLOTHL_COL	equ	10
PLOTHL_ROW	equ	12
PLOTHL_BASE	equ	14

_plot_horizontal_line:

	link	a6,#0
	movem.l	d0-5/a0,-(sp)

	movea.l	PLOTHL_BASE(a6),a0
	move.w	PLOTHL_ROW(a6),d0
	move.w	PLOTHL_COL(a6),d1
	move.w	PLOTHL_LENGTH(a6),d2

	cmp.w	#400,d0
	bhs	plothl_done

	cmp.w	#640,d1
	bhs	plothl_done

plothl_in_bounds:	

	move.w	d1,d3
	add.w	d2,d3	;col+length
	cmp.w	#640,d3	;>640
	bls	dont_change_hl_length

	move.w	d1,d2
	sub.w	#640,d2	;length = 640 - col

dont_change_hl_length:

	sub.w	#1,d2		;i = 01
	move.w	#1,d4		;i
	
	length_hl_loop:		

		move.w	PLOTHL_ROW(a6),d0
		move.w	PLOTHL_COL(a6),d1
		move.w	#80,d5		;bytes_per_row
	
		mulu.w	d0,d5		;row * bytes_per_row

		add.w	d4,d1		;col+i
		lsr.w	#3,d1		;(col+i)/8

		add.w	d0,d5		;= byte_index

		move.w	PLOTHL_COL(a6),d3
		add.w	d4,d3		;col+i
		and.w	#7,d3		;(col+i) % 8

		sub.w	#7,d3		;7-
		lsl.w	#1,d3		;bit_mask = 1 <<

		adda.l	d5,a0
		or.w	d3,(a0)		;base[byte_index] |= bit_mask

		add.w	#1,d4		;i++

	dbra.w	d2,length_hl_loop

plothl_done:
	
	movem.l	(sp)+,d0-5/a0
	unlk	a6
	rts

PLOTVL_LENGTH	equ	8
PLOTVL_COL	equ	10
PLOTVL_ROW	equ	12
PLOTVL_BASE	equ	14

_plot_vertical_line:

	link	a6,#0
	movem.l	d0-5/a0,-(sp)

	movea.l	PLOTVL_BASE(a6),a0
	move.w	PLOTVL_ROW(a6),d0
	move.w	PLOTVL_COL(a6),d1
	move.w	PLOTVL_LENGTH(a6),d2

	cmp.w	#400,d0
	bhs	plotvl_done

	cmp.w	#640,d1
	bhs	plotvl_done

plotvl_in_bounds:	

	move.w	d0,d3
	add.w	d2,d3	;row+length
	cmp.w	#640,d3	;>640
	bls	dont_change_vl_length

	move.w	d0,d2
	sub.w	#640,d2	;length = 640 - col

dont_change_vl_length:

	move.w	#0,d4		;i
	
	length_vl_loop:		
			
		move.w	PLOTVL_ROW(a6),d0
		move.w	PLOTVL_COL(a6),d1
		move.w	#80,d5		;bytes_per_row

		add.w	d4,d0		;row+i	
		mulu.w	d0,d5		;(row+i) * bytes_per_row

		lsr.w	#3,d1		;col/8

		add.w	d0,d5		;= byte_index

		move.w	PLOTVL_COL(a6),d3
		and.w	#7,d3		;col+i % 8

		sub.w	#7,d3		;7-
		lsl.w	#1,d3		;bit_mask = 1 <<

		adda.l	d5,a0
		or.w	d3,(a0)		;base[byte_index] |= bit_mask

		add.w	#1,d4		;i++

	dbra.w	d2,length_vl_loop
	
plotvl_done:
	
	movem.l	(sp)+,d0-5/a0
	unlk	a6
	rts

PLOTR_WIDTH	equ	8
PLOTR_LENGTH	equ	10
PLOTR_COL	equ	12
PLOTR_ROW	equ	14
PLOTR_BASE	equ	16
	
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

	move.w 	d3,-(sp) 	;UINT16 width
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_horizontal_line ;bottom
	
	move.w	d1,d4		;still need row for later
	add.w	d2,d4		;row + length
	sub.w	#1,d4		;(row + length) - 1
	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	addq.l 	#2,sp		;clear width from stack

	move.w 	d2,-(sp) 	;UINT16 length
	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 (row + length) - 1
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

PLOTQ_SIDE	equ	8
PLOTQ_COL	equ	10
PLOTQ_ROW	equ	12
PLOTQ_BASE	equ	14
	
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

END_COL		equ	8	
END_ROW		equ	10
START_COL	equ	12
START_ROW	equ	14
PLOTL_BASE	equ	16
	
_plot_line:	

	link	a6,#0
	movem.l	d0-4/a0,-(sp)
	
	movea.l	PLOTL_BASE(a6),a0
	move.w	START_ROW(a6),d0
	move.w	START_COL(a6),d1
	move.w	END_ROW(a6),d2
	move.w	END_COL(a6),d3

	move.w	#1,d4		;row_step
	move.w	#1,d5		;col_step

	cmp.w	d0,d2
	blo	row_not_neg

	move.w	#-1,d4

row_not_neg:

	cmp.w	d1,d3
	blo	col_not_neg

	move.w	#-1,d5
	
col_not_neg:

plotl_loop:	

	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_pixel

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	
	cmp.w	d0,d2
	beq	check_end_col	

	add.w	d4,d0	
	
check_end_col:
	
	cmp.w	d1,d3
	beq	check_loop	

	add.w	d5,d1

check_loop:
	
	cmp.w	d0,d2
	bne	plotl_loop

	cmp.w	d1,d3
	bne	plotl_loop
	
plotl_done:
	
	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts

DIRECTION	equ	8
PLOTT_HEIGHT	equ	10
BASE_LEN	equ	12
PLOTT_COL	equ	14
PLOTT_ROW	equ	16
PLOTT_BASE	equ	18
	
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
