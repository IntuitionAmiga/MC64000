  // Offset 0 : @render

  // r0 = pixel data address
  // r1 = width in pixels
  // r2 = height in pixels
  // r3 = cY (float pos, starting at yMin)
  // r4 = 16.0 (bailout)
  // r5 = xMin (float)
  // r6 = cX (float pos, starting at xMin)
  // r7 = fStep
  // r8 = x (int) pixel
  // r9 = iStep (1)

  _save       (_mr0|_mr2)                // 2 : save pixel base address
  _ld_32_f32  (16.0f, _r4)               // 3 : bailout = 16.0
  _ld_16_i32  (256,  _r9)                // 2 : max iters

.y_loop:
  _ldq        (0,    _r8)                // 1 : x = 0
  _move_32    (_r5,  _r6)                // 1 : cX = xMin

.x_loop:
  _move_32    (_r6,  _r10)               // 1 : zx = cX
  _move_32    (_r3,  _r11)               // 1 : zy = cY
  _ldq        (0,    _r12)               // 1 : n = 0

.iteration:
  _move_32    (_r10, _r13)               // 1
  _mul_f32    (_r10, _r13)               // 1 : zx2 = zx*zx
  _move_32    (_r11, _r14)               // 1
  _mul_f32    (_r11, _r14)               // 1 : zy2   = zy*zy

  _move_32    (_r6,  _r15)               // 1 : new_zx = cX
  _add_f32    (_r13, _r15)               // 1 : new_zx += zx2
  _sub_f32    (_r14, _r15)               // 1 : new_zx -= zy2

  _add_f32    (_r14, _r13)               // 1 : r14 = zx*zx + zy*zy (for loop test)

  _move_32    (_r10, _r14)               // 1 : tmp = zx
  _mul_f32    (_r11, _r14)               // 1 : tmp *= zy
  _add_f32    (_r14, _r14)               // 1 : tmp += tmp2
  _add_f32    (_r3,  _r14)               // 1 : tmp += cY (tmp = 2*zx*zy+cY)

  _move_32    (_r14, _r11)               // 1 : zy = tmp
  _move_32    (_r15, _r10)               // 1 : zx = new_zx
  _addi_16    (1,    _r12)               // 2 : n++

  _bgr_f32    (_r13, _r4, 2)             // 2 : bailout
  _bls_32     (_r12, _r9, -20)           // 2 : iteration loop

  // bailout
  _mul_u16    (_r12, _r12)               // 1 : out *= out - improve gradient
  _st_ripi_8  (_r12, _r0)                // 1 : out = n
  _add_f32    (_r7,  _r6)                // 1 : cX += fStep
  _addi_16    (1,    _r8)                // 2 : x += iStep

  _bls_32     (_r8, _r1, -(6+20+3+1))    // 2 : x loop

  _add_f32    (_r7, _r3)                 // 1 : cY += fStep
  _subi_16    (1,   _r2)                 // 2 : y += iStep
  _bnz_32     (_r2, -(5+5+6+20+1))       // 2 : y loop

  _restore    (_mr0|_mr2)                // 1
  _ret,
