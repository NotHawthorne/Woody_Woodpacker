#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>

struct	s_woody
{
	Elf64_Ehdr	*header;
	struct stat	s;
	int		addsz;
	int		fd;
};

int	infect(char *fname);

#endif
