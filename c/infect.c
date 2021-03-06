#include "woody.h"

#define _BUFFER_SIZE 256

const uint8_t buffer[_BUFFER_SIZE] = {
  0xb8, 0x01, 0x00, 0x00, 0x00, 0xbf, 0x01, 0x00, 0x00, 0x00,
  0x48, 0xbe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xba, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xe9, 0xca, 0xfa,
  0xbe, 0xba, 0x2e, 0x2e, 0x2e, 0x2e, 0x57, 0x4f, 0x4f, 0x44,
  0x59, 0x2e, 0x2e, 0x2e, 0x2e, 0x0a, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static int	load_file(char *fname, struct s_woody *w)
{
	Elf64_Shdr	*sect;
	Elf64_Shdr	*strtab;
	//Elf64_Shdr	new;
	void		*tst;
	unsigned long	new_os;
	int		fd;

	if ((w->fd = open(fname, O_RDONLY)) < 0 || (fstat(w->fd, &w->s) < 0))
		return (0);
	w->header = mmap(0, w->s.st_size, PROT_WRITE, MAP_PRIVATE, w->fd, 0);
	printf("%ld\n", w->header->e_entry);
	printf("%d\n", w->header->e_shnum);
	tst = w->header;
	sect = tst + w->header->e_shoff;
	strtab = &sect[w->header->e_shstrndx];
	char	*strs = tst + strtab->sh_offset;
	new_os = 0;
	for (int i = 0; i != w->header->e_shnum; i++)
	{
		new_os = (sect->sh_offset + sect->sh_size);
		printf("%s %lu %ld\n", (char*)(strs + sect->sh_name), sect->sh_offset, sect->sh_addr);
		sect++;
	}
	w->header->e_entry = w->header->e_shentsize + 8;
	w->header->e_shoff += _BUFFER_SIZE;

	fd = open("new", O_CREAT | O_RDWR);
	write(fd, (void*)w->header, w->header->e_entry);
	write(fd, buffer, _BUFFER_SIZE);
	write(fd, ((void*)w->header) + w->header->e_entry, w->s.st_size - w->header->e_entry);
	printf("%ld %ld\n", w->s.st_size, w->header->e_entry);
	close(fd);
	/*new.sh_name = 0x0;
	new.sh_type = SHT_PROGBITS;
	new.sh_flags = SHF_EXECINSTR;
	new.sh_addr = sect->sh_addr + sect->sh_size;
	new.sh_offset*/
	printf("PUT NEW SECTION HEADER AT OFFSET %lu\n", w->header->e_entry);
	return (1);
}

int		infect(char *fname)
{
	//Elf64_Ehdr	*h;
	//Elf64_Shdr	*s;
	struct s_woody	w;

	return (load_file(fname, &w));
}

int		main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	return (infect(argv[1]));
}
