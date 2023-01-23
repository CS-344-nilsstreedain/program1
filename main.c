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

struct movie *processFile(char *filePath) {
	FILE *movieFile = fopen(filePath, "r");
	
	char *currLine = NULL;
	size_t len = 0;
	ssize_t nread;
	
	struct movie *head = NULL;
	struct movie *tail = NULL;
	
	while ((nread = getline(&currLine, &len, movieFile)) != -1) {
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

void printMovie(struct movie* aMovie) {
	printf("%s, %s, %s, %s\n", aMovie->title, aMovie->year, aMovie->lang, aMovie->rating);
}

void printMovieList(struct movie *list) {
	while (list != NULL) {
		printMovie(list);
		list = list->next;
	}
}

int main(int argc, const char * argv[]) {
	if (argc < 2) {
		printf("You must provide the name of the file to process\n");
		printf("Example usage: ./students student_info1.txt\n");
		return EXIT_FAILURE;
	}
	struct movie *list = processFile(argv[1]);
	printMovieList(list);
	
	return EXIT_SUCCESS;
}
