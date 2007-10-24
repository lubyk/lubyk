#ifndef _NETWORK_H_
#define _NETWORK_H_
#include "node.h"


/** A Network is a group of nodes connected together and doing their job. The network is responsible for
  * putting the nodes to life (instanciating) and managing their connections. It is also the gateway from/to
  * editors. There may only be one network per thread. */
class Network
{
public:
  /** Instantiate a network from a text file. */
  Network(const std::string& pPath);
  ~Network();
  
  void run();
private:
  
  
  /** Parse a file and create objects with links. Return true on success, false in case of a parse error.
    * The file must contain a valid patch definition.
    * EBNF definition of a patch file:
    * patch       = { definition } ;
    * definition  = object | comment | link_list ;
    * 
    * object      = identifier , "=" , class , (parameters | whitespace | "end_of_file") ;
    * class       = "Capital" , { "letter" | "digit" | "underline" } ;
    * parameters  = "(" , "'", string,  "'", ")" ;
    * string      = { "any character except single quote" | "\'" } ;
    * 
    * comment     = "#" , "any character except end_of_line" , "end_of_line" ;
    * 
    * link_list   = "{" , [ws] , link , { [ws] , "," [ws] , link } , "}" ;
    * link        = identifier , "." , n , [ws] , "=>" , [ws] , n , "." , identifier
    * 
    * n           = "digit_exculding_zero" , { "digit" }
    * identifier  = "letter" { "letter" | "digit" | "underline" }
    * ws          = { "spaces or newlines" }
    */
  bool Network::parse_file(const std::string& pPath);
  
  
  /*** DATA ***/
  Hash<uint, Node*> mNodesById; /**< All nodes in the system, indexed by 'id'. */
  Hash<std::string, Node*> mNodesByName; /**< All nodes in the system, indexed by their patch name. */
  std::list<Node *> mNodesList; /**< All nodes in the system, list. */
};

#endif