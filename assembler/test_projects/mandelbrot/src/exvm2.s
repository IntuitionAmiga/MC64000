  ; Offset 0 : @render

  ; r1 = width in pixels
  ; r3 = cY (float pos, starting at yMin)
  ; r6 = cX (float pos, starting at xMin)
  ; r9 = iStep (1)

  ; a0 = pixel data address

  ;
  ; d4 = x (int) pixel
  ; d5 = y (int) in pixels
  ; d6 = width/height
  ; d7 = total pixels

  ; fp13 = 16.0 (bailout)
  ; fp14 = fStep
  ; fp15 = xMin (float)


  _save         _mr0|_mr2                ; 2 : save pixel base address
    fmove.s     #16.0, fp13               ; 3 : bailout = 16.0
    move.l      #256,  _r9                ; 2 : max iters

    move.l      d6,    d5

.y_loop:
    fmove.s     fp15,  _r6                 ; 1 : cX = xMin
    move.l      d6,     d4

.x_loop:
    fmove.s     _r6,  _r10                ; 1 : zx = cX
    fmove.s     _r3,  _r11                ; 1 : zy = cY
    move.q      #0,    _r12                ; 1 : n = 0

.iteration:
    fmove.s     _r10, _r13                ; 1
    fmul.s      _r10, _r13                ; 1 : zx2 = zx*zx
    fmove.s     _r11, _r14                ; 1
    fmul.s      _r11, _r14                ; 1 : zy2   = zy*zy

    fmove.s     _r6,  _r15                ; 1 : new_zx = cX
    fadd.s      _r13, _r15                ; 1 : new_zx += zx2
    fsub.s      _r14, _r15                ; 1 : new_zx -= zy2

    fadd.s      _r14, _r13                ; 1 : r14 = zx*zx + zy*zy (for loop test)

    fmove.s     _r10, _r14                ; 1 : tmp = zx
    fmul.s      _r11, _r14                ; 1 : tmp *= zy
    fadd.s      _r14, _r14                ; 1 : tmp += tmp2
    fadd.s      _r3,  _r14                ; 1 : tmp += cY (tmp = 2*zx*zy+cY)

    fmove.s     _r14, _r11                ; 1 : zy = tmp
    fmove.s     _r15, _r10                ; 1 : zx = new_zx
    add.w       #1,    _r12                ; 2 : n++

    fbgt.s      _r13, fp13, .bailout             ; 2 : bailout
    blt.w       _r12, _r9, .iteration           ; 2 : iteration loop

.bailout:
    _mul_u16    _r12, _r12                ; 1 : out *= out - improve gradient
    move.b      _r12, (a0)+                 ; 1 : out = n
    fadd.s      fp14,  _r6                 ; 1 : cX += fStep
    dbnz        d4, .x_loop
    fadd.s      fp14, _r3                 ; 1 : cY += fStep
    dbnz        d5, .y_loop

    _restore    _mr0|_mr2)                ; 1
    _ret,
