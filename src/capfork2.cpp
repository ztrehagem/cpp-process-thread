#include <iostream>

#define READ 0
#define WRITE 1

using namespace std;

void parent(int *ifields, int *ofields, const char *str) {
  close(ifields[READ]);
  close(ofields[WRITE]);

  for (int i = 0; str[i] != '\0'; i++) {
    // cout << "parent write : " << str[i] << endl;
    write(ifields[WRITE], &str[i], 1);
  }

  close(ifields[WRITE]);

  char c;
  while (read(ofields[READ], &c, 1) > 0) {
    // cout << "parent read : " << c << endl;
    cout << c;
  }

  close(ofields[READ]);
  cout << endl;
}

void child(int *ifields, int *ofields) {
  close(ifields[WRITE]);
  close(ofields[READ]);

  char c;
  while (read(ifields[READ], &c, 1) > 0) {
    // cout << "child read : " << c << endl;
    c = toupper(c);
    // cout << "child write : " << c << endl;
    write(ofields[WRITE], &c, 1);
  }

  close(ifields[READ]);
  close(ofields[WRITE]);
}

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    cout << "usage: ./capfork2 <string>" << endl;
    exit(EXIT_FAILURE);
  }

  int ifields[2], ofields[2];
  pid_t pid;

  if (pipe(ifields) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (pipe(ofields) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((pid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    child(ifields, ofields);
  } else {
    parent(ifields, ofields, argv[1]);
  }

  return 0;
}
