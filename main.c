#include "libft.h"
#include "libftprintf.h"

#define TRUE						1
#define FALSE						0

#define START_STATE					0
#define NUMBER_STATE				1
#define	OPEN_BRACKET_STATE			2
#define	CLOSE_BRACKET_STATE			3
#define	STAR_CLOSE_BRACKET_STATE	4
#define	PLUS_STATE					5
#define	STAR_PLUS_STATE				6
#define	MINUS_STATE					7
#define STAR_MINUS_STATE			8
#define MULTIPLY_STATE				9
#define STAR_MULTIPLY_STATE			10
#define DIVISION_STATE				11
#define STAR_DIVISION_STATE			12
#define STAR_NUMBER_AOB_STATE			13
#define NONE_STATE					14

#define	NUMBER_INDEX				0
#define	OPEN_BRACKET_INDEX			1
#define	CLOSE_BRACKET_INDEX			2
#define PLUS_INDEX					3
#define MINUS_INDEX					4
#define MULTIPLY_INDEX				5
#define DIVISION_INDEX				6
#define	NONE_INDEX					7

int get_index_from_char(char *s, int i)
{
	if (s[i] >= 48 && s[i] <= 57)
		return (NUMBER_INDEX);
	else if (s[i] == '(' && s[i + 1])
		return (OPEN_BRACKET_INDEX);
	else if (s[i] == ')')
		return (CLOSE_BRACKET_INDEX);
	else if (s[i] == '+' && s[i + 1])
		return (CLOSE_BRACKET_INDEX);
	else if (s[i] == '-' && s[i + 1])
		return (CLOSE_BRACKET_INDEX);
	else if (s[i] == 'x' && s[i + 1])
		return (CLOSE_BRACKET_INDEX);
	else if (s[i] == '/' && s[i + 1])
		return (CLOSE_BRACKET_INDEX);
	return (NONE_INDEX);
}

void put_char_in_stack(char (*stack)[512], char c)
{
	if (ft_strlen(*stack) >= 510)
		exit(0);
	(*stack)[ft_strlen(*stack)] = c;
}

int	is_acceptor(int state)
{
	if (state == CLOSE_BRACKET_STATE || state == PLUS_STATE
		|| state == MINUS_STATE || state == MULTIPLY_STATE
		|| state == DIVISION_STATE  || state == STAR_PLUS_STATE
		|| state == STAR_CLOSE_BRACKET_STATE || state == STAR_MINUS_STATE
		|| state == STAR_MULTIPLY_STATE || state == STAR_DIVISION_STATE
		|| state == STAR_NUMBER_AOB_STATE)
		return (TRUE);
	return (FALSE);
}

int is_star(int state)
{
	if (state == STAR_CLOSE_BRACKET_STATE || state == STAR_PLUS_STATE
		|| state == STAR_MINUS_STATE || state == STAR_MULTIPLY_STATE
		|| state == STAR_DIVISION_STATE || state == STAR_NUMBER_AOB_STATE)
		return (TRUE);
	return (FALSE);
}

void lexer(char *line, char ***tokens)
{
	int i;
	int j;
	char stack[512];
	static int A[4][9] = {
		{NUMBER_STATE, OPEN_BRACKET_STATE, CLOSE_BRACKET_STATE, PLUS_STATE, MINUS_STATE, MULTIPLY_STATE, DIVISION_STATE, NONE_STATE},
		{NUMBER_STATE, NONE_STATE, STAR_CLOSE_BRACKET_STATE, STAR_PLUS_STATE, STAR_MINUS_STATE, STAR_MULTIPLY_STATE, STAR_DIVISION_STATE, NONE_STATE},
		{STAR_NUMBER_AOB_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE}
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
		//ft_printf("index from char = %d\n", get_index_from_char(line[i]));
		// ft_printf("char = %c, next state = %d\n", line[i], next_state);
		next_state = A[current_state][get_index_from_char(line, i)];
		ft_printf("i[%d], char[%c] current state[%d] next_state[%d]\n", i, line[i], current_state, next_state);
		put_char_in_stack(&stack, line[i]);
		if (next_state == NONE_STATE)
		{
			ft_printf("Syntax error near character number %d\n", i + 1);
			exit(0);
		}
		// ETAT ACCEPTEUR
		else if (is_acceptor(next_state))
		{
			// ETAT ETOILE
			if (is_star(next_state))
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
	if (current_state == NUMBER_STATE)
		(*tokens)[j] = ft_strdup(stack);
	// ft_printf("current state after while = %d\n", current_state);
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
