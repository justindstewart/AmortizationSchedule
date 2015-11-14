;========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1=========2=========3=========4=========5=========6=========7**
;Author information
;  Author name: Justin Stewart
;  Author email: scubastew@csu.fullerton.edu
;  Author location: Long Beach, Calif.
;Course information
;  Course number: CPSC240
;  Assignment number: 3
;  Due date: 2015-Sep-10
;Project information
;  Project title: Amortization Schedule
;  Purpose: This project will prompt the user for a loan amount(principle), an interest rate, and a time frame of payback for the loan (in months) and return to the user
;           an amortization schedule that indicates to the user monthly payments and how much of each payment is paying interest vs. the principle.
;  Status: In progress.
;  Project files: amortmain.asm, amortdriver.c
;Module information
;  File name: amortmain.asm
;  This module's call name: amort 
;  Language: X86-64
;  Syntax: Intel
;  Date last modified: 2015-Sep-27
;  Purpose: Calculate and output the amortization schedule as calculated using the users input.
;  Future enhancements: None planned.
;Translator information
;  Linux: nasm -f elf64 -l amortmain.lis -o amortmain.o amortmain.asm
;References and credits
;  http://unixwiz.net/techtips/x86-jumps.html - Information on jumps.
;Format information
;  Page width: 172 columns
;  Begin comments: 61
;  Optimal print specification: Landscape, 7 points, monospace, 8½x11 paper
;Permission Information
;  This work is private and shall not be posted online, copied or referenced. 
;===== Begin area for source code =========================================================================================================================================

%include "savedata.inc"					    ;External macro used in backup of registers.
	
extern printf                                               ;External program used to output data, backup of registers is important if data is to be preserved.

extern scanf						    ;External program used to retrieve data, backup of registers is important if data is to be preserved.

extern paymentcalc					    ;External C function used to calculate the monthly payment.

global amort                                                ;Set a global call name in order to be called by another program, i.e. the driver.

segment .data                                               ;Initialized data are placed in this segment.

;===== Messages to be output to user ======================================================================================================================================

welcome db "Welcome to the Bank of New Vegas.", 10, 0

title db "Justin Stewart, Chief Loan Officer", 10, 10, 0

interestInput db "Please enter the current prime interest rate as a float number: ", 0

amountInput db "Enter the amounts of the loan: ", 0

timeInput db "Enter the time of the loan as a whole number of months: ", 0

schedule db "The repayment schedule for this loan is: ", 10, 10, 0

headerone db "Month		   Monthly		   Paid on	   	  Paid on		Principle", 10, 0

headertwo db "Number		   Payment		   Interest	   	  Principle		at end of month", 10, 0

lineout db "%ld		%9.2lf		%9.2lf		%9.2lf		%9.2lf", 10, 0

totals db "Totals		%9.2lf		%9.2lf		%9.2lf", 10, 0

thanks db 10, "Thank you for your inquiry at our bank.", 10, 0

farewell db "This program will now return the total interest paid to the driver.", 10, 0

;===== Declare output formats =============================================================================================================================================

stringformat db "%s", 0

;===== Declare input formats ==============================================================================================================================================

floatformat db "%lf", 0

intformat db "%ld", 0

segment .bss                                                ;Uninitialized data are declared in this segment.

align 64                                                    ;Insure that the next data declaration starts on a 64-byte boundary.
backuparea resb 952                                         ;Create an array for backup storage having 952 bytes.

;==========================================================================================================================================================================
;===== Begin the application here: Amortization Schedule Calculator =======================================================================================================
;==========================================================================================================================================================================

segment .text                                               ;Instructions for the program are placed in the following segment.

amort:                                                      ;Begin execution of the program here.

;The next two instructions must be performed at the start of every assembly program.
push       rbp                                              ;This marks the start of a new stack frame belonging to this execution of this function.
mov        rbp, rsp                                         ;rbp holds the address of the start of this new stack frame.  When this function returns to its caller rbp must
                                                            ;hold the same value it holds now.

;===== Output welcome message =============================================================================================================================================
;No important data in registers, therefore printf will be called without backup of any data.

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers.
mov        rdi, stringformat                                ;"%s"
mov        rsi, welcome                                     ;"Welcome to the Bank of New Vegas."
call printf                                                 ;An external function that handles output.

;===== Output title =======================================================================================================================================================
;No important data in registers, therefore printf will be called without backup of any data.

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers.
mov        rdi, stringformat                                ;"%s"
mov        rsi, title                                       ;"Justin Stewart, Chief Loan Officer"
call printf                                                 ;An external function that handles output.

