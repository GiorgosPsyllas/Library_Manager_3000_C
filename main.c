#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXBOOKS 100
#define MAXGENRES 34

int menuSelection();

void viewAll();

int newInputNum();
int newInputYear();
int newInputPages();
bool checkgenre();
void printgenres();

int deleteMenu();
void removeFromDb();

int updateBook();
int updateBookID();
int updateBookPages();
bool updateBookCheckgenre();
void updateBookDatabase();

void searchMenu();
void searchByID();
void searchByGenre();
void searchByPages();

void updateDatabase();

int checkFileLines();
void loadDatabase();

struct Book{
	int number;
	char title[30];
	char genre[25];
	int pages;
	int date;	
};


int main(void){
	
	bool running = true;
	int action, del_action, lines, i, tempbookID;
	int bookID = 0, bookYear = 0, bookPages = 0;
	char bookTitle[50], bookColor[25], bookGenre[25], bookGenreTemp[25];
	char decision;
	struct Book books[MAXBOOKS];


	loadDatabase(books);

	do{
		action = menuSelection();
		
		if(action > 6 || action < 0){
			system("cls");
			printf("Please enter a valid option.\n");
		}else if(action == 1){
			system("cls"); 
			loadDatabase(books);
			viewAll(books);
			system("cls");
		}
		else if(action == 2){
			system("cls"); 
			loadDatabase(books);
			bookID = newInputNum(books);
			running = true;
			getchar();
			do{
				printf("\t\t\t  Enter Book Title For New Entry\n");
				printf("\t\t\t\tEnter Title: ");
				gets(bookTitle);
				running = false;
			}while(running);
			
			for(i = 0; bookTitle[i] != '\0'; i++){
				if(bookTitle[i] == ' '){
					bookTitle[i] = '~';
				}
			}
			
			running = true;
			
			do{
				printf("\t\t\t  Enter Book Genre For New Entry\n");
				printf("\t\t\tDo you want to see available genres\n\t\t\t\t     (Y/N): ");
				scanf(" %c", &decision);
				decision = toupper(decision);
				if(decision == 'Y'){
					printgenres();
				}else if(decision == 'N'){
					printf("\t\t\t\tEnter genre: ");
					scanf("%s", bookGenre);
					strcpy(bookGenreTemp, bookGenre);
					running = checkgenre(bookGenreTemp);
				}else{
					system("cls");
					printf("\t\t\t\tInvalid Decision try again.\n");
					getchar();
				}
			}while(running);
			
			bookPages = newInputPages();
			
			bookYear = newInputYear();
			
			updateDatabase(bookID, bookTitle, bookGenre, bookPages, bookYear);
			loadDatabase(books);
			
		}else if(action == 3){
			system("cls");
			loadDatabase(books);
			del_action = deleteMenu(books);
			removeFromDb(books,del_action);
			
		}else if(action == 4){ 
			system("cls");
			loadDatabase(books);
			lines = checkFileLines("books.txt");
			bookID = updateBook(books);
			
			for(i = 0; i < lines; i++){
				if(bookID == books[i].number){
					bookID = books[i].number;
					strcpy(bookTitle, books[i].title);
					strcpy(bookGenre, books[i].genre);
					bookYear = books[i].date;
					bookPages = books[i].pages;
					break;
				}
			}
			tempbookID = bookID;

			bookID = updateBookNum(books, bookID);
			
			running = true;
			
			do{
				
				printf("\t\t\tDo you want to see available gernes (Y/N): ");
				getchar();
				scanf(" %c", &decision);
				decision = toupper(decision);
				if(decision == 'Y'){
					printf("\n");
					printgenres();
					
				}else if(decision == 'N'){
					
				printf("\t\t\tEnter genre (Current %s):", bookGenre);
				scanf("%s", &bookGenre);
				strcpy(bookGenreTemp, bookGenre);
				running = checkgenre(bookGenreTemp);
				running = updateBookCheckgenre(bookGenreTemp);
				}else{
					printf("\t\t\t    Invalid Decision try again.\n");
				}
				
			}while(running);
			
			bookYear = updateBookYear(bookYear);
			bookPages = updateBookPages(bookPages);
			
			updateBookDatabase(books, tempbookID, bookID, bookTitle, bookGenre, bookYear, bookPages);
			printf("\t\t\tSuccessfully Updated Record\n");
			
		}else if(action == 5){
			system("cls");
			loadDatabase(books);
			searchMenu(books);
		}
	}while(action != 0);
}


