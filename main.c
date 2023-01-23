//
//  main.c
//  program1
//
//  Created by Nils Streedain on 1/22/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {
	char *title;
	char *year;
	char *lang;
	char *rating;
	struct movie *next;
};

struct movie *createMovie(char *currLine) {
	struct movie *currMovie = malloc(sizeof(struct movie));
	
	char *saveptr;
	
	char *token = strtok_r(currLine, ",", &saveptr);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->title, token);
	
	token = strtok_r(NULL, ",", &saveptr);
	currMovie->year = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->year, token);
	
	token = strtok_r(NULL, ",", &saveptr);
	currMovie->lang = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->lang, token);
	
	token = strtok_r(NULL, "\n", &saveptr);
	currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->rating, token);
	
	currMovie->next = NULL;
	return currMovie;
}

struct movie *parseFile(const char *filePath) {
	struct movie *head = NULL;
	struct movie *tail = NULL;
	
	FILE *movieFile = fopen(filePath, "r");
	char *currLine = NULL;
	size_t len = 0;
	
	getline(&currLine, &len, movieFile);
	while (getline(&currLine, &len, movieFile) != -1) {
		struct movie *newNode = createMovie(currLine);
		
		if (head == NULL) {
			head = newNode;
			tail = newNode;
		} else {
			tail->next = newNode;
			tail = newNode;
		}
	}
	free(currLine);
	fclose(movieFile);
	return head;
}

int getLength(struct movie *list) {
	int length = 0;
	while (list != NULL) {
		length++;
		list = list->next;
	}
	return length;
}

void printMovieList(struct movie *list) {
	while (list != NULL) {
		printf("%s, %s, %s, %s\n", list->title, list->year, list->lang, list->rating);
		list = list->next;
	}
}

void listMovieListByYear(struct movie *list) {
	printf("Enter the year for which you want to see movies: ");
	int year, found = 0;
	scanf("%d", &year);
	while (list != NULL) {
		if (atoi(list->year) == year) {
			printf("%s\n", list->title);
			found = 1;
		}
		list = list->next;
	}
	if (!found)
		printf("No data about movies released in the year %d\n", year);
	printf("\n");
}

void listMovieListByLang(struct movie *list) {
	printf("Enter the language for which you want to see movies: ");
	int year, found = 0;
	scanf("%d", &year);
	while (list != NULL) {
		if (atoi(list->year) == year) {
			printf("%s\n", list->title);
			found = 1;
		}
		list = list->next;
	}
	if (!found)
		printf("No data about movies released in the year %d\n", year);
	printf("\n");
}

int main(int argc, const char * argv[]) {
	if (argc < 2) {
		printf("You must provide the name of the file to process\n");
		printf("Example usage: ./students student_info1.txt\n");
		return EXIT_FAILURE;
	}
	
	struct movie *list = parseFile(argv[1]);
	printf("Processed file %s and parsed data for %d movies\n", argv[1], getLength(list));
	
	while (1) {
		printf("1. Show movies released in the specified year\n2. Show highest rated movie for each year\n3. Show the title and year of release of all movies in a specific language\n4. Exit from the program\n\nEnter a choice from 1 to 4: ");
		
		int userInput;
		scanf("%d", &userInput);
		switch(userInput) {
			case 1:
				listMovieListByYear(list);
				break;
			case 2:
				printf("2\n");
				break;
			case 3:
//				printf("Enter the language for which you want to see movies: ");
				
				break;
			case 4:
				return EXIT_SUCCESS;
			default:
				printf("You entered an incorrect choice. Try again.\n\n");
		}
	}
}