;===== Output interest prompt =============================================================================================================================================
;No important data in registers, therefore printf will be called without backup of any data.

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers.
mov        rdi, stringformat                                ;"%s"
mov        rsi, interestInput                               ;"Please enter the current prime interest rate as a float number: "
call printf                                                 ;An external function that handles output

;===== Receive interest from user =========================================================================================================================================
;No important data in registers, therefore scanf will be called without backup of any data.

mov  qword rax, 0					    ;Zero indicates to scanf that there is no data to grab from lower registers.
mov 	   rdi, floatformat				    ;"%lf"
push qword 0						    ;Reserve 8 bytes of storage for the incomming number.
mov 	   rsi, rsp					    ;Give scanf a point to the reserved storage.

;Integer stack as of last instruction:
;
;          |------|
;   rsp+8: |rflags|
;          |------|
;   rsp:   |  0.0 | rsi
;          |------|

call scanf					    	    ;An external function that handles input from user.

;Integer stack as of last instruction:
;
;          |------|
;   rsp+8: |rflags|
;          |------|
;   rsp:   | APR  | rsi
;          |------|

movsd      xmm15, [rsp]					    ;Move the inputted APR into xmm15 to keep until paymentcalc function call.

pop	   rax						    ;Make free the storage that was used by scanf.

;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    APR  |
;         |-----------------------|

;Integer stack as of last instruction:
;
;          |------|
;   rsp:   |rflags|
;          |------|

;===== Store the value of (12) in xmm10 to be used for calculations =======================================================================================================

mov 	   rax, 0x4028000000000000			    ;Copy constant 0x4028000000000000 = (12) (decimal) into rax so it can be pushed to stack.
push       rax						    ;Place the constant on the integer stack.
movsd      xmm10, [rsp]					    ;Copy the constant from integer stack to xmm10.
pop        rax						    ;Discard the constant from the top of the stack.

;===== Divide the APR by 12 in order to acquire the periodic interest rate (PIR) ==========================================================================================

divsd xmm15, xmm10					    ;Divide the user inputted APR by 12 in order to acquire periodic interest rate (PIR).

;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|

;===== Output principle prompt ============================================================================================================================================
;Important data in top most registers, printf should NOT change this data. If it is corrupted, try backup at this point. 

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers.
mov        rdi, stringformat                                ;"%s"
mov        rsi, amountInput                                 ;"Enter the amount of the loan: "
call printf                                                 ;An external function that handles output.

;===== Receive principle from user ========================================================================================================================================
;Important data in top most registers, scanf should NOT change this data. If it is corrupted, try backup at this point. 

mov  qword rax, 0					    ;Zero indicates to scanf that there is no data to grab from lower registers.
mov 	   rdi, floatformat				    ;"%lf"
push qword 0						    ;Reserve 8 bytes of storage for the incomming number.
mov 	   rsi, rsp					    ;Give scanf a point to the reserved storage.

;Integer stack as of last instruction:
;
;          |------|
;   rsp+8: |rflags|
;          |------|
;   rsp:   |  0.0 | rsi
;          |------|

call scanf					    	    ;An external function that handles input from user.

;Integer stack as of last instruction:
;
;          |------|
;   rsp+8: |rflags|
;          |------|
;   rsp:   | LOAN | rsi
;          |------|

movsd      xmm14, [rsp]					    ;Move the inputted loan amount (principle) into xmm14 to keep until paymentcalc function call.

pop	   rax						    ;Make free the storage that was used by scanf.

;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|
;  ymm14: |  -    -     -    LOAN |
;         |-----------------------|

;Integer stack as of last instruction:
;
;          |------|
;   rsp:   |rflags|
;          |------|

;===== Output time prompt =================================================================================================================================================
;Important data in top most registers, printf should NOT change this data. If it is corrupted, try backup at this point. 

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers.
mov        rdi, stringformat                                ;"%s"
mov        rsi, timeInput                                   ;"Enter the time of the loan as a whole number of months: "
call printf                                                 ;An external function that handles output.

;===== Receive time from user =============================================================================================================================================
;Important data in top most registers, scanf should NOT change this data. If it is corrupted, try backup at this point. 

mov  qword rax, 0					    ;Zero indicates to scanf that there is no data to grab from lower registers.
mov 	   rdi, intformat				    ;"%ld"
push qword 0						    ;Reserve 8 bytes of storage for the incomming number.
mov 	   rsi, rsp					    ;Give scanf a point to the reserved storage.

