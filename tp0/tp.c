/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

extern info_t *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

const char *typeToChars(multiboot_uint32_t type)
{
   switch (type)
   {
   case MULTIBOOT_MEMORY_AVAILABLE:
      return "MULTIBOOT_MEMORY_AVAILABLE";

   case MULTIBOOT_MEMORY_RESERVED:
      return "MULTIBOOT_MEMORY_RESERVED";

   case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
      return "MULTIBOOT_MEMORY_ACPI_RECLAIMABLE";

   case MULTIBOOT_MEMORY_NVS:
      return "MULTIBOOT_MEMORY_NVS";
   }

   return "Wrong memory type";
}

void tp()
{
   debug("kernel mem [0x%p - 0x%p]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);

   multiboot_memory_map_t *entry = (multiboot_memory_map_t *)info->mbi->mmap_addr;
   while ((uint32_t)entry < (info->mbi->mmap_addr + info->mbi->mmap_length))
   {
      const char *typeStr = typeToChars(entry->type);
      printf("[0x%llx - 0x%llx] %s\n", entry->addr, (entry->addr + entry->len - 1), typeStr);
      entry++;
   }

   // q3
   int *ptr_in_available_mem;
   ptr_in_available_mem = (int *)0x0;
   debug("Available mem (0x0): before: 0x%x ", *ptr_in_available_mem); // read
   *ptr_in_available_mem = 0xaaaaaaaa;                                 // write
   debug("after: 0x%x\n", *ptr_in_available_mem);                      // check

   int *ptr_in_reserved_mem;
   ptr_in_reserved_mem = (int *)0xf0000;
   debug("Reserved mem (at: 0xf0000):  before: 0x%x ", *ptr_in_reserved_mem); // read
   *ptr_in_reserved_mem = 0xaaaaaaaa;                                         // write
   debug("after: 0x%x\n", *ptr_in_reserved_mem);                              // check

   // q4 out of meme
   int *ptr_out_of_mem;
   ptr_out_of_mem = (int *)0x901663f;
   debug("Out of mem (at: 0x901663f):  before: 0x%x ", *ptr_out_of_mem); // read
   *ptr_out_of_mem = 0xaaaaaaaa;                                         // write
   debug("after: 0x%x\n", *ptr_out_of_mem);
}
