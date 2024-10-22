/* GPLv2 (c) Airbus */
#include <debug.h>
#include <intr.h>

// Declare bp_handler as a naked function to suppress compiler-generated prologue/epilogue
void __attribute__((naked)) bp_handler()
{
	asm volatile(
		"pushl $-1\n\t"
		"pushl $0\n\t"
		"pusha\n\t"				// Save general-purpose registers
		"mov %esp, %eax\n\t"	// Question pourquoi ??
		"call bp_handler_c\n\t" // Call the C function
		"popa\n\t"				// Restore general-purpose registers
		// Question: Pourquoi fait dans idt.s et pas ici ?
		"add $8, %esp\n\t"
		"iret\n\t" // Return from interrupt
	);
}

// C function to handle the interrupt logic
void bp_handler_c()
{
	printf("bp_handler called!\n");
}

void bp_trigger()
{
	// interupt 3
	asm volatile("int3");
	printf("returned to bp_triger()\n");
}

void tp()
{
	idt_reg_t idtr_desc;
	get_idtr(idtr_desc);
	printf("idtr_desc.addr: 0x%lx\n", idtr_desc.addr);
	printf("idtr_desc.limit: 0x%x\n", idtr_desc.limit);

	// Add bp_handler in the IDT
	int_desc_t *IDT;
	IDT = (int_desc_t *)idtr_desc.addr;

	// Bp is the 3rd exception
	build_int_desc(&IDT[3], gdt_krn_seg_sel(1), (offset_t)bp_handler);

	// call bp_trigger
	bp_trigger();

	printf("returned to tp()\n");
}

// It can also be in a single function:
// void __attribute__((naked)) bp_handler(int_ctx_t *ctx)
// {
// 	asm volatile(
// 		"pushl $-1\n\t"
// 		"pushl $0\n\t"
// 		"pusha\n\t" // Save general-purpose registers
// 					// "mov %esp, %eax\n\t" // Question: pourquoi ??
// 					// "call bp_handler_c\n\t" // Call the C function
// 					// "popa\n\t"				// Restore general-purpose registers
// 					// // Question: Pourquoi fait dans idt.s et pas ici ?
// 					// "add $8, %esp\n\t"
// 					// "iret\n\t" // Return from interrupt
// 	);

// 	printf("bp_handler called! %x\n", ctx->eip.raw);

// 	asm volatile(
// 		// "pushl $-1\n\t"
// 		// "pushl $0\n\t"
// 		// "pusha\n\t"				// Save general-purpose registers
// 		// "mov %esp, %eax\n\t"	// Question pourquoi ??
// 		// "call bp_handler_c\n\t" // Call the C function
// 		"popa\n\t" // Restore general-purpose registers
// 		// Question: Pourquoi fait dans idt.s et pas ici ?
// 		"add $8, %esp\n\t"
// 		"iret\n\t" // Return from interrupt
// 	);
// }

// This did not work:
// // Declare bp_handler_wrapper
// void bp_handler_wrapper();

// // Define bp_handler_wrapper using inline assembly
// asm(
// 	".text\n"
// 	".global bp_handler_wrapper\n"
// 	".type bp_handler_wrapper, @function\n"
// 	"bp_handler_wrapper:\n"
// 	// not sure this is needed
// 	"    cli\n"				  // Disable interrupts
// 	"    pusha\n"			  // Save general-purpose registers (32 bytes)
// 	"    mov %esp, %eax\n"	  // Move stack pointer to EAX
// 	"    push %eax\n"		  // Push EAX (pointer to stack frame) as argument (4 bytes)
// 	"    call bp_handler_c\n" // Call your C handler function (pushes return address, 4 bytes)
// 	"    add $8, %esp\n"	  // Clean up the stack (remove argument and return address, total 8 bytes)
// 	"    popa\n"			  // Restore general-purpose registers (32 bytes)
// 	// not sure this is needed
// 	"    sti\n"	 // Re-enable interrupts
// 	"    iret\n" // Return from interrupt
// );