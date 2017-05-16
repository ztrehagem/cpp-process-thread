#include <iostream>

#define READ 0
#define WRITE 1

using namespace std;

struct arg_t {
  char *target_str = nullptr;
  bool opt_explain = false;
} args;

void parent(int *ifields, int *ofields, const char *str) {
  close(ifields[READ]);
  close(ofields[WRITE]);

  for (int i = 0; str[i] != '\0'; i++) {
    if (args.opt_explain) {
      cout << "parent write : " << str[i] << endl;
    }
    write(ifields[WRITE], &str[i], 1);
  }

  close(ifields[WRITE]);

  char c;
  while (read(ofields[READ], &c, 1) > 0) {
    if (args.opt_explain) {
      cout << "parent read : " << c << endl;
    } else {
      cout << c;
    }
  }

  close(ofields[READ]);
  cout << endl;
}

void child(int *ifields, int *ofields) {
  close(ifields[WRITE]);
  close(ofields[READ]);

  char c;
  while (read(ifields[READ], &c, 1) > 0) {
    if (args.opt_explain) {
      cout << "child read : " << c << endl;
    }
    c = toupper(c);
    if (args.opt_explain) {
      cout << "child write : " << c << endl;
    }
    write(ofields[WRITE], &c, 1);
  }

  close(ifields[READ]);
  close(ofields[WRITE]);
}

int main(int argc, char const *argv[]) {
  for (int i = 1; i < argc; i++) {
    const char *arg = argv[i];
    size_t len = strlen(arg);
    if (len >= 1 && arg[0] == '-') {
      if (len >= 2 && arg[1] == '-') {
        if (strcmp(&arg[2], "explain") == 0) {
          args.opt_explain = true;
        }
      }
    } else {
      args.target_str = new char[len+1];
      strcpy(args.target_str, arg);
    }
  }

  if (args.target_str == nullptr) {
    cout << "usage: ./capfork <string> [--explain]" << endl;
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
    parent(ifields, ofields, args.target_str);
  }

  delete[] args.target_str;

  return 0;
}
