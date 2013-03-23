#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

#ifdef DEBUG
  #define TRACE printf
#else
  #define TRACE
#endif

typedef struct {
  int a;
  int b;
} Result;

typedef struct Item_Tbl Item_Tbl;
Item_Tbl *Item_Tbl_New(int max_price);
void Item_Tbl_Delete(Item_Tbl *thiss);
void Item_Tbl_AddItem(Item_Tbl *thiss, int price, int item_id);
int Item_Tbl_ItemByPrice(Item_Tbl *thiss, int price, int second);

main()
{
   int ntests;  // Number of test cases
   int ctest;   // Current test case
   
   // Read number of test cases
   scanf("%d", &ntests);
   TRACE("Test cases: %d\n", ntests);
   
   // For each test case
   for(ctest = 1; ctest <= ntests; ctest++)
   {
     int       credit; 
     int       nitems; 
     int       citem;
     int       price;
     Item_Tbl *ItemsByPrize;

     scanf("%d", &credit);
     TRACE("Credit: %d\n", credit);
     // Only keep prizing information of items that are in range
     // of our credit amout.
     ItemsByPrize = Item_Tbl_New(credit - 1);
    
     // Get number of items
     scanf("%d", &nitems);
     TRACE("Number of items: %d\n", nitems);
    
     // Read the items and add the information to the table
     for (citem = 1; citem <= nitems; citem++)
     {
       scanf("%d", &price);
       Item_Tbl_AddItem(ItemsByPrize, price, citem);
       TRACE("citem=%d price=%d\n", citem, price);
     }

     // Given Item_Tbl methods the algorithm to find the two items
     // whose prize sum exactly the credit amount should be pretty
     // straightforward:

     // For each price that we saved
     while( (price = Item_Tbl_GetPrice(ItemsByPrize)) != 0 )
     {
       int item_a;
       int item_b;

       TRACE("price=%d price=%d\n", price, credit - price);
       // Check if there exists an item whose price is (credit amount - price),
       // if it does then that's the solution.
       if ( (item_a = Item_Tbl_ItemByPrice(ItemsByPrize, credit - price, 0)) != 0 )
       {
          int temp; 
          item_b = Item_Tbl_ItemByPrice(ItemsByPrize, price, 0);
          TRACE("item_a=%d item_b=%d\n", item_a, item_b);
          if (item_a == item_b) { item_b = Item_Tbl_ItemByPrice(ItemsByPrize, price, 1); }
          if (item_b == 0) { continue; }
          if (item_a > item_b)
          {
            temp = item_a;
            item_a = item_b;
            item_b = temp;
          }          
          printf("Case #%d: %d %d\n", ctest, item_a, item_b);
          break;
       }
     }

     Item_Tbl_Delete(ItemsByPrize);
   }
}

// Start Section A
// ====================================================================== //
// This section of code implements an abstract data type suitable to
// resolve the problem's constraints and requirements. It's not very 
// scalable but it will be fast for large number of items.
//   - The amount of memory it requires is ~3*N*sizeof(int), note that
//     N is NOT the number of items but the maximum credit amount.
//   - Insertions and searches are really cheap since they require 
//     only a couple of array acceses.
// Design notes:
//    - "item_ids" could be considered as a bi-dimensional array of the like
//      [n][2] where n is price indexed, and that stores the first two item
//      ids of the given prize.
//    - "prices" is an array with the list of the prices stored in the 
//      table, this is an optimization trade-off so we don't have to run the
//      entire "item_ids" array but only those entries with meaningful data.
//
struct Item_Tbl  {
  int  *item_ids;
  int  *prices;
  int   size;
  int   n_prices;
  int   price_idx;
};

//
// Constructor-like method. Returns a clean Item_Tbl.
//
Item_Tbl *Item_Tbl_New(int max_price)
{
  int size = max_price + 1;  // Compensate for giving up index/price 0
  Item_Tbl *thiss  = malloc(sizeof(Item_Tbl));
  thiss->item_ids  = malloc(sizeof(int) * size * 2);
  thiss->prices    = malloc(sizeof(int) * size);
  thiss->size      = size;
  thiss->price_idx = 0;
  thiss->n_prices  = 0;
  memset(thiss->item_ids, 0, sizeof(int) * size * 2);
  memset(thiss->prices, 0, sizeof(int) * size);
  return thiss;
}

//
// Destructor-like method
//
void Item_Tbl_Delete(Item_Tbl *thiss)
{
  free(thiss->item_ids);
  free(thiss->prices);
  free(thiss);
}

//
// AddItem - It adds item_id and its price to the table, however
//   it only "remembers" the first two item IDs of a given price.
//
void Item_Tbl_AddItem(Item_Tbl *thiss, int price, int item_id)
{
  if (price >= thiss->size) { return; }
  if (thiss->item_ids[price] == 0) 
  { 
    thiss->item_ids[price]           = item_id;
    thiss->prices[thiss->n_prices++] = price; 
  }
  else if(thiss->item_ids[price + thiss->size] == 0)
  {
    thiss->item_ids[price + thiss->size] = item_id;
  }
}

//
// ItemByPrice - Returns the item id of the item of a given price.
//
int Item_Tbl_ItemByPrice(Item_Tbl *thiss, int price, int second)
{
  int item_index = 0;
  if(price < thiss->size) { item_index = thiss->item_ids[price + (second * thiss->size)]; }
  return item_index;
}

//
// GetPrice - Function used to iterato through prices. It returns 0
//   when done traversing the prices.
//
int Item_Tbl_GetPrice(Item_Tbl *thiss)
{
  int price = 0;
  if(thiss->price_idx < thiss->n_prices) { price = thiss->prices[thiss->price_idx++]; }
  return price;
}
// =======================================================================//
// End Section A