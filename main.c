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

typedef struct	s_token
{
		char	*token;
		int		type;
}				t_token;

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

typedef struct	s_tokens_list
{
				t_list	*tokens_list;
				int		index;
				int		size;
}				t_tokens_list;

typedef struct		s_ast_token
{
	int				type;
	char			*data;
}					t_ast_token;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								FONCTIONS DELETE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 	free_token(void *content, size_t size)
{
	(void)size;
	ft_strdel(&((*((t_token **)(content)))->token));
	ft_memdel((void **)((t_token **)(content)));
	ft_memdel(&content);
}

void 	del_ptree_node(void *content, size_t size);

void 	free_ast_token(void *data)
{
	ft_strdel(&((t_ast_token *)(data))->data);
	ft_memdel(&data);
}

void 	free_ptree_node(t_ptree **tree)
{
	ft_lstdel(&((*tree)->childs_lst), del_ptree_node);
	ft_memdel((void **)tree);
}

void 	del_ptree_node(void *content, size_t size)
{
	(void)size;
	if (!content)
		return ;
	// Free un child
	ft_lstdel(&(*((t_ptree **)(content)))->childs_lst, del_ptree_node);
	free_ptree_node(((t_ptree **)(content)));
	ft_memdel(&content);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								FIN FONCTIONS DELETE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//								LIST TOKEN
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



int			get_type_of_token(char *token)
{
	if (!token)
		return (END_TYPE);
	else if (token[0] >= 48 && token[0] <= 57)
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

void 	create_token_struct(char *token, t_token **tok)
{
	// t_token		*tok;

	*tok = ft_memalloc(sizeof(t_token));
	(*tok)->token = ft_strdup(token);
	(*tok)->type = get_type_of_token(token);
	// return (tok);
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
	t_token	*token_struct;

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
			create_token_struct(stack, &token_struct);
			token = ft_lstnew(&token_struct, sizeof(t_token));
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
		create_token_struct(stack, &token_struct);
		token = ft_lstnew(&token_struct, sizeof(t_token));
		if ((*tokens_list) == NULL)
			(*tokens_list) = token;
		else
			ft_lstaddend(tokens_list, token);
	}
	create_token_struct(NULL, &token_struct);
	token = ft_lstnew(&token_struct, sizeof(t_token));
	ft_lstaddend(tokens_list, token);
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



void 	ft_ptree_create_node(t_ptree **tree, void *data, int type)
{
	static int	id = -1;

	id++;
	*tree = ft_memalloc(sizeof(t_ptree));
	(*tree)->id = id;
	(*tree)->data = data;
	(*tree)->type = type;
	(*tree)->childs_lst = NULL;
	(*tree)->nb_childs = 0;
	(*tree)->ast = NULL;
}

int		ft_ptree_add_child(t_ptree **tree, void *data, int type)
{
	t_list	*tmp;
	t_ptree	*child;

	if (!tree)
		return (FALSE);
	ft_ptree_create_node(&child, data, type);
	if (!(tmp = ft_lstnew(&child, sizeof(t_ptree *))))
	{
		ft_printf("erreur de lstnew dans add_child\n");
		return (FALSE);
	}
	if ((*tree)->nb_childs == 0)
	{
		(*tree)->childs_lst = tmp;
		(*tree)->nb_childs++;
		return (child->id);
	}
	ft_lstaddend(&(*tree)->childs_lst, tmp);
	(*tree)->nb_childs++;
	return (child->id);
}

// int		ft_ptree_add_child_with_id(t_ptree **tree, int id, void *data, int type)
// {
// 	t_list	*tmp;
// 	t_ptree	*child;
//
// 	if (!tree)
// 		return (-1);
// 	if ((*tree)->id == id)
// 	{
// 		ft_ptree_create_node(&child, data, type);
// 		tmp = ft_lstnew(&child, sizeof(t_ptree *));
// 		if ((*tree)->nb_childs == 0)
// 		{
// 			(*tree)->childs_lst = tmp;
// 			(*tree)->nb_childs++;
// 			return (child->id);
// 		}
// 		ft_lstaddend(&(*tree)->childs_lst, tmp);
// 		(*tree)->nb_childs++;
// 		return (child->id);
// 	}
// 	tmp = (*tree)->childs_lst;
// 	while (tmp)
// 	{
// 		ft_ptree_add_child_with_id(((t_ptree **)(tmp->content)), id, data, type);
// 		tmp = tmp->next;
// 	}
// 	return (-1);
// }

t_ptree		**ft_ptree_get_node_with_id(t_ptree **tree, int id)
{
	t_list	*tmp;

	if (!tree)
		return (NULL);
	if ((*tree)->id == id)
		return (tree);
	tmp = (*tree)->childs_lst;
	while (tmp)
	{
		if ((*((t_ptree **)(tmp->content)))->id == id)
			return (((t_ptree **)(tmp->content)));
		ft_ptree_get_node_with_id(((t_ptree **)(tmp->content)), id);
		tmp = tmp->next;
	}
	return (NULL);
}

int		ft_ptree_remove_node_with_id(t_ptree **tree, int id)
{
	t_list	*tmp;
	t_list	*prev;

	if (!tree)
		return (FALSE);
	tmp = (*tree)->childs_lst;
	if (tmp && (*((t_ptree **)(tmp->content)))->id == id)
	{
		prev = tmp;
		(*tree)->childs_lst = tmp->next;
		ft_lstdelone(&prev, del_ptree_node);
		(*tree)->nb_childs--;
		return (TRUE);
	}
	while (tmp)
	{
		if (tmp->next && (*((t_ptree **)(tmp->next->content)))->id == id)
		{
			prev = tmp->next;
			tmp->next = tmp->next->next;
			ft_lstdelone(&prev, del_ptree_node);
			free(prev);
			(*tree)->nb_childs--;
			return (TRUE);
		}
		ft_ptree_remove_node_with_id(((t_ptree **)(tmp->content)), id);
		tmp = tmp->next;
	}
	return (FALSE);
}

t_ptree		*get_childs_of_node(t_ptree *tree, int id)
{
	t_list *tmp;

	tmp = tree->childs_lst;
	while (tmp)
	{
		if (((t_ptree *)(tmp->content))->id == id)
			return (((t_ptree *)(tmp->content)));
		get_childs_of_node(((t_ptree *)(tmp->content)), id);
		tmp = tmp->next;
	}
	return (NULL);
}

void 		print_margin(int level)
{
	int i = -1;
	while (++i < level)
		ft_printf("\t");
}

void 		print_type(t_ptree **tree)
{
	if (tree)
		ft_printf("type: %d ", (*tree)->type);
}

void 		print_id(t_ptree **tree)
{
	if (tree)
		ft_printf("id: %d ", (*tree)->id);
}

void 		print_ptree_id(int level, t_ptree **tree)
{
	t_list *tmp;

	if (!tree)
		return ;
	print_margin(level);
	print_id(tree);
	tmp = (*tree)->childs_lst;
	while (tmp)
	{
		ft_printf("\n");
		print_ptree_id(level + 1, ((t_ptree **)(tmp->content)));
		tmp = tmp->next;
	}
}

void 		print_ptree_type(int level, t_ptree **tree)
{
	t_list *tmp;

	if (!tree)
		return ;
	print_margin(level);
	print_type(tree);
	tmp = (*tree)->childs_lst;
	while (tmp)
	{
		ft_printf("\n");
		print_ptree_type(level + 1, ((t_ptree **)(tmp->content)));
		tmp = tmp->next;
	}
}

char		*get_str_type(int type)
{
	if (type == NUMBER_TYPE)
		return ("NUMBER");
	else if (type == OPEN_BRACKET_TYPE)
		return ("(");
	else if (type == CLOSE_BRACKET_TYPE)
		return (")");
	else if (type == PLUS_TYPE)
		return ("+");
	else if (type == MINUS_TYPE)
		return ("-");
	else if (type == MULTIPLY_TYPE)
		return ("x");
	else if (type == DIVISION_TYPE)
		return ("/");
	else if (type == NONE_TYPE)
		return ("none");
	else if (type == S_RULE)
		return ("S_rule");
	else if (type == S_PRIME_RULE)
		return ("S_prime_rule");
	else if (type == T_RULE)
		return ("T_rule");
	else if (type == T_PRIME_RULE)
		return ("T_prime_rule");
	else if (type == U_RULE)
		return ("U_rule");
	else if (type == V_RULE)
		return ("V_rule");
	else if (type == START_TYPE)
		return ("START");
	else if (type == END_TYPE)
		return ("END");
	return ("bad type");
}

void 		print_ptree(int level, t_ptree **tree)
{
	t_list *tmp;

	if (!tree)
		return ;
	print_margin(level);
	ft_printf("id: %d, type: %s data: %s", (*tree)->id, get_str_type((*tree)->type), (*tree)->data);
	// print_type(tree);
	tmp = (*tree)->childs_lst;
	while (tmp)
	{
		ft_printf("\n");
		print_ptree(level + 1, ((t_ptree **)(tmp->content)));
		tmp = tmp->next;
	}
}



t_tokens_list		*create_tokens_list_struct(t_list *lst)
{
	t_tokens_list	*result;

	result = ft_memalloc(sizeof(t_tokens_list));
	result->index = 0;
	result->size = ft_lstlen(lst);
	result->tokens_list = lst;
	return (result);
}

int		test_current_token(t_tokens_list **tokens_list_struct, int type, t_ptree **subtree)
{
	t_list	*good_token;

	good_token = ft_lsti((*tokens_list_struct)->tokens_list, (*tokens_list_struct)->index);
	if ((*tokens_list_struct)->index + 1 > (*tokens_list_struct)->size)
		return (FALSE);
	// ft_printf("test token : token[%s] index[%d] size[%d] type[%d] vs type entre[%d]\n", ((t_token *)(good_token->content))->token, (*tokens_list_struct)->index, (*tokens_list_struct)->size, ((t_token *)(good_token->content))->type, type);
	// ft_printf("dans test current id arbre[%d] type[%d]\n", (*subtree)->id, (*subtree)->type);
	if ((*((t_token **)(good_token->content)))->type == type)
	{
		ft_ptree_add_child(subtree, (*((t_token **)(good_token->content)))->token, type);
		// ft_printf("\nDans test current token, retour add child [%d]\n", ft_ptree_add_child(subtree, NULL, type));
		(*tokens_list_struct)->index++;
		// print_ptree_type(0, subtree);
		return (TRUE);
	}
	return (FALSE);
}

// POUR TEST

int		S_function(t_tokens_list **tokens_list_struct, t_ptree **tree);

int		U_function(t_tokens_list **tokens_list_struct, t_ptree **tree)
{
	int		id;
	int		save_index;
	t_ptree		**tmp;

	id = ft_ptree_add_child(tree, NULL, U_RULE);
	tmp = ft_ptree_get_node_with_id(tree, id);
	save_index = (*tokens_list_struct)->index;

	if (test_current_token(tokens_list_struct, OPEN_BRACKET_TYPE, tmp) && S_function(tokens_list_struct, tmp) && test_current_token(tokens_list_struct, CLOSE_BRACKET_TYPE, tmp))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;
	// Reset du noeud courant

	ft_ptree_remove_node_with_id(tree, id);
	// free_ptree_node(tmp);

	id = ft_ptree_add_child(tree, NULL, S_RULE);
	tmp = ft_ptree_get_node_with_id(tree, id);

	if (test_current_token(tokens_list_struct, NUMBER_TYPE, tmp))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;

	ft_ptree_remove_node_with_id(tree, id);
	// free_ptree_node(tmp);

	return (FALSE);
}

int		T_prime_function(t_tokens_list **tokens_list_struct, t_ptree **tree)
{
	int		save_index;

	save_index = (*tokens_list_struct)->index;
	if (test_current_token(tokens_list_struct, MULTIPLY_TYPE, tree) && U_function(tokens_list_struct, tree) && T_prime_function(tokens_list_struct, tree))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;
	if (test_current_token(tokens_list_struct, DIVISION_TYPE, tree) && U_function(tokens_list_struct, tree) && T_prime_function(tokens_list_struct, tree))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;

	return (TRUE);
}

int		T_function(t_tokens_list **tokens_list_struct, t_ptree **tree)
{
	int		id;
	int		save_index;
	t_ptree		**tmp;

	id = ft_ptree_add_child(tree, NULL, T_RULE);
	tmp = ft_ptree_get_node_with_id(tree, id);
	save_index = (*tokens_list_struct)->index;

	if (U_function(tokens_list_struct, tmp) && T_prime_function(tokens_list_struct, tmp))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;

	ft_ptree_remove_node_with_id(tree, id);
	// free_ptree_node(tmp);

	return (FALSE);
}

int		S_prime_function(t_tokens_list **tokens_list_struct, t_ptree **tree)
{
	int		save_index;

	save_index = (*tokens_list_struct)->index;
	if (test_current_token(tokens_list_struct, PLUS_TYPE, tree) && S_function(tokens_list_struct, tree) && S_prime_function(tokens_list_struct, tree))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;
	if (test_current_token(tokens_list_struct, MINUS_TYPE, tree) && S_function(tokens_list_struct, tree) && S_prime_function(tokens_list_struct, tree))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;
	return (TRUE);
}

int		S_function(t_tokens_list **tokens_list_struct, t_ptree **tree)
{
	int		id;
	int		save_index;
	t_ptree		**tmp;

	// Creation du nouveau noeud
	id = ft_ptree_add_child(tree, NULL, S_RULE);
	tmp = ft_ptree_get_node_with_id(tree, id);
	save_index = (*tokens_list_struct)->index;
	if (T_function(tokens_list_struct, tmp) && S_prime_function(tokens_list_struct, tmp))
	{
		return (TRUE);
	}
	(*tokens_list_struct)->index = save_index;

	ft_ptree_remove_node_with_id(tree, id);
	// free_ptree_node(tmp);

	return (FALSE);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//					FIN PARSE TREE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//							AST
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 		number_procedure(t_ptree **tree)
{
	t_ast_token		*token;

	token = ft_memalloc(sizeof(t_ast_token));
	token->type = (*tree)->type;
	token->data	= ft_strdup((*tree)->data);
	(*tree)->ast = ft_btree_create_node(token);
}

void 		operator_procedure(t_ptree **tree)
{
	(*tree)->ast = NULL;
}

void 		U_procedure(t_ptree **tree)
{
	// t_ast_token		*token;
	t_list			*tmp;

	tmp = (*tree)->childs_lst;
	if ((*tree)->nb_childs == 3)
	{
		// token = ft_memalloc(sizeof(t_ast_token));
		// token->type = (*((t_ptree **)(tmp->next->content)))->type;
		// (*tree)->ast = ft_btree_create_node(token);
		// (*tree)->ast->left = (*((t_ptree **)(tmp->content)))->ast;
		// (*tree)->ast->right = (*((t_ptree **)(tmp->next->next->content)))->ast;
		(*tree)->ast = (*((t_ptree **)(tmp->next->content)))->ast;
	}
	else
		(*tree)->ast = (*((t_ptree **)(tmp->content)))->ast;
}

void 		rules_procedure(t_ptree **tree)
{
	t_ast_token		*token;
	t_list			*tmp;

	tmp = (*tree)->childs_lst;
	if ((*tree)->nb_childs == 3)
	{
		token = ft_memalloc(sizeof(t_ast_token));
		token->type = (*((t_ptree **)(tmp->next->content)))->type;
		token->data = ft_strdup((*((t_ptree **)(tmp->next->content)))->data);
		(*tree)->ast = ft_btree_create_node(token);
		// (*tree)->ast = (*((t_ptree **)(tmp->next->content)))->ast;
		(*tree)->ast->left = (*((t_ptree **)(tmp->content)))->ast;
		(*tree)->ast->right = (*((t_ptree **)(tmp->next->next->content)))->ast;
	}
	else
		(*tree)->ast = (*((t_ptree **)(tmp->content)))->ast;
}

void 		execute_post_order_procedure(t_ptree **tree)
{
	if ((*tree)->type == NUMBER_TYPE)
		number_procedure(tree);
	else if ((*tree)->type == PLUS_TYPE || (*tree)->type == MINUS_TYPE || (*tree)->type == MULTIPLY_TYPE || (*tree)->type == DIVISION_TYPE)
		operator_procedure(tree);
	else if ((*tree)->type == U_RULE)
		U_procedure(tree);
	else if ((*tree)->type == S_RULE || (*tree)->type == T_RULE || (*tree)->type == START_TYPE)
		rules_procedure(tree);
}

void 	generate_ast(t_ptree **tree)
{
	t_list *tmp;

	if (!tree)
		return ;
	tmp = (*tree)->childs_lst;
	while (tmp)
	{
		generate_ast(((t_ptree **)(tmp->content)));
		tmp = tmp->next;
	}
	execute_post_order_procedure(tree);
}

t_btree	*get_ast(t_ptree **tree)
{
	generate_ast(tree);
	return ((*tree)->ast);
}

// void 		print_ast(int level, t_btree *tree)
// {
// 	if (!tree)
// 		return ;
// 	print_margin(level);
// 	ft_printf("type: %s ", get_str_type(((t_ast_token *)(tree->data))->type));
// 	if (tree->left)
// 	{
// 		ft_printf("\n");
// 		ft_printf("left : ");
// 		print_ast(level + 1, tree->left);
// 	}
// 	if (tree->right)
// 	{
// 		ft_printf("\n");
// 		ft_printf("right : ");
// 		print_ast(level + 1, tree->right);
// 	}
// }

void		p_ast(void *tree)
{
	ft_printf("type = %s, data = %s\n", get_str_type(((t_ast_token *)(((t_btree *)(tree))->data))->type), ((t_ast_token *)(((t_btree *)(tree))->data))->data);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//							FIN AST
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//							CALCUL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int		calc(t_btree *tree)
{
	t_ast_token *node;

	node = ((t_ast_token *)(tree->data));
	// if (node->type == NUMBER_TYPE)
	// 	return (ft_atoi(node->data));
	if (node->type == PLUS_TYPE)
		return (calc(tree->left) + calc(tree->right));
	else if (node->type == MINUS_TYPE)
		return (calc(tree->left) - calc(tree->right));
	else if (node->type == MULTIPLY_TYPE)
		return (calc(tree->left) * calc(tree->right));
	else if (node->type == DIVISION_TYPE)
		return (calc(tree->left) / calc(tree->right));
	return (ft_atoi(node->data));
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//							FIN CALCUL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(int ac, char **av)
{
	t_list	*tokens_list;
	t_list	*tmp;
	t_tokens_list *tokens_list_struct;
	// t_ptree	*tree;

	// tree = NULL;
	tokens_list = NULL;
	if (ac != 2)
		return (0);
	lexer(av[1], &tokens_list);
	// DEBUG AFTER LEXER
	tmp = tokens_list;
	while (tmp)
	{
		ft_printf("token = %s, type = %s\n", (*((t_token **)(tmp->content)))->token, get_str_type((*((t_token **)(tmp->content)))->type));
		tmp = tmp->next;
	}
	//
	tokens_list_struct = create_tokens_list_struct(tokens_list);
	// ft_printf("\n\n");
	// ft_ptree_create_node(&tree, NULL, 1);
	// ft_ptree_add_child(&tree, NULL, 2);
	// ft_ptree_add_child(&tree, NULL, 3);
	// ft_ptree_add_child(&tree, NULL, 4);
	// ft_printf("test: %d\n", (*((t_ptree **)(tree->childs_lst->next->content)))->id);
	// ft_ptree_add_child(((t_ptree **)(tree->childs_lst->next->content)), NULL, 6);
	// ft_ptree_add_child_with_id(&tree, 2, NULL, 7);
	// ft_ptree_add_child_with_id(&tree, 4, NULL, 7);
	// // ft_ptree_add_child(((t_ptree **)(&tree->childs_lst->content)), ft_ptree_create_node(NULL, 7));
	// // ft_ptree_add_child(((t_ptree **)(&tree->childs_lst->content)), ft_ptree_create_node(NULL, 8));
	// print_ptree_id(0, &tree);
	// // ft_ptree_remove_node_with_id(&tree, 1);
	// // ft_printf("\nAPRES remove node\n");
	// ft_printf("\n TEST \n");
	// t_ptree **new_tree = ft_ptree_get_node_with_id(&tree, 2);
	// ft_printf("\n APRES TEST \n");
	// print_ptree_id(0, new_tree);
	//
	ft_printf("\n\nTEST DE S\n\n");

	t_ptree		*tree_s = NULL;

	ft_ptree_create_node(&tree_s, NULL, START_TYPE);
	S_function(&tokens_list_struct, &tree_s);
	ft_printf("apres s\n");
	if (tree_s)
		print_ptree(0, &tree_s);

	// AST

	t_btree *ast = get_ast(&tree_s);

	// Free lexer
	ft_lstdel(&(tokens_list_struct->tokens_list), free_token);
	ft_memdel((void **)&tokens_list_struct);

	// Free parser
	free_ptree_node(&tree_s);
	//
	// Free lexer
	// ft_lstdel(&(tokens_list_struct->tokens_list), free_token);
	// ft_memdel((void **)&tokens_list_struct);
	//
	ft_printf("\n PRINT AST : \n");
	ft_btree_apply_prefix(ast, p_ast);
	ft_printf("RESULTAT = %d\n", calc(ast));

	// Free lexer
	// ft_lstdel(&(tokens_list_struct->tokens_list), free_token);
	// // Free parser
	// ft_memdel((void **)&tokens_list_struct);
	// free_ptree_node(&tree_s);
	// Free ast
	ft_btree_del(&ast, free_ast_token);
	return (0);
}
