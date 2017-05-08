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

int checkAdventurer(int p, struct gameState *post, int treasureCount,
      int deckDiscardedCount, int addedCoins)
{
   int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
   int handDiscardedCount;
   struct gameState pre;
   memcpy (&pre, post, sizeof(struct gameState));

   int result;
   result = cardEffect(adventurer, choice1, choice2, choice3, post, handpos, &bonus);

   handDiscardedCount = 0; // 1 is correct value (discard Adventurer itself)

   if (treasureCount > 0)
   {
      // memcpy(pre.hand[p], post->hand[p], sizeof(int) * post->handCount[p]);
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
      pre.discardCount[p] = pre.discardCount[p] + deckDiscardedCount + handDiscardedCount;
      // pre.coins += addedCoins; // Add this for correct test

      printf("\nhand pre: %d  hand post: %d\n", pre.handCount[p], post->handCount[p]);
      printf("deck pre: %d  deck post: %d\n", pre.deckCount[p], post->deckCount[p]);
      printf("discard pre: %d  discard post: %d\n", pre.discardCount[p], post->discardCount[p]);
      printf("treasureCount: %d  deckDiscardedCount: %d  addedCoins: %d\n",
          treasureCount, deckDiscardedCount, addedCoins);
      printf("************************************************\n\n");
   }

   assert (result == 0);
   assert(memcmp(&pre, post, sizeof(struct gameState)) == 0);
}

int main ()
{

   int i, j, n, r, p, deckCount, discardCount, handCount;
   int treasureCount = 0; // keeps track of treasure cards gained
   int deckDiscardedCount = 0; // keeps track of cards discarded from deck
   int addedCoins = 0; // keeps track of coins gained from treasure cards

   // initial game parameters
   int seed = 1000;
   int numPlayers = 2;
   int player = 0;
   int k[10] = {adventurer, council_room, feast, gardens, mine,
      remodel, smithy, village, baron, great_hall};
   struct gameState G;
   int testDeck[5] = {minion, mine, cutpurse, copper, gold};


   printf ("Testing Adventurer card\n");

   printf ("RANDOM TESTS\n");

   SelectStream(2);
   PutSeed(3);

   // initialize a game state and player cards
   initializeGame(numPlayers, k, seed, &G);

   for (n = 0; n < 10; n++)
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
         printf("\n\n---------------handCount: %d\n\n", G.handCount[p]);
         int randHand[G.handCount[p]];

         for (j = 0; j < G.handCount[p]; j++)
         {
            randHand[j] = floor(Random() * treasure_map);
            printf("%d   ", randHand[j]);
         }
         memcpy(G.hand[p], randHand, sizeof(int) * G.handCount[p]);
      }

      /*-------------- Fill deck with random cards --------------*/
      for (p = 0; p < numPlayers; p++)
      {
         G.deckCount[p] = floor(Random() * MAX_DECK);
         // G.deckCount[p] = 5;
         printf("\n\n---------------deckCount: %d\n\n", G.deckCount[p]);
         int randDeck[G.deckCount[p]];

         treasureCount = 0;
         deckDiscardedCount = 0;
         addedCoins = 0;
         for (j = G.deckCount[p] - 1; j >= 0; j--)
         {
            randDeck[j] = floor(Random() * treasure_map);
           //  randDeck[j] = testDeck[j];
            printf("%d   ", randDeck[j]);

            // Keep track of treasure cards and discarded cards
            if (randDeck[j] == copper || randDeck[j] == silver || randDeck[j] == gold)
            {
               if (treasureCount < 2 && p == player)
               {
                  treasureCount++;
                  if (randDeck[j] == copper)
                     addedCoins += 1;
                  else if (randDeck[j] == silver)
                     addedCoins += 2;
                  else if (randDeck[j] == gold)
                     addedCoins += 3;
               }
            }
            else
            {
               if (treasureCount < 2 && p == player)
                  deckDiscardedCount++;
            }
         }
         memcpy(G.deck[p], randDeck, sizeof(int) * G.deckCount[p]);
         // memcpy(G.deck[p], testDeck, sizeof(int) * 5);

         printf("\ntreasureCount: %d\n", treasureCount);
         printf("addedCoins: %d\n", addedCoins);
         printf("deckDiscardedCount: %d\n", deckDiscardedCount);
      }

      /*-------------- Fill discard pile with random cards --------------*/
      for (p = 0; p < numPlayers; p++)
      {
         G.discardCount[p] = floor(Random() * MAX_DECK);
         printf("\n---------------discardCount: %d\n\n", G.discardCount[p]);
         int randDiscard[G.discardCount[p]];

         for (j = 0; j < G.discardCount[p]; j++)
         {
            randDiscard[j] = floor(Random() * treasure_map);
            printf("%d   ", randDiscard[j]);
         }
         memcpy(G.discard[p], randDiscard, sizeof(int) * G.discardCount[p]);
      }

      checkAdventurer(player, &G, treasureCount, deckDiscardedCount, addedCoins);
   }

   printf ("ALL TESTS OK\n");

   return 0;
}
