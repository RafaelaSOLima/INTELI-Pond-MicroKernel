org 0x7c00   

bits 16     

start:
    jmp main    

main:
    
    xor ax, ax      
    mov ds, ax      
    mov es, ax      
    
    cli             
    mov ss, ax      
    mov sp, 0x7c00  
    sti             
  
    mov si, hello_msg   
    call print_string
    
    mov si, prompt_msg  
    call print_string
    
    call read_char      
    
    mov bl, al          
   
    mov si, newline     
    call print_string
    
    mov si, echo_msg    
    call print_string
    
    mov al, bl         
   
    call print_char    
    
    jmp $              
    

print_string:
    lodsb              
    or al, al          
    jz end_print_string 
    
    call print_char  
    jmp print_string    
    
end_print_string:
    ret

print_char:
    mov ah, 0x0E        
    mov bh, 0           
    int 0x10            
    
    ret


read_char:
    mov ah, 0          
    int 0x16            
    
    ret


hello_msg db "Welcome to My Bootloader!", 0
prompt_msg db "Enter a character: ", 0
echo_msg db "You entered: ", 0
newline db 0x0D, 0x0A, 0     

times 510-($-$$) db 0   
dw 0xAA55              