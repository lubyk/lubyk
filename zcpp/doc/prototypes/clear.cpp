#include <list>
#include <cstdio>

#define MAX_NAME_LENGTH 20
struct A
{
  A(const char * pName) 
  {   
    int i = 0;
    while(pName[i] && i < MAX_NAME_LENGTH) {
      name_[i] = pName[i];
      i++;
    }
    name_[i] = '\0';
    printf("new [%s]\n", name_);
  }
  ~A() { printf("[%s] is dying...\n", name_); }
  char name_[MAX_NAME_LENGTH + 1];
};

int main()
{
  std::list<A*> list;
  
  list.push_back( new A("bob")   );
  list.push_back( new A("dylan") );
  
  printf("\nGoing to send 'clear'.\n");
  // list.clear(); <-- this does not send delete to the pointers
  for(std::list<A*>::iterator it=list.begin(); it!=list.end(); it++) {
    delete *it;
  }
  printf("\nClear sent.\n");
  return 0;
}