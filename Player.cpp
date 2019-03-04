/***************************************************************
 ** File:    Player.cpp
 ** Project: CMSC 202 Project 1, Spring 2017
 ** Author:  Johnlemuel Casilag
 ** Date:    3/9/17
 ** Section: 10
 ** E-mail:  cas6@umbc.edu
 **
 ** This file implements the Player class.
 **
 ***************************************************************/

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include "Player.h"
using namespace std;

// ---------------- Constructors ----------------

// Default constructor
Player::Player() {
  m_lost = false;
  m_extraTurn = false;
  m_hand.clear(); 
  m_name = "NULL";
}

/*   Name - SetPlayer()
     Desc - creates a new player based on the name
     Preconditions - takes a string from call
     Postconditions - m_name is updated
*/
void Player::SetPlayer(string name) {
  m_name = name;
}

/*   Name - PlayCard()
     Desc - allows the user to play a card
     Preconditions - it is the users turn
     Postconditions - card is played
*/
Card Player::PlayCard() {
  int card; // for user input cin
  Card none; // default return
  Card tempCard; // what is returned when player chooses a card
  if (m_hand.size() > 0) {
    DisplayHand();
    cout << "Select a card:" << endl;
    cin >> card;
    tempCard = m_hand[card];
    m_hand.erase(m_hand.begin() + card); // removes card from hand
  }
  if ( (int) m_hand.size() > 0) return tempCard;
  else return none; 
}

/*   Name - LoseCard()
     Desc - removes player card and returns the card
     Preconditions - a pair is played in game and this user is prompted
     Postconditions - card removed from player and given away
*/
Card Player::LoseCard(int card) {
  if (m_hand.size() > 0) {
    m_hand.erase(m_hand.begin() + card);
  }
  return m_hand[card];
}

/*   Name - HasCards()
     Desc - checks to see if player has cards in hands
     Preconditions - game was checking if player has cards
     Postconditions - returns boolean value if player has cards or not
*/
bool Player::HasCards() {
  if (m_hand.size() == 0) return false;
  else return true;
}

/*   Name - HasLost()
     Desc - checks to see if player has lost
     Preconditions - every turn checks this so they can't play again
     Postconditions - returns whether or not player lost
*/
bool Player::HasLost() {
  if (m_lost == true) {
  }
  return m_lost;

}

/*   Name - HasExtraTurn()
     Desc - checks to see if player has an extra turn from an attack
     Preconditions - checks every turn to see if player was attacked
     Postconditions - returns whether or not player has an extra turn
*/
bool Player::HasExtraTurn() {
  if (m_extraTurn == true) {
  }
  return m_extraTurn;
}

/*   Name - AddToHand()
     Desc - a card is added to the player's hand and checks what kind,
            and evaluates player's condition based on card drawn
     Preconditions - takes in card that was added
     Postconditions - returns player's condition
*/
int Player::AddToHand(Card cardAdded) {
  bool hasDiffuse = false;
  if (cardAdded.GetType() == 0) {
    cout << "Oh no! You drew Exploding Kitten: " << cardAdded.ToString() << endl;

    for (int i = 0; i < ((int) m_hand.size()); i++) { // checks entire hand if diffuse is present
      if (m_hand[i].GetType() == 1) {
	hasDiffuse = true;
	return 2; // 2 = drew exploding kitten, but has diffuse
      }
    }

    cout << "no diffuse" << endl;
    return 0; // 0 = drew exploding kitten, no diffuse
  }
  else {
    m_hand.push_back(cardAdded);
  }

  return 1; // 1 = player is safe
}

/*   Name - ReturnBomb()
     Desc - returns the bomb to the deck (unused)
     Preconditions - none
     Postconditions - none
*/
Card Player::ReturnBomb() {
  cout << "removed bomb" << endl;
  return m_hand[0];
}

// ---------------- Accessors ----------------

/*   Name - GetName()
     Desc - returns name of player for use
     Preconditions - calls the name
     Postconditions - returns the name
*/
string Player::GetName() {
  return m_name;
}

/*   Name - GetNumberOfCards()
     Desc - returns number of cards in player's hands
     Preconditions - calls the number
     Postconditions - returns the size of hand
*/
int Player::GetNumberOfCards() {
  return m_hand.size();
}

// ---------------- Facilitators ----------------

/*   Name - DisplayHand()
     Desc - displays cards in player's hand with descriptions
     Preconditions - player chose to view hand
     Postconditions - prints m_hand
*/
void Player::DisplayHand() {
  int special;
  for (int i = 0; i < ((int) m_hand.size()); i++) {
    special = m_hand[i].GetType();

    if (special < 6) { // special cards have an extra description attached
      switch (special) {
      case 1:
	cout << i << ". Defuse: " << m_hand[i].ToString() << endl;
	break;
      case 2:
	cout << i << ". Attack: " << m_hand[i].ToString() << endl;
	break;
      case 3:
	cout << i << ". Skip: " << m_hand[i].ToString() << endl;
	break;
      case 4:
	cout << i << ". Peek: " << m_hand[i].ToString() << endl;
	break;
      case 5:
	cout << i << ". Shuffle: " << m_hand[i].ToString() << endl;
	break;
      }
    }
    else cout << i << ". " << m_hand[i].ToString() << endl;
  }
}

/*   Name - SetLost()
     Desc - set's player condition as lost the game
     Preconditions - takes boolean if lost
     Postconditions - updates m_lost
*/
void Player::SetLost(bool hasLost) {
  if (hasLost) m_lost = hasLost;
  else hasLost = false;
}

/*   Name - SetExtraTurn()
     Desc - sets m_extraTurn to truth or false
     Preconditions - takes boolean if extra turn needed
     Postconditions - m_extraTurn is altered
*/
void Player::SetExtraTurn(bool extraTurn) {
  if (extraTurn) m_extraTurn = extraTurn;
  else m_extraTurn = false;
}