;Integer stack as of last instruction:
;
;          |------|
;   rsp+8: |rflags|
;          |------|
;   rsp:   |  0.0 | rsi
;          |------|

call scanf					    	    ;An external function that handles input from user.

;Integer stack as of last instruction:
;
;          |------|
;   rsp+8: |rflags|
;          |------|
;   rsp:   | TIME | rsi
;          |------|

mov        r15, [rsp]					    ;Move the inputted time into r15 to keep until paymentcalc function call.

pop	   rax						    ;Make free the storage that was used by scanf.

;General Purpose Registers as of last instruction:
;
;         |-----------------------|
;  r15:   |  -    -     -    TIME |
;         |-----------------------|

;Integer stack as of last instruction:
;
;          |------|
;   rsp:   |rflags|
;          |------|

;===== Move data into lower registers and call paymentcalc ================================================================================================================
;Important data in top most registers, paymentcalc should NOT change this data. If it is corrupted, try backup at this point. 

movsd 	   xmm0, xmm15					    ;Copy the calculated periodic rate (PIR) from xmm15 to xmm0 for function call.
movsd 	   xmm1, xmm14					    ;Copy the loan amount from xmm14 to xmm1 for function call.
mov 	   rdi, r15					    ;Copy the time(in months) from r15 to rdi for function call.

call paymentcalc					    ;Extern C functioned used to calculate the monthly payment.

movsd 	   xmm13, xmm0					    ;Copy the calculated monthly payment from xmm0 into xmm13.

;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|
;  ymm14: |  -    -     -    LOAN |
;         |-----------------------|
;  ymm13: |  -    -     -    MPAY |
;         |-----------------------|

;General Purpose Registers as of last instruction:
;
;         |-----------------------|
;  r15:   |  -    -     -    TIME |
;         |-----------------------|

;===== Heading Output =====================================================================================================================================================
;Important data in top most registers, printf should NOT change this data. If it is corrupted, try backup at this point. 

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers.
mov        rdi, stringformat                                ;"%s"
mov        rsi, schedule                                    ;"The repayment schedule for this loan is: "
call printf                                                 ;An external function that handles output.

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers.
mov        rdi, stringformat                                ;"%s"
mov        rsi, headerone                                   ;"Month		Monthly		Paid on		Paid on		Principle"
call printf                                                 ;An external function that handles output.

mov  qword rax, 0                                           ;Zero indicates to printf that there is no data to grab from lower registers
mov        rdi, stringformat                                ;"%s"
mov        rsi, headertwo                                   ;"Number		Payment		Interest	Principle	at end of month"
call printf                                                 ;An external function that handles output.

;===== Initialize registers to zero =======================================================================================================================================

push qword 0						    ;Push the constant onto the integer stack.
movsd      xmm12, [rsp]					    ;Copy the constant from integer stack to xmm12.
movsd      xmm11, [rsp]					    ;Copy the constant from integer stack to xmm11.
movsd      xmm10, [rsp]					    ;Copy the constant from integer stack to xmm10.
pop        rax						    ;Discard the constant.


;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|
;  ymm14: |  -    -     -    LOAN |
;         |-----------------------|
;  ymm13: |  -    -     -    MPAY |
;         |-----------------------|
;  ymm12: |  -    -     -      0  |
;         |-----------------------|
;  ymm11: |  -    -     -      0  |
;         |-----------------------|
;  ymm10: |  -    -     -      0  |
;         |-----------------------|

;===== Check condition ====================================================================================================================================================
;Checking to see if zero was input for the amount of time, in that case the program skips the loop and calculates the payment accordingly. Payment = Loan Amount.

cmp 	   r15, 0					    ;Compare the value in r15 to the constant 0, which is the time in months. If zero, jump to zerotimestart.
je zerotimestart					    ;If the above comparison turns out to be even then jump to zerotimestart below.

movsd 	   xmm0, xmm10					    ;Copy the total monthly payment paid into register xmm0 for output in zero month line.
movsd 	   xmm1, xmm12					    ;Copy the total paid on interest into register xmm1 for output in zero month line.
movsd 	   xmm2, xmm11					    ;Copy the total paid on principle into register xmm2 for output in zero month line.
movsd      xmm3, xmm14					    ;Copy the principle into register xmm3 for output in zero month line.
mov        rsi, 0					    ;Copy zero into rsi for output in zero month line.
mov  qword rax, 5                                           ;Five indicates to printf that there is five pieces of data to grab from the registers.
mov        rdi, lineout                                     ;"%ld		%9.2lf		%9.2lf		%9.2lf		%9.2lf"
call printf                                                 ;An external function that handles output.

