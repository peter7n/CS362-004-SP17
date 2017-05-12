/******************************************************************
 * randomtestcard2.c
 * Author: Peter Nguyen
 * Date: 5/14/17
 * CS362-400
 * Description: Random test generator for the card Great Hall
 *****************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define NUM_TESTS 10

int checkGreatHall(int p, struct gameState *post, int* failCount)
{
   int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
   struct gameState pre;
   memcpy (&pre, post, sizeof(struct gameState));

   printf("\nhand pre: ");
   int i;
   for (i = 0; i < pre.handCount[p] + 1; i++)
      printf("%d  ", pre.hand[p][i]);
   printf("\n");

   int result;
   result = cardEffect(great_hall, choice1, choice2, choice3, post, handpos, &bonus);

   int handDiscardedCount = 1;
   int deckDiscardedCount = 0;
   int newCards = 1;
   int addedCoins = 0;
   int addedActions = 1;

   // Add +1 card to hand
   memcpy(pre.deck[p], post->deck[p], sizeof(int) * post->deckCount[p]);
   memcpy(pre.discard[p], post->discard[p], sizeof(int) * post->discardCount[p]);
   // pre.hand[p][post->handCount[p] - 1] = post->hand[p][post->handCount[p] - 1];
   pre.playedCards[pre.playedCardCount] = post->playedCards[pre.playedCardCount];
   // pre.hand[p][handpos] = -1;
   //replace discarded card with last card in hand
   // pre.hand[p][handpos] = pre.hand[p][post->handCount[p] - 1];
   //set last card to -1
   // pre.hand[p][post->handCount[p] - 1] = -1;


   pre.hand[p][handpos] = post->hand[p][handpos];
   pre.hand[p][post->handCount[p]] = -1;


   // Add correct number of cards to hand count and adjust
   // deck count and discard count accordingly.
   pre.handCount[p] = pre.handCount[p] + newCards - handDiscardedCount;
   pre.deckCount[p] = pre.deckCount[p] - deckDiscardedCount - newCards;
   pre.discardCount[p] = pre.discardCount[p] + deckDiscardedCount;
   pre.coins += addedCoins;
   pre.playedCardCount++;

   // Add +1 action
   pre.numActions += addedActions;

   // Assert test results
   if (result != 0 || memcmp(&pre, post, sizeof(struct gameState)) != 0)
   {
      (*failCount)++;
      if (result !=0)
         printf("***Result not equal to 0\n");
      printf("////////////////////// FAILED!\n");
      printf("hand pre: %d  hand post: %d\n", pre.handCount[p], post->handCount[p]);
      printf("deck pre: %d  deck post: %d\n", pre.deckCount[p], post->deckCount[p]);
      printf("discard pre: %d  discard post: %d\n", pre.discardCount[p], post->discardCount[p]);
      printf("coins pre: %d  coins post: %d\n", pre.coins, post->coins);
      printf("numActions pre: %d  numActions post: %d\n", pre.numActions, post->numActions);
      printf("playedCardCount pre: %d  playedCardCount post: %d\n", pre.playedCardCount, post->playedCardCount);
      printf("hand pre: ");
      int i;
      for (i = 0; i < pre.handCount[p] + 1; i++)
         printf("%d  ", pre.hand[p][i]);
      printf("\nhand pst: ");
      for (i = 0; i < post->handCount[p] + 1; i++)
         printf("%d  ", post->hand[p][i]);
      printf("\n\n");
   }
   else
      printf(">> PASSED <<\n");
      printf("hand pre: ");
      for (i = 0; i < pre.handCount[p] + 1; i++)
         printf("%d  ", pre.hand[p][i]);
      printf("\nhand pst: ");
      for (i = 0; i < post->handCount[p] + 1; i++)
         printf("%d  ", post->hand[p][i]);
      printf("\n\n");
}

int main ()
{
   int i, j, n, r, p, deckCount, discardCount, handCount;
   int failCount = 0;

   // initial game parameters
   int seed = 1000;
   int numPlayers = 2;
   int player = 0;
   int k[10] = {adventurer, council_room, feast, gardens, mine,
      remodel, smithy, village, baron, great_hall};
   struct gameState G;
   int testDeck[5] = {minion, mine, cutpurse, copper, gold};

   printf ("Testing Great Hall card\n");

   printf ("RANDOM TESTS\n\n");

   SelectStream(2);
   PutSeed(3);

   // initialize a game state and player cards
   initializeGame(numPlayers, k, seed, &G);

   for (n = 0; n < NUM_TESTS; n++)
   {
      /*-------------- Fill game state with random values --------------*/
      for (i = 0; i < sizeof(struct gameState); i++)
      {
         ((char*)&G)[i] = floor(Random() * 256);
      }

      /*-------- Generate random values for key state variables --------*/
      // player = floor(Random() * numPlayers);
      player = 0;
      G.whoseTurn = player;
      G.numActions = floor(Random() * 100);
      G.coins = floor(Random() * 100);
      G.playedCardCount = 0;
      G.playedCards[0] = 0;

      /*-------------- Fill hand with random cards --------------*/
      for (p = 0; p < numPlayers; p++)
      {
         G.handCount[p] = floor(Random() * MAX_HAND);
         int randHand[G.handCount[p]];

         for (j = 0; j < G.handCount[p]; j++)
         {
            randHand[j] = floor(Random() * treasure_map);
         }
         memcpy(G.hand[p], randHand, sizeof(int) * G.handCount[p]);
      }

      /*-------------- Fill deck with random cards --------------*/
      for (p = 0; p < numPlayers; p++)
      {
         G.deckCount[p] = floor(Random() * MAX_DECK);
         // G.deckCount[p] = 5;
         int randDeck[G.deckCount[p]];

         for (j = G.deckCount[p] - 1; j >= 0; j--)
         {
            randDeck[j] = floor(Random() * treasure_map);
           //  randDeck[j] = testDeck[j];
         }
         memcpy(G.deck[p], randDeck, sizeof(int) * G.deckCount[p]);
         // memcpy(G.deck[p], testDeck, sizeof(int) * 5);
      }

      /*-------------- Fill discard pile with random cards --------------*/
      for (p = 0; p < numPlayers; p++)
      {
         G.discardCount[p] = floor(Random() * MAX_DECK);
         int randDiscard[G.discardCount[p]];

         for (j = 0; j < G.discardCount[p]; j++)
         {
            randDiscard[j] = floor(Random() * treasure_map);
         }
         memcpy(G.discard[p], randDiscard, sizeof(int) * G.discardCount[p]);
      }

      printf("Test: %d   ", n);
      checkGreatHall(player, &G, &failCount);
   }

   if (failCount)
      printf("\nFailed Tests: %d\n", failCount);
   else
      printf ("\nALL TESTS OK\n");

   return 0;
}
