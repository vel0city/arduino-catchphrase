#include "categoryMenu.h"
#include <SD.h>


CategoryMenu::CategoryMenu(void)
{
  loadCategoryNames();
};

CategoryMenu::~CategoryMenu(void) {};

void CategoryMenu::loadCategoryNames(void)
{
  File root = SD.open("/");
  
  while (true)
  {
    File entry = root.openNextFile();
    
    if (!entry)
    {
      return;
    }
    
    if (!entry.isDirectory())
    {
      category *temp;
      temp = (category*)malloc(sizeof(category));
      
      // Load up to the first ; to find the name of the category
      char value = ' ';
      while(value != ';')
      {
        value = entry.read();
        temp->name.concat(value);
        temp->next = head;
        head = temp;
      }
    }
  }
}
