#include "image.h"
#include <stdlib.h>
#include <string.h>

#define	EI_NIDENT	16

typedef unsigned int		Elf32_Addr;
typedef unsigned short		Elf32_Half;
typedef unsigned int		Elf32_Off;
typedef int			Elf32_Sword;
typedef unsigned int		Elf32_Word;

#define	EI_MAG0		0	/* e_ident[] indexes */
#define	EI_MAG1		1
#define	EI_MAG2		2
#define	EI_MAG3		3
#define	EI_CLASS	4	/* File class */
#define	EI_DATA		5	/* Data encoding */
#define	EI_VERSION	6	/* File version */
#define	EI_OSABI	7	/* Operating system/ABI identification */
#define	EI_ABIVERSION	8	/* ABI version */
#define	EI_PAD		9	/* Start of padding bytes */

#define	ELFMAG0		0x7f		/* EI_MAG */
#define	ELFMAG1		'E'
#define	ELFMAG2		'L'
#define	ELFMAG3		'F'
#define	ELFMAG		"\177ELF"
#define	SELFMAG		4

#define	ELFCLASSNONE	0		/* EI_CLASS */
#define	ELFCLASS32	1
#define	ELFCLASS64	2
#define	ELFCLASSNUM	3

#define	ELFDATANONE	0		/* EI_DATA */
#define	ELFDATA2LSB	1
#define	ELFDATA2MSB	2
#define	ELFDATANUM	3

#define	ET_NONE		0		/* e_type */
#define	ET_REL		1
#define	ET_EXEC		2
#define	ET_DYN		3
#define	ET_CORE		4
#define	ET_NUM		5
#define	ET_LOOS		0xfe00		/* OS specific range */
#define	ET_LOSUNW	0xfeff
#define	ET_SUNWPSEUDO	0xfeff
#define	ET_HISUNW	0xfeff
#define	ET_HIOS		0xfeff
#define	ET_LOPROC	0xff00		/* processor specific range */
#define	ET_HIPROC	0xffff

#define	ET_LOPROC	0xff00		/* processor specific range */
#define	ET_HIPROC	0xffff

#define	EM_NONE		0		/* e_machine */
#define	EM_M32		1		/* AT&T WE 32100 */
#define	EM_SPARC	2		/* Sun SPARC */
#define	EM_386		3		/* Intel 80386 */
#define	EM_68K		4		/* Motorola 68000 */
#define	EM_88K		5		/* Motorola 88000 */
#define	EM_486		6		/* Intel 80486 */
#define	EM_860		7		/* Intel i860 */
#define	EM_MIPS		8		/* MIPS RS3000 Big-Endian */
#define	EM_S370		9		/* IBM System/370 Processor */
#define	EM_MIPS_RS3_LE	10		/* MIPS RS3000 Little-Endian */
#define	EM_RS6000	11		/* RS6000 */
#define	EM_UNKNOWN12	12
#define	EM_UNKNOWN13	13
#define	EM_UNKNOWN14	14
#define	EM_PA_RISC	15		/* PA-RISC */
#define	EM_PARISC	EM_PA_RISC	/* Alias: GNU compatibility */
#define	EM_nCUBE	16		/* nCUBE */
#define	EM_VPP500	17		/* Fujitsu VPP500 */
#define	EM_SPARC32PLUS	18		/* Sun SPARC 32+ */
#define	EM_960		19		/* Intel 80960 */
#define	EM_PPC		20		/* PowerPC */
#define	EM_PPC64	21		/* 64-bit PowerPC */
#define	EM_S390		22		/* IBM System/390 Processor */
#define	EM_UNKNOWN22	EM_S390		/* Alias: Older published name */
#define	EM_UNKNOWN23	23
#define	EM_UNKNOWN24	24
#define	EM_UNKNOWN25	25
#define	EM_UNKNOWN26	26
#define	EM_UNKNOWN27	27
#define	EM_UNKNOWN28	28
#define	EM_UNKNOWN29	29
#define	EM_UNKNOWN30	30
#define	EM_UNKNOWN31	31
#define	EM_UNKNOWN32	32
#define	EM_UNKNOWN33	33
#define	EM_UNKNOWN34	34
#define	EM_UNKNOWN35	35
#define	EM_V800		36		/* NEX V800 */
#define	EM_FR20		37		/* Fujitsu FR20 */
#define	EM_RH32		38		/* TRW RH-32 */
#define	EM_RCE		39		/* Motorola RCE */
#define	EM_ARM		40		/* Advanced RISC Marchines ARM */
#define	EM_ALPHA	41		/* Digital Alpha */
#define	EM_SH		42		/* Hitachi SH */
#define	EM_SPARCV9	43		/* Sun SPARC V9 (64-bit) */
#define	EM_TRICORE	44		/* Siemens Tricore embedded processor */
#define	EM_ARC		45		/* Argonaut RISC Core, */
/*	Argonaut Technologies Inc. */
#define	EM_H8_300	46		/* Hitachi H8/300 */
#define	EM_H8_300H	47		/* Hitachi H8/300H */
#define	EM_H8S		48		/* Hitachi H8S */
#define	EM_H8_500	49		/* Hitachi H8/500 */
#define	EM_IA_64	50		/* Intel IA64 */
#define	EM_MIPS_X	51		/* Stanford MIPS-X */
#define	EM_COLDFIRE	52		/* Motorola ColdFire */
#define	EM_68HC12	53		/* Motorola M68HC12 */
#define	EM_MMA		54		/* Fujitsu MMA Mulimedia Accelerator */
#define	EM_PCP		55		/* Siemens PCP */
#define	EM_NCPU		56		/* Sony nCPU embedded RISC processor */
#define	EM_NDR1		57		/* Denso NDR1 microprocessor */
#define	EM_STARCORE	58		/* Motorola Star*Core processor */
#define	EM_ME16		59		/* Toyota ME16 processor */
#define	EM_ST100	60		/* STMicroelectronics ST100 processor */
#define	EM_TINYJ	61		/* Advanced Logic Corp. TinyJ */
/*	embedded processor family */
#define	EM_AMD64	62		/* AMDs x86-64 architecture */
#define	EM_X86_64	EM_AMD64	/* (compatibility) */

