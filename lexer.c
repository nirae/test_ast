#include "calculator.h"

/*
**	Retourne le type du token
*/

static int			get_type_of_token(char *token)
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

/*
**	Initialise une structure representant le token
*/

static void 	create_token_struct(char *token, t_token **tok)
{
	*tok = ft_memalloc(sizeof(t_token));
	(*tok)->token = ft_strdup(token);
	(*tok)->type = get_type_of_token(token);
}

/*
**	Retourne l'index de la colonne du tableau automate correspondant au caractere
*/

static int get_index_from_char(char *s, int i)
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

/*
**	Push un caractere dans la stack
*/

static void put_char_in_stack(char (*stack)[512], char c)
{
	if (ft_strlen(*stack) >= 510)
	{
		ft_printf("Stack depassee!");
		exit(0);
	}
	(*stack)[ft_strlen(*stack)] = c;
}

/*
**	Detecte si l'etat est accepteur ou pas
*/

static int	is_acceptor(int state)
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

/*
**	Detecte si l'etat est etoile ou pas
*/

static int is_star(int state)
{
	if (state == STAR_CLOSE_BRACKET_STATE || state == STAR_PLUS_STATE
		|| state == STAR_MINUS_STATE || state == STAR_MULTIPLY_STATE
		|| state == STAR_DIVISION_STATE || state == STAR_NUMBER_AOB_STATE)
		return (TRUE);
	return (FALSE);
}

/*
**	Free un token
*/

static void 	free_token(void *content, size_t size)
{
	(void)size;
	ft_strdel(&((*((t_token **)(content)))->token));
	ft_memdel((void **)((t_token **)(content)));
	ft_memdel(&content);
}

/*
**	Delete proprement la liste des tokens
*/

void 	delete_list_tokens(t_list **tokens_list)
{
	ft_lstdel(tokens_list, free_token);
}

/*
**	The main function of the Lexer
*/

void lexer(char *line, t_list **tokens_list)
{
	//	Declaration de la stack
	char stack[512];
	// Tableau representant l'automate
	static int A[4][9] = {
		{NUMBER_STATE, OPEN_BRACKET_STATE, CLOSE_BRACKET_STATE, PLUS_STATE, MINUS_STATE, MULTIPLY_STATE, DIVISION_STATE, NONE_STATE},
		{NUMBER_STATE, NONE_STATE, STAR_CLOSE_BRACKET_STATE, STAR_PLUS_STATE, STAR_MINUS_STATE, STAR_MULTIPLY_STATE, STAR_DIVISION_STATE, NONE_STATE},
		{STAR_NUMBER_AOB_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE, NONE_STATE}
	};
	// Etat courant
	int current_state;
	// Etat suivant donne par l'automate
	int next_state;
	// Maillon de liste pour creer les tokens
	t_list	*token;
	// Maillon correspondant a un token
	t_token	*token_struct;
	int i;

	// On selectionne l'etat de depart
	current_state = START_STATE;
	// Clean la stack
	ft_bzero((void *)&stack, 512);
	// Boucle sur chaque charactere de la string
	i = -1;
	while (line[++i])
	{
		// Determine le prochain etat grace a l'automate
		next_state = A[current_state][get_index_from_char(line, i)];
		// Push dans la stack
		put_char_in_stack(&stack, line[i]);
		// Si c'est un etat d'erreur -> Syntax error
		if (next_state == NONE_STATE)
		{
			ft_printf("Syntax error near character number %d\n", i + 1);
			exit(0);
		}
		// Si c'est un etat accepteur
		else if (is_acceptor(next_state))
		{
			// Si c'est un etat etoile
			if (is_star(next_state))
			{
				// Retire le dernier caractere de la stack
				stack[ft_strlen(stack) - 1] = 0;
				// Recule d'un caractere dans la string
				i--;
			}
			// Creer un token avec la stack
			create_token_struct(stack, &token_struct);
			// Creer un maillon de liste avec ce token
			token = ft_lstnew(&token_struct, sizeof(t_token));
			// Ajoute a la liste des tokens
			if ((*tokens_list) == NULL)
				(*tokens_list) = token;
			else
				ft_lstaddend(tokens_list, token);
			// Nettoie la stack
			ft_strclr(stack);
			// On retourne sur l'etat de depart
			current_state = START_STATE;
			// Reprends la boucle au debut
			continue ;
		} // Fin de l'etat accepteur on a eu un token et on continue
		// L'etat courant devient le suivant
		current_state = next_state;
	} // Fin de la boucle
	// Si le dernier etat etait un nombre on doit prendre le dernier token
	if (current_state == NUMBER_STATE)
	{
		// Creer un token avec la stack
		create_token_struct(stack, &token_struct);
		// Creer un maillon de liste avec ce token
		token = ft_lstnew(&token_struct, sizeof(t_token));
		// Ajoute a la liste des tokens
		if ((*tokens_list) == NULL)
			(*tokens_list) = token;
		else
			ft_lstaddend(tokens_list, token);
	}
	// create_token_struct(NULL, &token_struct);
	// token = ft_lstnew(&token_struct, sizeof(t_token));
	// ft_lstaddend(tokens_list, token);
}