int menuSelection(){

    char user_input = ' ';
    int action;
    bool running = true;
    
    system("cls");
    
    do{
    printf("\t\t\t Welcome to Library Manager 3000\n");
    printf("\t\t\t    'Chapter 0: The Menu'\n\n");
    printf("\t\t    __________________   __________________\n");
    printf("\t\t.-/|                  \\ /                  |\\-.\n");
    printf("\t\t||||                   '                   ||||\n");
    printf("\t\t||||                                       ||||\n");
    printf("\t\t||||            1. Show Library            ||||\n");
    printf("\t\t||||            2. Insert Book             ||||\n");
    printf("\t\t||||            3. Delete Book             ||||\n");
    printf("\t\t||||            4. Rewrite Book            ||||\n");
    printf("\t\t||||            5. Search Book(s)          ||||\n");
    printf("\t\t||||            0. Exit Program            ||||\n");
    printf("\t\t||||                                       ||||\n");
    printf("\t\t||||                                       ||||\n");
    printf("\t\t||||__________________   __________________||||\n");
    printf("\t\t||/===================\\_/===================\\||\n");
    printf("\t\t`--------------------~___~-------------------''\n");
   
	
	 	printf("\n\n");
	 	printf("\t\t\t       Enter your choice: ");
	    scanf("%c", &user_input); 
		
		if(isdigit(user_input) == 1){ 
			switch(user_input){
				case '0':
					action = 0;
					running = false;
					break;
				case '1':
					action = 1;
					running = false;
					break;
				case '2':
					action = 2;
					running = false;
					break;
				case '3':
					action = 3;
					running = false;
					break;
				case '4':
					action = 4;
					running = false;
					break;
				case '5':
					action = 5;
					running = false;
					break;
				default:
					system("cls");
					break;
				}
					
		}else{
			system("cls");
		}
	}while(running);


 	return action;   
}


void viewAll(){
	
	int i, j, lines;
	char title[50], color[25], genre[25];
	int num, year, pages;
	
	FILE *file = fopen("books.txt", "r");
	
	lines = checkFileLines("books.txt");
    printf("\t\t\t     Welcome to Library Manager 3000\n");
    printf("\t\t\t     'Chapter 1: Library Showcase'\n\n");

	

	for(i = 0; i < lines; i++){
		fscanf(file,"%d %s %s %d %d", &num, title, genre, &pages, &year);
		for(j = 0; title[j] != '\0'; j++){
			if(title[j] == '~'){
				title[j] = ' ';
			}
		}
		if(i == 0){
			printf("\t\t\t\t\t!~ %dst Book ~!\n", i+1);
		}else if(i == 1){
			printf("\t\t\t\t\t!~ %dnd Book ~!\n", i+1);
		}else if(i == 2){
			printf("\t\t\t\t\t!~ %drd Book ~!\n", i+1);
		}else{
			printf("\t\t\t\t\t!~ %dth Book ~!\n", i+1);
		}
		printf("\t\t\t     ____\n");
		printf("\t\t\t    /////|\n");
		printf("\t\t\t   ///// |   ID: %d\t\t\t\n", num);
		printf("\t\t\t  |~~~|  |   Title: %s\n", title);
		printf("\t\t\t  |===|  |   Genre: %s\n", genre);
		printf("\t\t\t  |   |  |   Total Pages: %d\n", pages);
		printf("\t\t\t  |   |  |   Year Published: %d\n", year);
		printf("\t\t\t  |   | /\n");
		printf("\t\t\t  |===|/\n");
		printf("\t\t\t  '---'\n");
		printf("\n");

	}

	printf("\n\n");
	printf("\t\t\tPress any key to return to main menu\n\t\t\t\t\t");
	system("pause > nul");

	fclose(file);

	return;
}

int newInputNum(struct Book *books){

	bool running = true, duplicate = false;
	int i, j, v = 0, temp, bookID, lines;
	char decision;
	
	lines = checkFileLines("books.txt");
	
	int takenNumbers[lines];
	
	for(i = 0; i < lines; i++){
		takenNumbers[i] = books[i].number;
	}

	for(i = 0; i < lines; i++){
		for(j = 0; j < lines - 1; j++){
			if(takenNumbers[j] > takenNumbers[j+1]){		
				temp = takenNumbers[j];
				takenNumbers[j] = takenNumbers[j+1];
				takenNumbers[j+1] = temp;
			}
		}
	}

	do{
		
		printf("\t\t\t Welcome to Library Manager 3000\n");
    	printf("\t\t\t    'Chapter 2: Inserting a Book'\n\n");
		printf("\t\t\t\tEnter Book's ID\n");
		printf("\t\t\t(Acceptable ID Numbers 100-999)\n");
		printf("\t\t\tDo you want to see already existing IDs\n\t\t\t\t     (Y/N): ");
		scanf(" %c", &decision);
		decision = toupper(decision);
		if(decision == 'Y'){
			system("cls");
			printf("\t\t\t\tAlready existing IDs:\n");
			printf("\t\t\t");
			for(i = 0; i < lines; i++){
				printf("%d", takenNumbers[i]);
				if(i +1 != lines){
					printf(", ");
				}
				if(v == 7){
					printf("\n\t\t\t");
					v = 0;
				}
				v += 1;
			}
			printf("\n\n\t\t\tPress any key to go back to inserting.\n\t\t\t\t\t");
			system("pause > nul");
			system("cls");
		}else if(decision == 'N'){
			system("cls");
			printf("\t\t\t\tEnter Book's ID\n");
			printf("\t\t\t(Acceptable ID Numbers 100-999)\n");
			printf("\t\t\t\tEnter ID: ");
			scanf("%d", &bookID);
		
			
			duplicate = false; 
			for(i = 0; i < lines; i++){
				if(bookID == books[i].number){
					system("cls");
					printf("\t\t\tRecord with ID %d already exists.\n\n", bookID);
					duplicate = true; 
				}	
			}
			if(bookID > 99 && bookID < 1000 && duplicate == false){
				running = false;
			}
			if(bookID < 99 || bookID > 1000){
				printf("\t\t\t    Invalid ID please try again.\n");
			}
		}else{
			system("cls");
			printf("\t\t\t    Invalid Decision try again.\n");
			getchar();
		}

	}while(running);
	
	return bookID;
	
}

