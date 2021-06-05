#pragma once

// #include "devices/memory.hpp"
#include "TranslationUnit.hpp"

namespace postrisc {

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
typedef union _tlb_ppn_t {
    u64    address;
    struct {
        u64    v    : 1;                         // validation bit
        u64    ar   : bits_per_access;           // access rights
        u64    f    : 1;                         // fixed bit
        u64    p    : 1;                         // present bit
        u64    d    : 1;                         // dirty bit
        u64    a    : 1;                         // access bit
        u64    ma   : bits_per_memory_attribute; // memory attributes
        u64    pl   : bits_per_privilegy;        // privileged level
        u64    ppn  : bits_per_ppn;              // physical page number
    } u;
    bool valid(void) const { return u.v != 0; }
    bool fixed(void) const { return u.f != 0; }
    u64 get_ppa(void) const { return address & ~util::makemask<u64>(bits_per_minpageoffset); }
} tlb_ppn_t;

typedef union _tlb_vpn_t {
    u64    address;
    struct {
        u64    ps    : bits_per_pagesize; // pagesize
        u64    vpn   : bits_per_vpn;      // virtual page number
    } u;

    unsigned int get_page_size(void) const {
        return static_cast<unsigned>(address & util::makemask<u64>(bits_per_pagesize));
    }

    u64 get_vpa(void) const {
        return address & ~util::makemask<u64>(bits_per_minpageoffset);
    }
} tlb_vpn_t;

typedef struct _tlb_entry_t {
    tlb_ppn_t    ppn;
    tlb_vpn_t    vpn;
} tlb_entry_t;


class TranslateBuffer {
public:
    unsigned int                 size;
    unsigned int                 insert_head;
    unsigned int                 type;
    unsigned int                 level;
    std::array<tlb_entry_t, 16>  entry;

    Result translate_address(u64 address, unsigned int rights, unsigned int level) const;
    Result insert_translate_buffer(tlb_ppn_t gpr, u64 ifa);
};

}  // namespace postrisc
