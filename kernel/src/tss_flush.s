.global tss_flush   
tss_flush:
   mov $0x002B, %ax       
   ltr %ax    
   ret
