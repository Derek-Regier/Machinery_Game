	xdef	_plot_pixel
	xdef	_clear_screen
	xdef	_clear_region
	
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
	or.w	(a0),d3		;base[byte_index] |= bit_mask

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
	movem.l	d0-3/a0,-(sp)

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

	width_loop:

		length_loop:
			
			;clear_pixel(...)

		dbra.w	d2,length_loop				
			
	dbra.w	d3,width_loop

clearre_done:

	movem.l	(sp),d0-3/a0
	unlk	a6
	rts