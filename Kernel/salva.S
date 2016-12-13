.text
.data
.global RestoreContext, GoTask, CallSWI, MoveToSP, SystemContext, Interrupt

Interrupt:
stmfd sp,{sp}^
nop
sub sp,sp,#4
stmfd sp!,{r0-r12}
mrs r1,spsr
stmfd sp!,{r1,r14}
mov r0,sp
ldr pc,=Select
mov pc,lr

GoTask:
mov sp,r0
msr cpsr,#0x10
mov pc,r1
mov pc,lr

CallSWI:
swi
mov pc,lr

MoveToSP:
mov sp,r0
mov pc,lr

SystemContext:
stmfd sp,{sp}^
nop
sub sp,sp,#4
stmfd sp!,{r0-r12}
add lr,lr,#4
mrs r0,spsr
stmfd sp!,{r0,r14}
mov r0,sp
ldr pc,=DoSystemCall
mov pc,lr

RestoreContext:
ldmfd r0!,{r1,r14}
msr spsr_cxsf,r1
mov sp,r0
ldmfd sp!,{r0-r12}
ldmfd sp,{sp}^
nop
add sp,sp,#4
subs pc,r14,#4
