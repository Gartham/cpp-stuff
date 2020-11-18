#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>

#define VER "0.1"
using namespace std;

void list(FILE *fh) {

	HANDLE ch = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(ch);
	char c;
	while ((c = fgetc(fh)) != EOF) {
		switch (c) {
		case 'w':
			SetConsoleTextAttribute(ch, FOREGROUND_BLUE);
			break;
		case 'n':
			SetConsoleTextAttribute(ch, FOREGROUND_RED);
			break;
		case 'f':
			SetConsoleTextAttribute(ch, FOREGROUND_GREEN | FOREGROUND_RED);
			break;
		default:
			SetConsoleTextAttribute(ch, 15);
			break;
		}

		fgetc(fh); // Handle comma.

		char buff[256];
		bool escaped = false;
		while (fgets(buff, 256, fh)) {
			for (int i = 0; buff[i]; i++) {
				switch (buff[i]) {
				case '\\':
					if (escaped)
						cout << '\\';
//						putchar('\\');
					escaped ^= true;
					break;
				case ',':
					if (escaped)
//						putchar(',');
						cout << ',';
					else
						// TODO Handle error?
//						putchar(',');
						cout << ',';
					break;
				case '\n':
					if (!escaped)
						goto END_ENTRY;
					// @suppress("No break at end of case")
				default:
//					putchar(buff[i]);
					cout << buff[i];
					break;
				}
			}
		}
		END_ENTRY: SetConsoleTextAttribute(ch, 15);
		cout << '\n';
//		printf("\n");
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
//		printf("Lister(ine) version %s.", VER);
		cout << "Lister(ine) version %s.";
	} else {
		if (argc == 2) {
			FILE *fp = fopen(args[1], "a+");
			if (!fp) {
//				printf("Failed to open the file.");
				cout << "Failed to open the file.";
			} else {
				prompt(fp);
			}
		} else if (argc > 2) {
			if (args[2][0] == '-') {

				switch (tolower(args[2][1])) {
				case 'a': {
					if (argc != 5) {
						HANDLE ch = GetStdHandle(STD_OUTPUT_HANDLE);
//						printf(
//								"To append, you need to provide the text to append (as one argument) followed by the status of the item, one of ");
						cout
								<< "To append, you need to provide the text to append (as one argument) followed by the status of the item, one of ";
						SetConsoleTextAttribute(ch, FOREGROUND_BLUE);
						cout << 'w';
//						putchar('w');
						SetConsoleTextAttribute(ch, 15);
//						printf(", ");
						cout << ", ";
						SetConsoleTextAttribute(ch, FOREGROUND_RED);
//						putchar('n');
						cout << 'n';
						SetConsoleTextAttribute(ch, 15);
//						printf(", or ");
						cout << ", or ";
						SetConsoleTextAttribute(ch, FOREGROUND_GREEN);
//						putchar('f');
						cout << 'f';
						SetConsoleTextAttribute(ch, 15);
//						putchar('.');
						cout << '.';
					} else {
						FILE *fp = fopen(args[1], "a");
						append(fp, args[3], args[4][0]);
					}
				}
					break;
				case 'l':
					if (argc != 3)
//						printf("This option takes no arguments.");
						cout << "This option takes no arguments.";
					else {
						FILE *fp = fopen(args[1], "r");
						list(fp);
					}
					break;
				default:
//					printf("Unknown argument: %s", args[2]);
					cout << "Unknown argument: " << args[2];
					break;
				}
			} else {
//				printf("Cannot understand arguments.");
				cout << "Cannot understand arguments.";
			}
		}
	}
	return 0;
}