int newInputYear(){
	
	bool running = true;
	int bookYear;
	do{
		printf("\t\t\t     Enter Book's Year Published\n");
		printf("\t\t\t   (Acceptable Years 1900 to 2024)\n");
		printf("\t\t\t\tEnter Year: ");
		if(scanf("%d", &bookYear) == 1){
			if(bookYear >= 1900 && bookYear <= 2024){
			running = false;
		}
		}else{
			printf("\n\t\t\tEnter NUMBERS only! (1900 to 2024)\n");
			while (getchar() != '\n');
		}
	}while(running);
	
	return bookYear;
}


int newInputPages(){
	
	bool running = true;
	int bookPages;
	do{
		printf("\t\t\t\tEnter Book's Pages\n");
		printf("\t\t\t   (Acceptable Pages 1 to 1500)\n");
		printf("\t\t\t\tEnter Total Pages: ");
		if(scanf("%d", &bookPages) == 1){
			if(bookPages >= 1 && bookPages <= 1500){
			running = false;
		}
		}else{
			printf("\n\t\t\tEnter NUMBERS only! (1 to 1500)\n");
			while (getchar() != '\n');
		}
	}while(running);
	
	return bookPages;
}


bool checkgenre(char *genre){ 

	bool pass = true; 
	int i, j, lines;
	char genreToCheck[25];
	
	FILE *file = fopen("genres.txt", "r");
	lines = checkFileLines("genres.txt");	
	
	for (i = 0; genre[i] != '\0'; i++) {
        genre[i] = toupper(genre[i]);
    }
	
	for(i = 0; i <= MAXGENRES; i++){
		fscanf(file, "%s", genreToCheck);
		for(j = 0; genreToCheck[j] != '\0'; j++){
			
			genreToCheck[j] = toupper(genreToCheck[j]);
		}
		if (strcmp(genreToCheck, genre) == 0) { 
			pass = false;
		    break;
		    }
	}
	if(pass == true){ 
		printf("\t\t\t    Invalid Book Genre.\n");
		getchar();
	}
	fclose(file);
	return pass;
	
}


void printgenres(){
	
	int i, v = 1, lines;
	char genre[25];
	
	FILE *file = fopen("genres.txt", "r");
	
	lines = checkFileLines("genres.txt");
	
	printf("\t\t\t\tAvailable Book Genres:\n");
	printf("\n\t\t\t");
	for(i = 0; i <= MAXGENRES; i++){
		fscanf(file, "%s", &genre);
		printf("%s", genre);
		if(i + 1 != MAXGENRES+1){
			printf(", ");
		}
		if(v == 4){
			printf("\n\t\t\t");
			v = 0;
		}
		v += 1;
	}
	printf("\n\n");
	fclose(file);
	
	return;
	
}


int deleteMenu(struct Book *books){
	
	int i, j, v = 0, lines, temp, action;
	bool found = true;
	
	system("cls");
	printf("\t\t\t Welcome to Library Manager 3000\n");
    printf("\t\t\t    'Chapter 3: Deleting a Book'\n\n");
	
	lines = checkFileLines("books.txt");
	int arrayNumber[lines];

	for(i = 0; i < lines; i++){ 
		arrayNumber[i] = books[i].number;
	}
	
	for(i = 0; i < lines - 1; i++){
		for(j = 0; j < lines - i - 1; j++){ 
			if(arrayNumber[j] > arrayNumber[j+1]){
				temp = arrayNumber[j];
				arrayNumber[j] = arrayNumber[j+1];
				arrayNumber[j+1] = temp;
			}
		}
	}
	do{
	printf("\t\t\t\tAvailable Books:\n\t\t\t");
	for(i = 0; i < lines; i++){
		printf("%d", arrayNumber[i]); 
		if(i +1 != lines){
					printf(", ");
				}
				if(v == 7){
					printf("\n\t\t\t");
					v = 0;
				}
				v += 1;
			}
	printf("\n\t\t\t  Select Book ID to delete: ");
	scanf("%d", &action);
	
	for(i = 0; i < lines; i++){
		if(action == books[i].number){
			found = false;
		}	
	}
	if(found == true){
		printf("\t\t\t\tThere is no Book with the ID: %d\n", action);
		getchar();
	}
	}while(found);

	return action; 

}


