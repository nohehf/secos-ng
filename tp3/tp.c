/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>

tss_t TSS;

void userland()
{
   printf("Welcome to userland!");
   asm volatile("mov %eax, %cr0");
}

void tp()
{
   printf("SETTING UP GDT\n");

   seg_desc_t my_gdt[7];
   my_gdt[0].raw = (raw64_t)0ULL;
   my_gdt[1].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   my_gdt[1].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   my_gdt[1].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   my_gdt[1].type = 11;        // Code,RX //:4;      /* segment type */ 1011
   my_gdt[1].s = 1;            //: 1;      /* descriptor type */
   my_gdt[1].dpl = 0;          // ring0    //:2;      /* descriptor privilege level */
   my_gdt[1].p = 1;            //: 1;      /* segment present flag */
   my_gdt[1].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   my_gdt[1].avl = 1;          //: 1;      /* available for fun and profit */
   my_gdt[1].l = 0;            // 32bits     //:1;      /* longmode */
   my_gdt[1].d = 1;            //: 1;      /* default length, depend on seg type */
   my_gdt[1].g = 1;            //: 1;      /* granularity */
   my_gdt[1].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   my_gdt[2].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   my_gdt[2].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   my_gdt[2].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   my_gdt[2].type = 3;         // data,RW //:4;      /* segment type */ 0011
   my_gdt[2].s = 1;            //: 1;      /* descriptor type */
   my_gdt[2].dpl = 0;          // ring0    //:2;      /* descriptor privilege level */
   my_gdt[2].p = 1;            //: 1;      /* segment present flag */
   my_gdt[2].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   my_gdt[2].avl = 1;          //: 1;      /* available for fun and profit */
   my_gdt[2].l = 0;            // 32 bits   //:1;      /* longmode */
   my_gdt[2].d = 1;            //: 1;      /* default length, depend on seg type */
   my_gdt[2].g = 1;            //: 1;      /* granularity */
   my_gdt[2].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   my_gdt[3].limit_1 = 0x20;   //: 16;     /* bits 00-15 of the segment limit */
   my_gdt[3].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   my_gdt[3].base_2 = 0x60;    //: 8;      /* bits 16-23 of the base address */
   my_gdt[3].type = 3;         // data,RW //:4;      /* segment type */
   my_gdt[3].s = 1;            //: 1;      /* descriptor type */
   my_gdt[3].dpl = 0;          // ring0    //:2;      /* descriptor privilege level */
   my_gdt[3].p = 1;            //: 1;      /* segment present flag */
   my_gdt[3].limit_2 = 0x0;    //: 4;      /* bits 16-19 of the segment limit */
   my_gdt[3].avl = 1;          //: 1;      /* available for fun and profit */
   my_gdt[3].l = 0;            // 32 bits   //:1;      /* longmode */
   my_gdt[3].d = 1;            //: 1;      /* default length, depend on seg type */
   my_gdt[3].g = 0;            //: 1;      /* granularity */
   my_gdt[3].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   my_gdt[4].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   my_gdt[4].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   my_gdt[4].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   my_gdt[4].type = 11;        // Code,RX //:4;      /* segment type */
   my_gdt[4].s = 1;            //: 1;      /* descriptor type */
   my_gdt[4].dpl = 3;          // ring3    //:2;      /* descriptor privilege level */
   my_gdt[4].p = 1;            //: 1;      /* segment present flag */
   my_gdt[4].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   my_gdt[4].avl = 1;          //: 1;      /* available for fun and profit */
   my_gdt[4].l = 0;            // 32bits     //:1;      /* longmode */
   my_gdt[4].d = 1;            //: 1;      /* default length, depend on seg type */
   my_gdt[4].g = 1;            //: 1;      /* granularity */
   my_gdt[4].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   my_gdt[5].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   my_gdt[5].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   my_gdt[5].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   my_gdt[5].type = 3;         // data,RW //:4;      /* segment type */
   my_gdt[5].s = 1;            //: 1;      /* descriptor type */
   my_gdt[5].dpl = 3;          // ring3    //:2;      /* descriptor privilege level */
   my_gdt[5].p = 1;            //: 1;      /* segment present flag */
   my_gdt[5].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   my_gdt[5].avl = 1;          //: 1;      /* available for fun and profit */
   my_gdt[5].l = 0;            // 32 bits   //:1;      /* longmode */
   my_gdt[5].d = 1;            //: 1;      /* default length, depend on seg type */
   my_gdt[5].g = 1;            //: 1;      /* granularity */
   my_gdt[5].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   gdt_reg_t my_gdtr;
   my_gdtr.addr = (long unsigned int)my_gdt;
   my_gdtr.limit = sizeof(my_gdt) - 1;
   set_gdtr(my_gdtr);

   printf("GDT SET\n");

   asm volatile("push %ss");
   asm volatile("push %esp");
   asm volatile("pushf"); // push EFLAGS
   // // Push CS: Use segment 4 (code RX ring 3) -> CS = 0b100 << 3 = 0x20
   asm volatile("push $0x20");
   // // Push EIP (address of userland function)
   asm volatile("push %0" ::"r"((offset_t)userland));

   // TSS
   // TSS.s0.esp = get_ebp();
   // TSS.s0.ss = gdt_krn_seg_sel(2);
   // set_tr(ts_sel);

   asm volatile("iret");
}
