#ifndef CALCULATOR_H
# define CALCULATOR_H

#include "libft.h"
#include "libftprintf.h"

#define TRUE						1
#define FALSE						0

/*
**	LEXER
*/

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
#define STAR_NUMBER_AOB_STATE		13
#define NONE_STATE					14

#define	NUMBER_INDEX				0
#define	OPEN_BRACKET_INDEX			1
#define	CLOSE_BRACKET_INDEX			2
#define PLUS_INDEX					3
#define MINUS_INDEX					4
#define MULTIPLY_INDEX				5
#define DIVISION_INDEX				6
#define	NONE_INDEX					7

/*
**	PARSING
*/

#define	START_TYPE					666
#define	NUMBER_TYPE					0
#define	OPEN_BRACKET_TYPE			1
#define	CLOSE_BRACKET_TYPE			2
#define	PLUS_TYPE					3
#define	MINUS_TYPE					4
#define	MULTIPLY_TYPE				5
#define	DIVISION_TYPE				6
#define	NONE_TYPE					7
#define	S_RULE						8
#define	S_PRIME_RULE				88
#define T_RULE						9
#define	T_PRIME_RULE				99
#define	U_RULE						10
#define	V_RULE						11
#define	END_TYPE					-666

/*
**	STRUCT FOR LEXER
*/

typedef struct	s_token
{
		char	*token;
		int		type;
}				t_token;

typedef struct	s_tokens_list
{
				t_list	*tokens_list;
				int		index;
				int		size;
}				t_tokens_list;

/*
**	STRUCT FOR PARSING
*/

typedef struct			s_ptree
{
		int				id;
		char			*data;
		int				type;
		//struct s_ptree	**childs;
		t_list			*childs_lst;
		int				nb_childs;
		t_btree			*ast;
}						t_ptree;

typedef struct		s_ast_token
{
	int				type;
	char			*data;
}					t_ast_token;

void 		lexer(char *line, t_list **tokens_list);
void 		delete_list_tokens(t_list **tokens_list);

void 		ft_print_tokens(t_list *tokens_list);
void		ft_print_parsing_tree(int level, t_ptree **tree);
void 		ft_print_ast(t_btree *ast);

void 		parsing_tree(t_list **tokens_list, t_ptree **tree);
void 		delete_parsing_tree(t_ptree **tree);

t_btree		*get_ast(t_ptree **tree);
void 		delete_ast(t_btree *ast);

#endif
