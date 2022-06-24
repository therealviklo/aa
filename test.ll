; ModuleID = 'test.ll'
source_filename = "test.ll"
target triple = "x86_64-pc-linux-gnu"

@0 = private unnamed_addr constant [6 x i8] c"%lli\0A\00", align 1

declare void @free(ptr)

define void @freeILL(ptr %0) {
entry:
  %1 = ptrtoint ptr %0 to i64
  %2 = icmp ne i64 %1, 0
  br i1 %2, label %3, label %6

3:                                                ; preds = %entry
  %4 = getelementptr { i64, ptr }, ptr %0, i32 0, i32 1
  %5 = load ptr, ptr %4, align 8
  call void @freeILL(ptr %5)
  call void @free(ptr %0)
  br label %6

6:                                                ; preds = %3, %entry
  ret void
}

define i64 @fst(ptr %0) {
entry:
  %1 = getelementptr { i64, ptr }, ptr %0, i32 0, i32 0
  %2 = load i64, ptr %1, align 4
  ret i64 %2
}

define i32 @main(i32 %0, ptr %1) {
entry:
  %2 = alloca ptr, align 8
  %3 = call ptr @pushILL(ptr null, i64 2)
  %4 = call ptr @pushILL(ptr %3, i64 1)
  store ptr %4, ptr %2, align 8
  %5 = load ptr, ptr %2, align 8
  %6 = call ptr @rst(ptr %5)
  %7 = call i64 @fst(ptr %6)
  %8 = call i32 (ptr, ...) @printf(ptr bitcast ([6 x i8]* @0 to ptr), i64 %7)
  %9 = load ptr, ptr %2, align 8
  call void @freeILL(ptr %9)
  ret i32 0
}

define ptr @pushILL(ptr %0, i64 %1) {
entry:
  %2 = alloca ptr, align 8
  %3 = call ptr @malloc(i64 16)
  store ptr %3, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = getelementptr { i64, ptr }, ptr %4, i32 0, i32 0
  store i64 %1, ptr %5, align 4
  %6 = load ptr, ptr %2, align 8
  %7 = getelementptr { i64, ptr }, ptr %6, i32 0, i32 1
  store ptr %0, ptr %7, align 8
  %8 = load ptr, ptr %2, align 8
  ret ptr %8
}

define ptr @rst(ptr %0) {
entry:
  %1 = getelementptr { i64, ptr }, ptr %0, i32 0, i32 1
  %2 = load ptr, ptr %1, align 8
  ret ptr %2
}

declare i32 @printf(ptr, ...)

declare ptr @malloc(i64)

declare i32 @puts(ptr)

declare i32 @scanf(ptr, ...)