#define	EM_PDSP		63		/* Sony DSP Processor */
#define	EM_UNKNOWN64	64
#define	EM_UNKNOWN65	65
#define	EM_FX66		66		/* Siemens FX66 microcontroller */
#define	EM_ST9PLUS	67		/* STMicroelectronics ST9+8/16 bit */
/*	microcontroller */
#define	EM_ST7		68		/* STMicroelectronics ST7 8-bit */
/*	microcontroller */
#define	EM_68HC16	69		/* Motorola MC68HC16 Microcontroller */
#define	EM_68HC11	70		/* Motorola MC68HC11 Microcontroller */
#define	EM_68HC08	71		/* Motorola MC68HC08 Microcontroller */
#define	EM_68HC05	72		/* Motorola MC68HC05 Microcontroller */
#define	EM_SVX		73		/* Silicon Graphics SVx */
#define	EM_ST19		74		/* STMicroelectronics ST19 8-bit */
/*	microcontroller */
#define	EM_VAX		75		/* Digital VAX */
#define	EM_CRIS		76		/* Axis Communications 32-bit */
/*	embedded processor */
#define	EM_JAVELIN	77		/* Infineon Technologies 32-bit */
/*	embedded processor */
#define	EM_FIREPATH	78		/* Element 14 64-bit DSP Processor */
#define	EM_ZSP		79		/* LSI Logic 16-bit DSP Processor */
#define	EM_MMIX		80		/* Donald Knuth's educational */
/*	64-bit processor */
#define	EM_HUANY	81		/* Harvard University */
/*	machine-independent */
/*	object files */
#define	EM_PRISM	82		/* SiTera Prism */
#define	EM_AVR		83		/* Atmel AVR 8-bit microcontroller */
#define	EM_FR30		84		/* Fujitsu FR30 */
#define	EM_D10V		85		/* Mitsubishi D10V */
#define	EM_D30V		86		/* Mitsubishi D30V */
#define	EM_V850		87		/* NEC v850 */
#define	EM_M32R		88		/* Mitsubishi M32R */
#define	EM_MN10300	89		/* Matsushita MN10300 */
#define	EM_MN10200	90		/* Matsushita MN10200 */
#define	EM_PJ		91		/* picoJava */
#define	EM_OPENRISC	92		/* OpenRISC 32-bit embedded processor */
#define	EM_ARC_A5	93		/* ARC Cores Tangent-A5 */
#define	EM_XTENSA	94		/* Tensilica Xtensa architecture */
#define	EM_NUM		95

