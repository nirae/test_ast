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

#define	NUMBER_TYPE					0
#define	OPEN_BRACKET_TYPE			1
#define	CLOSE_BRACKET_TYPE			2
#define	PLUS_TYPE					3
#define	MINUS_TYPE					4
#define	MULTIPLY_TYPE				5
#define	DIVISION_TYPE				6
#define	NONE_TYPE					7

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								LIST TOKEN
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct	s_token
{
		char	*token;
		int		type;
}				t_token;

int			get_type_of_token(char *token)
{
	if (token[0] >= 48 && token[0] <= 57)
		return (NUMBER_TYPE);
	else if (ft_strequ(token, "("))
		return (OPEN_BRACKET_TYPE);
	else if (ft_strequ(token, ")"))
		return (CLOSE_BRACKET_TYPE);
	else if (ft_strequ(token, "+"))
		return (PLUS_TYPE);
	else if (ft_strequ(token, "-"))
		return (MINUS_TYPE);
	else if (ft_strequ(token, "x"))
		return (MULTIPLY_TYPE);
	else if (ft_strequ(token, "/"))
		return (DIVISION_TYPE);
	return (NONE_TYPE);
}

t_token		*create_token_struct(char *token)
{
	t_token		*tok;

	tok = ft_memalloc(sizeof(t_token));
	tok->token = ft_strdup(token);
	tok->type = get_type_of_token(token);
	return (tok);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								FIN LIST TOKEN
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								LEXER
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

void lexer(char *line, t_list **tokens_list)
{
	int i;
	char stack[512];
	static int A[4][9] = {
		{NUMBER_STATE, OPEN_BRACKET_STATE, CLOSE_BRACKET_STATE, PLUS_STATE, MINUS_STATE, MULTIPLY_STATE, DIVISION_STATE, NONE_STATE},
		{NUMBER_STATE, NONE_STATE, STAR_CLOSE_BRACKET_STATE, STAR_PLUS_STATE, STAR_MINUS_STATE, STAR_MULTIPLY_STATE, STAR_DIVISION_STATE, NONE_STATE},
		{STAR_NUMBER_AOB_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE}
	};
	int current_state;
	int next_state;
	t_list	*token;

	current_state = START_STATE;
	ft_bzero((void *)&stack, 512);
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
			token = ft_lstnew(create_token_struct(stack), sizeof(t_token));
			if ((*tokens_list) == NULL)
				(*tokens_list) = token;
			else
				ft_lstaddend(tokens_list, token);
			ft_strclr(stack);
			current_state = START_STATE;
			continue ;
		}
		current_state = next_state;
	}
	if (current_state == NUMBER_STATE)
	{
		token = ft_lstnew(create_token_struct(stack), sizeof(t_token));
		if ((*tokens_list) == NULL)
			(*tokens_list) = token;
		else
			ft_lstaddend(tokens_list, token);
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								FIN LEXER
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//					PARSE TREE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct			s_ptree
{
		int				id;
		void			*data;
		int				type;
		//struct s_ptree	**childs;
		t_list			*childs_lst;
		int				nb_childs;
}						t_ptree;

void 	ft_ptree_create_node(t_ptree **tree, void *data, int type)
{
	// t_ptree		*tree;
	static int	id = -1;

	id++;
	*tree = ft_memalloc(sizeof(t_ptree));
	(*tree)->id = id;
	(*tree)->data = data;
	(*tree)->type = type;
	(*tree)->childs_lst = NULL;
	(*tree)->nb_childs = 0;
	// return (&tree);
}

// void	ft_ptree_tab_del(t_ptree ***tab)
// {
// 	int		i;
//
// 	if (!*tab || !(*tab)[0])
// 		return ;
// 	i = -1;
// 	while ((*tab)[++i])
// 		ft_memdel((void **)&(*tab)[i]);
// 	ft_memdel((void **)&(*tab)[i]);
// 	ft_memdel((void **)tab);
// 	*tab = NULL;
// }
//
// int		ft_realloc_ptree_tab(t_ptree ***tab, t_ptree *elem)
// {
// 	int		i;
// 	t_ptree	**tmp;
//
// 	if (*tab == NULL || !elem)
// 		return (FALSE);
// 	i = -1;
// 	while ((*tab)[++i])
// 		;
// 	if (!(tmp = ft_memalloc((i + 2) * sizeof(t_ptree *))))
// 		return (FALSE);
// 	i = -1;
// 	while ((*tab)[++i])
// 		if (!(tmp[i] = (*tab)[i]))
// 			return (FALSE);
// 	if (!(tmp[i] = elem))
// 		return (FALSE);
// 	tmp[i + 1] = NULL;
// 	// FREE
// 	//ft_ptree_tab_del(tab);
// 	*tab = tmp;
// 	return (TRUE);
// }

// int		ft_ptree_add_child(t_ptree **tree, t_ptree *child)
// {
// 	if (!(*tree))
// 		return (FALSE);
// 	if ((*tree)->nb_childs == 0)
// 	{
// 		(*tree)->childs = ft_memalloc(sizeof(t_ptree *) * 2);
// 		(*tree)->childs[0] = child;
// 		(*tree)->childs[1] = NULL;
// 		(*tree)->nb_childs++;
// 		return (TRUE);
// 	}
// 	ft_realloc_ptree_tab(&(*tree)->childs, child);
// 	(*tree)->nb_childs++;
// 	return (TRUE);
// }

int		ft_ptree_add_child(t_ptree **tree, void *data, int type)
{
	t_list	*tmp;
	t_ptree	*child;

	if (!(*tree))
		return (FALSE);
	ft_ptree_create_node(&child, data, type);
	tmp = ft_lstnew(child, sizeof(t_ptree *));
	if ((*tree)->nb_childs == 0)
	{
		(*tree)->childs_lst = tmp;
		(*tree)->nb_childs++;
		return (TRUE);
	}
	ft_lstaddend(&(*tree)->childs_lst, tmp);
	(*tree)->nb_childs++;
	return (TRUE);
}

// int		ft_ptree_add_child_with_id(t_ptree *tree, t_ptree *child, int id)
// {
// 	t_list	*tmp;
//
// 	if (!tree)
// 		return (FALSE);
// 	if (tree->id == id)
// 	{
// 		tmp = ft_lstnew(child, sizeof(t_ptree *));
// 		if (tree->nb_childs == 0)
// 		{
// 			tree->childs_lst = tmp;
// 			tree->nb_childs++;
// 			return (TRUE);
// 		}
// 		ft_lstaddend(&tree->childs_lst, tmp);
// 		tree->nb_childs++;
// 		return (TRUE);
// 	}
// 	tmp = tree->childs_lst;
// 	while (tmp)
// 	{
// 		ft_ptree_add_child_with_id(((t_ptree *)(tmp->content)), child, id);
// 		tmp = tmp->next;
// 	}
// 	return (FALSE);
// }


// t_ptree		*get_childs_of_node(t_ptree *tree, int id)
// {
// 	t_list *tmp;
//
// 	tmp = tree->childs_lst;
// 	while (tmp)
// 	{
// 		if (((t_ptree *)(tmp->content))->id == id)
// 			return (((t_ptree *)(tmp->content)));
// 		get_childs_of_node(((t_ptree *)(tmp->content)), id);
// 		tmp = tmp->next;
// 	}
// 	return (NULL);
// }

void 		print_margin(int level)
{
	int i = -1;
	while (++i < level)
		ft_printf("\t");
}

void 		print_type(t_ptree *tree)
{
	ft_printf("type: %d ", tree->type);
}

void 		print_id(t_ptree *tree)
{
	ft_printf("type: %d ", tree->id);
}

void 		print_ptree(int level, t_ptree *tree)
{
	t_list *tmp;

	print_margin(level);
	print_id(tree);
	tmp = tree->childs_lst;
	while (tmp)
	{
		ft_printf("\n");
		print_ptree(level + 1, ((t_ptree *)(tmp->content)));
		tmp = tmp->next;
	}
}

typedef struct	s_tokens_list
{
				t_list	*tokens_list;
				int		index;
				int		size;
}				t_tokens_list;

t_tokens_list		*create_tokens_list_struct(t_list *lst)
{
	t_tokens_list	*result;

	result = ft_memalloc(sizeof(t_tokens_list));
	result->index = 0;
	result->size = ft_lstlen(lst);
	result->tokens_list = lst;
	return (result);
}

// int		test_current_token(t_tokens_list **tokens_list_struct, int type, t_ptree **subtree)
// {
// 	if ((*tokens_list_struct)->index + 1 > (*tokens_list_struct)->size)
// 		return (FALSE);
// 	if (((t_token *)((*tokens_list_struct)->tokens_list->content))->type == type)
// 	{
// 		ft_ptree_add_child((subtree), ft_ptree_create_node(NULL, type));
// 		(*tokens_list_struct)->index++;
// 		return (TRUE);
// 	}
// 	return (FALSE);
// }

// POUR TEST

// int		S(t_tokens_list **tokens_list_struct, t_ptree **tree)
// {
//
// }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//					FIN PARSE TREE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int ac, char **av)
{
	t_list	*tokens_list;
	t_list	*tmp;
	t_tokens_list *tokens_list_struct;
	t_ptree	*tree;

	tree = NULL;
	tokens_list = NULL;
	if (ac != 2)
		return (0);
	lexer(av[1], &tokens_list);
	// DEBUG AFTER LEXER
	tmp = tokens_list;
	while (tmp)
	{
		ft_printf("token = %s, type = %d\n", ((t_token *)(tmp->content))->token, ((t_token *)(tmp->content))->type);
		tmp = tmp->next;
	}
	//
	tokens_list_struct = create_tokens_list_struct(tokens_list);
	ft_printf("\n\n");
	ft_ptree_create_node(&tree, NULL, 1);
	ft_ptree_add_child(&tree, NULL, 2);
	ft_ptree_add_child(&tree, NULL, 3);
	ft_ptree_add_child(&tree, NULL, 4);
	ft_printf("test: %d\n", ((t_ptree *)(tree->childs_lst->next->content))->id);
	// ft_ptree_add_child((t_ptree **)((t_ptree **)(tree->childs_lst->next->content)), NULL, 6);
	// ft_ptree_add_child(((t_ptree **)(&tree->childs_lst->content)), ft_ptree_create_node(NULL, 7));
	// ft_ptree_add_child(((t_ptree **)(&tree->childs_lst->content)), ft_ptree_create_node(NULL, 8));
	print_ptree(0, tree);
	return (0);
}
