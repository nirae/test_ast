#include "calculator.h"

int		calc(t_btree *tree)
{
	t_ast_token *node;

	node = ((t_ast_token *)(tree->data));
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

int main(int ac, char **av)
{
	t_list		*tokens_list;
	t_ptree		*tree_s;
	t_btree		*ast;
	int			result;

	tokens_list = NULL;
	tree_s = NULL;
	ast = NULL;
	if (ac != 2)
		return (0);
	// Lexer sur premier argument
	lexer(av[1], &tokens_list);
	// Print les tokens
	// ft_print_tokens(tokens_list);
	// Premier parsing syntaxique
	parsing_tree(&tokens_list, &tree_s);
	// Print l'arbre de parsing
	//ft_print_parsing_tree(0, &tree_s);
	// AST
	ast = get_ast(&tree_s);
	// Print l"AST
	//ft_print_ast(ast);

	// Delete la liste des tokens
	delete_list_tokens(&tokens_list);
	// Delete le premier arbre syntaxique
	delete_parsing_tree(&tree_s);

	// Calcul
	result = calc(ast);

	// Delete AST
	delete_ast(ast);

	ft_printf("%d\n", result);
	return (0);
}
