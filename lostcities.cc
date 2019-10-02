#include <iostream> //std::cout
#include <vector> //std::vector
#include <ctime> //std::time
#include <algorithm> //std::random_shuffle
using namespace std;

#define NUMSHUFFLES 7
#define SUITCOUNT 5
#define SUITLIST "Red", "Green", "Blue", "White", "Yellow", "Purple"

struct cardObj
{
	int cardValue;
	string cardColor;
};

typedef std::vector<cardObj*> deckObj;
typedef std::vector<deckObj*> playSpace;



//converts color of card to index in color array
int colorToIndex(cardObj* colorCard)
{
	const char* suitList[] = {SUITLIST};
	for(int i = 0; i < SUITCOUNT; i++)
	{
		if(colorCard->cardColor == suitList[i])
			return i;
	}
}

//returns whether or not B is higher value than A
bool Compare_By_Value(cardObj* cardA, cardObj* cardB)
	{return cardA->cardValue < cardB->cardValue;}

//returns whether or not B is higher color than A
bool Compare_By_Color(cardObj* cardA, cardObj* cardB)
	{return colorToIndex(cardA) < colorToIndex(cardB);}

//returns whether a card in a playspace is playable
bool isPlayable(cardObj* playCard, playSpace* playAttempt)
{
	int ind = colorToIndex(playCard);
	deckObj* obsDeck = (*playAttempt)[ind];
	if((obsDeck->size() == 0) || (obsDeck->back()->cardValue <= playCard->cardValue))
		return true;
	else
		return false;
}

//sorts a deck by color and value
deckObj* Sort_Deck(deckObj* sortyDeck)
{
	std::sort(sortyDeck->begin(), sortyDeck->end(), Compare_By_Value);
	std::sort(sortyDeck->begin(), sortyDeck->end(), Compare_By_Color);
	return sortyDeck;
}

//shuffles a deck object
void shuffleDeck(deckObj* shufflyDeck)
{
	for(int i = 0; i < NUMSHUFFLES; i++)
		random_shuffle(shufflyDeck->begin(), shufflyDeck->end());
}

//scores a given pile
int score_pile(deckObj* currPile)
{
	if(currPile->size() == 0)
		return 0;
	int currScore = -20;
	int currMult = 1;
	for(int i = 0; i < currPile->size(); i++)
	{
		if((*currPile)[i]->cardValue == 0)
			currMult += 1;
		else
			currScore += (*currPile)[i]->cardValue;
	}
	currScore *= currMult;
	return currScore;
}

//scores a given player's playSpace
int score_all_piles(playSpace* myPlayspace)
{
	int totalScore = 0;
	for(int i = 0; i < myPlayspace->size()-1; i++)
		{totalScore += score_pile((*myPlayspace)[i]);}
	return totalScore;
}

//deals card from given deck to given player hand
//takes pointer to a deckObj and a pointer to a deck obj
void dealCard(deckObj* dealTo, deckObj* dealFrom)
{
	dealTo->push_back(dealFrom->back());
	dealFrom->pop_back();
}

//takes 2 pointers to a deckObj and a pointer to a deck obj
void setupHands(deckObj* playerOneHand, deckObj* playerTwoHand, deckObj* drawPile)
{
	for(int i = 0; i < 8; i++)
	{
		dealCard(playerOneHand, drawPile);
		dealCard(playerTwoHand, drawPile);
	}
}

//builds and returns the main deck object
deckObj* buildDeck()
{
	const char* suitList[] = {SUITLIST};
	string currColor;
	deckObj* mainDeck = new deckObj;
	cardObj* newCard;

	for(int s = 0; s < SUITCOUNT; s++)
	{
		currColor = suitList[s];
		for(int i = -1; i <= 10; i++)
		{
			newCard = new cardObj;
			newCard->cardColor = currColor;
			if(i < 2)
				newCard->cardValue = 0;
			else
				newCard->cardValue = i;

			mainDeck->push_back(newCard);
		}
	}
	return mainDeck;
}

