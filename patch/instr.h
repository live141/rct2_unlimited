#ifndef _INSTR_H_
#define _INSTR_H_

#define INSTR_INVALID 0
#define INSTR_ADD 1
#define INSTR_PUSH 2
#define INSTR_POP 3
#define INSTR_OR 4
#define INSTR_ADC 5
#define INSTR_SBB 6
#define INSTR_AND 7
#define INSTR_DAA 8
#define INSTR_SUB 9
#define INSTR_DAS 10
#define INSTR_XOR 11
#define INSTR_AAA 12
#define INSTR_CMP 13
#define INSTR_AAS 14
#define INSTR_R16/32 15
#define INSTR_PUSHA 16
#define INSTR_PUSHAD 17
#define INSTR_POPA 18
#define INSTR_POPAD 19
#define INSTR_BOUND 20
#define INSTR_ARPL 21
#define INSTR_IMUL 22
#define INSTR_INS 23
#define INSTR_OUTS 24
#define INSTR_JO 25
#define INSTR_JNO 26
#define INSTR_JB 27
#define INSTR_JNB 28
#define INSTR_JZ 29
#define INSTR_JNZ 30
#define INSTR_JBE 31
#define INSTR_JNBE 32
#define INSTR_JS 33
#define INSTR_JNS 34
#define INSTR_JP 35
#define INSTR_JNP 36
#define INSTR_JL 37
#define INSTR_JNL 38
#define INSTR_JLE 39
#define INSTR_JNLE 40
#define INSTR_TEST 41
#define INSTR_XCHG 42
#define INSTR_MOV 43
#define INSTR_LEA 44
#define INSTR_NOP 45
#define INSTR_PAUSE 46
#define INSTR_CBW 47
#define INSTR_CWDE 48
#define INSTR_CWD 49
#define INSTR_CDQ 50
#define INSTR_CALLF 51
#define INSTR_FWAIT 52
#define INSTR_PUSHF 53
#define INSTR_PUSHFD 54
#define INSTR_POPF 55
#define INSTR_POPFD 56
#define INSTR_SAHF 57
#define INSTR_LAHF 58
#define INSTR_MOVS 59
#define INSTR_CMPS 60
#define INSTR_STOS 61
#define INSTR_LODS 62
#define INSTR_SCAS 63
#define INSTR_ROL 64
#define INSTR_ROR 65
#define INSTR_RCL 66
#define INSTR_RCR 67
#define INSTR_SHL 68
#define INSTR_SHR 69
#define INSTR_SAL 70
#define INSTR_SAR 71
#define INSTR_RETN 72
#define INSTR_LES 73
#define INSTR_LDS 74
#define INSTR_ENTER 75
#define INSTR_LEAVE 76
#define INSTR_RETF 77
#define INSTR_INT 78
#define INSTR_INTO 79
#define INSTR_IRET 80
#define INSTR_IRETD 81
#define INSTR_AAM 82
#define INSTR_AAD 83
#define INSTR_SALC 84
#define INSTR_XLAT 85
#define INSTR_FADD 86
#define INSTR_FMUL 87
#define INSTR_FCOM 88
#define INSTR_FCOMP 89
#define INSTR_FSUB 90
#define INSTR_FSUBR 91
#define INSTR_FDIV 92
#define INSTR_FDIVR 93
#define INSTR_FLD 94
#define INSTR_FXCH 95
#define INSTR_FST 96
#define INSTR_FNOP 97
#define INSTR_FSTP 98
#define INSTR_FLDENV 99
#define INSTR_FCHS 100
#define INSTR_FABS 101
#define INSTR_FTST 102
#define INSTR_FXAM 103
#define INSTR_FLDCW 104
#define INSTR_FLD1 105
#define INSTR_FLDL2T 106
#define INSTR_FLDL2E 107
#define INSTR_FLDPI 108
#define INSTR_FLDLG2 109
#define INSTR_FLDLN2 110
#define INSTR_FLDZ 111
#define INSTR_FNSTENV 112
#define INSTR_FSTENV 113
#define INSTR_F2XM1 114
#define INSTR_FYL2X 115
#define INSTR_FPTAN 116
#define INSTR_FPATAN 117
#define INSTR_FXTRACT 118
#define INSTR_FPREM1 119
#define INSTR_FDECSTP 120
#define INSTR_FINCSTP 121
#define INSTR_FNSTCW 122
#define INSTR_FSTCW 123
#define INSTR_FPREM 124
#define INSTR_FYL2XP1 125
#define INSTR_FSQRT 126
#define INSTR_FSINCOS 127
#define INSTR_FRNDINT 128
#define INSTR_FSCALE 129
#define INSTR_FSIN 130
#define INSTR_FCOS 131
#define INSTR_FIADD 132
#define INSTR_FCMOVB 133
#define INSTR_FIMUL 134
#define INSTR_FCMOVE 135
#define INSTR_FICOM 136
#define INSTR_FCMOVBE 137
#define INSTR_FICOMP 138
#define INSTR_FCMOVU 139
#define INSTR_FISUB 140
#define INSTR_FISUBR 141
#define INSTR_FUCOMPP 142
#define INSTR_FIDIV 143
#define INSTR_FIDIVR 144
#define INSTR_FILD 145
#define INSTR_FCMOVNB 146
#define INSTR_FISTTP 147
#define INSTR_FCMOVNE 148
#define INSTR_FIST 149
#define INSTR_FCMOVNBE 150
#define INSTR_FISTP 151
#define INSTR_FCMOVNU 152
#define INSTR_FNCLEX 153
#define INSTR_FCLEX 154
#define INSTR_FNINIT 155
#define INSTR_FINIT 156
#define INSTR_FUCOMI 157
#define INSTR_FCOMI 158
#define INSTR_FFREE 159
#define INSTR_FRSTOR 160
#define INSTR_FUCOM 161
#define INSTR_FUCOMP 162
#define INSTR_FNSAVE 163
#define INSTR_FSAVE 164
#define INSTR_FNSTSW 165
#define INSTR_FSTSW 166
#define INSTR_FADDP 167
#define INSTR_FMULP 168
#define INSTR_FCOMPP 169
#define INSTR_FSUBRP 170
#define INSTR_FSUBP 171
#define INSTR_FDIVRP 172
#define INSTR_FDIVP 173
#define INSTR_FBLD 174
#define INSTR_FUCOMIP 175
#define INSTR_FBSTP 176
#define INSTR_FCOMIP 177
#define INSTR_LOOPNZ 178
#define INSTR_LOOPZ 179
#define INSTR_LOOP 180
#define INSTR_JCXZ 181
#define INSTR_IN 182
#define INSTR_OUT 183
#define INSTR_CALL 184
#define INSTR_JMP 185
#define INSTR_JMPF 186
#define INSTR_INT1 187
#define INSTR_HLT 188
#define INSTR_CMC 189
#define INSTR_NOT 190
#define INSTR_NEG 191
#define INSTR_MUL 192
#define INSTR_DIV 193
#define INSTR_IDIV 194
#define INSTR_CLC 195
#define INSTR_STC 196
#define INSTR_CLI 197
#define INSTR_STI 198
#define INSTR_CLD 199
#define INSTR_STD 200
#define INSTR_INC 201
#define INSTR_DEC 202
#define INSTR_SLDT 203
#define INSTR_STR 204
#define INSTR_LLDT 205
#define INSTR_LTR 206
#define INSTR_VERR 207
#define INSTR_VERW 208
#define INSTR_SGDT 209
#define INSTR_VMCALL 210
#define INSTR_VMLAUNCH 211
#define INSTR_VMRESUME 212
#define INSTR_VMXOFF 213
#define INSTR_SIDT 214
#define INSTR_MONITOR 215
#define INSTR_MWAIT 216
#define INSTR_LGDT 217
#define INSTR_XGETBV 218
#define INSTR_XSETBV 219
#define INSTR_LIDT 220
#define INSTR_SMSW 221
#define INSTR_LMSW 222
#define INSTR_INVLPG 223
#define INSTR_RDTSCP 224
#define INSTR_LAR 225
#define INSTR_LSL 226
#define INSTR_CLTS 227
#define INSTR_INVD 228
#define INSTR_WBINVD 229
#define INSTR_UD2 230
#define INSTR_MOVUPS 231
#define INSTR_MOVSS 232
#define INSTR_MOVUPD 233
#define INSTR_MOVSD 234
#define INSTR_MOVHLPS 235
#define INSTR_MOVLPS 236
#define INSTR_MOVLPD 237
#define INSTR_MOVDDUP 238
#define INSTR_MOVSLDUP 239
#define INSTR_UNPCKLPS 240
#define INSTR_UNPCKLPD 241
#define INSTR_UNPCKHPS 242
#define INSTR_UNPCKHPD 243
#define INSTR_MOVLHPS 244
#define INSTR_MOVHPS 245
#define INSTR_MOVHPD 246
#define INSTR_MOVSHDUP 247
#define INSTR_PREFETCHNTA 248
#define INSTR_PREFETCHT0 249
#define INSTR_PREFETCHT1 250
#define INSTR_PREFETCHT2 251
#define INSTR_HINT_NOP 252
#define INSTR_MOVAPS 253
#define INSTR_MOVAPD 254
#define INSTR_CVTPI2PS 255
#define INSTR_CVTSI2SS 256
#define INSTR_CVTPI2PD 257
#define INSTR_CVTSI2SD 258
#define INSTR_MOVNTPS 259
#define INSTR_MOVNTPD 260
#define INSTR_CVTTPS2PI 261
#define INSTR_CVTTSS2SI 262
#define INSTR_CVTTPD2PI 263
#define INSTR_CVTTSD2SI 264
#define INSTR_CVTPS2PI 265
#define INSTR_CVTSS2SI 266
#define INSTR_CVTPD2PI 267
#define INSTR_CVTSD2SI 268
#define INSTR_UCOMISS 269
#define INSTR_UCOMISD 270
#define INSTR_COMISS 271
#define INSTR_COMISD 272
#define INSTR_WRMSR 273
#define INSTR_RDTSC 274
#define INSTR_RDMSR 275
#define INSTR_RDPMC 276
#define INSTR_SYSENTER 277
#define INSTR_SYSEXIT 278
#define INSTR_GETSEC 279
#define INSTR_PSHUFB 280
#define INSTR_PHADDW 281
#define INSTR_PHADDD 282
#define INSTR_PHADDSW 283
#define INSTR_PMADDUBSW 284
#define INSTR_PHSUBW 285
#define INSTR_PHSUBD 286
#define INSTR_PHSUBSW 287
#define INSTR_PSIGNB 288
#define INSTR_PSIGNW 289
#define INSTR_PSIGND 290
#define INSTR_PMULHRSW 291
#define INSTR_PBLENDVB 292
#define INSTR_BLENDVPS 293
#define INSTR_BLENDVPD 294
#define INSTR_PTEST 295
#define INSTR_PABSB 296
#define INSTR_PABSW 297
#define INSTR_PABSD 298
#define INSTR_PMOVSXBW 299
#define INSTR_PMOVSXBD 300
#define INSTR_PMOVSXBQ 301
#define INSTR_PMOVSXWD 302
#define INSTR_PMOVSXWQ 303
#define INSTR_PMOVSXDQ 304
#define INSTR_PMULDQ 305
#define INSTR_PCMPEQQ 306
#define INSTR_MOVNTDQA 307
#define INSTR_PACKUSDW 308
#define INSTR_PMOVZXBW 309
#define INSTR_PMOVZXBD 310
#define INSTR_PMOVZXBQ 311
#define INSTR_PMOVZXWD 312
#define INSTR_PMOVZXWQ 313
#define INSTR_PMOVZXDQ 314
#define INSTR_PCMPGTQ 315
#define INSTR_PMINSB 316
#define INSTR_PMINSD 317
#define INSTR_PMINUW 318
#define INSTR_PMINUD 319
#define INSTR_PMAXSB 320
#define INSTR_PMAXSD 321
#define INSTR_PMAXUW 322
#define INSTR_PMAXUD 323
#define INSTR_PMULLD 324
#define INSTR_PHMINPOSUW 325
#define INSTR_INVEPT 326
#define INSTR_INVVPID 327
#define INSTR_MOVBE 328
#define INSTR_CRC32 329
#define INSTR_ROUNDPS 330
#define INSTR_ROUNDPD 331
#define INSTR_ROUNDSS 332
#define INSTR_ROUNDSD 333
#define INSTR_BLENDPS 334
#define INSTR_BLENDPD 335
#define INSTR_PBLENDW 336
#define INSTR_PALIGNR 337
#define INSTR_PEXTRB 338
#define INSTR_PEXTRW 339
#define INSTR_PEXTRD 340
#define INSTR_EXTRACTPS 341
#define INSTR_PINSRB 342
#define INSTR_INSERTPS 343
#define INSTR_PINSRD 344
#define INSTR_DPPS 345
#define INSTR_DPPD 346
#define INSTR_MPSADBW 347
#define INSTR_PCMPESTRM 348
#define INSTR_PCMPESTRI 349
#define INSTR_PCMPISTRM 350
#define INSTR_PCMPISTRI 351
#define INSTR_CMOVO 352
#define INSTR_CMOVNO 353
#define INSTR_CMOVB 354
#define INSTR_CMOVNB 355
#define INSTR_CMOVZ 356
#define INSTR_CMOVNZ 357
#define INSTR_CMOVBE 358
#define INSTR_CMOVNBE 359
#define INSTR_CMOVS 360
#define INSTR_CMOVNS 361
#define INSTR_CMOVP 362
#define INSTR_CMOVNP 363
#define INSTR_CMOVL 364
#define INSTR_CMOVNL 365
#define INSTR_CMOVLE 366
#define INSTR_CMOVNLE 367
#define INSTR_MOVMSKPS 368
#define INSTR_MOVMSKPD 369
#define INSTR_SQRTPS 370
#define INSTR_SQRTSS 371
#define INSTR_SQRTPD 372
#define INSTR_SQRTSD 373
#define INSTR_RSQRTPS 374
#define INSTR_RSQRTSS 375
#define INSTR_RCPPS 376
#define INSTR_RCPSS 377
#define INSTR_ANDPS 378
#define INSTR_ANDPD 379
#define INSTR_ANDNPS 380
#define INSTR_ANDNPD 381
#define INSTR_ORPS 382
#define INSTR_ORPD 383
#define INSTR_XORPS 384
#define INSTR_XORPD 385
#define INSTR_ADDPS 386
#define INSTR_ADDSS 387
#define INSTR_ADDPD 388
#define INSTR_ADDSD 389
#define INSTR_MULPS 390
#define INSTR_MULSS 391
#define INSTR_MULPD 392
#define INSTR_MULSD 393
#define INSTR_CVTPS2PD 394
#define INSTR_CVTPD2PS 395
#define INSTR_CVTSS2SD 396
#define INSTR_CVTSD2SS 397
#define INSTR_CVTDQ2PS 398
#define INSTR_CVTPS2DQ 399
#define INSTR_CVTTPS2DQ 400
#define INSTR_SUBPS 401
#define INSTR_SUBSS 402
#define INSTR_SUBPD 403
#define INSTR_SUBSD 404
#define INSTR_MINPS 405
#define INSTR_MINSS 406
#define INSTR_MINPD 407
#define INSTR_MINSD 408
#define INSTR_DIVPS 409
#define INSTR_DIVSS 410
#define INSTR_DIVPD 411
#define INSTR_DIVSD 412
#define INSTR_MAXPS 413
#define INSTR_MAXSS 414
#define INSTR_MAXPD 415
#define INSTR_MAXSD 416
#define INSTR_PUNPCKLBW 417
#define INSTR_PUNPCKLWD 418
#define INSTR_PUNPCKLDQ 419
#define INSTR_PACKSSWB 420
#define INSTR_PCMPGTB 421
#define INSTR_PCMPGTW 422
#define INSTR_PCMPGTD 423
#define INSTR_PACKUSWB 424
#define INSTR_PUNPCKHBW 425
#define INSTR_PUNPCKHWD 426
#define INSTR_PUNPCKHDQ 427
#define INSTR_PACKSSDW 428
#define INSTR_PUNPCKLQDQ 429
#define INSTR_PUNPCKHQDQ 430
#define INSTR_MOVD 431
#define INSTR_MOVQ 432
#define INSTR_MOVDQA 433
#define INSTR_MOVDQU 434
#define INSTR_PSHUFW 435
#define INSTR_PSHUFLW 436
#define INSTR_PSHUFHW 437
#define INSTR_PSHUFD 438
#define INSTR_PSRLW 439
#define INSTR_PSRAW 440
#define INSTR_PSLLW 441
#define INSTR_PSRLD 442
#define INSTR_PSRAD 443
#define INSTR_PSLLD 444
#define INSTR_PSRLQ 445
#define INSTR_PSRLDQ 446
#define INSTR_PSLLQ 447
#define INSTR_PSLLDQ 448
#define INSTR_PCMPEQB 449
#define INSTR_PCMPEQW 450
#define INSTR_PCMPEQD 451
#define INSTR_EMMS 452
#define INSTR_VMREAD 453
#define INSTR_VMWRITE 454
#define INSTR_HADDPD 455
#define INSTR_HADDPS 456
#define INSTR_HSUBPD 457
#define INSTR_HSUBPS 458
#define INSTR_SETO 459
#define INSTR_SETNO 460
#define INSTR_SETB 461
#define INSTR_SETNB 462
#define INSTR_SETZ 463
#define INSTR_SETNZ 464
#define INSTR_SETBE 465
#define INSTR_SETNBE 466
#define INSTR_SETS 467
#define INSTR_SETNS 468
#define INSTR_SETP 469
#define INSTR_SETNP 470
#define INSTR_SETL 471
#define INSTR_SETNL 472
#define INSTR_SETLE 473
#define INSTR_SETNLE 474
#define INSTR_CPUID 475
#define INSTR_BT 476
#define INSTR_SHLD 477
#define INSTR_RSM 478
#define INSTR_BTS 479
#define INSTR_SHRD 480
#define INSTR_FXSAVE 481
#define INSTR_FXRSTOR 482
#define INSTR_LDMXCSR 483
#define INSTR_STMXCSR 484
#define INSTR_XSAVE 485
#define INSTR_LFENCE 486
#define INSTR_XRSTOR 487
#define INSTR_MFENCE 488
#define INSTR_SFENCE 489
#define INSTR_CLFLUSH 490
#define INSTR_CMPXCHG 491
#define INSTR_LSS 492
#define INSTR_BTR 493
#define INSTR_LFS 494
#define INSTR_LGS 495
#define INSTR_MOVZX 496
#define INSTR_POPCNT 497
#define INSTR_BTC 498
#define INSTR_BSF 499
#define INSTR_BSR 500
#define INSTR_MOVSX 501
#define INSTR_XADD 502
#define INSTR_CMPPS 503
#define INSTR_CMPSS 504
#define INSTR_CMPPD 505
#define INSTR_CMPSD 506
#define INSTR_MOVNTI 507
#define INSTR_PINSRW 508
#define INSTR_SHUFPS 509
#define INSTR_SHUFPD 510
#define INSTR_CMPXCHG8B 511
#define INSTR_VMPTRLD 512
#define INSTR_VMCLEAR 513
#define INSTR_VMXON 514
#define INSTR_VMPTRST 515
#define INSTR_ADDSUBPD 516
#define INSTR_ADDSUBPS 517
#define INSTR_PADDQ 518
#define INSTR_PMULLW 519
#define INSTR_MOVQ2DQ 520
#define INSTR_MOVDQ2Q 521
#define INSTR_PMOVMSKB 522
#define INSTR_PSUBUSB 523
#define INSTR_PSUBUSW 524
#define INSTR_PMINUB 525
#define INSTR_PAND 526
#define INSTR_PADDUSB 527
#define INSTR_PADDUSW 528
#define INSTR_PMAXUB 529
#define INSTR_PANDN 530
#define INSTR_PAVGB 531
#define INSTR_PAVGW 532
#define INSTR_PMULHUW 533
#define INSTR_PMULHW 534
#define INSTR_CVTPD2DQ 535
#define INSTR_CVTTPD2DQ 536
#define INSTR_CVTDQ2PD 537
#define INSTR_MOVNTQ 538
#define INSTR_MOVNTDQ 539
#define INSTR_PSUBSB 540
#define INSTR_PSUBSW 541
#define INSTR_PMINSW 542
#define INSTR_POR 543
#define INSTR_PADDSB 544
#define INSTR_PADDSW 545
#define INSTR_PMAXSW 546
#define INSTR_PXOR 547
#define INSTR_LDDQU 548
#define INSTR_PMULUDQ 549
#define INSTR_PMADDWD 550
#define INSTR_PSADBW 551
#define INSTR_MASKMOVQ 552
#define INSTR_MASKMOVDQU 553
#define INSTR_PSUBB 554
#define INSTR_PSUBW 555
#define INSTR_PSUBD 556
#define INSTR_PSUBQ 557
#define INSTR_PADDB 558
#define INSTR_PADDW 559
#define INSTR_PADDD 560

#endif
