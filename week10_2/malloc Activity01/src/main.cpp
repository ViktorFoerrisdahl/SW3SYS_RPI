#include <cstring>  // memset() defined here
#include <iostream>

int main(int argc, char* argv[]) {
  int nbytes = 2000 * 1024 * 1024; // # bytes to allocate

  char* mem = (char*)malloc(nbytes * sizeof(char)); // Allocate memory

  if (mem == nullptr) {  // If malloc() fails
    perror("malloc()");  // Print error
    exit(EXIT_FAILURE);  // Exit program
  } else { // If malloc() succeeds
    std::cout << (nbytes / 1024) / 1024 << " mB allocated\n";
    std::cout << "Memory allocated at " << (void*)mem << "\n";
  }

  std::cout << "Press ENTER to fill memory..."; getchar(); // Wait until keypress

  memset(mem, 0, nbytes);  // Fill memory with 0s

  std::cout << "Done.\nPress ENTER to free allocated memory..."; getchar(); // Wait until keypress

  free(mem); // Free allocated memory

  std::cout << "Memory freed\n";

  return 0;
}
