#include "mimas_application.h"

int main (int argc, char* argv[]) {
  return JUCE_NAMESPACE::JUCEApplication::main(argc, argv, new MimasApplication());
}