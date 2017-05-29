/******************************************************************
 * cardtest2.c
 * Author: Peter Nguyen
 * Date: 4/30/17
 * CS362-400
 * Description: Unit test for the card Adventurer
 *****************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "adventurer"

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
  int handDiscarded = 1;
  int deckDiscarded = 0;
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
  int testDeck[5] = {minion, mine, cutpurse, copper, gold};
  int testDeck2[5] = {copper, gold, minion, mine, cutpurse};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
  printf("init coins = %d\n", G.coins);
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: +2 treasure cards --------------
	printf("\n---------- TEST 1: +2 treasure cards {minion, mine, cutpurse, copper, gold} ----------\n");

  // copy deck with 1 copper and 1 gold at "top" of player0 deck
  G.deckCount[thisPlayer] = 5;
  memcpy(G.deck[thisPlayer], testDeck, sizeof(int) * G.deckCount[thisPlayer]);
	
  // copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	
  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	newCards = 2;
  handDiscarded = 1;
  deckDiscarded = 0;
	xtraCoins = 4;

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], 
      G.handCount[thisPlayer] + newCards - handDiscarded);
	failFlag = assertEqual(testG.handCount[thisPlayer], 
      G.handCount[thisPlayer] + newCards - handDiscarded, failFlag);
	
  printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards - deckDiscarded + shuffledCards);
	failFlag = assertEqual(testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards - deckDiscarded + shuffledCards, failFlag);
	
  printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], 
      G.discardCount[thisPlayer] + handDiscarded + deckDiscarded);
	failFlag = assertEqual(testG.discardCount[thisPlayer], 
      G.discardCount[thisPlayer] + handDiscarded + deckDiscarded, failFlag);
  
  printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	failFlag = assertEqual(testG.coins, G.coins + xtraCoins, failFlag);

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
  
	// ----------- TEST 2: +2 treasure cards (different deck) --------------
	printf("\n----------TEST 2: +2 treasure cards {copper, gold, minion, mine, cutpurse} ----------\n");
  
  // copy deck with 1 copper and 1 gold at "bottom" of player0 deck
  G.deckCount[thisPlayer] = 5;
  memcpy(G.deck[thisPlayer], testDeck2, sizeof(int) * G.deckCount[thisPlayer]);
	
  // copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	
  cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

	newCards = 2;
  handDiscarded = 1;
	deckDiscarded = 3;
  xtraCoins = 4;

	printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], 
      G.handCount[thisPlayer] + newCards - handDiscarded);
	failFlag = assertEqual(testG.handCount[thisPlayer], 
      G.handCount[thisPlayer] + newCards - handDiscarded, failFlag);
	
  printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards - deckDiscarded + shuffledCards);
	failFlag = assertEqual(testG.deckCount[thisPlayer], 
      G.deckCount[thisPlayer] - newCards - deckDiscarded + shuffledCards, failFlag);
	
  printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], 
      G.discardCount[thisPlayer] + handDiscarded + deckDiscarded);
	failFlag = assertEqual(testG.discardCount[thisPlayer], 
      G.discardCount[thisPlayer] + handDiscarded + deckDiscarded, failFlag);
  
  printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins);
	failFlag = assertEqual(testG.coins, G.coins + xtraCoins, failFlag);

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


