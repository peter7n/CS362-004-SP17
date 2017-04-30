/******************************************************************
 * cardtest4.c
 * Author: Peter Nguyen
 * Date: 4/30/17
 * CS362-400
 * Description: Unit test for the card Village
 *****************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "village"

int assertEqual(int x, int y, int failFlag)
{
  if (x != y)
  {
    printf("//// TEST FAILED! ////\n");
    failFlag = 1;
  }
  return failFlag;
}

int main() 
{
  int failFlag = 0;
  int newCards = 0;
  int otherPlayerNewCards = 0;
  int discarded = 1;
  int xtraCoins = 0;
  int shuffledCards = 0;

  int i, j, m;
  int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
  int remove1, remove2;
  int seed = 1000;
  int numPlayers = 2;
  int thisPlayer = 0;
  int otherPlayer = 1;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
  printf("init coins = %d\n", G.coins);
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: +4 treasure cards --------------
	printf("\n---------- TEST 1: +1 card, +2 actions ----------\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	cardEffect(village, choice1, choice2, choice3, &testG, handpos, &bonus);

	newCards = 1;
	xtraCoins = 0;

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], 
      G.handCount[thisPlayer] + newCards - discarded);
	failFlag = assertEqual(testG.handCount[thisPlayer], 
      G.handCount[thisPlayer] + newCards - discarded, failFlag);
	
  printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards + shuffledCards);
	failFlag = assertEqual(testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards + shuffledCards, failFlag);
	
  printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],
      G.discardCount[thisPlayer]);
	failFlag = assertEqual(testG.discardCount[thisPlayer], G.discardCount[thisPlayer], failFlag);

  printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	failFlag = assertEqual(testG.coins, G.coins + xtraCoins, failFlag);

  // TEST: +2 actions
  printf("\nTEST: +2 actions\n");

  printf("actions count = %d, expected = %d\n", testG.numActions, G.numActions + 2);
  failFlag = assertEqual(testG.numActions, G.numActions + 2, failFlag);
  
	// TEST: cards come from player's own deck
	printf("\nTEST: cards come from player0's own deck\n");
	
  printf("player0 deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards + shuffledCards);
	failFlag = assertEqual(testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards + shuffledCards, failFlag);
  
  printf("player1 deck count = %d, expected = %d\n", testG.deckCount[otherPlayer],
      G.deckCount[otherPlayer]);
	failFlag = assertEqual(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], failFlag);

	// TEST: no state change occurs for other player 
	printf("\nTEST: no state change occurs for other player (player1)\n");
  
	printf("player1 hand count = %d, expected = %d\n", testG.handCount[otherPlayer], 
      G.handCount[otherPlayer]);
	failFlag = assertEqual(testG.handCount[otherPlayer], G.handCount[otherPlayer], failFlag);

  printf("player1 deck count = %d, expected = %d\n", testG.deckCount[otherPlayer],
      G.deckCount[otherPlayer]);
	failFlag = assertEqual(testG.deckCount[otherPlayer], G.deckCount[otherPlayer], failFlag);

  printf("player1 discard count = %d, expected = %d\n", testG.discardCount[otherPlayer],
      G.discardCount[otherPlayer]);
	failFlag = assertEqual(testG.discardCount[otherPlayer], G.discardCount[otherPlayer], failFlag);

	// TEST: no state change occurs to victory card and kingdom card piles 
	printf("\nTEST: no state change occurs to victory card and kingdom card piles\n");

  // check victory cards
  printf("estate card count = %d, expected = %d\n", testG.supplyCount[estate],
      G.supplyCount[estate]);
  failFlag = assertEqual(testG.supplyCount[estate], G.supplyCount[estate], failFlag);

  printf("duchy card count = %d, expected = %d\n", testG.supplyCount[duchy],
      G.supplyCount[duchy]);
  failFlag = assertEqual(testG.supplyCount[duchy], G.supplyCount[duchy], failFlag);

  printf("province card count = %d, expected = %d\n", testG.supplyCount[province],
      G.supplyCount[province]);
  failFlag = assertEqual(testG.supplyCount[province], G.supplyCount[province], failFlag);

  // check kingdom cards
  for (i = adventurer; i <= treasure_map; i++)       	//loop all cards
  {
    printf("kingdom card %d count = %d, expected = %d\n", i, testG.supplyCount[i],
        G.supplyCount[i]);
    failFlag = assertEqual(testG.supplyCount[i], G.supplyCount[i], failFlag);
  }
  
  
  
  
  
  
  
  
  
  if (!failFlag)
    printf("\n*********************** ALL TESTS PASSED! ***********************\n");
  else
    printf("\n/////////////////////// ONE OR MORE TESTS FAILED //////////////////////\n");

	return 0;
}


