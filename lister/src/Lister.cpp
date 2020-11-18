#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>

#define VER "0.1"

void list(FILE *fh) {

	HANDLE ch = GetStdHandle(STD_OUTPUT_HANDLE);

	char c;
	while ((c = fgetc(fh)) != EOF) {
		switch (c) {
		case 'w':
			SetConsoleTextAttribute(ch, FOREGROUND_BLUE);
			printf("- [•");
			break;
		case 'n':
			SetConsoleTextAttribute(ch, FOREGROUND_RED);
			printf("- [ ");
			break;
		case 'f':
			SetConsoleTextAttribute(ch, FOREGROUND_GREEN | FOREGROUND_RED);
			printf("- [x");
			break;
		default:
			SetConsoleTextAttribute(ch, 15);
			break;
		}
		printf("] ");

		fgetc(fh); // Consume comma.

		char buff[256];
		bool escaped = false;
		while (fgets(buff, 256, fh)) {
			for (int i = 0; buff[i]; i++) {
				switch (buff[i]) {
				case '\\':
					if (escaped)
						putchar('\\');
					escaped ^= true;
					break;
				case ',':
					if (escaped)
						putchar(',');
					else
						// TODO Handle error?
						putchar(',');
					break;
				case '\n':
					if (!escaped)
						goto END_ENTRY;
					// @suppress("No break at end of case")
				default:
					putchar(buff[i]);
					break;
				}
			}
		}
		END_ENTRY: SetConsoleTextAttribute(ch, 15);
		printf("\n");
	}

	fclose(fh);
}

void append(FILE *fh, char *item, char status) {
	fputc(status, fh);
	fputc(',', fh);
	for (int i = 0; item[i]; i++)
		switch (item[i]) {
		case '\n':
			fputs("\\\n", fh);
			break;
		case ',':
			fputs("\\,", fh);
			break;
		default:
			fputc(item[i], fh);
			break;
		}
	fputc('\n', fh);
	fclose(fh);
}

void prompt(FILE *fh) {

}

int main(int argc, char *args[]) {
	if (argc == 1) {
		printf("Lister(ine) version %s.", VER);
	} else {
		if (argc == 2) {
			FILE *fp = fopen(args[1], "a+");
			if (!fp) {
				printf("Failed to open the file.");
			} else {
				prompt(fp);
			}
		} else if (argc > 2) {
			if (args[2][0] == '-') {

				switch (tolower(args[2][1])) {
				case 'a': {
					if (argc != 5) {
						HANDLE ch = GetStdHandle(STD_OUTPUT_HANDLE);
						printf(
								"To append, you need to provide the text to append (as one argument) followed by the status of the item, one of ");
						SetConsoleTextAttribute(ch, FOREGROUND_BLUE);
						putchar('w');
						SetConsoleTextAttribute(ch, 15);
						printf(", ");
						SetConsoleTextAttribute(ch, FOREGROUND_RED);
						printf("n");
						SetConsoleTextAttribute(ch, 15);
						printf(", or ");
						SetConsoleTextAttribute(ch, FOREGROUND_GREEN);
						printf("f");
						SetConsoleTextAttribute(ch, 15);
						putchar('.');
					} else {
						FILE *fp = fopen(args[1], "a");
						append(fp, args[3], args[4][0]);
					}
				}
					break;
				case 'l':
					if (argc != 3)
						printf("This option takes no arguments.");
					else {
						FILE *fp = fopen(args[1], "r");
						list(fp);
					}
					break;
				default:
					printf("Unknown argument: %s", args[2]);
					break;
				}
			} else {
				printf("Cannot understand arguments.");
			}
		}
	}
	return 0;
}

