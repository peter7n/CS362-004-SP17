/******************************************************************
 * unittest1.c
 * Author: Peter Nguyen
 * Date: 4/30/17
 * CS362-400
 * Description: Unit test for function updateCoins
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
  
  handCount = 5;
  
  printf("-------------------- TESTING updateCoins function --------------------\n\n");

  for (playerCount = 0; playerCount < numPlayers; playerCount++)
  {
    printf("---------- TESTING PLAYER %d -------------\n", playerCount);
    testGame.handCount[playerCount] = 5;
    
    for (bonusCount = 0; bonusCount <= maxBonus; bonusCount++)
    {
      // Reset test hand
      testHand[0] = remodel;
      testHand[1] = smithy;
      testHand[2] = village;
      testHand[3] = baron;
      testHand[4] = great_hall;
      memcpy(testGame.hand[playerCount], testHand, sizeof(int) * handCount);
      
      // Fill up the existing test hand with 0 to 5 coppers
      for (i = 0; i < maxHandCount; i++)
      {
        testHand[i] = copper;
        memcpy(testGame.hand[playerCount], testHand, sizeof(int) * handCount);
        updateCoins(playerCount, &testGame, bonusCount);
        printf("%d coppers + %d bonus = %d coins\n", (i + 1), bonusCount, testGame.coins);
        failFlag = myAssert(testGame.coins, ((i + 1) * 1) + bonusCount, failFlag);
      }
   
      // Reset test hand
      testHand[0] = remodel;
      testHand[1] = smithy;
      testHand[2] = village;
      testHand[3] = baron;
      testHand[4] = great_hall;
      memcpy(testGame.hand[playerCount], testHand, sizeof(int) * handCount);
      
      // Fill up the existing test hand with 0 to 5 silvers 
      for (i = 0; i < maxHandCount; i++)
      {
        testHand[i] = silver;
        memcpy(testGame.hand[playerCount], testHand, sizeof(int) * handCount);
        updateCoins(playerCount, &testGame, bonusCount);
        printf("%d silvers + %d bonus = %d coins\n", (i + 1), bonusCount, testGame.coins);
        failFlag = myAssert(testGame.coins, ((i + 1) * 2) + bonusCount, failFlag);
      }
   
      // Reset test hand
      testHand[0] = remodel;
      testHand[1] = smithy;
      testHand[2] = village;
      testHand[3] = baron;
      testHand[4] = great_hall;
      memcpy(testGame.hand[playerCount], testHand, sizeof(int) * handCount);
      
      // Fill up the existing test hand with 0 to 5 golds
      for (i = 0; i < maxHandCount; i++)
      {
        testHand[i] = gold;
        memcpy(testGame.hand[playerCount], testHand, sizeof(int) * handCount);
        updateCoins(playerCount, &testGame, bonusCount);
        printf("%d golds + %d bonus = %d coins\n", (i + 1), bonusCount, testGame.coins);
        failFlag = myAssert(testGame.coins, ((i + 1) * 3) + bonusCount, failFlag);
      }
   
    }
  } 
  
  if (!failFlag)
    printf("*********************** ALL TESTS PASSED! ***********************\n");
  
  return 0;
}