//print the top of each card in the three playspaces
void printPlaySpaceTops(playSpace* playerSpace, playSpace* boardSpace, playSpace* otherPlayerSpace)
{
	const char* suitList[] = {SUITLIST};
	cout<<endl;
	cout << "\t| YOUR PLAYSPACE: | DISCARD PILES: | THEIR PLAYSPACE: |" << endl;
	for(int i = 0; i < SUITCOUNT; i++)
	{
		cout << " " << suitList[i] <<"\t| ";
		if((*playerSpace)[i]->size() > 0)
		{
			cout << (*playerSpace)[i]->back()->cardValue << "\t";
			cout << score_pile((*playerSpace)[i])<< "\t  | ";
		}
		else
			{cout << "NIL\t\t  | ";}

		if((*boardSpace)[i]->size() > 0)
			cout << (*boardSpace)[i]->back()->cardValue << "\t\t   | ";
		else
			cout << "NIL\t\t   | ";

		if((*otherPlayerSpace)[i]->size() > 0)
		{
			cout << (*otherPlayerSpace)[i]->back()->cardValue << "\t";
			cout << score_pile((*otherPlayerSpace)[i])<< "\t      |" << endl;
		}
		else
			{cout << "NIL\t      | " << endl;}
	}
	cout << endl;
}

//print a deck and whether or not its cards are playable
void printDeck(deckObj* printyDeck, playSpace* deckOrigin)
{
	cout << " YOUR HAND:\t|  INDEX:  |  PLAYABLE?:" << endl;
	for(int i = 0; i < printyDeck->size(); i++)
	{
		cout << " " << (*printyDeck)[i]->cardColor << "\t ";
		cout << (*printyDeck)[i]->cardValue << "\t|  ";
		cout << i+1 <<"\t   |  ";
		if((deckOrigin != NULL) && (isPlayable((*printyDeck)[i], deckOrigin)))
			cout << "Y";
		else
			cout << "N";
		cout << endl;
	}
	cout << endl;
}

//prints the suits and their indices
void printSuits()
{
	const char* suitList[] = {SUITLIST};
	
	cout << "SUIT:\t|\tINDEX:" << endl;
	for (int i = 0; i < SUITCOUNT; i++)
		{cout << suitList[i] << "\t|\t" << i+1 << endl;}
}

