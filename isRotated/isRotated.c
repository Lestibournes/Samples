#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool isRotated(char* s1, char* s2) {
	if (strlen(s1) == strlen(s2)) {
		for (int r = 0; r < strlen(s1); r++) {
			for (int i = 0; i < strlen(s1); i++) {
				if (s1[i] != s2[(i + r) % strlen(s1)]) break;
				else if (i == strlen(s1) - 1) return true;
			}
		}
	}

	return false;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Requires 2 strings as arguments.\n");
		return 1;
	}

	printf("%s\n", isRotated(argv[1], argv[2]) ? "true" : "false");
}