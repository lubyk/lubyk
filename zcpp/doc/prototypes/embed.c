// This is a stupid test to embed ruby into C
/* compile with 
gcc -o embed embed.c -I/opt/local/include -I/opt/local/lib/ruby/1.8/powerpc-darwin8.7.0 -I/opt/local/lib/ruby/site_ruby/1.8 -I. -lruby
*/
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ruby.h>
#include <signal.h>

static int running;

#include <signal.h>
#include <unistd.h>

void stop_running( int signal ) {
  running = 0;
}

// keep on looking at the modification date of the file 'embed.rb' and eval it if it changes
int main () {
  time_t test_mod_time = 0;
  struct stat attrib;
  int    load_function_sym;
  VALUE file_name;
  VALUE loader;
  int count = 0;
  // register stop_running
  signal( SIGINT, stop_running );

  
  ruby_init();
  ruby_init_loadpath();
  ruby_script("lubyk");
  rb_require("embed_loader.rb");
  
  file_name = rb_str_new2("embed.rb");
  loader    = rb_class_new_instance(0,0,rb_const_get(rb_cObject, rb_intern("Loader")));
  load_function_sym = rb_intern("secure_load");

  running = 1;
  
  while(running) {
    sleep(0.1);
    count = count + 1;
    // get attributes
    stat("embed.rb", &attrib);
    if (test_mod_time != attrib.st_mtime) {
      printf("Finally a change (%i)\n", count);
      count = 0;
      test_mod_time = attrib.st_mtime;
      rb_funcall(loader, load_function_sym, 1, file_name);
    }
  }
  printf("\nbye...\n");
  ruby_finalize();
  exit(0);
}