	xref	_do_VBL_ISR
	xref	_do_IKBD_ISR

	xdef    _vbl_isr
_vbl_isr:
    movem.l d0-d7/a0-a6,-(sp)
    jsr     _do_VBL_ISR
    movem.l (sp)+,d0-d7/a0-a6
    rte

    xdef    _ikbd_isr
_ikbd_isr:
    movem.l d0-d7/a0-a6,-(sp)
    jsr     _do_IKBD_ISR
    movem.l (sp)+,d0-d7/a0-a6
    rte