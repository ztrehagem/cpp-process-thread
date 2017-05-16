#include <iostream>

#define READ 0
#define WRITE 1

using namespace std;

void parent(int *fields) {
  close(fields[WRITE]);

  char c;
  while (read(fields[READ], &c, 1) > 0) {
    cout << c;
  }

  close(fields[READ]);
  cout << endl;
}

void child(int *fields, const char *str) {
  close(fields[READ]);

  for (int i = 0; str[i] != '\0'; i++) {
    char c = toupper(str[i]);
    write(fields[WRITE], &c, 1);
  }

  close(fields[WRITE]);
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    cout << "usage: ./capfork <string>" << endl;
    exit(EXIT_FAILURE);
  }

  int fields[2];
  pid_t pid;

  if (pipe(fields) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((pid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    child(fields, argv[1]);
  } else {
    parent(fields);
  }

  return 0;
}