#define	EV_NONE		0		/* e_version, EI_VERSION */
#define	EV_CURRENT	1
#define	EV_NUM		2


#define	ELFOSABI_NONE		0	/* No extensions or unspecified */
#define	ELFOSABI_SYSV		ELFOSABI_NONE
#define	ELFOSABI_HPUX		1	/* Hewlett-Packard HP-UX */
#define	ELFOSABI_NETBSD		2	/* NetBSD */
#define	ELFOSABI_LINUX		3	/* Linux */
#define	ELFOSABI_UNKNOWN4	4
#define	ELFOSABI_UNKNOWN5	5
#define	ELFOSABI_SOLARIS	6	/* Sun Solaris */
#define	ELFOSABI_AIX		7	/* AIX */
#define	ELFOSABI_IRIX		8	/* IRIX */
#define	ELFOSABI_FREEBSD	9	/* FreeBSD */
#define	ELFOSABI_TRU64		10	/* Compaq TRU64 UNIX */
#define	ELFOSABI_MODESTO	11	/* Novell Modesto */
#define	ELFOSABI_OPENBSD	12	/* Open BSD */
#define	ELFOSABI_OPENVMS	13	/* Open VMS */
#define	ELFOSABI_NSK		14	/* Hewlett-Packard Non-Stop Kernel */
#define	ELFOSABI_AROS		15	/* Amiga Research OS */
#define	ELFOSABI_ARM		97	/* ARM */
#define	ELFOSABI_STANDALONE	255	/* standalone (embedded) application */

typedef struct {
  unsigned char	e_ident[EI_NIDENT];	/* ident bytes */
  Elf32_Half	e_type;			/* file type */
  Elf32_Half	e_machine;		/* target machine */
  Elf32_Word	e_version;		/* file version */
  Elf32_Addr	e_entry;		/* start address */
  Elf32_Off	e_phoff;		/* phdr file offset */
  Elf32_Off	e_shoff;		/* shdr file offset */
  Elf32_Word	e_flags;		/* file flags */
  Elf32_Half	e_ehsize;		/* sizeof ehdr */
  Elf32_Half	e_phentsize;		/* sizeof phdr */
  Elf32_Half	e_phnum;		/* number phdrs */
  Elf32_Half	e_shentsize;		/* sizeof shdr */
  Elf32_Half	e_shnum;		/* number shdrs */
  Elf32_Half	e_shstrndx;		/* shdr string index */
} Elf32_Ehdr;

typedef struct {
  Elf32_Word	p_type;		/* entry type */
  Elf32_Off	p_offset;	/* file offset */
  Elf32_Addr	p_vaddr;	/* virtual address */
  Elf32_Addr	p_paddr;	/* physical address */
  Elf32_Word	p_filesz;	/* file size */
  Elf32_Word	p_memsz;	/* memory size */
  Elf32_Word	p_flags;	/* entry flags */
  Elf32_Word	p_align;	/* memory/file alignment */
} Elf32_Phdr;

#define	PT_NULL		0		/* p_type */
#define	PT_LOAD		1
#define	PT_DYNAMIC	2
#define	PT_INTERP	3
#define	PT_NOTE		4
#define	PT_SHLIB	5
#define	PT_PHDR		6
#define	PT_TLS		7
#define	PT_NUM		8

#define	PT_LOOS		0x60000000	/* OS specific range */

/*
 * Note: The amd64 psABI defines that the UNWIND program header
 *	 should reside in the OS specific range of the program
 *	 headers.
 */
#define	PT_SUNW_UNWIND	0x6464e550	/* amd64 UNWIND program header */
#define	PT_GNU_EH_FRAME	PT_SUNW_UNWIND

#define	PT_LOSUNW	0x6ffffffa
#define	PT_SUNWBSS	0x6ffffffa	/* Sun Specific segment */
#define	PT_SUNWSTACK	0x6ffffffb	/* describes the stack segment */
#define	PT_SUNWDTRACE	0x6ffffffc	/* private */
#define	PT_SUNWCAP	0x6ffffffd	/* hard/soft capabilities segment */
#define	PT_HISUNW	0x6fffffff
#define	PT_HIOS		0x6fffffff