void removeFromDb(struct Book *books, int idToDel){ 
	
	int i, id, year, pages, lines; 
	char title[50], genre[25];
	
	FILE *file1 = fopen("books.txt", "r");
	FILE *file2 = fopen("temp.txt", "a");
	
	lines = checkFileLines("books.txt");
	
	for(i = 0; i < lines; i++){
		fscanf(file1,"%d %s %s %d %d\n", &id, title, genre, &pages, &year);
		if(idToDel != id){
			fprintf(file2,"%d %s %s %d %d\n", id, title, genre, pages, year);
			books[i].number = id;
			strcpy(books[i].title, title);
			strcpy(books[i].genre, genre);
			books[i].pages = pages;
			books[i].date = year;
		}
	}
	
	fclose(file1);
	fclose(file2);
	remove("books.txt");
	rename("temp.txt", "books.txt");
	sleep(1);
}


int updateBook(struct Book *books){
	
	int i, j, v = 0, lines, temp, action;
	bool found = false;
	char decision;
	system("cls");
	printf("\t\t\t Welcome to Library Manager 3000\n");
    printf("\t\t\t    'Chapter 4: Rewriting a Book'\n\n");
	printf("\t\t\t\t    Update Book\n");
	
	lines = checkFileLines("books.txt");
	
	int arrayNumber[lines];
	

	for(i = 0; i < lines; i++){
		arrayNumber[i] = books[i].number;
	}
	
	for(i = 0; i < lines - 1; i++){
		for(j = 0; j < lines - i - 1; j++){
			if(arrayNumber[j] > arrayNumber[j+1]){
				temp = arrayNumber[j];
				arrayNumber[j] = arrayNumber[j+1];
				arrayNumber[j+1] = temp;
			}
		}
	}
	do{
		printf("\t\t\tDo you want to see already existing IDs\n\t\t\t\t     (Y/N): ");
		scanf(" %c", &decision);
		decision = toupper(decision);
		if(decision == 'Y'){
			system("cls");
			printf("\t\t\t\tAvailable Book IDs:\n");
			printf("\t\t\t");
			for(i = 0; i < lines; i++){
				printf("%d", arrayNumber[i]);
				if(i +1 != lines){
					printf(", ");
				}
				if(v == 7){
					printf("\n\t\t\t");
					v = 0;
				}
				v += 1;
			}
			printf("\n\n\t\t\tPress any key to go back to inserting.\n\t\t\t\t\t");
			system("pause > nul");
			system("cls");
		}else if(decision == 'N'){	
			printf("\n\t\t\t     Select Book ID to update: ");
			scanf("%d", &action);
			
			for(i = 0; i < lines; i++){
				if(action == books[i].number){
					found = true;
				}	
			}
			if(found == false){
				printf("\t\t\t   There is no Book with the ID: %d\n", action);
			}
	} else{
		printf("\n\t\t\t\tInvalid Option!\n");
		getchar();
	}
	
	}while(!found);

	return action; 
	
}


int updateBookNum(struct Book *books, int currentNum ){

	bool running = true, duplicate = false;
	int i, bookID, lines;
	
	lines = checkFileLines("books.txt");

	do{
		printf("\t\t\t   New ID Number (Current %d):", currentNum);
		scanf("%d", &bookID); 
		duplicate = false;
		for(i = 0; i < lines; i++){
			if(bookID == books[i].number && bookID != currentNum){
				printf("\t\t\tBook with ID number %d already exists.\n", bookID);
				duplicate = true;
			}	
				
		}
		if(bookID > 99 && bookID < 1000 && duplicate == false){ 
			running = false;
		}
		if(bookID < 99 || bookID > 1000){ 
			printf("\t\t\tPlease enter a valid ID number (100 to 999).\n");
		}

	}while(running);
	
	return bookID; 
	
	
}

int updateBookYear(int* currentDate){
	
	bool running = true;
	int bookYear;
	
	do{
		printf("\n\t\t\t\tNew Year (Current %d): ", currentDate);
		if(scanf("%d", &bookYear) == 1){
			if(bookYear >= 1900 && bookYear <= 2024){	
				running = false;
			}else{
				printf("\t\t\tPlease enter a valid date (1900 to 2024).\n");
			}
		}else{
			printf("\t\t\tPlease enter a valid year (1900-2024)\n");
			while (getchar() != '\n');
		}
	
	}while(running);
	
	return bookYear;
	
}


