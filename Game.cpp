/***************************************************************
 ** File:    Game.cpp
 ** Project: CMSC 202 Project 1, Spring 2017
 ** Author:  Johnlemuel Casilag
 ** Date:    3/9/17
 ** Section: 10
 ** E-mail:  cas6@umbc.edu
 **
 ** This file implements the Game class, which essentially
 ** runs the program using the Card and Player classes.
 ** This runs the game "Exploding Kittens".
 **
 ***************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "Player.h"
#include "Game.h"

using namespace std;

// ---------------- Constructors ----------------


/*   Name - Game()
     Desc - Initializes game
     Preconditions - valid constants that contain txt files for the deck and bombs
     Postconditions - runs game
 */ 
Game::Game() {
  cout << "Welcome to Exploding Kittens!" << endl
       << "A Game By Elan Lee, Shane Small, and Matthew Inman" << endl << endl
       << "How many players would you like?" << endl
       << "Choose 2 - 4 players: " << endl;
  int numPlayers; // for cin to find number of players
  cin >> numPlayers;
  SetNumPlayers(numPlayers); // function call to set number of players
  cout << "Loading deck..." << endl;
  LoadDeck(DECK_NAME); // function call to open deck file

  m_deck.erase(m_deck.begin()); // erase of first element due to white space error

  cout << "Shuffling the cards..." << endl;
  Shuffle(); // function call to shuffle deck

  cout << "Dealing cards..." << endl;
  for (int i = 0; i < m_numPlayers; i++) {
    for (int j = 0; j < INIT_CARDS; j++) {
      m_players[i].AddToHand(DrawCard()); // deals cards while creating new players at the same time
    }
  }

  cout << "Shuffling in the exploding kittens!" << endl;
  AddBombCards(BOMB_NAME); // adds "bombs" to deck then shuffles
  Shuffle();
  PlayGame(); // main function that runs the game
}


/*   Name - PlayGame()
     Desc - Runs the entire game
     Preconditions - assumes game is initialized
     Postconditions - announces winner
 */