#define	PT_LOPROC	0x70000000	/* processor specific range */
#define	PT_HIPROC	0x7fffffff

#define	PF_R		0x4		/* p_flags */
#define	PF_W		0x2
#define	PF_X		0x1

#define	PF_MASKOS	0x0ff00000	/* OS specific values */
#define	PF_MASKPROC	0xf0000000	/* processor specific values */

#define	PF_SUNW_FAILURE	0x00100000	/* mapping absent due to failure */

#define	PN_XNUM		0xffff		/* extended program header index */

typedef struct {
  Elf32_Word	sh_name;	/* section name */
  Elf32_Word	sh_type;	/* SHT_... */
  Elf32_Word	sh_flags;	/* SHF_... */
  Elf32_Addr	sh_addr;	/* virtual address */
  Elf32_Off	sh_offset;	/* file offset */
  Elf32_Word	sh_size;	/* section size */
  Elf32_Word	sh_link;	/* misc info */
  Elf32_Word	sh_info;	/* misc info */
  Elf32_Word	sh_addralign;	/* memory alignment */
  Elf32_Word	sh_entsize;	/* entry size if table */
} Elf32_Shdr;

#define	SHT_NULL		0		/* sh_type */
#define	SHT_PROGBITS		1
#define	SHT_SYMTAB		2
#define	SHT_STRTAB		3
#define	SHT_RELA		4
#define	SHT_HASH		5
#define	SHT_DYNAMIC		6
#define	SHT_NOTE		7
#define	SHT_NOBITS		8
#define	SHT_REL			9
#define	SHT_SHLIB		10
#define	SHT_DYNSYM		11
#define	SHT_UNKNOWN12		12
#define	SHT_UNKNOWN13		13
#define	SHT_INIT_ARRAY		14
#define	SHT_FINI_ARRAY		15
#define	SHT_PREINIT_ARRAY	16
#define	SHT_GROUP		17
#define	SHT_SYMTAB_SHNDX	18
#define	SHT_NUM			19

/* Solaris ABI specific values */
#define	SHT_LOOS		0x60000000	/* OS specific range */
#define	SHT_LOSUNW		0x6ffffff1
#define	SHT_SUNW_symsort	0x6ffffff1
#define	SHT_SUNW_tlssort	0x6ffffff2
#define	SHT_SUNW_LDYNSYM	0x6ffffff3
#define	SHT_SUNW_dof		0x6ffffff4
#define	SHT_SUNW_cap		0x6ffffff5
#define	SHT_SUNW_SIGNATURE	0x6ffffff6
#define	SHT_SUNW_ANNOTATE	0x6ffffff7
#define	SHT_SUNW_DEBUGSTR	0x6ffffff8
#define	SHT_SUNW_DEBUG		0x6ffffff9
#define	SHT_SUNW_move		0x6ffffffa
#define	SHT_SUNW_COMDAT		0x6ffffffb
#define	SHT_SUNW_syminfo	0x6ffffffc
#define	SHT_SUNW_verdef		0x6ffffffd
#define	SHT_SUNW_verneed	0x6ffffffe
#define	SHT_SUNW_versym		0x6fffffff
#define	SHT_HISUNW		0x6fffffff
#define	SHT_HIOS		0x6fffffff

/* GNU/Linux ABI specific values */
#define	SHT_GNU_verdef		0x6ffffffd
#define	SHT_GNU_verneed		0x6ffffffe
#define	SHT_GNU_versym		0x6fffffff

#define	SHT_LOPROC	0x70000000	/* processor specific range */
#define	SHT_HIPROC	0x7fffffff

#define	SHT_LOUSER	0x80000000
#define	SHT_HIUSER	0xffffffff

#define	SHF_WRITE		0x01		/* sh_flags */
#define	SHF_ALLOC		0x02
#define	SHF_EXECINSTR		0x04
#define	SHF_MERGE		0x10
#define	SHF_STRINGS		0x20
#define	SHF_INFO_LINK		0x40
#define	SHF_LINK_ORDER		0x80
#define	SHF_OS_NONCONFORMING	0x100
#define	SHF_GROUP		0x200
#define	SHF_TLS			0x400

#define	SHF_MASKOS	0x0ff00000	/* OS specific values */


#define	SHF_MASKPROC	0xf0000000	/* processor specific values */

