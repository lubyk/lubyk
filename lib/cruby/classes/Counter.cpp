class Counter : public Node
{
public:
  init ()
  {
    mCounter = 0;
    declare_inlet<Counter,&Counter::increment_counter>();
  }
  
  int counter() { return mCounter; }
  
  void increment_counter()
  {
    mCounter++;
  }
private:
  int mCounter;
}