void Game::PlayGame() {
  int ifWon = 0; // checks to see if game is won, initialized to 0
  int turnCount = 0; // turn counter to check which player's turn it is
  int userInput; // variable for various user inputs, typically for the beginning menu
  int playerCond; // sets player condition, whether player has a bomb, diffuse, etc.
  int tempNum; // a temporary number for storage
  bool turnAgain = false; // checks to see if a player can go again if attacked
  int maxNumPlayers = m_numPlayers; // initial number of players because m_numPlayers changes
  Card lastCard; // Card class of the last card
  int lastCardType; // type of the last card
  Player *currPlayer; // pointer to the current player
  Player *nextPlayer; // pointer to the next player after a turn is over
  while (ifWon == 0) { // while loop allows game to repeat until game is won
    currPlayer = &m_players[turnCount];
    if (currPlayer->HasExtraTurn()) { // checks if previous turn was an attack, if so, player will go twice this time
      turnAgain = true;
    }
    currPlayer->SetExtraTurn(false); // reinitializes current player's extra turn condition
    
    for (tempNum = 0; tempNum < m_numPlayers; tempNum++) {
      tempNum %= m_numPlayers;
      if (!m_players[tempNum].HasLost()) { // assignment for the nextPlayer pointer, takes
	                                   // into consideration if next player lost
	break;
      }
    }
    tempNum++;
    nextPlayer = &m_players[turnCount + tempNum]; // completed assignment for pointer


    if (currPlayer->HasLost()) { // if player was attacked, turn count increases
      turnCount++;
      if (turnCount >= maxNumPlayers) turnCount = 0;
    }

    if (!currPlayer->HasLost()) { // first checks if player has lost. if so, cannot play
      userInput = 0;
      cout << "Num cards remaining: " << m_deck.size() << endl << endl;
      while (userInput != 3) {
	if (currPlayer->HasCards()) { // checks if player has cards. if player has no cards, they only have one option
	  cout << "It is Player " << currPlayer->GetName() << "'s turn" << endl
	       << "What would you like to do:" << endl
	       << "1. View Cards" << endl
	       << "2. Play A Card" << endl
	       << "3. Draw A Card" << endl;
	  cin >> userInput;
	  switch (userInput) { // switch statement for first menu
	  case 1:
	    cout << endl;
	    currPlayer->DisplayHand(); // displays current player's hand
	    cout << endl;
	    break;
	    
	  case 2:
	    cout << endl;
	    lastCard = currPlayer->PlayCard(); // displays current player's hand and prompts to play a card

	    lastCardType = lastCard.GetType();
	    
	    m_discard.push_back(lastCard); // places card played into the discard pile
	    userInput = HandleCard(lastCardType); // determines how the game should react based on card type played
	    
	    if (userInput == 2) { // 2 = an attack
	      nextPlayer->SetExtraTurn(true);
	      userInput = 3;
	    }

	    if ((int) m_discard.size() > 1) {
	      Card lastCard = m_discard.back(); // assigns last card in discard pile to lastCard
	      m_discard.pop_back(); // removes last element so we can access the second to last card
	      Card secLastCard = m_discard.back();

	      if (lastCard.GetType() == secLastCard.GetType()) { // checks to see if last/seclastcard type are equivalent
		cout << "You played a pair! "
		     << "You can now steal a card from another player!" << endl
		     << "Select a Player: " << endl;

		int stealPlayer; // int value of which user wants to steal from
		for (int i = 0; i < m_numPlayers; i++) {
		  if (!(m_players[i].GetName() == currPlayer->GetName())) { // condition is made so that 
		                                                            //  player cant steal from themself
		    cout << "Player " << m_players[i].GetName() << endl;
		  }
		}
		cin >> stealPlayer;

		Card stealCard; // actual card user stole
		int whatCard; // index of stealPlayer's card the player will steal from their hand
		for (int j = 0; j < m_players[stealPlayer - 1].GetNumberOfCards(); j++) { // displays each
		  cout << "Card " << j << endl;
		}
		cin >> whatCard;
		stealCard = m_players[stealPlayer - 1].LoseCard(whatCard);
		currPlayer->AddToHand(stealCard); // adds stolen card to player's hand
		
		userInput = 3; // ends turn
	      }
	    }

	    cout << endl;
	    break;
	    
	  case 3:
	    playerCond = currPlayer->AddToHand(DrawCard()); // draws card from the deck and checks if it
	                                                    // is a bomb & if player has a diffuse card
	    if (playerCond == 2) { // condition: bomb was drawn, but player has diffuse card 
	      cout << "But you have a defuse card! Play it or lose!" << endl;
	      lastCard = currPlayer->PlayCard(); // prompts user to play a card
	      if (lastCard.GetType() == 1) { // if card is a diffuse card, player still in game
		cout << "You diffused the the Exploding Kitten!" << endl
		     << "Placing Exploding Kitten back into deck and shuffling..." << endl;
		m_deck.push_back(lastCard);
		Shuffle(); // bomb card is placed back into deck and shuffled
	      }
	      else {
		playerCond = 0; // if player chooses anything other than diffuse, they lose
	      }
	    }
	    if (playerCond == 0) { // player draws bomb without a diffuse card
	      currPlayer->SetLost(true);
	      cout << "Sorry! You're out of the game!" << endl;
	      m_numPlayers--;
	    }
	    break;

	  }
	  playerCond = 1;
	}
	else { // if player has no cards left, they are prompted to draw a card
	  while (userInput != 3) {
	    cout << "You are out of cards! Enter 3 to draw a card." << endl;
	    cin >> userInput;
	  }
	  currPlayer->AddToHand(DrawCard());
	}
      }

      if (!turnAgain) { // turnCounter is only increased if no one was attacked
	turnCount++;
	if (turnCount >= maxNumPlayers) turnCount = 0;
      }
      turnAgain = false;
    }
    if (m_numPlayers == 1) { // once only one player is left, they win
      currPlayer = &m_players[turnCount];
      cout << "The winner is Player " << currPlayer->GetName() << "!" << endl;
      break;
    }
  }
}

/*   Name - HandleCard()
     Desc - determines what will happen after a card is played
     Preconditions - takes the type of card played
     Postconditions - returns a number for further action
*/
int Game::HandleCard(int type) {
  
  switch (type) { // switch statement used to determine the outcome
  case 1:
    cout << "Explosive kitty difused!" << endl;
    break;
  case 2:
    cout << "You attacked the next player!" << endl;
    return 2;
    break;
  case 3:
    cout << "Skipped your turn!" << endl;
    return 3;
    break;
  case 4:
    cout << "You looked into the future!!" << endl;
    Peek();
    break;
  case 5:
    cout << "You shuffled the deck!" << endl;
    Shuffle();
    break;
  }
  return 0;
}

