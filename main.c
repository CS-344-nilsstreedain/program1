/**
 * @file main.c
 * @author Nils Streedain
 * @date January 23, 2023
 * @brief This file contains a collection of functions that parse and manipulate movie
 * data read from a file, including creating a linked list of movie structs, finding the length
 * of the list, searching for movies by year, and listing the highest rated movie from each
 * year.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct movie
 * @brief Representation of a movie, including its title, year, languages, rating and a
 * pointer to the next movie in a linked list.
 * @var title		- Title of the movie
 * @var year 		- Year of the movie release
 * @var langs 	- Languages in which the movie is available
 * @var rating 	- Rating of the movie
 * @var next 		- Pointer to the next movie in a linked list
 */
struct movie {
	char *title;
	int year;
	char *langs;
	float rating;
	struct movie *next;
};


/**
 * @brief Create a movie struct from a string representing a line from a file
 * @param currLine - A string representing a line from a file, formatted as:
 * "title,year,languages,rating\n"
 * @return Pointer to a newly allocated movie struct, with its fields populated according
 * to the input string.
 */
struct movie *createMovieFromLine(char *currLine) {
	struct movie *currMovie = malloc(sizeof(struct movie));
	
	char *saveptr;
	
	char *token = strtok_r(currLine, ",", &saveptr);
	currMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->title, token);
	
	currMovie->year = atoi(strtok_r(NULL, ",", &saveptr));
	
	token = strtok_r(NULL, ",", &saveptr);
	currMovie->langs = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currMovie->langs, token);
	
	currMovie->rating = strtof(strtok_r(NULL, "\n", &saveptr), NULL);
	
	currMovie->next = NULL;
	return currMovie;
}

/**
 * @brief Parse a file containing movie data and create a linked list of movie structs
 * @param filePath - Path to the file containing movie data, formatted as:
 * "title,year,languages,rating\n" for each movie.
 * @return Pointer to the head of the linked list of movie structs
 */
struct movie *parseFile(const char *filePath) {
	struct movie *head = NULL;
	struct movie *tail = NULL;
	
	FILE *movieFile = fopen(filePath, "r");
	char *currLine = NULL;
	size_t len = 0;
	
	getline(&currLine, &len, movieFile);
	while (getline(&currLine, &len, movieFile) != -1) {
		struct movie *newNode = createMovieFromLine(currLine);
		
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

/**
 * @brief Calculate the length of a linked list of movie structs
 * @param list - Pointer to the head of the linked list of movie structs
 * @return The number of elements in the list
 */
int getLength(struct movie *list) {
	int length = 0;
	while (list != NULL) {
		length++;
		list = list->next;
	}
	return length;
}

/**
 * @brief Check if a movie is available in a specific language
 * @param movie - Pointer to a movie struct
 * @param lang - The language to check for
 * @return 1 if the movie is available in the specified language, 0 otherwise
 */
int movieInLang(struct movie *movie, char *lang) {
	char langs[100];
	strcpy(langs, movie->langs);
	
	char *saveptr;
	char *token = strtok_r(langs, "[;]", &saveptr);
	while (token != NULL) {
		if (!strcasecmp(token, lang))
			return 1;
		token = strtok_r(NULL, "[;]", &saveptr);
	}
	return 0;
}

/**
 * @brief List the titles of movies released in a specific year
 * @param list - Pointer to the head of the linked list of movie structs
 */
void listMoviesByYear(struct movie *list) {
	printf("Enter the year for which you want to see movies: ");
	int year, found = 0;
	scanf("%d", &year);
	while (list != NULL) {
		if (list->year == year) {
			printf("%s\n", list->title);
			found = 1;
		}
		list = list->next;
	}
	if (!found)
		printf("No data about movies released in the year %d\n", year);
	printf("\n");
}

/**
 * @brief Check if a movie is the highest rated movie of its year in a list of movies
 * @param movie - Pointer to a movie struct
 * @param list - Pointer to the head of the linked list of movie structs to check against
 * @return 1 if the movie is the highest rated movie of its year in the list, 0 otherwise
 */
int checkRatings(struct movie *movie, struct movie *list) {
	while (list != NULL) {
		if ((movie->year == list->year) && ((movie->rating < list->rating) || ((movie->rating == list->rating) && (strcmp(movie->title, list->title) < 0))))
			return 0;
		list = list->next;
	}
	return 1;
}

/**
 * @brief List the title of the highest rated movie of each year in a list of movies
 * @param list - Pointer to the head of the linked list of movie structs
 */
void listTopMovieEachYear(struct movie *list) {
	struct movie *listCopy = list;
	while (list != NULL) {
		if (checkRatings(list, listCopy))
			printf("%d, %.1f, %s\n", list->year, list->rating, list->title);
		list = list->next;
	}
	printf("\n");
}

/**
 * @brief List the title of the movies available in a specific language
 * @param list - Pointer to the head of the linked list of movie structs
 * @param lang - The language to check for
 */
void listMoviesByLang(struct movie *list) {
	printf("Enter the language for which you want to see movies: ");
	char lang[20];
	scanf("%s", lang);
	int found = 0;
	while (list != NULL) {
		if (movieInLang(list, lang)) {
			printf("%i %s\n", list->year, list->title);
			found = 1;
		}
		list = list->next;
	}
	if (!found)
		printf("No data about movies released in %s\n", lang);
	printf("\n");
}

/**
 * @brief The main function, it reads a file containing movie data, create a linked list of
 * movie structs and call other functions to parse and manipulate the data
 * @param argc - Number of command line arguments
 * @param argv - Array of command line arguments, argv[1] should be the filepath to
 * the movie data file
 * @return 0 if the program runs successfully, 1 otherwise
 */
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
				listMoviesByYear(list);
				break;
			case 2:
				listTopMovieEachYear(list);
				break;
			case 3:
				listMoviesByLang(list);
				break;
			case 4:
				return EXIT_SUCCESS;
			default:
				printf("You entered an incorrect choice. Try again.\n\n");
		}
	}
}
