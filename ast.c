#include "calculator.h"

/*
**	Procedure pour un nombre
*/

static void 		number_procedure(t_ptree **tree)
{
	t_ast_token		*token;

	token = ft_memalloc(sizeof(t_ast_token));
	token->type = (*tree)->type;
	token->data	= ft_strdup((*tree)->data);
	(*tree)->ast = ft_btree_create_node(token);
}

/*
**	Procedure pour un operateur
*/

static void 		operator_procedure(t_ptree **tree)
{
	(*tree)->ast = NULL;
}

/*
**	Procedure pour un noeud de la regle U
*/

static void 		U_procedure(t_ptree **tree)
{
	t_list			*tmp;

	tmp = (*tree)->childs_lst;
	if ((*tree)->nb_childs == 3)
		(*tree)->ast = (*((t_ptree **)(tmp->next->content)))->ast;
	else
		(*tree)->ast = (*((t_ptree **)(tmp->content)))->ast;
}

/*
**	Procedure pour un noeud regle sauf U
*/

static void 		rules_procedure(t_ptree **tree)
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
		(*tree)->ast->left = (*((t_ptree **)(tmp->content)))->ast;
		(*tree)->ast->right = (*((t_ptree **)(tmp->next->next->content)))->ast;
	}
	else
		(*tree)->ast = (*((t_ptree **)(tmp->content)))->ast;
}

/*
**	Execute la bonne procedure en fonction du type du noeud
*/

static void 		execute_post_order_procedure(t_ptree **tree)
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

/*
**	Genere l'AST grace a l'arbre de syntaxe
*/

static void 	generate_ast(t_ptree **tree)
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

static void 	free_ast_token(void *data)
{
	ft_strdel(&((t_ast_token *)(data))->data);
	ft_memdel(&data);
}

void 		delete_ast(t_btree *ast)
{
	ft_btree_del(&ast, free_ast_token);
}

/*
**	Construit et retourne l'AST
*/

t_btree		*get_ast(t_ptree **parsing_tree)
{
	generate_ast(parsing_tree);
	return ((*parsing_tree)->ast);
}
