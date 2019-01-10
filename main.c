#include "libft.h"
#include "libftprintf.h"

#define START_STATE 0
#define CHAR_STATE 1
#define	IGNORE_STATE_SPACE 2
#define	IGNORE_STATE_DOTCOMMA 3
#define	SPACE_STATE 4
#define	DOTCOMMA_STATE_STAR 5
#define	DOTCOMMA_STATE 6
#define	NONE_STATE 7

#define	CHAR_INDEX 0
#define	DOTCOMMA_INDEX 1
#define	SPACE_INDEX	2

int get_index_from_char(char c)
{
	if (c >= 97 && c <= 122)
		return (CHAR_INDEX);
	else if (c == ';')
		return (DOTCOMMA_INDEX);
	else if (c == ' ')
		return (SPACE_INDEX);
	return (NONE_STATE);
}

void put_char_in_stack(char (*stack)[512], char c)
{
	if (ft_strlen(*stack) >= 510)
		exit(0);
	(*stack)[ft_strlen(*stack)] = c;
}

void lexer(char *line, char ***tokens)
{
	int i;
	int j;
	char stack[512];
	static int A[4][4] = {
		{CHAR_STATE, DOTCOMMA_STATE, IGNORE_STATE_SPACE},
		{CHAR_STATE, DOTCOMMA_STATE_STAR, SPACE_STATE},
		{CHAR_STATE, DOTCOMMA_STATE, IGNORE_STATE_SPACE},
		{CHAR_STATE, IGNORE_STATE_DOTCOMMA, IGNORE_STATE_SPACE}
	};
	int current_state;
	int next_state;

	current_state = START_STATE;
	ft_bzero((void *)&stack, 512);
	(void)tokens;
	j = 0;
	i = -1;
	while (line[++i])
	{
		// ft_printf("index from char = %d\n", get_index_from_char(line[i]));
		// ft_printf("char = %c, next state = %d\n", line[i], next_state);
		next_state = A[current_state][get_index_from_char(line[i])];
		ft_printf("i[%d], char[%c] current state[%d] next_state[%d]\n", i, line[i], current_state, next_state);
		if (next_state != IGNORE_STATE_DOTCOMMA && next_state != IGNORE_STATE_SPACE)
			put_char_in_stack(&stack, line[i]);
		if (next_state == NONE_STATE)
		{
			ft_printf("erreur\n");
			exit(0);
		}
		// ETAT ACCEPTEUR
		else if (next_state == SPACE_STATE || next_state == DOTCOMMA_STATE_STAR || next_state == DOTCOMMA_STATE)
		{
			// ETAT ETOILE
			if (next_state == DOTCOMMA_STATE_STAR || next_state == SPACE_STATE)
			{
				stack[ft_strlen(stack) - 1] = 0;
				i--;
			}
			ft_printf("!!!!!!!!!!!!!!!!!!!!!!\netat stack ici = %s\n!!!!!!!!!!\n", stack);
			(*tokens)[j] = ft_strdup(stack);
			j++;
			ft_strclr(stack);
			current_state = START_STATE;
			continue ;
		}
		current_state = next_state;
	}
}

int main(int ac, char **av)
{
	char **tokens;
	int i;

	tokens = ft_memalloc(sizeof(char *) * 512);
	if (ac != 2)
		return (0);
	lexer(av[1], &tokens);
	i = -1;
	while (tokens[++i])
		ft_printf("token [%d] = %s\n", i, tokens[i]);
	return (0);
}