int updateBookPages(int* currentPages){
	
	bool running = true;
	int bookPages;
	
	do{
		printf("\n\t\t\t\tNew Total Pages (Current %d): ", currentPages);
		if(scanf("%d", &bookPages) == 1){
			if(bookPages <= 1500 && bookPages >= 1){	
				running = false;
			}else{
				printf("\t\t\tPlease enter a valid page number (1 to 1500).\n");
			}
		}else{
			printf("\t\t\tPlease enter a valid page number (1 to 1500)\n");
			while (getchar() != '\n');
		}
	
	}while(running);
	
	return bookPages;
	
}


bool updateBookCheckgenre(char *genre){
	
	bool pass = true; 
	int i, j, lines;
	char genreToCheck[25];
	
	FILE *file = fopen("genres.txt", "r");
	lines = checkFileLines("genres.txt");	

	for(i = 0; i < lines; i++){
		fscanf(file, "%s", genreToCheck); 
		for(j = 0; genreToCheck[j] != '\0'; j++){
			
			genreToCheck[j] = toupper(genreToCheck[j]); 
		}
		if (strcmp(genreToCheck, genre) == 0) {
			pass = false;
		    break;
		    }
	}
	if(pass == true){ 
		printf("\t\t\tPlease enter a valid Book genre.\n");
	}
	fclose(file);
	return pass;
	
}

void updateBookDatabase(struct Book *books, int tempNum, int bookID, char* newTitle, char* newgenre, int bookYear, int bookPages) {
    int i, id, lines, year, pages;
    char title[30], genre[25];

	FILE *file2 = fopen("temp.txt", "a");

    lines = checkFileLines("books.txt");

    for (i = 0; i < lines; i++) {
        if (tempNum == books[i].number) {
            books[i].number = bookID;
            strcpy(books[i].title, newTitle);
            strcpy(books[i].genre, newgenre);
            books[i].date = bookYear;
            books[i].pages = bookPages;
        }
    }
    
    for(i = 0; i < lines; i++){
		id = books[i].number;
		strcpy(title, books[i].title);
		strcpy(genre, books[i].genre);
		pages = books[i].pages;
		year = books[i].date;
    	fprintf(file2,"%d\t%s\t%s\t%d\t%d\n", id, title, genre, pages, year);
    	
	}

	fclose(file2);
	remove("books.txt");
	rename("temp.txt", "books.txt");
	sleep(1);

	return;

}


void searchMenu(struct Book *books){
	
	bool running = true;
	char user_input;
	
	do{
		printf("\t\t\t Welcome to Library Manager 3000\n");
    	printf("\t\t\t    'Chapter 5: The Search'\n\n");	
	    printf("\t\t    __________________   __________________\n");
	    printf("\t\t.-/|                  \\ /                  |\\-.\n");
	    printf("\t\t||||                   '                   ||||\n");
	    printf("\t\t||||                                       ||||\n");
	    printf("\t\t||||                                       ||||\n");
	    printf("\t\t||||            1. Search By ID            ||||\n");
	    printf("\t\t||||            2. Search By Genre         ||||\n");
	    printf("\t\t||||            3. Search By Pages         ||||\n");
	    printf("\t\t||||            0. Back to Menu            ||||\n");
	    printf("\t\t||||                                       ||||\n");
	    printf("\t\t||||                                       ||||\n");
	    printf("\t\t||||                                       ||||\n");
	    printf("\t\t||||__________________   __________________||||\n");
	    printf("\t\t||/===================\\_/===================\\||\n");
	    printf("\t\t`--------------------~___~-------------------''\n");
		
	 	printf("\n\n");
	 	printf("\t\t\t       Enter your choice: ");
		scanf("%c", &user_input);
		
		if(isdigit(user_input) == 1){
			switch(user_input){
				case '0':
					getchar();
					running = false;
					break;
				case '1':
					getchar();
					searchByID(books);
					running = false;
					break;
				case '2':
					getchar();
					searchByGenre(books);
					running = false;
					break;
				case '3':
					getchar();
					searchByPages(books);
					running = false;
					break;
				default:
					getchar();
					system("cls");
					break;
				}		
		}else{
			system("cls");
		}
	}while(running);
	
	
}


