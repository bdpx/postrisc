#include <fcntl.h>
#include <libelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#if 0
unsigned char code[] = {
    0xB8, 0x3C, 0x00, 0x00, 0x00, // mov rax, 60
    0xBF, 0x00, 0x00, 0x00, 0x00, // mov rdi, 0
    0x0F, 0x05                    // syscall
};

#define LOADADDR 0x08048000

int main(int argc, char const *argv[])
{
    int fd;
    Elf *e;
    Elf_Scn *scn;
    Elf_Data *data;
    Elf64_Ehdr *ehdr;
    Elf64_Phdr *phdr;
    Elf64_Shdr *shdr;

    if (argc != 2)
    {
        std::cerr << "usage: %s filename\n", argv[0]);
    }

    if (elf_version(EV_CURRENT) == EV_NONE)
    {
        std::cerr << "elf_version is ev_none, wtf? %s\n", elf_errmsg(-1));
    }

    if ((fd = open(argv[1], O_WRONLY | O_CREAT, 0777)) < 0)
    {
        std::cerr << "open failed: %s\n", elf_errmsg(-1));
    }

    if ((e = elf_begin(fd, ELF_C_WRITE, NULL)) == NULL)
    {
        std::cerr << "elf_begin failed: %s\n", elf_errmsg(-1));
    }

    if ((ehdr = elf64_newehdr(e)) == NULL)
    {
        std::cerr << "elf64_newehdr failed: %s\n", elf_errmsg(-1));
    }

    ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
    ehdr->e_ident[EI_CLASS] = ELFCLASS64;
    ehdr->e_machine = EM_X86_64;
    ehdr->e_type = ET_EXEC;

    size_t ehdrsz = elf64_fsize(ELF_T_EHDR, 1, EV_CURRENT);
    size_t phdrsz = elf64_fsize(ELF_T_PHDR, 1, EV_CURRENT);
    ehdr->e_entry = LOADADDR + ehdrsz + phdrsz;

    if ((phdr = elf64_newphdr(e, 1)) == NULL)
    {
        std::cerr << "elf64_newphdr failed: %s\n", elf_errmsg(-1));
    }

    if ((scn = elf_newscn(e)) == NULL)
    {
        std::cerr << "elf_newscn failed: %s\n", elf_errmsg(-1));
    }

    if ((data = elf_newdata(scn)) == NULL)
    {
        std::cerr << "elf64_newdata failed: %s\n", elf_errmsg(-1));
    }

    data->d_align = 1;
    data->d_off = 0LL;
    data->d_buf = code;
    data->d_type = ELF_T_BYTE;
    data->d_size = sizeof(code);
    data->d_version = EV_CURRENT;

    if ((shdr = elf64_getshdr(scn)) == NULL)
    {
        std::cerr << "elf64_getshdr %s\n", elf_errmsg(-1));
    }

    shdr->sh_name = 1;
    shdr->sh_type = SHT_PROGBITS;
    shdr->sh_flags = SHF_EXECINSTR | SHF_ALLOC;
    shdr->sh_addr = LOADADDR + ehdrsz + phdrsz;

    if (elf_update(e, ELF_C_NULL) < 0)
    {
        std::cerr << "elf_update failed: %s\n", elf_errmsg(-1));
    }

    phdr->p_type = PT_LOAD;
    phdr->p_offset = 0;
    phdr->p_filesz = ehdrsz + phdrsz + sizeof(code);
    phdr->p_memsz = phdr->p_filesz;
    phdr->p_vaddr = LOADADDR;
    phdr->p_paddr = phdr->p_vaddr;
    phdr->p_align = 4;
    phdr->p_flags = PF_X | PF_R;

    elf_flagphdr(e, ELF_C_SET, ELF_F_DIRTY);

    if (elf_update(e, ELF_C_WRITE) < 0) {
        std::cerr << "elf_update failed: %s\n", elf_errmsg(-1));
    }

    elf_end(e);
    close(fd);

    return 0;
}

#endif







/*
* Create an ELF object .
*/
#include <err.h>
#include <fcntl.h>
#include <libelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

uint32_t hash_words [] = {
    0x01234567,
    0x89abcdef,
    0xdeadc0de
};

std::vector<char> shstrtab_vdata;

size_t add_name(std::vector<char>& strtab, const char *name)
{
    const size_t offset = strtab.size();
    for (size_t i = 0; name[i] != '\0'; i++) {
        strtab.push_back(name[i]);
    }
    strtab.push_back('\0');
    return offset;
}

std::vector<char> text_data(64*16, 't');
std::vector<char> rodata_data(56*16, 'r');
std::vector<char> data_data(96*16, 'd');

static void make_text(Elf *e)
{
    Elf_Scn *scn = elf_newscn(e);
    if (!scn)
        std::cerr << " elf_newscn() failed: " << elf_errmsg(-1);

    Elf_Data *data = elf_newdata(scn);
    if (!data)
        std::cerr << " elf_newdata() failed: " << elf_errmsg(-1);

    data->d_buf = text_data.data();
    data->d_size = text_data.size();
    data->d_off = 0LL;
    data->d_type = ELF_T_BYTE;
    data->d_align = 1024 * 16;
    data->d_version = EV_CURRENT;

    Elf64_Shdr *shdr = elf64_getshdr(scn);
    if (!shdr)
        std::cerr << " elf64_getshdr() failed: " << elf_errmsg(-1);

    shdr->sh_name = add_name(shstrtab_vdata, ".text");
    shdr->sh_type = SHT_STRTAB;
    shdr->sh_flags = SHF_STRINGS | SHF_ALLOC;
    shdr->sh_entsize = 0;
    shdr->sh_offset = 1024 * 16;
    shdr->sh_addr = 1024 * 16;
}