mov 	   rcx, 0					    ;Set the counter register to zero in preperation for the loop.

;===== beginloop: start the loop code here ================================================================================================================================

beginloop:						    ;Begin loop code here.					    

;===== Check condition ====================================================================================================================================================

cmp 	   rcx, r15					    ;Compare the value in rcx to the value in r15, which is the loan in months.
jge loopfinish						    ;If the above comparison turns out to be greater than, or even then jump to loopfinish below.

;===== Begin body of code here ============================================================================================================================================

movsd 	   xmm1, xmm14					    ;Copy the principle amount into xmm1 in order to calculate how much of the monthly payment is interest.	
mulsd 	   xmm1, xmm15					    ;Multiply the principle by the periodic interest rate to calculate amount paid on interest (poi).
addsd 	   xmm12, xmm1					    ;Add the calculated paid on interest (POI) to an accumulator for totals output at the end.

;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|
;  ymm14: |  -    -     -    LOAN |
;         |-----------------------|
;  ymm13: |  -    -     -    MPAY |
;         |-----------------------|
;  ymm12: |  -    -     -    TPOI |
;         |-----------------------|
;  ymm11: |  -    -     -      0  |
;         |-----------------------|
;  ymm10: |  -    -     -      0  |
;         |-----------------------|
;  ymm1:  |  -    -     -     POI |
;         |-----------------------|

movsd 	   xmm2, xmm13					    ;Copy the monthly payment amount into xmm2 in order to calculate what is actually paid on principle.
subsd 	   xmm2, xmm1					    ;Subtract the calculated paid on interest from the monthly amount to get the amount paid on principle (pop).
addsd 	   xmm11, xmm2					    ;Add the calculated paid on principle (POP) to an accumulator for totals output at the end.

;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|
;  ymm14: |  -    -     -    LOAN |
;         |-----------------------|
;  ymm13: |  -    -     -    MPAY |
;         |-----------------------|
;  ymm12: |  -    -     -    TPOI |
;         |-----------------------|
;  ymm11: |  -    -     -    TPOP |
;         |-----------------------|
;  ymm10: |  -    -     -      0  |
;         |-----------------------|
;  ymm2:  |  -    -     -     POP |
;         |-----------------------|
;  ymm1:  |  -    -     -     POI |
;         |-----------------------|

subsd 	   xmm14, xmm2					    ;Subtract the calculated paid on principle amount from the principle amount to update it after each payment.
addsd      xmm10, xmm13					    ;Add the calculated monthly payment to an accumulator for totals output at the end.

;AVX Registers as of last instruction:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|
;  ymm14: |  -    -     -    LOAN | subtract paid on principle (POP) from loan and place here
;         |-----------------------|
;  ymm13: |  -    -     -    MPAY |
;         |-----------------------|
;  ymm12: |  -    -     -    TPOI |
;         |-----------------------|
;  ymm11: |  -    -     -    TPOP |
;         |-----------------------|
;  ymm10: |  -    -     -    TPAY |
;         |-----------------------|
;  ymm2:  |  -    -     -     POP |
;         |-----------------------|
;  ymm1:  |  -    -     -     POI |
;         |-----------------------|

mainbackup backuparea                                       ;Backup registers before function call

;===== Output information line ============================================================================================================================================
;There is important data in the top registers and in the counter is kept in the lower SEE registers. Backup was performed, printf is safe to call

movsd	   xmm3, xmm14					    ;Copy the principle amount into xmm3 for printf function call to output.
mov  qword rax, 5                                           ;There are 5 pieces of data in the registers that printf needs to use.
mov        rdi, lineout                            	    ;"%ld		%9.2lf		%9.2lf		%9.2lf		%9.2lf"
mov        rsi, rcx	                                    ;Copy the current loop iteration number to lower register for output.
inc	   rsi						    ;Increment the loop iteration number to match up with the current month for ouput.
movsd 	   xmm0, xmm13					    ;Copy the monthly payment amount into xmm0 for printf call to output.
call printf                                                 ;An external function that handles output.

mainrestore backuparea					    ;Restore registers after printf call has destroyed them, mainly rcx register which is counter for loop.

;===== Increment counter and jump back ====================================================================================================================================

inc rcx							    ;Increment the loop counter.

jmp beginloop						    ;Jump back to beginloop.

;===== Loop exit point ====================================================================================================================================================

loopfinish:						    ;Finish loop here and resume program execution.	    

