#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void	*fct_t1(void *x)
{
	printf("t\n");
	int	*a = (int *) x;
	*a = 100;
printf ("dd\n");
	pthread_exit(x);
}

int main()
{
	pthread_t	t1;
	int		x = 0;
	void		*c;

	if (pthread_create(&t1, NULL, fct_t1, &x) != 0)
		return (write(1, "error\n", 6), 1);
	sleep(1);
	printf ("x= %d\n", x);
	printf ("c= %d\n", *(int *) c);

	if (pthread_join(t1, &c) != 0)
		return (write(1, "join\n", 5), 1);
	printf("join c= %d\n", *(int *) c);
}
