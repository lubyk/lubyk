#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <pthread.h>
#include "params.h"
#include "hash.h"
#include "node.h"


#define MAX_TOKEN_SIZE 2048

typedef enum action_types_ {
  NO_ACTION,
	CREATE_INSTANCE,
	CREATE_LINK
} action_types_t;

class Rubyk;

class Command
{
public:
  Command(std::istream& pInput, std::ostream& pOutput) : mInput(&pInput), mOutput(&pOutput)
  { initialize(); }
  
  Command ()
  {
    mInput = &std::cin;
    mOutput = &std::cout;
    initialize();
  }
  
  virtual ~Command() {}
  
  /** This method creates a new thread to listen for incomming commands. */
  void listen (std::istream& pInput, std::ostream& pOutput) ;
  
  /** Wrapper to call member method. */
  static void * call_do_listen(void * cmd) {
    return (void*) ((Command*)cmd)->do_listen();
  }
  
  /** Stop listening for incomming commands. */
  void close ();
  
  /** Clear the current command. */
  void clear ();
  
  /** Ragel parser. */
  void parse (const char * pStr) {
    parse(std::string(pStr));
  }
  void parse (const std::string& pStr);
  
  /** Used for testing. */
  void set_output (std::ostream& pOutput)
  { mOutput = &pOutput; }
  
  /** Used by rubyk server. */
  void set_server (Rubyk& pServer)
  { mServer = &pServer; }
  
  /** Used for testing. */
  void set_input (std::istream& pInput)
  { mInput = &pInput; }
  
  /** Do not print command results back. */
  void set_silent (bool pSilent)
  { mSilent = pSilent;}
  
  void set_thread_id(pthread_t& pId)
  { mThread = pId; }
	
protected:
  /** Constructor, set default values. */
  void initialize();
  
  /** Code executed in a separate thread. Runs until 'mQuit' is true. */
  virtual int do_listen();
  
  /** PARSER RELATED CALLBACKS **/
  
  /** Set a variable from the current token content. */
  void set_from_token (std::string& pElem);
  
  /** Set the class name. */
  void set_class_from_token  ();
  
  /** Set the 'class' parameter ('value' for Value, 'metro' for Metro, etc). */
  void add_value_from_token ();
  
  /** Set a parameter. */
  void set_parameter  (const std::string& pKey, const std::string& pValue);
  
  /** Create an instance. */
  void create_instance ();
  
  /** Create a link. If the link cannot be created right now because all variables aren't set yet, the link will be kept in Rubyk's link buffer until all variables are found. */
  void create_link ();
  
  /** Remove a link. */
  void remove_link ();
  
  /** Execute a method on an instance. */
  void execute_method ();
  
  /** Execute a class method. */
  void execute_class_method ();

  /** Execute a command or inspect instance. */
  void execute_command ();
  
  /** Read a line from input stream. */
  virtual bool getline(char ** pBuffer, size_t pSize)
  {
    if (mInput->eof()) return false;
    mInput->getline(*pBuffer,pSize);
    return true;
  }
  
  /** Save a line in edit history. */
  virtual void saveline(const char * pLine) {}
  
  /** Free a line. */
  virtual void freeline(char * pLine) {}
    
  /** Token for the parser. */
  char mToken[MAX_TOKEN_SIZE + 1];
  unsigned int mTokenIndex;
  unsigned int mCurrentState; /**< Current parser state between blocks. */
  
	action_types_t mAction;
  
  /** Command parts. */
	std::string    mVariable, mMethod, mClass, mKey, mValue, mFrom, mTo;
	Params         mParameters;
  int            mFromPort, mToPort;
  Rubyk *        mServer;
  
  /** IO management. */
  pthread_t mThread;
  std::istream * mInput;
  std::ostream * mOutput;
  
  bool mQuit;
  bool mSilent;
};

#ifdef USE_READLINE
extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

class InteractiveCommand : public Command
{
public:
  InteractiveCommand(std::istream& pInput, std::ostream& pOutput) : Command(pInput,pOutput)
  {
    read_history(history_path().c_str());
  }
  
  InteractiveCommand() 
  {
    // read readline history
    read_history(history_path().c_str());
  }
  
  virtual ~InteractiveCommand()
  {
    // save readline history
    write_history(history_path().c_str());
  }
  
  virtual bool getline(char ** pBuffer, size_t pSize)
  {
    *pBuffer = readline("> "); // FIXME: this would not work if input is not stdin...
    return *pBuffer != NULL;
  }
  
  virtual void saveline(const char * pLine)
  {
    if (strlen(pLine) > 0) {
      add_history(pLine);
    }
  }
  
  virtual void freeline(char * pLine)
  {
    free(pLine);
  }
  
private:
  std::string history_path()
  {
    std::string home(getenv("HOME"));
    home.append("/.rubyk_history");
    return home;
  }
};

#else

class InteractiveCommand : public Command
{
public:
  InteractiveCommand(std::istream& pInput, std::ostream& pOutput) : Command(pInput,pOutput) {}
  InteractiveCommand() {}
  
  virtual bool getline(char ** pBuffer, size_t pSize)
  {
    *mOutput << "> ";
    if (mInput->eof()) return false;
    mInput->getline(*pBuffer,pSize);
    return true;
  }
};

#endif // USE_READLINE

#endif // _COMMAND_H_