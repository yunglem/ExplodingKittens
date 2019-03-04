/***************************************************************
 ** File:    Card.cpp
 ** Project: CMSC 202 Project 1, Spring 2017
 ** Author:  Johnlemuel Casilag
 ** Date:    3/9/17
 ** Section: 10
 ** E-mail:  cas6@umbc.edu
 **
 ** This file implements the Card class.
 **
 ***************************************************************/

#include <string>
#include <iostream>
#include "Card.h"
using namespace std;

// ---------------- Constructors ----------------

// Default constructor
Card::Card()
{
}

/*   Name - SetCard()
     Desc - creates card type and description
     Preconditions - card created during file i/o
     Postconditions - card created
*/
void Card::SetCard(int type, string description)
{
  if (type >= 0 && type <= 9) {
    m_type = type;
  }
  else {
    m_type = 0;
  }
  m_desc = description;
}

// ---------------- Accessors ----------------

/*   Name - GetType()
     Desc - returns int type of card for use
     Preconditions - none
     Postconditions - returns int type
*/
int Card::GetType() {
  return m_type;
}

/*   Name - ToString()
     Desc - returns string description of card for use
     Preconditions - none
     Postconditions - returns string description
*/
string Card::ToString() {
  return m_desc;
}

