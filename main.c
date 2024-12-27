//including libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

	//variables
	int c2 = 0; //unused
	int choice,n;
	int inbookno;
	char inbookname[100];
	char inauthorname[100];
	int inpubyear;
	char inavail[10];
	
	//initialising file handler variable
	FILE* fptr;


struct bookinfo{ //defining structure 
	int bookno; //stores book id
	char bookname[100]; //stores book name
	char authorname[100]; //stores author name
	int pubyear; //store publication year
	char avail[10]; //stores availability
};
struct bookinfo book[1501]; //initiasing array to store books 

//function to keep count of number of books
void bookcount() {
    fptr = fopen("main.txt", "r");
    if (fptr == NULL) {
        c2 = 0; // No books in file
        return;
    }
    char line[256];
    c2 = 0;
    while (fgets(line, sizeof(line), fptr)) //while loop will run till it fgets is true
	{
        if (strstr(line, "Book Number:") != NULL) //check for substring in string inside the file
		{
            c2++; // Count each "Book Number" entry
        }
    }
    fclose(fptr); //close the file
}



//function to add a book
void addbook(int bookno,char bookname[100],char authorname[100],int pubyear,char avail[10], FILE* fptr){
	if (bookno < 0 || bookno >= 1501 || book[bookno].bookno != 0) //check if details for the books already exists
	{
    printf("\nBook details for book %d already exist!", bookno);
    return;
	}
	else{ 
		//adding details to the structure
		book[bookno].bookno = bookno;
		strcpy(book[bookno].bookname,bookname);
		strcpy(book[bookno].authorname,authorname);
		book[bookno].pubyear = pubyear;
		strcpy(book[bookno].avail,avail);
		printf("\nOperation Done!");
		
		// adding details to main.txt
		fprintf(fptr, "\nBook Number: %d\n", bookno);
    	fprintf(fptr, "Book Name: %s", bookname);
    	fprintf(fptr, "Author Name: %s", authorname);
    	fprintf(fptr, "Publication Year: %d", pubyear);
    	fprintf(fptr, "\nAvailability: %s", avail);
    	fprintf(fptr, "----------------------------------\n");
		}
		c2++; //increament of book count (c2) after addition of new book
	}
		
//function to search book through book name
void searchbook(char bookname[]) {
    fptr = fopen("main.txt", "r"); //opening file in read mode
    if (fptr == NULL) //check for any error in opening file
	{
    	printf("\nFile does not exist.");
    	return;
		}
    char line[256]; //variable to store lines read from the file
    bool found = false, isBook = false;
    int bookID = 0;

    printf("\nSearching for the book...\n");
    while (fgets(line, sizeof(line), fptr)) //extract lines from the file
	 {
        if (strstr(line, "Book Number:") != NULL) //checking for substring in string inside file
		{
            bookID++; // Increment Book ID for each book in the file
        }
        //checking for substring in  string inside the file and checking bookname as substring inside the lines of the file
        if (strstr(line, "Book Name:") != NULL && strstr(line, bookname) != NULL)
		{
            found = true; //found
            isBook = true;
            printf("\nBook Found with Book ID: %d\n", bookID);
        }
        if (isBook) //runs when booksname is found in the file 
		{
            printf("%s", line); // Print all lines belonging to the book
            if (strstr(line, "----------------------------------") != NULL) {
                isBook = false; // End of current book details
            }
        }
    }
    if (!found) //runs when booksname is not found in the file
	{
        printf("\nBook not found.\n");
    }
    fclose(fptr); //close the file
}

