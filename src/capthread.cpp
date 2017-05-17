#include <iostream>
#include <pthread.h>

using namespace std;

void capitalize(void *str_) {
  char *str = (char *)str_;

  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
  }
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    cerr << "usage: " << argv[0] << " <string>" << endl;
    exit(EXIT_FAILURE);
  }

  pthread_t thread;
  if (pthread_create(&thread , NULL, (void *(*)(void *))capitalize , (void *)argv[1]) < 0) {
    perror("pthread_create");
    exit(EXIT_FAILURE);
  }
  pthread_join(thread, NULL);

  cout << argv[1] << endl;

  return 0;
}