void searchByID(struct Book *books){
	
	int i, j, v, lines, temp, num, year, pages, bookID;
	char title[50], genre[25];
	bool running = true, found = false;
	char decision;
	lines = checkFileLines("books.txt");
	
	int arrayNumber[lines];
	system("cls");

	for(i = 0; i < lines; i++){
		arrayNumber[i] = books[i].number;
	}
	
	for(i = 0; i < lines - 1; i++){
		for(j = 0; j < lines - i - 1; j++){ 
			if(arrayNumber[j] > arrayNumber[j+1]){
				temp = arrayNumber[j];
				arrayNumber[j] = arrayNumber[j+1];
				arrayNumber[j+1] = temp;
			}
		}
	}


	printf("\n");
	
	do{
		
		printf("\t\t\tDo you want to see already existing IDs\n\t\t\t\t     (Y/N): ");
		scanf(" %c", &decision);
		decision = toupper(decision);
		if(decision == 'Y'){
			system("cls");
			printf("\t\t\t\tAlready existing IDs:\n");
			printf("\t\t\t");
			for(i = 0; i < lines; i++){
				printf("%d", arrayNumber[i]);
				if(i +1 != lines){
					printf(", ");
				}
				if(v == 7){
					printf("\n\t\t\t");
					v = 0;
				}
				v += 1;
			}
			printf("\n\n\t\t\tPress any key to go back to inserting.\n\t\t\t\t\t");
			system("pause > nul");
			system("cls");
		}else if(decision == 'N'){
		printf("\t\t\t  Enter Book ID you want to search: ");		
		scanf("%d", &bookID);
		
		for(i = 0; i < lines; i++){
			if(bookID == books[i].number){
				num = books[i].number;
				strcpy(title, books[i].title);
				strcpy(genre, books[i].genre);
				year = books[i].date;
				pages =books[i].pages;
			for(j = 0; title[j] != '\0'; j++){
				if(title[j] == '~'){
					title[j] = ' ';
					}
				}
						
				printf("\t\t\t     ____\n");
				printf("\t\t\t    /////|\n");
				printf("\t\t\t   ///// |   ID: %d\t\t\t\n", num);
				printf("\t\t\t  |~~~|  |   Title: %s\n", title);
				printf("\t\t\t  |===|  |   Genre: %s\n", genre);
				printf("\t\t\t  |   |  |   Total Pages: %d\n", pages);
				printf("\t\t\t  |   |  |   Year Published: %d\n", year);
				printf("\t\t\t  |   | /\n");
				printf("\t\t\t  |===|/\n");
				printf("\t\t\t  '---'\n");
				printf("\n");
				running = false;
				break;
			}
		}
		
		if(true == false){
			printf("\t\t\t\t  Invalid Number.\n");
			printf("\t\t\t\t    Try Again.\n");
			
		}
		}else{
			system("cls");
			printf("\t\t\t    Invalid Decision try again.\n");
			getchar();
		}
		
		
	}while(running);
	
	printf("\t\t\t   End of search, press any key to continue\n\t\t\t\t\t   ");
	system("pause > nul");
	system("cls");

	return;
	
}


void searchByGenre(struct Book *books){
	
	int i, j, k, lines, inputLength;
	char decision, title[50], genreToSearch[25], genreToPrint[25], availablegenres[MAXBOOKS][25];
	bool pass = false, running = true;
	lines = checkFileLines("books.txt");

	int samegenreCheck[MAXGENRES] = {0};

	system("cls");
	printf("\t\t\t\tSearch Based on Genres\n");

	for(i = 0; i < lines; i++){
        strcpy(availablegenres[i], books[i].genre);
	}
	
	do {
	    printf("\t\t\t  Do you want to see available Genres (Y/N)\n\t\t\t\t\t");
	    scanf(" %c", &decision);
	    decision = toupper(decision);
		getchar();
			
	    if (decision == 'Y') {
	        system("cls");
	        printf("\t\t\t\tAvailable Genres:\n");
	
	        for (i = 0; i < lines; i++) {
	            if (!samegenreCheck[i]) {
	                strcpy(genreToPrint, availablegenres[i]);
	
	                for (k = 0; genreToPrint[k] != '\0'; k++) {
	                    genreToPrint[k] = tolower(genreToPrint[k]);
	                }
	                genreToPrint[0] = toupper(genreToPrint[0]);
	                printf("\t\t\t\t     %s,\n", genreToPrint);
	            }
	
	            for (j = i + 1; j < lines; j++) {
	                if (strcmp(availablegenres[i], availablegenres[j]) == 0) {
	                    samegenreCheck[j] = 1;
	                }
	            }
	        }
	        printf("\n\n\t\t\tPress any key to go back to inserting.\n\t\t\t\t\t");
			system("pause > nul");
			system("cls");
	    } else if (decision == 'N') {
	        system("cls");
	        printf("\t\t\t  Enter Genre to Search: ");
	        scanf("%s", genreToSearch);
	
	        for (i = 0; genreToSearch[i] != '\0'; i++) {
	            genreToSearch[i] = toupper(genreToSearch[i]);
	        }
	
	        for (i = 0; i < lines; i++) {
	            for (j = 0; availablegenres[i][j] != '\0'; j++) {
	                availablegenres[i][j] = toupper(availablegenres[i][j]);
	            }
	            if (strcmp(genreToSearch, availablegenres[i]) == 0) {
	                pass = true;
	                strcpy(genreToSearch, availablegenres[i]);
	                break;
	            }
	        }
	
	        printf("\t\t\t_______________________________________\n");
	
	        for (i = 0; i < lines; i++) {
	            strcpy(genreToPrint, books[i].genre);
				strcpy(title, books[i].title);
	            for (j = 0; genreToPrint[j] != '\0'; j++) {
	                genreToPrint[j] = toupper(genreToPrint[j]);
	            }
	            for(j = 0; title[j] != '\0'; j++){
					if(title[j] == '~'){
					title[j] = ' ';
					}
				}
	
	            if (strcmp(genreToSearch, genreToPrint) == 0) {
						printf("\t\t\t     ____\n");
						printf("\t\t\t    /////|\n");
						printf("\t\t\t   ///// |   ID: %d\n", books[i].number);
						printf("\t\t\t  |~~~|  |   Title: %s\n", title);
						printf("\t\t\t  |===|  |   Genre: %s\n", books[i].genre);
						printf("\t\t\t  |   |  |   Total Pages: %d\n", books[i].pages);
						printf("\t\t\t  |   |  |   Year Published: %d\n", books[i].date);
						printf("\t\t\t  |   | /\n");
						printf("\t\t\t  |===|/\n");
						printf("\t\t\t  '---'\n");
						printf("\n");
	            }
	        }
	        
	        printf("\t\t\t   End of search, press any key to continue\n\t\t\t\t\t   ");
			system("pause > nul");
			system("cls");
		    running = false;
		    break;
	    } else {
	        printf("\t\t\t    Invalid Decision. Try again.\n");
	        getchar();
	    }
	} while (running);

	printf("\n");
	system("cls");
	
	return;
}

