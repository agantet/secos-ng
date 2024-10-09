/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>

void userland() {
   asm volatile ("mov %eax, %cr0");
}

void print_gdt_content(gdt_reg_t gdtr_ptr) {
    seg_desc_t* gdt_ptr;
    gdt_ptr = (seg_desc_t*)(gdtr_ptr.addr);
    int i=0;
    while ((uint32_t)gdt_ptr < ((gdtr_ptr.addr) + gdtr_ptr.limit)) {
        uint32_t start = gdt_ptr->base_3<<24 | gdt_ptr->base_2<<16 | gdt_ptr->base_1;
        uint32_t end;
        if (gdt_ptr->g) {
            end = start + ( (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1) <<12) + 4095;
        } else {
            end = start + (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1);
        }
        debug("%d ", i);
        debug("[0x%x ", start);
        debug("- 0x%x] ", end);
        debug("seg_t: 0x%x ", gdt_ptr->type);
        debug("desc_t: %d ", gdt_ptr->s);
        debug("priv: %d ", gdt_ptr->dpl);
        debug("present: %d ", gdt_ptr->p);
        debug("avl: %d ", gdt_ptr->avl);
        debug("longmode: %d ", gdt_ptr->l);
        debug("default: %d ", gdt_ptr->d);
        debug("gran: %d ", gdt_ptr->g);
        debug("\n");
        gdt_ptr++;
        i++;
    }
}

void displaySelector()
{
    debug("ss: %x\n", get_ss() >> 3);
    debug("cs: %x\n", get_cs() >> 3);
    debug("ds: %x\n", get_ds() >> 3);
    debug("es: %x\n", get_es() >> 3);
    debug("fs: %x\n", get_fs() >> 3);
    debug("gs: %x\n", get_gs() >> 3);
}

void setDataSelector(unsigned int v)
{
    set_ss(v);
    set_ds(v);
    set_es(v);
    set_fs(v);
    set_gs(v);
}


void tp() {
	gdt_reg_t gdt1;
    get_gdtr(gdt1);
    print_gdt_content(gdt1);

    displaySelector();

    seg_desc_t tab[3] = {0};

    seg_desc_t desc_code =
    {
        .base_1 = 0x0000,
        .base_2 = 0x00,
        .base_3 = 0x00,
        .limit_1 = 0xffff,
        .limit_2 = 0xf,
        .type = 11, //segment type
        .s = 1,     //descriptor type
        .dpl = 0,   //ring 0
        .p = 1,     //segment present flag
        .avl = 1,   //available for fun
        .l = 0,     //longmode
        .d = 1,
        .g = 0
    };

    tab[1] = desc_code;

    seg_desc_t desc_data =
    {
        .base_1 = 0x0000,
        .base_2 = 0x00,
        .base_3 = 0x00,
        .limit_1 = 0xffff,
        .limit_2 = 0xf,
        .type = 2,  //segment type
        .s = 1,     //descriptor type (0 : system (call, int ...), 1: segment type)
        .dpl = 0,   //ring 0
        .p = 1,     //segment present flag
        .avl = 1,   //available for fun
        .l = 0,     //longmode
        .d = 1,
        .g = 0
    };

    tab[2] = desc_data;

    gdt_reg_t gdt2 = {.limit = sizeof(tab) - 1, .desc=tab};

    set_gdtr(gdt2);

    print_gdt_content(gdt2);

    // Variable code avec 1 sur 13 bits
    // unsigned int codeSelectorIndice = 0b0000000000001000;

    // set_cs(codeSelectorIndice);

    // Variable data avec 2 sur 13 bits
    unsigned int dataSelectorIndice = 0b0000000000010000;

    setDataSelector(dataSelectorIndice);

    displaySelector();

}
