/******************************************************************
 * unittest4.c
 * Author: Peter Nguyen
 * Date: 4/30/17
 * CS362-400
 * Description: Unit test for function fullDeckCount
 *****************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int myAssert(int x, int y, int failFlag)
{
  if (x == y)
    printf("TEST PASSED!\n");
  else
  {
    printf("TEST FAILED!\n");
    failFlag = 1;
  }
  return failFlag;
}

int main() 
{
  int failFlag = 0;
  int result;
  int i, j;
  int seed = 1000;
  int numPlayers = 2;
  int maxBonus = 10;
  int playerCount;
  int handCount;
  int bonusCount;
  struct gameState testGame;
  int maxHandCount = 5;
  int k[10] = { adventurer, council_room, feast, gardens, mine,
               remodel, smithy, village, baron, great_hall };
  int testHand[5] = { remodel, smithy, village, baron, great_hall };
  int testDeck[5] = { remodel, smithy, village, baron, great_hall };
  int testDiscard[5] = { remodel, smithy, village, baron, great_hall };
  
  initializeGame(numPlayers, k, seed, &testGame); // initialize a new game
  
  printf("-------------------- TESTING fullDeckCount function --------------------\n\n");

  for (playerCount = 0; playerCount < numPlayers; playerCount++)
  {
    printf("---------- TESTING PLAYER %d -------------\n", playerCount);
    
    // Loop that tests for cards from adventurer to mine
    for (j = adventurer; j < mine + 1; j++)
    {
      // Set hand, deck and discard to the same 5 cards
      testGame.handCount[playerCount] = 5;
      testGame.deckCount[playerCount] = 5;
      testGame.discardCount[playerCount] = 5;
      memcpy(testGame.hand[playerCount], testHand, sizeof(int) * testGame.handCount[playerCount]);
      memcpy(testGame.deck[playerCount], testDeck, sizeof(int) * testGame.deckCount[playerCount]);
      memcpy(testGame.discard[playerCount], testDiscard, sizeof(int) * testGame.discardCount[playerCount]);

      // Find count of particular card
      result = fullDeckCount(playerCount, j, &testGame);
      printf("Testing: count of card type %d: Expected: 0 Actual: %d\n", j, result);
      failFlag = myAssert(result, 0, failFlag);

      // Fill up hand, deck and discard with particular card from 0 to 5
      for (i = 0; i < 5; i++)
      {
        testGame.hand[playerCount][i] = j;
        testGame.discard[playerCount][i] = j;
        testGame.deck[playerCount][i] = j;

        result = fullDeckCount(playerCount, j, &testGame);
        printf("Testing: count of card type %d: Expected: %d Actual: %d\n", j, (i + 1) * 3, result);
        failFlag = myAssert(result, (i + 1) * 3, failFlag);
      }
      
    }
  } 

  if (!failFlag)
    printf("*********************** ALL TESTS PASSED! ***********************\n");
  
  return 0;
}
