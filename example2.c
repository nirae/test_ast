//Structure contenant la liste de tokens et un index qui pointe sur le token en cours d’analyse par le parser.
ListToken
{
	//Liste de tokens.
	List<Token> tokens;
	//index dans la liste de tokens.
	int indexInList;
};

//La fonction testCurrentToken permet de tester si le token pointé correspond bien à celui attendu.
bool testCurrentToken(ListToken l, int type, Tree subtree)
{
	//Si l’index sur lequel on est est en dehors de la liste. (On ajoute 1 à l’index, car quand on est sur l’élément 0, c’est comme si nous avions une liste de 1 élément, si la taille de la liste est inférieur à indexInList+1, nous savons que nous débordons de la liste.)
	if (l.indexInList + 1 > l.indexInList)
	{
		return false;
	}
	//On teste si le type du token courant est bien celui attendu.
	if (l.tokens.get(l.indexInList) == type)
	{
		//C’est le cas, donc on peut ajouter un noeud à l’arbre qui représente le token.
		subtree.addChild(l.tokens.get(l.indexInList));
		//Le token courant a été traité donc on peut avancer d’un cran dans la liste de token.
		l.indexInList++;
		//On retourne true car tout s’est bien passé.
		return true;
	}
	//Si le type du token courant n’est pas celui attendu, alors on retourne simplement false.
	return false;
}

//On a notre liste de tokens.
ListToken l;
//Au moment ou l’algorithme va commencer on initialise l’index de token à 0 car le parseur commence par le premier token.
l.indexInList = 0;

//Fonction S, c’est la première fonction appellée par notre parser. Elle prend en paramètre un arbre (tree). Cet arbre sera l’arbre syntaxique à la fin de l’exécution du parser.
S(tree)
{
	//Lorsqu’on rentre dans une règle, on ajoute un noeud représentant la règle de grammaire utilisée.
	Node subtree = tree.addChild(createNode(RULE_S));
	//On sauvegarde l’index de token pour pouvoir revenir en arrière si la règle de grammaire ne convient pas et qu’on a consumé quelques tokens pour rien.
	int saveIndex = l.indexInList;
	//On test si la transformation que la règle de grammaire permet de faire est possible, si c’est le cas, on renvoie true, car la règle a pu s’appliquer.
	if (testCurrentToken(l, OPEN_BRACKET, subtree) && S(subtree) && testCurrentToken(l, CLOSE_BRACKET, subtree))
	{
		return true;
	}
	//Si on en est là, c’est qu’on est pas rentré dans le if précédent, et donc que la dérivation n’est pas possible. On revient donc en arrière en rétablissant l’index sur le token du début de la fonction.
	indexInList = saveIndex;
	//On teste si la seconde transformation est possible, si c’est le cas on renvoie true.
	if (testCurrentToken(l, EPSILON, subtree))
	{
		return true;
	}
	//si ce n’est pas le cas on rétablit l’index.
	l.indexInList = saveIndex;
	//On supprime le noeud qui représentait la règle de grammaire afin de n’en avoir aucune trace.
	tree.removeNode(subTree);
	//On renvoie false, car la règle de grammaire n’a pas pu être appliquée.
	return false;
}
