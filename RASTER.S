	xdef	_plot_pixel
	xdef	_clear_screen

	
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


BASE	equ	8
_clear_screen:

	link	a6,#0
	movem.l	d0/a0,-(sp)

	move.w	#8000,d0
	movea.l	BASE(a6),a0

loop:
	move.w	#0,(a0)
	adda.l	#1,(a0)
	dbra.w	d0,loop
	

	movem.l	(sp),d0/a0
	unlk	a6
	rts
