/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) 
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) 
{
  //inicializuje celou tabulku
  for (int i = 0; i < HT_SIZE; i++) 
  {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) 
{
  //ziskame index klice
  int index_of_key = get_hash(key);
  //ziskame prvek podle indexu
  ht_item_t *curr_item = (*table)[index_of_key];
  
  //prochazime itemy
  while (curr_item != NULL) 
  {
    //pokud se klice shoduji vratim item
    if (strcmp(curr_item->key, key) == 0) 
    {
      return curr_item;
    } 
    else
      curr_item = curr_item->next; //jinak se posunu dal
  }
  //jinak vracim null
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) 
{
  //pokusim se nejprve hledat podle klice
  ht_item_t *found = ht_search(table, key);
  if (found == NULL) 
  {
    //pokud nenajdu tvorim novy klic
    ht_item_t *new = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (new == NULL) //kontrola alokace
    {
      return;
    }
    
    //nahraju hodnoty
    new->value = value;
    new->key = malloc(strlen(key) + 1);
    if (new->key == NULL) //kontrola alokace pro klic
    {
      return;
    }
    //prekopiruji novy klic
    strcpy(new->key, key);
    //nastavim prvni ve fronte
    new->next = (*table)[get_hash(key)];
    (*table)[get_hash(key)] = new;
  } 
  else
    found->value = value; //jinak vracim nalezeno

}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) 
{
  //vyuziti funkce pro vyhledani
  ht_item_t *found = ht_search(table, key);
  if (found == NULL) 
  {
    return NULL; //nenalezeno vracim null
  } 
  else 
  {
    return &found->value; //jinak vracim ukazatel
  }
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  //ziskam index
  int index = get_hash(key);
  //prvek z indexu
  ht_item_t *current = (*table)[index];
  ht_item_t *prev = NULL;

  //dokud budou existovat prvky v rade
  while (current != NULL) 
  {
    //porovnam pro nalezeni stejneho klice
    if (strcmp(current->key, key) == 0)
    {
      if (prev == NULL) 
      {
        //nastavim dalsi na prvni
        (*table)[index] = current->next;
      } 
      else 
      {
        //napojim zpet
        prev->next = current->next;
      }
      //uvolnuji vsechny soucasti
      free(current->key);
      free(current);
      return;
    }
    //pokud nebyl posouvam se dal v listu
    prev = current;
    current = current->next;
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) 
{
  //projdeme vsechny prvky
  for (int i = 0; i < HT_SIZE; i++) 
  {
    //nacteme kazdy prvek
    ht_item_t *current = (*table)[i];
    //dokud neni null uvolnuji vsechny jeho soucasit(cely list)
    while (current != NULL) 
    {
      //promenna pro posun
      ht_item_t *next_item = current->next;
      free(current->key);
      free(current);
      current = next_item;
    }
    //jinak nastavim pro jistotu na null
    (*table)[i] = NULL;
  }
}
