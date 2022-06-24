; ModuleID = 'test.ll'
source_filename = "test.ll"
target triple = "x86_64-pc-linux-gnu"

define i32 @main(i32 %0, i8** nocapture readonly %1) local_unnamed_addr {
entry:
  %.not = icmp eq i32 %0, 2
  br i1 %.not, label %2, label %common.ret

common.ret:                                       ; preds = %entry, %strlen.exit
  ret i32 0

2:                                                ; preds = %entry
  %3 = getelementptr i8*, i8** %1, i64 1
  %4 = load i8*, i8** %3, align 8
  %5 = load i8, i8* %4, align 1
  %.not4.i = icmp eq i8 %5, 0
  br i1 %.not4.i, label %strlen.exit, label %.lr.ph.i

.lr.ph.i:                                         ; preds = %2, %.lr.ph.i
  %.06.i = phi i64 [ %6, %.lr.ph.i ], [ 0, %2 ]
  %.035.i = phi i8* [ %7, %.lr.ph.i ], [ %4, %2 ]
  %6 = add i64 %.06.i, 1
  %7 = getelementptr i8, i8* %.035.i, i64 1
  %8 = load i8, i8* %7, align 1
  %.not.i = icmp eq i8 %8, 0
  br i1 %.not.i, label %strlen.exit, label %.lr.ph.i

strlen.exit:                                      ; preds = %.lr.ph.i, %2
  %.0.lcssa.i = phi i64 [ 0, %2 ], [ %6, %.lr.ph.i ]
  tail call void @printint(i64 %.0.lcssa.i)
  br label %common.ret
}

; Function Attrs: nofree norecurse nosync nounwind readonly
define i64 @strlen(i8* nocapture readonly %0) local_unnamed_addr #0 {
entry:
  %1 = load i8, i8* %0, align 1
  %.not4 = icmp eq i8 %1, 0
  br i1 %.not4, label %._crit_edge, label %.lr.ph

.lr.ph:                                           ; preds = %entry, %.lr.ph
  %.06 = phi i64 [ %2, %.lr.ph ], [ 0, %entry ]
  %.035 = phi i8* [ %3, %.lr.ph ], [ %0, %entry ]
  %2 = add i64 %.06, 1
  %3 = getelementptr i8, i8* %.035, i64 1
  %4 = load i8, i8* %3, align 1
  %.not = icmp eq i8 %4, 0
  br i1 %.not, label %._crit_edge, label %.lr.ph

._crit_edge:                                      ; preds = %.lr.ph, %entry
  %.0.lcssa = phi i64 [ 0, %entry ], [ %2, %.lr.ph ]
  ret i64 %.0.lcssa
}

declare void @printint(i64) local_unnamed_addr

attributes #0 = { nofree norecurse nosync nounwind readonly }
