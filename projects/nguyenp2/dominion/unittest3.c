/******************************************************************
 * unittest3.c
 * Author: Peter Nguyen
 * Date: 4/30/17
 * CS362-400
 * Description: Unit test for function handCard
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
  
  printf("-------------------- TESTING handCard function --------------------\n\n");

  memcpy(testGame.hand[0], testHand, sizeof(int) * maxHandCount);
  memcpy(testGame.hand[1], testHand, sizeof(int) * maxHandCount);
  
  for (i = 0; i < 5; i++)
  {
    result = handCard(i, &testGame);
    switch (i)
    {
      case 0 :
        printf("Expected: remodel Actual: %d\n", result);
        failFlag = myAssert(result, remodel, failFlag);
        break;
      case 1 :
        printf("Expected: smithy Actual: %d\n", result);
        failFlag = myAssert(result, smithy, failFlag);
        break;
      case 2 :
        printf("Expected: village Actual: %d\n", result);
        failFlag = myAssert(result, village, failFlag);
        break;
      case 3 :
        printf("Expected: baron Actual: %d\n", result);
        failFlag = myAssert(result, baron, failFlag);
        break;
      case 4 :
        printf("Expected: great_hall Actual: %d\n", result);
        failFlag = myAssert(result, great_hall, failFlag);
        break;
    }
  }
  
  if (!failFlag)
    printf("*********************** ALL TESTS PASSED! ***********************\n");
  
  return 0;
}
