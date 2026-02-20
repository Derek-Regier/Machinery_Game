	xdef	_plot_pixel
	xdef	_clear_screen
	xdef	_clear_region
	xdef	_plot_horizontal_line
	xdef	_plot_vertical_line
	
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
	
	movem.l	(sp),d0-3/a0
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
	

	movem.l	(sp),d0/a0
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

	movem.l	(sp),d0-4/a0
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
	
	movem.l	(sp),d0-5/a0
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
	
	movem.l	(sp),d0-5/a0
	unlk	a6
	rts
