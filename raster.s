	xdef	_plot_pixel
	
BASE	equ	8
ROW	equ	10	
COL	equ	12

_plot_pixel:	

	link	a6
	movem.l	d0-7/a0-6,-(sp)

	cmp.w	#ROW,#400
	blo	done
	bhs	done

	cmp.w	#COL,#640
	blo	done
	bhs	done

in_bounds:

	move.w	#80,d0		;bytes_per_row

	mulu.w	#ROW,d0

	move.w	#COL,d1
	divu.w	#8,d1

	add.w	d0,d1

	move.w	#COL,d2
	divu.w	#8,d2
	swap.w	d2

	sub.w	#7,d2
	rol.w	#1,d2

done:
	
	movem.l	(sp),d0-7/a0-6
	unlk	a6
	rts

	