/*   Name - Peek()
     Desc - shows the first three cards in m_deck
     Preconditions - player played a peek card
     Postconditions - none
*/
void Game::Peek() {
  cout << "The top three cards in the deck are:" << endl;
  for (int i = 0; i < ((int) PEEK_NUM); i++) { // PEEK_NUM is maximum number of cards to peek
    cout << i << ". " << m_deck[i].ToString() << endl;
  }
}

/*   Name - SetNumPlayers()
     Desc - determines number of players and creates the players
     Preconditions - a user inputed number for amount of players
     Postconditions - players are created
*/
void Game::SetNumPlayers(int numPlayers) {
  m_numPlayers = numPlayers;
  Player currPlayer;
  stringstream ss; // stringstream is created to take intAsString into a string for setting the name
  string intAsString;
  for (int i = 0; i < numPlayers; i++) {
    ss << i + 1;
    intAsString = ss.str();
    m_players[i] = currPlayer;
    m_players[i].SetPlayer(intAsString);
    ss.str(""); // ss is cleared
  }
}

/*   Name - LoadDeck()
     Desc - loads a deck from a file
     Preconditions - takes a file string
     Postconditions - deck created
*/
int Game::LoadDeck(string fileString) {
  int i = 1;
  int type;
  string desc;
  Card currCard;
  string str;
  ifstream deckFile;
  deckFile.open(fileString.c_str());
  while (getline(deckFile, str)) {
    if (i % 2 == 1) {
      type = atoi(str.c_str()); // type must be converted to int
    }
    else {
      desc = str;
    }
    m_deck.push_back(currCard);
    m_deck[i - 1].SetCard(type, desc); // creates a new element for the card class
    i++;
  }
  int halfDeck = ((int) m_deck.size()) / 2; // for some reason, elements are doubled,
                                            // so this block of code was added to remove the doubles
  for (int j = 0; j < halfDeck; j++) {
    m_deck.erase(m_deck.begin() + j);
  }
  return 0;
}

/*   Name - AddBombCards()
     Desc - adds the bombs to the deck
     Preconditions - file string for bombs
     Postconditions - deck with bombs
*/
int Game::AddBombCards(string fileString){ // the exact same thing as LoadDeck but with bomb.txt
  int i = 1;
  int type;
  string desc;
  Card currCard;
  string str;
  vector<Card> tempVector;
  ifstream deckFile;
  deckFile.open(fileString.c_str());
  while (getline(deckFile, str)) {
    if (i % 2 == 1) {
      type = atoi(str.c_str());
    }
    else {
      desc = str;
    }
    tempVector.push_back(currCard);
    tempVector[i - 1].SetCard(type, desc);
    i++;
  }
  int halfDeck = ((int) tempVector.size()) / 2;
  for (int j = 0; j < halfDeck; j++) {
    tempVector.erase(tempVector.begin() + j);
  }

  for (int k = 0; k < (int) tempVector.size(); k++) {
    m_deck.push_back(tempVector[k]);
  }

  return 0;
}

/*   Name - Shuffle()
     Desc - shuffles m_deck
     Preconditions - either player has played a card or game is initialized
     Postconditions - deck is shuffled
*/
void Game::Shuffle() {
  srand(RAND_SEED);
  int randomNum;
  Card tempCard;
  for (int k = 0; k < 5; k++) {
    for (int i = 0; i < ((int) m_deck.size()); i++) { // shuffles the deck by storing a random card in the 
                                                      // deck, then swapping the current iteration to the random
                                                      // card. shuffles for 5 times
      randomNum = rand() % ((int) m_deck.size());
      tempCard = m_deck[randomNum];
      m_deck[randomNum] = m_deck[i];
      m_deck[i] = tempCard;
    }
  }
}

/*   Name - DrawCard()
     Desc - takes the card from the top of the deck
     Preconditions - initializing (dealing) or player drawing a card
     Postconditions - card is removed from m_deck and given to player's m_hand
*/
Card Game::DrawCard() {
  Card cardDrawn;
  cardDrawn = m_deck[0];
  m_deck.erase(m_deck.begin()); // erases first element of m_deck
  return cardDrawn;
}
