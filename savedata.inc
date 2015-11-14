;========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1=========2=========3=========4=========5=========6=========7**
;Author: Floyd Holliday
;Email: holliday@ecs.fullerton.edu
;
;File name: savedata.inc
;Purpose: This file contains only macros.  Each macro implements a process that is frequently found in applications built in X86 language.
;This file is not assembled alone.  The assembler will assemble this file when assembling the source program that includes this file.
;Status: This macro has been tested extensively by many students with no discovered errors.  The two macros have been briefly tested in the environment where some para-
;meters are passed to the subprogram via the system stack.  During that brief testing of passing data on the stack no errors were discovered, however, that situation,
;pass by stack, should be subjected to further testing.

;System requirements: 1.  x86 64-bit CPU.
;                     2.  Instructions xsave and xrstor are present.
;Run-time requirements: the one input parameter is a pointer to (at least) 952 bytes of available storage.
;Caution:  It is important that instructions xsave and xrestor be implemented in the processor.  Without those instructions this software will fail (halt suddendly).

;Usage: This software is created as a teaching tool for the first semester assembly class.  Students enrolled in the course are expected to learn more from complete working
;sample programs than from reading a textbook.  This macro is one piece of several source files that together constitute one single program.  This software is for use only
;by students enrolled in the professor's assembly language programming class.  All other use is illegal and prohibited.

;Learning: There are multiple sources of knowledge when learning assembly programming.  Here are some sources ordered from highly important to less important:
;   1. Complete functioning well-documented open-source X86 programs.
;   2. On-line sources: forums and sample programs.
;   3. Discussions with the instructor
;   4. Textbooks
;   5. Peers 
;===========================================================================================================================================================================

%macro mainbackup 1
;One parameter is required; it must be a pointer to at least 952 bytes of available storage.
;
;The purpose of this macro is to back up (almost) all data in the CPU.  Registers rsp, rip, rax are intentionally not backed up.  All other GPRs are backed up and all
;registers in state components are backed up (provided the platform supports instructions xsave and xrstor).

;Preconditions: what must be true before calling this macro
;1.  The single parameter passed to this macro must be an array of size 952 bytes or more.  That array will be used to store the data held in the state components.
;2.  The underlying platform must include an X86 processor that supports the xsave instruction.

;Postconditions: what will be true after a call to this macro completes
;1.  All data stored in state components will be copied to the array parameter: using the first 832 bytes of the array.
;2.  Fifteen GPRs will be backed up into the array parameter: using the next 120 bytes of the array following the state component data.

segment .data

.nostatecomponentbackup db "Warning: unable to backup state components: processing continues without benefit of backup.", 10, 0

segment .text                                               ;Begin the executable section of this macro

;========== Section that backs up General Purpose Registers ===============================================================================================================

mov        [%1+104*8], rbp                                  ;Back up rbp
mov        [%1+105*8], rbx                                  ;Back up rbx
mov        [%1+106*8], rcx                                  ;Back up rcx
mov        [%1+107*8], rdx                                  ;Back up rdx
mov        [%1+108*8], rdi                                  ;Back up rdi
mov        [%1+109*8], rsi                                  ;Back up rsi
mov        [%1+110*8], r8                                   ;Back up r8
mov        [%1+111*8], r9                                   ;Back up r9
mov        [%1+112*8], r10                                  ;Back up r10
mov        [%1+113*8], r11                                  ;Back up r11
mov        [%1+114*8], r12                                  ;Back up r12
mov        [%1+115*8], r13                                  ;Back up r13
mov        [%1+116*8], r14                                  ;Back up r14
mov        [%1+117*8], r15                                  ;Back up r15
pushf                                                       ;The rflags register is on top of the stack.
pop        r15                                              ;A copy of rflags is in r15
mov        [%1+118*8], r15                                  ;A copy of rflags is in [%1+118*8], where it should be.
mov        r15, [%1+117*8]                                  ;Return to r15 the value it should be holding.

;========== Section that backs up state component data ====================================================================================================================

;Obtain a copy of the state component bit map
mov        rax, 13                                          ;13 is a parameter passed to cpuid; 13 is given in the Intel X86 programming guide
mov        rcx, 0                                           ;0 is a parameter passed to cpuid
cpuid
;Now the bitmap is in edx:eax.  Instruction cpuid is known to modify rbx.

;Insure that the bitmap includes no more than the 3 state components found in the Core-i series of microprocessors.  In the future when X86 processor have more than 3
;state components this macro will need to be modified to handle the larger number of components.
mov        r15, 111b                                        ;7 is represented by 111 in base 2
and        rax, r15                                         ;This instruction zeros out all bits of rax except the lowest 3 bits, which are left unchanged.
xor        rdx, rdx                                         ;This is a fast zero out of rdx

;Make a copy of all the data in all the state components.  The bitmap in edx:eax is an input parameter to xsave.
xsave      [%1]

