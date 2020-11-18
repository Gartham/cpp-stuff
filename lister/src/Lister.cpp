#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define VER "0.1"

void list(FILE *fh) {

	char c;
	while ((c = fgetc(fh)) != EOF) {
		printf("[\u001B");
		switch (c) {
		case 'w':
			printf("[33m\u25FC");
			break;
		case 'n':
			printf("[31m ");
			break;
		case 'f':
			printf("[32mx");
			break;
		default:
			printf("[97m~");
			break;
		}
		printf("] ");

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
		END_ENTRY: printf("\u001B[97m\n");
	}

	fclose(fh);
}

void append(FILE *fh, char *item, char status) {
	fputs("\n", fh);
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
		} else if (tolower(argc == 5)) {
			if (args[2][0] == '-') {

				switch (args[2][1]) {
				case 'a': {
					FILE *fp = fopen(args[1], "a");
					append(fp, args[3], args[4][0]);
				}
					break;
				case 'l':

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