#define	SHN_UNDEF	0		/* special section numbers */
#define	SHN_LORESERVE	0xff00
#define	SHN_LOPROC	0xff00		/* processor specific range */
#define	SHN_HIPROC	0xff1f
#define	SHN_LOOS	0xff20		/* OS specific range */
#define	SHN_LOSUNW	0xff3f
#define	SHN_SUNW_IGNORE	0xff3f
#define	SHN_HISUNW	0xff3f
#define	SHN_HIOS	0xff3f
#define	SHN_ABS		0xfff1
#define	SHN_COMMON	0xfff2
#if defined(__APPLE__)
#define SHN_MACHO_64	0xfffd		/* Mach-o_64 direct string access */
#define SHN_MACHO	0xfffe		/* Mach-o direct string access */
#endif /* __APPLE__ */
#define	SHN_XINDEX	0xffff		/* extended sect index */
#define	SHN_HIRESERVE	0xffff

bool process_elf( Image_t *image ) {
  Elf32_Ehdr *elf32_eheader = (Elf32_Ehdr *)get_raw_image_at_offset( image, 0, sizeof( Elf32_Ehdr ));

  if( !elf32_eheader )
    return false;

  if( elf32_eheader->e_ehsize != sizeof(Elf32_Ehdr) )
    return false;

  if( elf32_eheader->e_ident[EI_MAG0] != ELFMAG0 || elf32_eheader->e_ident[EI_MAG1] != ELFMAG1 || elf32_eheader->e_ident[EI_MAG2] != ELFMAG2 || elf32_eheader->e_ident[EI_MAG3] != ELFMAG3 )
    return false;

  if( elf32_eheader->e_ident[EI_CLASS] != ELFCLASS32 || elf32_eheader->e_ident[EI_DATA] != ELFDATA2LSB || elf32_eheader->e_ident[EI_VERSION] != EV_CURRENT || elf32_eheader->e_ident[EI_OSABI] != ELFOSABI_SYSV )
    return false;

  if( elf32_eheader->e_machine != EM_386 )
    return false;

  if( elf32_eheader->e_phentsize != sizeof(Elf32_Phdr) || elf32_eheader->e_shentsize != sizeof(Elf32_Shdr) )
    return false;

  image->architecture_type = IA32;
  image->image_base = 0;
  image->relative_virtual_entry_point = elf32_eheader->e_entry;
  image->number_of_sections = elf32_eheader->e_phnum;
  image->stack_size = 1048576;
  Elf32_Phdr *elf32_pheader = (Elf32_Phdr *)get_raw_image_at_offset( image, elf32_eheader->e_phoff, sizeof( Elf32_Phdr) );

  if( !elf32_pheader)
    return false;

  image->sections = (ImageSection_t *)calloc( image->number_of_sections, sizeof(ImageSection_t) );

  for( uint32_t i=0; i < elf32_eheader->e_phnum; i++ ) {
    if( elf32_pheader[i].p_type == PT_LOAD && elf32_pheader[i].p_vaddr % 4096 == 0 && elf32_pheader[i].p_offset % 4096 == 0 ) {
      image->sections[i].buffer = (unsigned char *)get_raw_image_at_offset( image, elf32_pheader[i].p_offset, elf32_pheader[i].p_filesz );
      if( image->sections[i].buffer == NULL ) {
        free( image->sections );
        return false;
      }
    }
    else
      image->sections[i].buffer = NULL;

    memset( image->sections[i].name, 0, sizeof( ((ImageSection_t *) 0)->name));
    image->sections[i].raw_size = elf32_pheader[i].p_filesz;
    image->sections[i].relative_virtual_base_address = elf32_pheader[i].p_vaddr;
    image->sections[i].virtual_size = elf32_pheader[i].p_memsz;

    image->sections[i].characteristics = 0;
    if( elf32_pheader[i].p_flags &  PF_R)
      image->sections[i].characteristics |= SECTION_CHARACTERISTIC_MEM_READ;
    if( elf32_pheader[i].p_flags &  PF_W)
      image->sections[i].characteristics |= SECTION_CHARACTERISTIC_MEM_WRITE;
    if( elf32_pheader[i].p_flags &  PF_X)
      image->sections[i].characteristics |= SECTION_CHARACTERISTIC_MEM_EXECUTE;
  }

  return true;
}