;Registers rcx, rdx, r15 were modified directly above this point.  Therefore, their original values are restored here.  rax was modified but its value is not restored.
mov        rcx, [%1+106*8]                                  ;rcx receives the value it had before this macro was called.
mov        rdx, [%1+107*8]                                  ;rdx receives  "    "   "   "    "     "     "    "    "
mov        r15, [%1+117*8]                                  ;r15 receives  "    "   "   "    "     "     "    "    "

;========== End section that backs up state component data ================================================================================================================

;;;push rdi                                                    ;Save the value of rdi on the stack
;;;push rsi                                                    ;Save the value of rsi on the stack
;;;push rdx

;;;Output a message stating that state component data were not backed up.
;;;mov rdi, 1                                                  ;Input parameter rdi must be 1
;;;mov rsi, .nostatecomponentbackup                            ;"Warning: unable to backup state components: processing continues without benefit of backup."
;;;mov rdx, 93                                                 ;Parameter rdx holds number of characters to be outputted to stdout; don't forget trailing non-printing chars.
;;;mov rax, 1                                                  ;rax must be 1.  Try omitting a value for rax: the string will not be printed.
;;;syscall

;;;pop        rdx                                              ;Restore value to rdx
;;;pop        rsi                                              ;   "      "   "  rsi
;;;pop        rdi                                              ;   "      "   "  rdi



;At this time the value of rbp has been stored at [%1+104*8].  Quadwords [%1+0*8] through [%1+103*8], 832 total bytes, are used for back up of state components 1, 2, & 3.

%endmacro                                                   ;Control now returns to the activator of this macro.

;==========================================================================================================================================================================
%macro mainrestore 1
;Parameter 1 is a pointer to at least 952 bytes of available storage.


;The purpose of this macro is to restore data to all the state component registers from the array %1, and to restore data to (almost) all GPRs from the system stack.

;Preconditions
;1.  The single parameter is an array of at least 832 bytes, and that array contains the data to be restored to the registers.
;2.  The CPU of the current machine must support the xrstor instruction.

;Postconditions
;1.  Original values corresponding to the time the backup macro was invoked will be restored in all GPRs and all state component registers.
;2   Registers xmm0, rax, rip, rsp will not have original values restored.

;Begin the executable section of this macro

;Save copies of data in rax and xmm0.  These two registers will not be restored.  Their values will be preserved.
push       rax                                              ;Save current value of rax
push qword 0                                                ;Create storage for xmm0
movsd      [rsp], xmm0                                      ;Copy the value in xmm0 to the new storage.

;========== Section that restores state component data ====================================================================================================================

;Obtain a copy of the state component bit map
mov        rax, 13                                          ;13 is a parameter passed to cpuid; 13 is given in the Intel X86 programming guide
mov        rcx, 0                                           ;0 is a parameter passed to cpuid
cpuid
;Now the bitmap is in edx:eax.  Instruction cpuid is know to modify rbx.

;Insure that the bitmap includes no more than the 3 state components found in the Core-i series of microprocessors.
mov        r15, 111b                                        ;7 is represented by 111 in base 2
and        rax, r15                                         ;This instruction zeros out all bits of rax except the lowest 3 bits, which are left unchanged.
xor        rdx, rdx                                         ;This is a fast zero out of rdx

;Copy data from heap space %1 to each register of the statecomponents.  The bitmap in edx:eax is an input parameter to xrstor.
xrstor     [%1]

;========== Section that restores data to GPRs ============================================================================================================================

mov        r15, [%1+118*8]                                  ;Copy the rflag data to r15
push       r15                                              ;Push the data belonging to rflag onto the stack
popf                                                        ;The previously saved value has been restore to rflags
mov        r15, [%1+117*8]                                  ;Restore r15
mov        r14, [%1+116*8]                                  ;Restore r14
mov        r13, [%1+115*8]                                  ;Restore r13
mov        r12, [%1+114*8]                                  ;Restore r12
mov        r11, [%1+113*8]                                  ;Restore r11
mov        r10, [%1+112*8]                                  ;Restore r10
mov        r9,  [%1+111*8]                                  ;Restore r9
mov        r8,  [%1+110*8]                                  ;Restore r8
mov        rsi, [%1+109*8]                                  ;Restore rsi
mov        rdi, [%1+108*8]                                  ;Restore rdi
mov        rdx, [%1+107*8]                                  ;Restore rdx
mov        rcx, [%1+106*8]                                  ;Restore rcx
mov        rbx, [%1+105*8]                                  ;Restore rbx
mov        rbp, [%1+104*8]                                  ;Restore rbp

;Restore data to both xmm0 and rax.  These two values are preserved because the caller of this macro may want to use one of them as a return value.
movsd      xmm0, [rsp]                                      ;Restore the former value of xmm0
pop        rax                                              ;Discard top of stack
pop        rax                                              ;Restore the former value of rax

%endmacro                                                   ;Control now returns to the activator of this macro.
;========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1=========2=========3=========4=========5=========6=========7**
