// build with
// $ g++ -lreadline readline.cpp -o test

#include <iostream>

extern "C" {
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
}

int main() {
  char * buffer;
  std::string history_path("./readline_history");
  // std::string history_path(getenv("HOME").append("/.your_super_application_history");
  read_history(history_path.c_str());
  
  while(strcmp("q", (buffer = readline("> ")) )) {
    if (buffer) {
      std::cout << "You typed '" << buffer << "'.\n";
    
      if (strlen(buffer) > 0) {
        add_history(buffer);
      }
      free(buffer);
    } else {
      std::cout << "Could not get input line.\n";
    }
  }
  
  write_history(history_path.c_str());
}