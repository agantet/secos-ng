/* GPLv2 (c) Airbus */
#ifndef __INTR_H__
#define __INTR_H__

#include <types.h>
#include <segmem.h>
#include <gpr.h>
#include <excp.h>

#define IDT_NR_DESC                   256
#define IDT_ISR_ALGN                  16

#define BIOS_VIDEO_INTERRUPT          0x10
#define BIOS_DISK_INTERRUPT           0x13
#define BIOS_MISC_INTERRUPT           0x15
#define BIOS_KBD_INTERRUPT            0x16
#define BIOS_BOOT_INTERRUPT           0x19

/*
** BIOS services related to MISC_INTERRUPT
*/

/* AX values */
#define BIOS_GET_SMAP                 0xe820
#define BIOS_SMAP_ID                  0x534d4150
#define BIOS_SMAP_ERROR               0x86

#define BIOS_GET_EXT_MEM_32           0xe881
#define BIOS_GET_EXT_MEM              0xe801

#define BIOS_DISABLE_A20              0x2400
#define BIOS_ENABLE_A20               0x2401
#define BIOS_STATUS_A20               0x2402
#define BIOS_SUPPORT_A20              0x2403

/* AH values */

#define BIOS_GET_BIG_MEM              0x8a
#define BIOS_OLD_GET_EXT_MEM          0x88

typedef struct cpu_context
{
   raw32_t         nr;
   excp_err_code_t err;
   raw32_t         eip;
   raw32_t         cs;
   eflags_reg_t    eflags;

   /* if pvl level change */
   raw32_t         esp;
   raw32_t         ss;

} __attribute__((packed)) cpu_ctx_t;


typedef struct interrupt_context
{
   gpr_ctx_t gpr;
   cpu_ctx_t;

} __attribute__((packed)) int_ctx_t;

typedef void (*isr_t)(int_ctx_t*);

/*
** Interrupt descriptor
*/
typedef union interrupt_descriptor
{
   struct
   {
      uint64_t  offset_1:16;    /* bits 00-15 of the isr offset */
      uint64_t  selector:16;    /* isr segment selector */
      uint64_t  ist:3;          /* stack table: only 64 bits */
      uint64_t  zero_1:5;       /* must be 0 */
      uint64_t  type:4;         /* interrupt/trap gate */
      uint64_t  zero_2:1;       /* must be zero */
      uint64_t  dpl:2;          /* privilege level */
      uint64_t  p:1;            /* present flag */
      uint64_t  offset_2:16;    /* bits 16-31 of the isr offset */

   } __attribute__((packed));

   raw64_t;

} __attribute__((packed)) int_desc_t;

/*
** Interrupt descriptor table
*/
typedef struct interrupt_descriptor_table_register
{
   uint16_t        limit;           /* dt limit = size - 1 */
   union                            /* base address */
   {
      offset_t     addr;
      int_desc_t   *desc;
   };

} __attribute__((packed)) idt_reg_t;

#define build_int_desc(_dsc_, _cs_, _isr_)                              \
   ({                                                                   \
      raw32_t addr = {.raw = _isr_};                                    \
      (_dsc_)->raw      = addr.wlow;                                    \
      (_dsc_)->selector = _cs_;                                         \
      (_dsc_)->type     = SEG_DESC_SYS_INTR_GATE_32;                    \
      (_dsc_)->offset_2 = addr.whigh;                                   \
      (_dsc_)->p        = 1;                                            \
   })

#define get_idtr(aLocation)       \
   asm volatile ("sidt %0"::"m"(aLocation):"memory")

#define set_idtr(val)             \
   asm volatile ("lidt  %0"::"m"(val):"memory")

void intr_init();
void intr_hdlr(int_ctx_t*) __regparm__(1);

#endif
