  ;==========================
  ; Hien thi LCD
  ;========================== 
  $mod51

  U equ 31       		; memory location to hold upper nibble
  L equ 32      		; memory location to hold lower nibble
 
  Port equ P2      	; data port to connect LCD
  RS equ P2.0    	; RS pin connection
  RW equ P2.2    	; RW pin connection
  EN equ P2.1    	; EN pin connection
 ;============================
  ; Connection of Port
  ; Port.4 = DB4
  ; Port.5 = DB5
  ; Port.6 = DB6
  ; Port.7 = DB7
;============================
SS MACRO L1
	MOV	R1,#0
	MOV	DPTR,L1
	LCALL lcd_puts
ENDM	
;===========================
  ORG 0000h
  CLR RW
  ACALL init
  SS #STRING1
  MOV A, #0c0H        ; switch to 2nd line of LCD
  ACALL lcd_cmd 
  SS #STRING2
  SJMP $    		; INFINITE LONG LOOP
;===========================
; Separator
;=========================== 
  separator:
   MOV U,A      	; save A at temp location U
   ANL U,#0F0H   	; mask it  with 0Fh (28h & F0h = 20h)
   SWAP A        	; swap nibble (28h => 82H)
   ANL A,#0F0H  	; mask it with 0fh (82h & f0h = 80h)
   MOV L,A       	; save it at temp location L
   RET           		; return
;========================
; Move To Port
; MOV port,A      	; put content of A to port
; ANL port,#0x0FH
; ORL port,A
;========================
   move_to_Port:
   MOV C,Acc.4
   MOV port.4,C
   MOV C,Acc.5
   MOV port.5,C
   MOV C,Acc.6
   MOV port.6,C
   MOV C,Acc.7
   MOV port.7,C
   
   SETB EN   			; make EN high
   ACALL DELAY  		; call a short delay routine
   CLR EN    			; clear EN
   ACALL DELAY  		; short delay
   RET    			; return      
;=================================
;LCD command
;=================================
   lcd_cmd:
    CLR RS    			; clear rs, going to send command
    ACALL separator    	; separate the command and save to U and L
    MOV A, U    		; copy U to A
    ACALL move_to_port 	; move content of a to port
    MOV A, L           		; copy L to A
    ACALL move_to_port 	; move content of a to port
    RET      			; return
;=================================
; LCD data
;=================================
  lcd_data:
   SETB RS           		; RS=1, going to send DATA
   ACALL separator   		; separate the data and save to U & L
   MOV A, U          		; copy U to A
   ACALL move_to_port 	; send it to LCD
   MOV A, L          		; copy L to A
   ACALL move_to_port 	; send it to LCD
   RET               		; return
;=================================
; Initilization
;================================= 
init:
   ACALL delay    		; some delay to lcd after power on
   ACALL delay
   CLR port.4
   SETB port.5
   CLR port.6
   CLR port.7  			; send 20h to LCD to set 4 bit mode
   CLR RS  			; after that we can use lcd_cmd
   SETB EN        		; make EN switching
   ACALL delay
   CLR EN
   MOV A, #28H
   ACALL lcd_cmd
   MOV A, #0CH
   ACALL lcd_cmd
   MOV A, #06H
   ACALL lcd_cmd
   MOV A, #01H
   ACALL lcd_cmd
   RET
;=================================
lcd_puts:
	MOV	A,R1
	MOVC	A,@A+DPTR	
	LCALL	lcd_data
	INC	R1
	CJNE	R1,#15,lcd_puts
	RET
;================================= 
delay:
   MOV R6, #5FH
  L2: MOV R7,#3FH
  L1: DJNZ R7, L1
   DJNZ R6, L2
   RET
;=================================
STRING1: DB 'Bao khong uong'
STRING2: DB ' ruou bia con '	

end	
