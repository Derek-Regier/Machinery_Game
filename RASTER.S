	xdef	_plot_pixel
	xdef	_clear_screen
	xdef	_clear_region
	xdef	_plot_horizontal_line
	xdef	_plot_vertical_line
	xdef	_plot_rectangle
	xdef	_plot_square
	xdef	_plot_line
	xdef	_plot_triangle
	
BASE	equ	8
ROW	equ	10	
COL	equ	12
	
_plot_pixel:	

	link	a6,#0
	movem.l	d0-3/a0,-(sp)


	move.w	ROW(a6),d0
	cmp.w	#400,d0
	blo	done

	move.w	COL(a6),d1
	cmp.w	#640,d1
	blo	done

in_bounds:

	move.w	#80,d2		;bytes_per_row

	mulu.w	d0,d2		;row * bytes_per_row
		
	lsr.w	#3,d1		;col/8

	add.w	d0,d2		;= byte_index

	move.w	COL(a6),d3
	and.w	#7,d3		;col % 8

	sub.w	#7,d3		;7-
	lsl.w	#1,d3		;bit_mask = 1 <<

	movea.l	BASE(a6),a0

	adda.l	d2,a0
	or.w	d3,(a0)		;base[byte_index] |= bit_mask

done:
	
	movem.l	(sp)+,d0-3/a0
	unlk	a6
	rts


CLEAR_BASE	equ	8
_clear_screen:

	link	a6,#0
	movem.l	d0/a0,-(sp)

	move.w	#8000,d0
	movea.l	CLEAR_BASE(a6),a0

loop:
	move.w	#0,(a0)
	adda.l	#1,a0
	dbra.w	d0,loop
	

	movem.l	(sp)+,d0/a0
	unlk	a6
	rts

CLEARRE_BASE	equ	8
CLEAR_ROW	equ	12
CLEAR_COL	equ	14
CLEAR_LENGTH	equ	16
CLEAR_WIDTH	equ	18
	
_clear_region:

	link	a6,#0
	movem.l	d0-4/a0,-(sp)

	movea.l	CLEARRE_BASE(a6),a0
	move.w	CLEAR_ROW(a6),d0
	move.w	CLEAR_COL(a6),d1
	move.w	CLEAR_LENGTH(a6),d2
	move.w	CLEAR_WIDTH(a6),d3

	cmp.w	#400,d0
	blo	clearre_done

	cmp.w	#640,d1
	blo	clearre_done

clearre_in_bounds:

	move.w	d0,d4
	add.w	d3,d4	;row+width
	cmp.w	#400,d4	;>400
	bls	dont_change_width

	move.w	d0,d3
	sub.w	#400,d3	;width = 400 - row
	
dont_change_width:

	move.w	d1,d4
	add.w	d2,d4	;col+length
	cmp.w	#640,d4	;>640
	bls	dont_change_length

	move.w	d1,d2
	sub.w	#640,d2	;length = 640 - col

dont_change_length:

	move.w	#0,d4		;c
	move.w	#0,d5		;r
	
	width_loop:

		length_loop:
			
			move.w	CLEAR_ROW(a6),d0
			move.w	CLEAR_COL(a6),d1

			add.w	d5,d0 	;current_row = row + r
			add.w	d4,d1 	;current_col = col + c

			mulu.w	#20,d0 	;current_row * words_per_row

			move.w	d1,d6 	;still need current_col so copying d1
			lsr.w	#5,d6 	;current_col / 32

			add.w	d0,d6 	;word_index=

			and.w	#7,d1	;current_col % 8

			sub.w	#31,d1	;bit_position = 31 -

			lsl.w	#1,d1 	;1 << bit_position
			not.w	d1	;~(1 << bit_position)
			adda.l	d6,a0
			and.w	d1,(a0)	;base[word_index] &= ~(1<<bit_position)
	
			add.w	#1,d4 	;c++
		dbra.w	d2,length_loop				

		add.w	#1,d5	;r++
	dbra.w	d3,width_loop

clearre_done:

	movem.l	(sp)+,d0-4/a0
	unlk	a6
	rts

PLOTHL_BASE	equ	8
PLOTHL_ROW	equ	12
PLOTHL_COL	equ	14
PLOTHL_LENGTH	equ	16

_plot_horizontal_line:

	link	a6,#0
	movem.l	d0-5/a0,-(sp)

	movea.l	PLOTHL_BASE(a6),a0
	move.w	PLOTHL_ROW(a6),d0
	move.w	PLOTHL_COL(a6),d1
	move.w	PLOTHL_LENGTH(a6),d2

	cmp.w	#400,d0
	blo	plothl_done

	cmp.w	#640,d1
	blo	plothl_done

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

PLOTVL_BASE	equ	8
PLOTVL_ROW	equ	12
PLOTVL_COL	equ	14
PLOTVL_LENGTH	equ	16

_plot_vertical_line:

	link	a6,#0
	movem.l	d0-5/a0,-(sp)

	movea.l	PLOTVL_BASE(a6),a0
	move.w	PLOTVL_ROW(a6),d0
	move.w	PLOTVL_COL(a6),d1
	move.w	PLOTVL_LENGTH(a6),d2

	cmp.w	#400,d0
	blo	plotvl_done

	cmp.w	#640,d1
	blo	plotvl_done

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

PLOTL_BASE	equ	8
START_ROW	equ	12
START_COL	equ	14
END_ROW		equ	16
END_COL		equ	18
	
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
	bhi	row_not_neg

	move.w	#-1,d4

row_not_neg:

	cmp.w	d1,d3
	bhi	col_not_neg

	move.w	#-1,d5
	
col_not_neg:

plotl_loop:	

	cmp.w	d0,d2
	beq	plotl_done

	cmp.w	d1,d3
	beq	plotl_done

	move.w 	d1,-(sp) 	;UINT16 col
	move.w 	d0,-(sp)	;UINT16 row
	move.l 	a0,-(sp)	;UINT32 *base

	jsr	_plot_pixel

	addq.l 	#4,sp		;clear base from stack
	addq.l 	#2,sp 		;clear row from stack
	addq.l 	#2,sp 		;clear col from stack
	
	cmp.w	d0,d2
	beq	check_end_col

	cmp.w	d1,d3
	beq	plotl_done
	
plotl_done:
	
	movem.l	(sp)+,d0-4/a0
	unlk	a6
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
