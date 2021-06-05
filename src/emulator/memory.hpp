#pragma once

#include "devices/memory.hpp"
#include "units/translation_unit.hpp"

namespace postrisc
{

/***************************************************************************
* translation (tlb/vpt format)
*
*            3210987654321098765432109876543210987654321098765432109876543210
* ppn(both): ppppppppppppppppppppppppppppppppppppppppppppppppppaaaxxmmmmadpfv
* itr(both): --------kkkkkkkkkkkkkkkkkkkkkkkk------------------dddddd--------
* vpn(tlb):  rrrrvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv--------------
* sid(tlb):  --------rrrrrrrrrrrrrrrrrrrrrrrr--------------------------------
* tag(vpt):  tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
* lnk(vpt):  --------lllllllllllllllllllllllllllllllllllllllllllllllllll-----
*
***************************************************************************/
typedef union {
    uint64_t    address;
    struct {
        uint64_t    v    : 1;                         // validation bit
        uint64_t    ar   : bits_per_access;           // access rights
        uint64_t    f    : 1;                         // fixed bit
        uint64_t    p    : 1;                         // present bit
        uint64_t    d    : 1;                         // dirty bit
        uint64_t    a    : 1;                         // access bit
        uint64_t    ma   : bits_per_memory_attribute; // memory attributes
        uint64_t    pl   : bits_per_privilegy;        // privileged level
        uint64_t    ppn  : bits_per_ppn;              // physical page number
    } u;
    bool valid(void) const { return u.v != 0; }
    bool fixed(void) const { return u.f != 0; }
    uint64_t get_ppa(void) const { return address & ~util::makemask(bits_per_minpageoffset); }
} tlb_ppn_t;

typedef union {
    uint64_t    address;
    struct {
        uint64_t    ps    : bits_per_shamt;    // pagesize
        uint64_t    vpn   : bits_per_vpn;      // virtual page number
    } u;

    unsigned int get_page_size(void) const {
        return static_cast<unsigned>(address & util::makemask(bits_per_shamt));
    }

    uint64_t get_vpa(void) const {
        return address & ~util::makemask(bits_per_minpageoffset);
    }
} tlb_vpn_t;

typedef struct {
    tlb_ppn_t    ppn;
    tlb_vpn_t    vpn;
} tlb_entry_t;


class CTranslateBuffer
{
    public:
        unsigned int                 size;
        unsigned int                 insert_head;
        unsigned int                 type;
        unsigned int                 level;
        std::array<tlb_entry_t, 16>  entry;

        CStatus translate_address(uint64_t address, unsigned int rights, unsigned int level) const;
        CStatus insert_translate_buffer(tlb_ppn_t gpr, uint64_t ifa);
};
}  // namespace postrisc
