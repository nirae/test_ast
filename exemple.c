//On commence sur l’état de départ
currentState = start;
while ((c = Flux.readNextChar()) != EOF)
{
	//On sauvegarde le caractère lut dans une pile.
	putCharInStack(c);
	//On récupère l’état de transition depuis l’application A.
	nextState = A[CurrentState][c];
	if (notDefined(nextState))
	{
		//Si l’état de transition n’existe pas, on gère l’erreur.
		printf(« Erreur ! »);
		exit(0);
	}
	if (isAcceptor(nextState))
	{
		//Si l’état est accepteur, alors on doit générer un nouveau token grâce aux caractères sur la pile.
		tok = createToken(state);
		//On vide la pile.
		flushStack();
		//On sauvegarde le token dans une liste.
		addTokenToList(tok);
		//On passe à l’état de départ car on vient de voir que la transition nous fait passer dans un état accepteur.
		currentState = start;
	}
	//Si la transition est bien définie, on la prend.
	currentState = nextState;
}

currentState = start;
while ((c = Flux.readNextChar()) != EOF) {
	//On sauvegarde le caractère lut dans une pile.
	putCharInStack(c);
	//On récupère l’état de transition depuis l’application A.
	nextState = A[CurrentState][c];
	if (notDefined(nextState)) {
		//Si l’état de transition n’existe pas, on gère l’erreur.
		printf(« Erreur ! »);
		exit(0);
	}
	if (isAcceptor(nextState)) {
		if (isStar(nextState)) {
			//Si l’état accepteur est étoilé, alors on revient en arrière d’un caractère dans le flux et on retire le dernier caractère de la pile..
			Flux.rewindOneChar();
			popCharFromStack();
		}
		//Si l’état est accepteur, alors on doit générer un nouveau token grâce aux caractères sur la pile.
		tok = createToken(state);
		//On vide la pile.
		flushStack();
		//On sauvegarde le token dans une liste.
		addTokenToList(tok);
		//On passe à l’état de départ car on vient de voir que la transition nous fait passer dans un état accepteur.
		currentState = start;
	}
	//Si la transition est bien définie, on la prend.
	currentState = nextState;
}
