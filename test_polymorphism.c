#include <stdio.h>
#include <stdlib.h>

typedef struct		s_first
{
	int				a;
	int				b;
}					t_first;

typedef struct		s_second
{
	int				a;
	int				b;
	int				c;
}					t_second;

void	print_first_struct(t_first *first)
{
	printf("dans fonction first a = %d, b = %d\n", first->a, first->b);
}

int		main(void)
{
	t_second *second = malloc(sizeof(second));
	second->a = 1;
	second->b = 2;
	second->c = 3;
	
	printf("a = %d, b = %d\n", ((t_first *)(second))->a, ((t_first *)(second))->b);
	print_first_struct((t_first *)second);

	return (0);
}
