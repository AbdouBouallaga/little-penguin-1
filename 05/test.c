#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	name[8];
	int	fd;
	int	res;

	memset(name, 0, 8);

	fd = open("/dev/fortytwo", O_RDWR);

	res = read(fd, name, 8);
	printf("res - %s | %d\n", name, res);
	strlcpy(name, "babdelka", 8);
	
	res = write(fd, "babdelka", 8);
	printf("res - %d\n", res);

	res = write(fd, "babiddelka", 8);
        printf("res - %d\n", res);

	return (0);
}
