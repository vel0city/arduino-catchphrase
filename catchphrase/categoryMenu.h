#include <WString.h>


struct category                                              
{                                                               
  String name;
  category *next;             // the reference to the next node
};

class CategoryMenu
{
  public:
  int chosenLevel;
  int selectedCategoryCount;
  CategoryMenu();
  ~CategoryMenu();
  void drawMenu(void);
  
  private:
  category *head;
  void loadCategoryNames(void);
};

