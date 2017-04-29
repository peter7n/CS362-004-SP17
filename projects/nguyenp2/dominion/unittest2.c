/******************************************************************
 * unittest2.c
 * Author: Peter Nguyen
 * Date: 4/30/17
 * CS362-400
 * Description: Unit test for function isGameOver
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
  int i;
  int suppliesDepleted = 0;
  int result;
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
  // A test hand with random cards in it
  int testHand[5] = { remodel, smithy, village, baron, great_hall };
  
  initializeGame(numPlayers, k, seed, &testGame); // initialize a new game
  
  printf("-------------------- TESTING isGameOver function --------------------\n\n");

  // Test game over at start of game
  printf("Testing: no supplies depleted\n");
  result = isGameOver(&testGame);
  printf("Expected: 0 Actual: %d\n", result);
  failFlag = myAssert(result, 0, failFlag);

  // Testing game over when province card supply is exhausted
  printf("Testing: province card supply = 0\n");
  testGame.supplyCount[province] = 0;
  result = isGameOver(&testGame);
  printf("Expected: 1 Actual: %d\n", result);
  failFlag = myAssert(result, 1, failFlag);

  // Test game over when 1 to 24 of supplies are exhausted
  testGame.supplyCount[province] = 8;

  for (i = 0; i < 25; i++)
  {
    printf("Testing: supply %d = 0\n", i);
    testGame.supplyCount[i] = 0;
    suppliesDepleted++;
    result = isGameOver(&testGame);
    if (suppliesDepleted >= 3)
    {
      printf("Expected: 1 Actual: %d\n", result);
      failFlag = myAssert(result, 1, failFlag);
    }
    else
    {
      printf("Expected: 0 Actual: %d\n", result);
      failFlag = myAssert(result, 0, failFlag);
    } 
  }

  if (!failFlag)
    printf("*********************** ALL TESTS PASSED! ***********************\n");
  
  return 0;
}