void searchByPages(struct Book *books){
	
	int i, j, decision, lines;
	char title[50];
	bool running = true;
	
	lines = checkFileLines("books.txt");
	
	system("cls");
	
	do{
		printf("\t\t\t\t1. Short Stories (0-50)\n");
		printf("\t\t\t\t2. Short Novels (50-100)\n");
		printf("\t\t\t\t3. Standard Novels (100-300)\n");
		printf("\t\t\t\t4. Long Novels (300-600)\n");
		printf("\t\t\t\t5. Epic Novels (600+)\n");
		printf("\t\t\t\t0. Back To Search Menu\n");
		printf("\t\t\t\t  Enter your option: ");
		if(scanf("%d", &decision) == 1){
			if(decision == 0){
				running = false;
				break;
			}else if(decision == 1){
				printf("\n");
					for(i = 0; i < lines; i++){
						strcpy(title, books[i].title);
						for(j = 0; title[j] != '\0'; j++){
							if(title[j] == '~'){
							title[j] = ' ';
							}
						}
					if(books[i].pages <= 50){
						printf("\t\t\t     ____\n");
						printf("\t\t\t    /////|\n");
						printf("\t\t\t   ///// |   ID: %d\n", books[i].number);
						printf("\t\t\t  |~~~|  |   Title: %s\n", title);
						printf("\t\t\t  |===|  |   Genre: %s\n", books[i].genre);
						printf("\t\t\t  |   |  |   Total Pages: %d\n", books[i].pages);
						printf("\t\t\t  |   |  |   Year Published: %d\n", books[i].date);
						printf("\t\t\t  |   | /\n");
						printf("\t\t\t  |===|/\n");
						printf("\t\t\t  '---'\n");
						printf("\n");
					}
				}
				
				running = false;
			}else if(decision == 2){
				printf("\n");
				for(i = 0; i < lines; i++){strcpy(title, books[i].title);
						for(j = 0; title[j] != '\0'; j++){
							if(title[j] == '~'){
							title[j] = ' ';
							}
						}
					if(books[i].pages <= 100 && books[i].pages >= 50){
						printf("\t\t\t     ____\n");
						printf("\t\t\t    /////|\n");
						printf("\t\t\t   ///// |   ID: %d\n", books[i].number);
						printf("\t\t\t  |~~~|  |   Title: %s\n", title);
						printf("\t\t\t  |===|  |   Genre: %s\n", books[i].genre);
						printf("\t\t\t  |   |  |   Total Pages: %d\n", books[i].pages);
						printf("\t\t\t  |   |  |   Year Published: %d\n", books[i].date);
						printf("\t\t\t  |   | /\n");
						printf("\t\t\t  |===|/\n");
						printf("\t\t\t  '---'\n");
						printf("\n");
					}
				}
				
				running = false;
			}else if(decision == 3){
				printf("\n");
				for(i = 0; i < lines; i++){strcpy(title, books[i].title);
						for(j = 0; title[j] != '\0'; j++){
							if(title[j] == '~'){
							title[j] = ' ';
							}
						}
					if(books[i].pages <= 300 && books[i].pages >= 100){
						printf("\t\t\t     ____\n");
						printf("\t\t\t    /////|\n");
						printf("\t\t\t   ///// |   ID: %d\n", books[i].number);
						printf("\t\t\t  |~~~|  |   Title: %s\n", title);
						printf("\t\t\t  |===|  |   Genre: %s\n", books[i].genre);
						printf("\t\t\t  |   |  |   Total Pages: %d\n", books[i].pages);
						printf("\t\t\t  |   |  |   Year Published: %d\n", books[i].date);
						printf("\t\t\t  |   | /\n");
						printf("\t\t\t  |===|/\n");
						printf("\t\t\t  '---'\n");
						printf("\n");
					}
				}
				
				running = false;
			}else if(decision == 4){
				printf("\n");
				for(i = 0; i < lines; i++){strcpy(title, books[i].title);
						for(j = 0; title[j] != '\0'; j++){
							if(title[j] == '~'){
							title[j] = ' ';
							}
						}
					if(books[i].pages <= 600 && books[i].pages >= 300){
						printf("\t\t\t     ____\n");
						printf("\t\t\t    /////|\n");
						printf("\t\t\t   ///// |   ID: %d\n", books[i].number);
						printf("\t\t\t  |~~~|  |   Title: %s\n", title);
						printf("\t\t\t  |===|  |   Genre: %s\n", books[i].genre);
						printf("\t\t\t  |   |  |   Total Pages: %d\n", books[i].pages);
						printf("\t\t\t  |   |  |   Year Published: %d\n", books[i].date);
						printf("\t\t\t  |   | /\n");
						printf("\t\t\t  |===|/\n");
						printf("\t\t\t  '---'\n");
						printf("\n");
					}
				}
				
				running = false;
			}else if(decision == 5){
				printf("\n");
				for(i = 0; i < lines; i++){strcpy(title, books[i].title);
						for(j = 0; title[j] != '\0'; j++){
							if(title[j] == '~'){
							title[j] = ' ';
							}
						}
					if(books[i].pages >= 600){
						printf("\t\t\t     ____\n");
						printf("\t\t\t    /////|\n");
						printf("\t\t\t   ///// |   ID: %d\n", books[i].number);
						printf("\t\t\t  |~~~|  |   Title: %s\n", title);
						printf("\t\t\t  |===|  |   Genre: %s\n", books[i].genre);
						printf("\t\t\t  |   |  |   Total Pages: %d\n", books[i].pages);
						printf("\t\t\t  |   |  |   Year Published: %d\n", books[i].date);
						printf("\t\t\t  |   | /\n");
						printf("\t\t\t  |===|/\n");
						printf("\t\t\t  '---'\n");
						printf("\n");
					}
				}
				
				running = false;
			}else{
				printf("\t\t\t\t  Please enter numbers only (1-5)");
				getchar();
			}
		}else{
			printf("\t\t\t\t  Please enter numbers only (1-5)");
			getchar();
		}
		
		
	}while(running);
	
	system("pause > nul");
	
}



