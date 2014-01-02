/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused)))
{

	unsigned int i = 0;
        char *char_buf = buf;
	/*test if it's reading from stdin*/
        if (fd != STDIN_FILENO) {
                return -EBADF;
        }
	/*test if it writes outside the lower boundry of SDRAM*/
	if((unsigned int)char_buf < 0xa0000000){
		return -EFAULT;
	}
	/*test if it writes on the U-BOOT Code area*/
        if((unsigned int)(char_buf + count) > 0xa3f00000){
                return -EFAULT;
        }
        while(i < count){
		char c = getc();//get the charactor from stdin
		/*test if the input charactor is EOT*/
                if(c == 4){                                                     
                        return i;                                               
                }                                                               
		/*test if the input charactor is backspace or delete*/
                else if((c == 8)||(c == 127)){                                  
                        i-=2;                                                   
                        puts("\b \b");//if so, delete the char in the buffer and print "\b \b"                                          
                }                                                               
		/*test if the input charactor is new line*/
                else if((c == 10)||(c == 13)){                                  
                       char_buf[i] = '\n';                                          
                        puts("\n");   
                        return i+1;                                             
                }                                                               
		/*if none of the above*/
                else{                                                           
                       char_buf[i] = c; //save the charactor into the buffer                                            
                        putc(c);        //echo the charactor                                         
                }                                                               
                i++;      
	}
	return i;
	
}

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{

	unsigned int i;
	const char* char_buf = buf;
	/*test if it's writting to stdout*/
        if(fd != STDOUT_FILENO){
                return -EBADF;
        }
	/*test if it reads from outside the SDRAM*/
	// if((bufValue + count) > 0xa3ffffff){                                   
 //                return -EFAULT;                                                 
 //        }                                        
	/*test if it reads from StrataFlash ROM*/                               
	// if(((bufValue+count)> 0x00ffffff)){
	// 	return -EFAULT;
	// }
        if (!valid_addr(buf, count, RAM_START_ADDR, RAM_END_ADDR) && 
            !valid_addr(buf, count, FLASH_START_ADDR, FLASH_END_ADDR))
            return -EFAULT;
	/*print out the chars in the buffer*/
        for(i = 0; i < count; i++){                                             
                putc(char_buf[i]);                                                   
        }                                                                       
	return i;  
	
}

