USB communication hook:

...
      0x39880: 0xf7ff 0xff5f  BL        0x39742
      0x39884: 0x2800         CMP       R0, #0
      0x39886: 0xf040 0x80fd  BNE.W     0x39a84
------------------------- => patch to jump out to hook
      0x3988a: 0xf89d 0x000c  LDRB.W    R0, [SP, #0xc]
      0x3988e: 0x284d         CMP       R0, #77                 ; 0x4d
      0x39890: 0xf040 0x80f8  BNE.W     0x39a84
      0x39894: 0xf89d 0x000d  LDRB.W    R0, [SP, #0xd]
      0x39898: 0x2802         CMP       R0, #2
      0x3989a: 0xf040 0x80f3  BNE.W     0x39a84
      0x3989e: 0xf7ea 0xfcbd  BL        0x2421c
------------------------- <= come back to 0x398a2 from hook if above is through in hook OR jump back from hook to 0x39a84
      0x398a2: 0x2110         MOVS      R1, #16                 ; 0x10
      0x398a4: 0xf20f 0x2020  ADR.W     R0, . + 0x224           ; 0x39ac8
      0x398a8: 0xf7ea 0xfcb5  BL        0x24216
...
