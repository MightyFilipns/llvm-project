; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=x86_64-- -mattr=avx512bf16,avx512fp16      | FileCheck %s

define void @test(<2 x ptr> %ptr) {
; CHECK-LABEL: test:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    testb %al, %al
; CHECK-NEXT:    je .LBB0_1
; CHECK-NEXT:  # %bb.2: # %loop.127.preheader
; CHECK-NEXT:    retq
; CHECK-NEXT:  .LBB0_1: # %ifmerge.89
; CHECK-NEXT:    vbroadcastss %xmm0, %xmm0
; CHECK-NEXT:    vpxor %xmm1, %xmm1, %xmm1
; CHECK-NEXT:    vpbroadcastw (%rax), %xmm2
; CHECK-NEXT:    vpunpcklwd {{.*#+}} xmm1 = xmm1[0],xmm2[0],xmm1[1],xmm2[1],xmm1[2],xmm2[2],xmm1[3],xmm2[3]
; CHECK-NEXT:    vmulps %xmm1, %xmm0, %xmm0
; CHECK-NEXT:    vmovlps %xmm0, (%rax)
entry:
  br label %then.13

then.13:                                          ; preds = %entry
  %0 = fpext bfloat poison to float
  br i1 poison, label %loop.127.preheader, label %ifmerge.89

ifmerge.89:                                       ; preds = %then.13
  %.splatinsert144 = insertelement <2 x float> poison, float %0, i64 0
  %.splat145 = shufflevector <2 x float> %.splatinsert144, <2 x float> poison, <2 x i32> zeroinitializer
  %1 = tail call <2 x bfloat> @llvm.masked.gather.v2bf16.v2p0(<2 x ptr> undef, i32 2, <2 x i1> <i1 true, i1 true>, <2 x bfloat> poison)
  %2 = fpext <2 x bfloat> %1 to <2 x float>
  %3 = fmul fast <2 x float> %.splat145, %2
  %4 = fadd fast <2 x float> zeroinitializer, %3
  store <2 x float> %4, ptr poison, align 4
  unreachable

loop.127.preheader:                               ; preds = %then.13
  ret void
}

declare <2 x bfloat> @llvm.masked.gather.v2bf16.v2p0(<2 x ptr>, i32 immarg, <2 x i1>, <2 x bfloat>)
