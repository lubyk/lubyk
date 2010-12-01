#include <list>
#include <iostream>

int main()
{
  std::list<int> list;
  std::list<int>::iterator it  = list.begin();
  std::list<int>::iterator end = list.end();
  int i = 0;
  
  std::cout << "empty list\n";
  while(it != end) {
    int val = *it;
    std::cout << i << ": " << val << "\n";
    it = list.erase(it); // tell thread to stop
    i++;
  }
  
  i = 0;
  std::cout << "two elements\n";
  list.push_back(1);
  list.push_back(2);
  it = list.begin();
  while(it != end) {
    int val = *it;
    std::cout << i << ": " << val << "\n";
    it = list.erase(it); // tell thread to stop
    i++;
  }
  
}