//function to update book details
// Function to update book details
void updatebookdetails() {
    int bookID;
    char bookname[100];
    char newAvailability[10];
    bool found = false;
    
    // Prompt user to choose search method
    printf("\nUpdate Book Details\n");
    printf("1. Update by Book ID\n");
    printf("2. Update by Book Name\n");
    printf("Choose an option: ");
    int option;
    scanf("%d", &option); //storing the input in the variable 'option'
    getchar(); // Consume newline

    if (option == 1) //runs when option 1 is chosen
	{
        printf("\nEnter Book ID: ");
        scanf("%d", &bookID); //stores the input in the variable 'bookID'
        getchar(); // Consume newline
    } else if (option == 2) //runs when option 2 is chosen
	{
        printf("\nEnter Book Name: ");
        fgets(bookname, sizeof(bookname), stdin); //stores the input in the variable 'bookname'
        bookname[strcspn(bookname, "\n")] = '\0'; // Remove newline
    } else //runs when none of the option is chosen or invalid input is entered
	{
        printf("\nInvalid option. Returning to main menu.\n");
        return; //returns from the function
    }

    // Open file for reading and a temporary file for writing
    fptr = fopen("main.txt", "r"); //opening the file in read mode
    FILE *tempFile = fopen("temp.txt", "w"); //creating and opening temporary file in write mode
    if (fptr == NULL || tempFile == NULL) //checks for any error in opening the files
	{
        printf("\nError opening file.");
        return; //returns from the function
    }
    char line[256];
    bool isBook = false;
    int currentBookID = 0;
    while (fgets(line, sizeof(line), fptr)) //runs till fgets gets lines from the file
	{
        if (strstr(line, "Book Number:") != NULL) //checks if "Book Number" is a substring of the line from the file
		{
            currentBookID++; // Increment Book ID for each book
        }

        if ((option == 1 && currentBookID == bookID) ||
            (option == 2 && strstr(line, "Book Name:") != NULL && strstr(line, bookname) != NULL)) //checks for the substring
			{
            found = true;
            isBook = true;
            fprintf(tempFile, "%s", line); // Write the line to temp file

            // Update availability when "Availability:" is found
        } else if (isBook && strstr(line, "Availability:") != NULL) //checks for "Availability" as substring of line
		{
            printf("\nEnter new availability status (yes/no): ");
            fgets(newAvailability, sizeof(newAvailability), stdin); //stores the new string value 
            newAvailability[strcspn(newAvailability, "\n")] = '\0'; // Remove newline
            fprintf(tempFile, "Availability: %s\n", newAvailability); // Write updated availability
            isBook = false;
        } else {
            fprintf(tempFile, "%s", line); // Copy all other lines
        }
    }

    fclose(fptr); //close the file
    fclose(tempFile); //close the file

    if (!found) //file not found
	{
        printf("\nBook not found.\n");
        remove("temp.txt"); //removes temporary file
        return; //returns from the function
    }

    // Replace original file with updated file
    remove("main.txt");
    rename("temp.txt", "main.txt");

    printf("\nBook details updated successfully!\n");
}

//function to remove a book's details
void deletebook() {
    int bookID;
    char bookname[100];
    bool found = false;

    // Prompt user to choose search method
    printf("\nDelete a Book\n");
    printf("1. Delete by Book ID\n");
    printf("2. Delete by Book Name\n");
    printf("Choose an option: ");
    int option;
    scanf("%d", &option); //stores input from the user to "option"
    getchar(); // Consume newline

    if (option == 1) //runs if option 1 is chosen
	{
        printf("\nEnter Book ID: ");
        scanf("%d", &bookID); //storees input from the user to "bookID"
        getchar(); // Consume newline
    } else if (option == 2) //runs if option 2 is chosen
	{
        printf("\nEnter Book Name: ");
        fgets(bookname, sizeof(bookname), stdin); //stores the string value
        bookname[strcspn(bookname, "\n")] = '\0'; // Remove newline
    } else //runs if none of the options is chosen or invalid input
	{
        printf("\nInvalid option. Returning to main menu.\n");
        return; //returns from the function
    }

    // Open file for reading and a temporary file for writing
    fptr = fopen("main.txt", "r"); //opens the file in read mode
    FILE *tempFile = fopen("temp.txt", "w"); //creats and opens the file in write mode
    if (fptr == NULL || tempFile == NULL) //checks for errors in opening files
	{
        printf("\nError opening file.");
        return; //returns from the function
    }

    char line[256];
    bool isBook = false;
    int currentBookID = 0;
    int newBookID = 0;

    while (fgets(line, sizeof(line), fptr))//runs till fgets gets lines as inputs from the file 
	{
        if (strstr(line, "Book Number:") != NULL) //runs if Book Number is found as substring=
		{
            currentBookID++; // Increment Book ID for each book
        }

        if ((option == 1 && currentBookID == bookID) ||
            (option == 2 && strstr(line, "Book Name:") != NULL && strstr(line, bookname) != NULL)) {
            found = true;
            isBook = true;
            continue; // Skip the current book's details
        }

        if (isBook && strstr(line, "----------------------------------") != NULL) {
            isBook = false; // End of current book details
            continue;
        }

        if (!isBook) {
            // Rewrite book numbers sequentially
            if (strstr(line, "Book Number:") != NULL) {
                newBookID++;
                fprintf(tempFile, "Book Number: %d\n", newBookID);
            } else {
                fprintf(tempFile, "%s", line); // Copy all other lines
            }
        }
    }

    fclose(fptr); //close the file
    fclose(tempFile); //close the file

    if (!found) //runs when book not found
	{
        printf("\nBook not found.\n");
        remove("temp.txt"); //removes temporary file
        return; //retunrs from the function
    }

    // Replace original file with updated file
    remove("main.txt"); 
    rename("temp.txt", "main.txt");

    // Update book count
    bookcount();

    printf("\nBook deleted successfully and book numbers updated!\n");
}