//take the player's turn
void takeTurn(playSpace* currPlayer, playSpace* boardState, playSpace* otherPlayer)
{
	int playInd = -1;
	char playLoc;
	bool firstTry = true, drawnDiscard = false;
	deckObj* playerHand = (*currPlayer)[SUITCOUNT];
	deckObj* drawDeck;
	cardObj* cardToPlay;

	printPlaySpaceTops(currPlayer, boardState, otherPlayer); //print playspace...
	printDeck(playerHand, currPlayer); //... and your hand

	while(!((playInd > -1) && (playInd < 8))) //get input -- what card will you play?
	{
		if(firstTry == false)
			cout << "INVALID INPUT." << endl;
		cout << "INPUT AN INDEX TO SELECT A CARD FOR PLAY." << endl;
		cin >> playInd;
		playInd--;
		firstTry = false;
	}
	cardToPlay = (*playerHand)[playInd];

	//cout << (isPlayable(cardToPlay, currPlayer)) << endl;
	if ((isPlayable(cardToPlay, currPlayer))) //get input -- where will you play it to?
	{
		firstTry = true;
		while(!((playLoc == 'p') || (playLoc == 'd')))
		{
			if(firstTry == false)
				cout << "INVALID INPUT." << endl;
			cout << "PLAY TO YOUR PILES OR DISCARD PILES?\t(p / d)" << endl;
			cin >> playLoc;
			tolower(playLoc);
			firstTry = false;
		}
	}
	else
		{playLoc = 'd';}

	int chosenInd = colorToIndex(cardToPlay); //get the color index of chosen card
	if(playLoc == 'p')
		{(*currPlayer)[chosenInd]->push_back(cardToPlay);} //play it if play was chosen 
	else
		{(*boardState)[chosenInd]->push_back(cardToPlay);} //else discard it

	playerHand->erase(playerHand->begin()+playInd); //then remove iit from player hand

	printPlaySpaceTops(currPlayer, boardState, otherPlayer); //print playspace...

	char oldPlayLoc = playLoc;	
	firstTry = true; drawnDiscard = false; //update our variables
	while((playLoc != 'm') && (drawnDiscard != true)) // get input -- where will you draw from?
	{
		if(firstTry == false)
			cout << "INVALID INPUT." << endl;
		cout << "WOULD YOU LIKE TO DRAW FROM THE MAIN DRAW PILE OR A DISCARD PILE?\t(m / d)" << endl;
		cin >> playLoc;
		tolower(playLoc);

		if(playLoc != 'm') //if we chose to draw from discard, make sure that's okay
		{
			printSuits();
			cout << "CHOOSE AN INDEX TO DRAW FROM." << endl;
			cin >> playInd;
			playInd--;
			if ( (!( (playInd == chosenInd) && (oldPlayLoc == 'd') )) && ( (*boardState)[playInd]->size() > 0) )
				{drawnDiscard = true;}
		}
		firstTry = false;
	}
	
	if(playLoc == 'm')
		drawDeck = (*boardState)[SUITCOUNT]; //draw from draw if chosen
	else
		drawDeck = (*boardState)[playInd]; //else, draw from chosen discard
	dealCard(playerHand, drawDeck);

	//update player information
	playerHand = Sort_Deck(playerHand);
	printPlaySpaceTops(currPlayer, boardState, otherPlayer); //print playspace...
	printDeck(playerHand, currPlayer);
	cout << "PRESS ENTER TO END PLAYER TURN..." << endl;
	cin.get();
	cin.get();
	cout << endl << endl << endl << endl << endl << endl << endl;
}

//play the game
void playGame(playSpace* playerOne, playSpace* playerTwo, playSpace* boardState)
{
	const char* suitList[] = {SUITLIST};
	deckObj* mainDeck = (*boardState)[SUITCOUNT];

	int currPlayer = 1;
	while(mainDeck->size() > 0)
	{
		if (currPlayer > 0)
			takeTurn(playerOne, boardState, playerTwo);
		else
			takeTurn(playerTwo, boardState, playerOne);
		currPlayer *= -1;
	}
}

//free up some memory
void deletePlayspaceObjects(playSpace* delSpace)
{
	for (int i = 0; i < delSpace->size(); i++)
		{delete ((*delSpace)[i]);}
}

int main()
{
	srand(unsigned (time(0)));
	deckObj* mainDeck = buildDeck();
	deckObj* playerOneHand = new deckObj;
	deckObj* playerTwoHand = new deckObj;

	shuffleDeck(mainDeck);
	setupHands(playerOneHand, playerTwoHand, mainDeck);
	playerOneHand = Sort_Deck(playerOneHand);
	playerTwoHand = Sort_Deck(playerTwoHand);

	playSpace* playerOne = new playSpace;
	playSpace* playerTwo = new playSpace;
	playSpace* boardState = new playSpace;

	for(int i = 0; i < SUITCOUNT; i++)
	{
		playerOne->push_back(new deckObj);
		playerTwo->push_back(new deckObj);
		boardState->push_back(new deckObj);
	}

	playerOne->push_back(playerOneHand);
	playerTwo->push_back(playerTwoHand);
	boardState->push_back(mainDeck);

	playGame(playerOne, playerTwo, boardState);

	cout << score_all_piles(playerOne) << endl;
	cout << score_all_piles(playerTwo) << endl;

	deletePlayspaceObjects(playerOne);
	deletePlayspaceObjects(playerTwo);
	deletePlayspaceObjects(boardState);

	return 0;
}