;===== Check condition ====================================================================================================================================================
;Checking to see if the loop was completed, if the counter has exited the loop completed then we will skip the zerotimestart function.

cmp rcx, r15						    ;Compare the loop counter to the total loan time to check for completeness.
je zerotimefinish					    ;If above compare is true, jump to zerotimefinish in order to skip the function.

;===== zerotimestart: begin function here =================================================================================================================================

zerotimestart:						    ;Begin zero month code here.

;AVX Registers as of jump to zerotimestart:
;
;         |-----------------------|
;  ymm15: |  -    -     -    PIR  |
;         |-----------------------|
;  ymm14: |  -    -     -    LOAN |
;         |-----------------------|
;  ymm13: |  -    -     -    MPAY |
;         |-----------------------|
;  ymm12: |  -    -     -      0  |
;         |-----------------------|
;  ymm11: |  -    -     -      0  |
;         |-----------------------|
;  ymm10: |  -    -     -      0  |
;         |-----------------------|

;===== Output zeromonth line ==============================================================================================================================================
;Important data in top most registers, printf should NOT change this data. If it is corrupted, try backup at this point. 

movsd 	   xmm0, xmm13					    ;Copy the total monthly payment paid into register xmm0 for output.
movsd 	   xmm1, xmm12					    ;Copy the total paid on interest into register xmm1 for output.
movsd 	   xmm2, xmm13					    ;Copy the total paid on principle into register xmm2 for output.
subsd      xmm14, xmm13					    ;Subtract the monthly payment amount from the principle amount.
movsd      xmm3, xmm14					    ;Copy the principle into register xmm3 for output in zero month line.
mov        rsi, 0					    ;Copy zero into rsi for output in zero month line.
mov  qword rax, 5                                           ;Five indicates to printf that there is five pieces of data to grab from the registers.
mov        rdi, lineout                                     ;"%ld		%9.2lf		%9.2lf		%9.2lf		%9.2lf"
call printf                                                 ;An external function that handles output.

movsd      xmm10, xmm13					    ;Copy the total monthly payment amount into the total monthly payments.
movsd      xmm11, xmm13					    ;Copy the total monthly payment amount into the total principle paid.
zerotimefinish:						    ;Finish zero time function here.

;===== Output totals ======================================================================================================================================================
;Important data in top most registers, printf should NOT change this data. If it is corrupted, try backup at this point. 

movsd 	   xmm0, xmm10					    ;Copy the total calculated monthly payments to xmm0 for output by printf.
movsd 	   xmm1, xmm12					    ;Copy the total calculated interest paid to xmm1 for output by printf.
movsd 	   xmm2, xmm11					    ;Copy the total calculated principle paid to xmm2 for output by printf.
mov  qword rdi, totals                                      ;"Totals		%1.2lf		%1.2lf		%1.2lf"
mov  qword rax, 3                                           ;Three pieces of data are needed for printf.
call printf					   	    ;An external function that handles output.				


;===== Say thanks =========================================================================================================================================================
;Important data in top most registers, printf should NOT change this data. If it is corrupted, try backup at this point.

mov  qword rdi, stringformat                                ;A little good-bye message will be outputted.
mov  qword rsi, thanks                                      ;"Thank you for your inquiry at our bank."
mov  qword rax, 0                                           ;Zero says that no data values from SSE registers are used by printf
call printf					   	    ;An external function that handles output.					


;===== Say farewell =======================================================================================================================================================
;Important data in top most registers, printf should NOT change this data. If it is corrupted, try backup at this point.

mov  qword rdi, stringformat                                ;A little good-bye message will be outputted.
mov  qword rsi, farewell                                    ;"This program will now return the total interest paid to the driver."
mov  qword rax, 0                                           ;Zero says that no data values from SSE registers are used by printf.
call printf					   	    ;An external function that handles output.				

;===== Set the value to be returned to the caller =========================================================================================================================
;Presently the value to be returned to the caller is in xmm12.  That value needs to be copied to xmm0.

movsd 	   xmm0, xmm12					    ;A copy of total interest paid is now in xmm0.

pop        rbp                                              ;Restore the value rbp held when this function began execution.

;Now the stack is in the same state as when the application area was entered.  It is safe to leave this application area.
;==========================================================================================================================================================================
;===== End of the application: Amortization Schedule Calculator ===========================================================================================================
;==========================================================================================================================================================================

ret                                                         ;Pop an 8-byte integer from the system stack and return to calling function.
;===== End of program amort ===============================================================================================================================================
;========1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1=========2=========3=========4=========5=========6=========7**

