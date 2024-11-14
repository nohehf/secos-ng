/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>
#include <string.h>

tss_t TSS;

void userland()
{
   printf("Welcome to userland!");
   asm volatile("mov %eax, %cr0");
}

// /* GPLv2 (c) Airbus */
// #include <debug.h>
// #include <segmem.h>
// #include <string.h>

// #define c0_idx 1
// #define d0_idx 2
// #define c3_idx 3
// #define d3_idx 4
#define ts_idx 5

// #define c0_sel gdt_krn_seg_sel(c0_idx)
#define d0_sel gdt_krn_seg_sel(2)
// #define c3_sel gdt_usr_seg_sel(c3_idx)
// #define d3_sel gdt_usr_seg_sel(d3_idx)
#define ts_sel gdt_krn_seg_sel(ts_idx)

tss_t TSS;
seg_desc_t GDT[6];

#define tss_dsc(_dSc_, _tSs_)                  \
   ({                                          \
      raw32_t addr = {.raw = _tSs_};           \
      (_dSc_)->raw = sizeof(tss_t);            \
      (_dSc_)->base_1 = addr.wlow;             \
      (_dSc_)->base_2 = addr._whigh.blow;      \
      (_dSc_)->base_3 = addr._whigh.bhigh;     \
      (_dSc_)->type = SEG_DESC_SYS_TSS_AVL_32; \
      (_dSc_)->p = 1;                          \
   })

void tp()
{
   printf("SETTING UP GDT\n");

   GDT[0].raw = (raw64_t)0ULL;
   GDT[1].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   GDT[1].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   GDT[1].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   GDT[1].type = 11;        // Code,RX //:4;      /* segment type */ 1011
   GDT[1].s = 1;            //: 1;      /* descriptor type */
   GDT[1].dpl = 0;          // ring0    //:2;      /* descriptor privilege level */
   GDT[1].p = 1;            //: 1;      /* segment present flag */
   GDT[1].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   GDT[1].avl = 1;          //: 1;      /* available for fun and profit */
   GDT[1].l = 0;            // 32bits     //:1;      /* longmode */
   GDT[1].d = 1;            //: 1;      /* default length, depend on seg type */
   GDT[1].g = 1;            //: 1;      /* granularity */
   GDT[1].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   GDT[2].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   GDT[2].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   GDT[2].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   GDT[2].type = 3;         // data,RW //:4;      /* segment type */ 0011
   GDT[2].s = 1;            //: 1;      /* descriptor type */
   GDT[2].dpl = 0;          // ring0    //:2;      /* descriptor privilege level */
   GDT[2].p = 1;            //: 1;      /* segment present flag */
   GDT[2].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   GDT[2].avl = 1;          //: 1;      /* available for fun and profit */
   GDT[2].l = 0;            // 32 bits   //:1;      /* longmode */
   GDT[2].d = 1;            //: 1;      /* default length, depend on seg type */
   GDT[2].g = 1;            //: 1;      /* granularity */
   GDT[2].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   GDT[3].limit_1 = 0x20;   //: 16;     /* bits 00-15 of the segment limit */
   GDT[3].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   GDT[3].base_2 = 0x60;    //: 8;      /* bits 16-23 of the base address */
   GDT[3].type = 3;         // data,RW //:4;      /* segment type */
   GDT[3].s = 1;            //: 1;      /* descriptor type */
   GDT[3].dpl = 0;          // ring0    //:2;      /* descriptor privilege level */
   GDT[3].p = 1;            //: 1;      /* segment present flag */
   GDT[3].limit_2 = 0x0;    //: 4;      /* bits 16-19 of the segment limit */
   GDT[3].avl = 1;          //: 1;      /* available for fun and profit */
   GDT[3].l = 0;            // 32 bits   //:1;      /* longmode */
   GDT[3].d = 1;            //: 1;      /* default length, depend on seg type */
   GDT[3].g = 0;            //: 1;      /* granularity */
   GDT[3].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   GDT[4].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   GDT[4].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   GDT[4].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   GDT[4].type = 11;        // Code,RX //:4;      /* segment type */
   GDT[4].s = 1;            //: 1;      /* descriptor type */
   GDT[4].dpl = 3;          // ring3    //:2;      /* descriptor privilege level */
   GDT[4].p = 1;            //: 1;      /* segment present flag */
   GDT[4].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   GDT[4].avl = 1;          //: 1;      /* available for fun and profit */
   GDT[4].l = 0;            // 32bits     //:1;      /* longmode */
   GDT[4].d = 1;            //: 1;      /* default length, depend on seg type */
   GDT[4].g = 1;            //: 1;      /* granularity */
   GDT[4].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   GDT[5].limit_1 = 0xffff; //: 16;     /* bits 00-15 of the segment limit */
   GDT[5].base_1 = 0x0000;  //: 16;     /* bits 00-15 of the base address */
   GDT[5].base_2 = 0x00;    //: 8;      /* bits 16-23 of the base address */
   GDT[5].type = 3;         // data,RW //:4;      /* segment type */
   GDT[5].s = 1;            //: 1;      /* descriptor type */
   GDT[5].dpl = 3;          // ring3    //:2;      /* descriptor privilege level */
   GDT[5].p = 1;            //: 1;      /* segment present flag */
   GDT[5].limit_2 = 0xf;    //: 4;      /* bits 16-19 of the segment limit */
   GDT[5].avl = 1;          //: 1;      /* available for fun and profit */
   GDT[5].l = 0;            // 32 bits   //:1;      /* longmode */
   GDT[5].d = 1;            //: 1;      /* default length, depend on seg type */
   GDT[5].g = 1;            //: 1;      /* granularity */
   GDT[5].base_3 = 0x00;    //: 8;      /* bits 24-31 of the base address */
   gdt_reg_t GDTr;
   GDTr.addr = (long unsigned int)GDT;
   GDTr.limit = sizeof(GDT) - 1;
   set_gdtr(GDTr);

   printf("GDT SET\n");

   printf("SETTING UP TSS\n");

   TSS.s0.esp = get_ebp();
   TSS.s0.ss = d0_sel;
   // tss_dsc(&GDT[ts_idx], (offset_t)&TSS);

   raw32_t addr = {.raw = (offset_t)&TSS};
   (&GDT[ts_idx])->limit_1 = sizeof(tss_t);
   (&GDT[ts_idx])->base_1 = addr.wlow;
   (&GDT[ts_idx])->base_2 = addr._whigh.blow;
   (&GDT[ts_idx])->base_3 = addr._whigh.bhigh;
   (&GDT[ts_idx])->type = SEG_DESC_SYS_TSS_AVL_32;
   (&GDT[ts_idx])->p = 1;

   set_tr(ts_sel);

   printf("TSS SET\n");

   asm volatile("push %ss");
   asm volatile("push %esp");
   asm volatile("pushf"); // push EFLAGS
   // // Push CS: Use segment 4 (code RX ring 3) -> CS = 0b100 << 3 = 0x20
   asm volatile("push $0x20");
   // // Push EIP (address of userland function)
   asm volatile("push %0" ::"r"((offset_t)userland));

   asm volatile("iret");
}
