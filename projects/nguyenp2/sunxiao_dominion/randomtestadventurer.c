/******************************************************************
 * randomtestcard1.c
 * Author: Peter Nguyen
 * Date: 5/14/17
 * CS362-400
 * Description: Random test generator for the card Adventurer
 *****************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define NUM_TESTS 2000

int checkAdventurer(int p, struct gameState *post, int treasureCount,
      int deckDiscardedCount, int addedCoins, int* failCount)
{
   int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
   int handDiscardedCount;
   struct gameState pre;
   memcpy (&pre, post, sizeof(struct gameState));

   int result;
   result = cardEffect(adventurer, choice1, choice2, choice3, post, handpos, &bonus);

   handDiscardedCount = 1; // discard the Adventurer card itself

   memcpy(pre.deck[p], post->deck[p], sizeof(int) * post->deckCount[p]);
   memcpy(pre.discard[p], post->discard[p], sizeof(int) * post->discardCount[p]);
   if (treasureCount == 1)
      pre.hand[p][post->handCount[p] - 1] = post->hand[p][post->handCount[p] - 1];
   else if (treasureCount == 2)
   {
      pre.hand[p][post->handCount[p] - 2] = post->hand[p][post->handCount[p] - 2];
      pre.hand[p][post->handCount[p] - 1] = post->hand[p][post->handCount[p] - 1];
   }

   // Add correct number of treasure cards to hand count and adjust
   // deck count and discard count accordingly.
   pre.handCount[p] = pre.handCount[p] + treasureCount - handDiscardedCount;
   pre.deckCount[p] = pre.deckCount[p] - deckDiscardedCount - treasureCount;
   pre.discardCount[p] = pre.discardCount[p] + deckDiscardedCount;

   // Assert test results
   if (result != 0 || memcmp(&pre, post, sizeof(struct gameState)) != 0)
   {
      (*failCount)++;
      printf("////////////////////// FAILED!\n");
      printf("hand pre: %d  hand post: %d\n", pre.handCount[p], post->handCount[p]);
      printf("deck pre: %d  deck post: %d\n", pre.deckCount[p], post->deckCount[p]);
      printf("discard pre: %d  discard post: %d\n", pre.discardCount[p], post->discardCount[p]);
      printf("treasureCount: %d  deckDiscardedCount: %d  addedCoins: %d\n\n",
          treasureCount, deckDiscardedCount, addedCoins);
   }
   else
      printf(">> PASSED <<\n");
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

   int treasureCount[numPlayers]; // keeps track of treasure cards gained
   int deckDiscardedCount[numPlayers]; // keeps track of cards discarded from deck
   int addedCoins[numPlayers]; // keeps track of coins gained from treasure cards

   printf ("Testing Adventurer card\n");

   printf ("RANDOM TESTS\n\n");

   SelectStream(2);
   PutSeed(3);

   // initialize a game state and player cards
   initializeGame(numPlayers, k, seed, &G);

   for (n = 0; n < NUM_TESTS; n++)
   {
      // Fill game state with random values
      for (i = 0; i < sizeof(struct gameState); i++)
      {
         ((char*)&G)[i] = floor(Random() * 256);
      }

      // player = floor(Random() * numPlayers);
      player = 0;
      G.whoseTurn = player;

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
         int randDeck[G.deckCount[p]];

         treasureCount[p] = 0;
         deckDiscardedCount[p] = 0;
         addedCoins[p] = 0;
         for (j = G.deckCount[p] - 1; j >= 0; j--)
         {
            randDeck[j] = floor(Random() * treasure_map);

            // Keep track of treasure cards and discarded cards
            if (randDeck[j] == copper || randDeck[j] == silver || randDeck[j] == gold)
            {
               if (treasureCount[p] < 2)
               {
                  treasureCount[p]++;
                  if (randDeck[j] == copper)
                     addedCoins[p] += 1;
                  else if (randDeck[j] == silver)
                     addedCoins[p] += 2;
                  else if (randDeck[j] == gold)
                     addedCoins[p] += 3;
               }
            }
            else
            {
               if (treasureCount[p] < 2)
                  deckDiscardedCount[p]++;
            }
         }
         memcpy(G.deck[p], randDeck, sizeof(int) * G.deckCount[p]);
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
      checkAdventurer(player, &G, treasureCount[player],
            deckDiscardedCount[player], addedCoins[player], &failCount);
   }

   if (failCount)
      printf("\nFailed Tests: %d\n", failCount);
   else
      printf ("\nALL TESTS OK\n");

   return 0;
}