void updateDatabase(int* bookID, char* bookTitle[30], char* bookGenre[25], int* bookPages, int* bookYear){
	
	FILE *file = fopen("books.txt", "a");
	

	fprintf(file, "%d\t%s\t%s\t%d\t%d\n", bookID, bookTitle, bookGenre, bookPages, bookYear);
	printf("\t\t\t\tUpdate Completed!\a\n");
	printf("\t\t\t   (Press any key to continue)\n\t\t\t\t\t");
	system("pause > nul");
	fclose(file);

}


int checkFileLines(char *filename){
	
	FILE *file = fopen(filename, "r");
	
	if (file == NULL){
		
		printf("File doesn't exist!");	
		return;
	}
	char line[1000];
	char c;
	int count = 0;
	
	while((c = fgetc(file)) != EOF){
		if (c == '\n'){
			count += 1;
		}
	}
	fclose(file);
	return count;
}



void loadDatabase(struct Book *books){ 
	
	FILE *file = fopen("books.txt", "r"); 
	
	int i, bookID, bookPages, bookYear, lines;
	char bookTitle[50], bookGenre[25];
	
	lines = checkFileLines("books.txt");
	
	for(i = 0; i < lines; i++){ 
		
		fscanf(file,"%d %s %s %d %d\n", &bookID, bookTitle, bookGenre, &bookPages, &bookYear); 
		
		books[i].number = bookID;
		strcpy(books[i].title, bookTitle);
		strcpy(books[i].genre, bookGenre);
		books[i].pages = bookPages;
		books[i].date = bookYear;
	}
	
	fclose(file);
	
	return;
	
}