static void make_data(Elf *e)
{
    Elf_Scn *scn = elf_newscn(e);
    if (!scn)
        std::cerr << " elf_newscn() failed: " << elf_errmsg(-1);

    Elf_Data *data = elf_newdata(scn);
    if (!data)
        std::cerr << " elf_newdata() failed: " << elf_errmsg(-1);

    data->d_buf = data_data.data();
    data->d_size = data_data.size();
    data->d_off = 0LL;
    data->d_type = ELF_T_BYTE;
    data->d_align = 1024 * 16;
    data->d_version = EV_CURRENT;

    Elf64_Shdr *shdr = elf64_getshdr(scn);
    if (!shdr)
        std::cerr << " elf64_getshdr() failed: " << elf_errmsg(-1);

    shdr->sh_name = add_name(shstrtab_vdata, ".data");
    shdr->sh_type = SHT_STRTAB;
    shdr->sh_flags = SHF_STRINGS | SHF_ALLOC;
    shdr->sh_entsize = 0;
    shdr->sh_offset = 2 * 1024 * 16;
    shdr->sh_addr = 2 * 1024 * 16;
}

int main(int argc, char ** argv)
{
    add_name(shstrtab_vdata, "");

    Elf_Scn * scn;
    Elf_Data * data;
    Elf64_Phdr * phdr;
    Elf64_Shdr * shdr;

    if (argc != 2)
        std::cerr << " usage : " << argv[0] << " file - name";

    if (elf_version(EV_CURRENT) == EV_NONE)
        std::cerr << "ELF library initialization failed: " << elf_errmsg(-1);

    const int fd = open(argv[1], O_WRONLY | O_CREAT, 0777);
    if (fd < 0)
        std::cerr << "open failed" <<  argv[1];

    Elf *e = elf_begin(fd, ELF_C_WRITE, NULL);
    if (!e)
        std::cerr << " elf_begin() failed: " << elf_errmsg(-1);

    Elf64_Ehdr *ehdr = elf64_newehdr(e);
    if (!ehdr)
        std::cerr << " elf64_newehdr() failed: " << elf_errmsg(-1);

    ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
    ehdr->e_ident[EI_CLASS] = ELFCLASS64;
    ehdr->e_machine = EM_X86_64;
    ehdr->e_type = ET_EXEC;


    // create a new section
    Elf_Scn *shstrtab_scn = elf_newscn(e);
    // Get its header
    Elf64_Shdr *shstrtab_shdr = elf64_getshdr(shstrtab_scn);

    shstrtab_shdr->sh_name = add_name(shstrtab_vdata, ".shstrtab");
    shstrtab_shdr->sh_type = SHT_STRTAB; // Set type to String Table
    shstrtab_shdr->sh_flags = SHF_STRINGS; // Indicate it contains strings
    shstrtab_shdr->sh_entsize = 0; // Not applicable for string tables
    shstrtab_shdr->sh_addralign = 1; // Byte alignment


    if ((phdr = elf64_newphdr(e, 1)) == NULL)
        std::cerr << " elf64_newphdr() failed: " << elf_errmsg(-1);

    if ((scn = elf_newscn(e)) == NULL)
        std::cerr << " elf_newscn() failed: " << elf_errmsg(-1);
    if ((data = elf_newdata(scn)) == NULL)
        std::cerr << " elf_newdata() failed: " << elf_errmsg(-1);

    data->d_align = 4;
    data->d_off = 0LL;
    data->d_buf = hash_words;
    data->d_type = ELF_T_WORD;
    data->d_size = sizeof(hash_words);
    data->d_version = EV_CURRENT;

    if ((shdr = elf64_getshdr(scn)) == NULL)
        std::cerr << " elf64_getshdr() failed: " << elf_errmsg(-1);

    shdr->sh_name = add_name(shstrtab_vdata, ".foo");
    shdr->sh_type = SHT_HASH;
    shdr->sh_flags = SHF_ALLOC;
    shdr->sh_entsize = 0;

    make_text(e);
    make_data(e);


    Elf_Data *shstrtab_data = elf_newdata(shstrtab_scn);
    shstrtab_data->d_buf = shstrtab_vdata.data();
    shstrtab_data->d_size = shstrtab_vdata.size();
    shstrtab_data->d_off = 0;
    shstrtab_data->d_type = ELF_T_BYTE;
    shstrtab_data->d_align = 1;
    shstrtab_data->d_version = EV_CURRENT;

    ehdr->e_shstrndx = elf_ndxscn(shstrtab_scn);

    //if (elf_update(e, ELF_C_NULL) < 0)
    //    std::cerr << " elf_update(NULL) failed: " << elf_errmsg(-1);

    phdr->p_type = PT_PHDR;
    phdr->p_offset = ehdr->e_phoff;
    phdr->p_filesz = elf64_fsize(ELF_T_PHDR, 1, EV_CURRENT);
    elf_flagphdr(e, ELF_C_SET, ELF_F_DIRTY);

/*
    phdr->p_type = PT_LOAD;
    phdr->p_offset = 0;
    phdr->p_filesz = ehdrsz + phdrsz + sizeof(code);
    phdr->p_memsz = phdr->p_filesz;
    phdr->p_vaddr = LOADADDR;
    phdr->p_paddr = phdr->p_vaddr;
    phdr->p_align = 4;
    phdr->p_flags = PF_X | PF_R;
*/

    if (elf_update(e, ELF_C_WRITE) < 0)
        std::cerr << " elf_update() failed: " << elf_errmsg(-1);

    elf_end(e);
    close(fd);
    return 0;;
}