// Function to display all books
void displaybooks() {
	fptr = fopen("main.txt","r"); //opening file in reading mode
	if (fptr == NULL) //runs if error in opening file
	{
    printf("\nFile does not exist.");
    return;
		}
	char line[100]; //variable to store lines from the file
	printf("\nDisplaying all the books!\n");
    if(fgets(line,sizeof(line),fptr) == NULL) //checks if there is details available to show
	{
        printf("\nNo books are currently stored.\n");
    } else {
        while(fgets(line,sizeof(line),fptr)) //extracts lines from the file
		{
        	printf("%s",line);
		}
    }
    fclose(fptr); //close the file
}

//main function 		
int main(){
	
	bookcount(); //initialising bookcount function to count the number of books in the file in the starting
	while(1) //loop to run the program continously untill user choose to exit 
	{
		
	//options
	printf("\nLibrary Management System\n");
	printf("\n1. Add a book.");
	printf("\n2. Search for a book.");
	printf("\n3. Update book details.");
	printf("\n4. Delete a book");
	printf("\n5. Display all books");
	printf("\n6. Exit\n");
	printf("\nChoose a option: ");
	scanf("\n%d",&choice); //stores user's choice from the options
	switch(choice)
	{
		case 1:
			fptr = fopen("main.txt","a+");
			if(fptr == NULL){
				printf("\nOperation Failed!");
				printf("\nFile does not exist or File creation failed.");	
			}
			else{
				printf("\nHow many book you want to add?: ");
				scanf("\n%d",&n);\
				getchar(); //to consume new line
			for(int i = 1;i<=n && i<=(c2+n);i++){
				if (c2 + n > 1501) {
    				printf("\nMaximum books limit has been reached!");
    				break;
					}
				else{
				printf("\nEnter details of the book");
				inbookno = i+c2;
				printf("\nName of the book: ");
				fgets(inbookname,sizeof(inbookname),stdin);
				printf("\nAuthor name: ");
				fgets(inauthorname,sizeof(inauthorname),stdin);
				printf("\nPublication Year: ");
				scanf("\n%d",&inpubyear);
				getchar(); //to consume new line 
				printf("\nAvailability of the book(yes/no): ");
				fgets(inavail,sizeof(inavail),stdin);
				addbook(inbookno,inbookname,inauthorname,inpubyear,inavail,fptr);
				c2 = i + c2;
				}
			}
			fclose(fptr);
			break;
		case 2:
    		printf("\nSearching for a book...");
    		getchar(); // Consume newline
    		printf("\nEnter Book Name to search: ");
    		fgets(inbookname, sizeof(inbookname), stdin);
    		inbookname[strcspn(inbookname, "\n")] = '\0'; // Remove newline character
    		searchbook(inbookname); //calling function to search for the book
    		break; //break the case
		case 3:
			updatebookdetails(); //calling function to update book details
			break; //break the case
		case 4:
			deletebook(); //calling delete book function
			break; //break the case
		case 5:
			displaybooks(); //calling function to display all books
			break; //break the case
		case 6:
			printf("\nExiting the program!");
			return 0; //this will exit the function as it returns 0 and while will break
		}
	}
	
}
}