#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

int rentbookinc = 0, returnbookinc = 0;

struct user
{
    char surname[256];
    int userID, bookID, strikes, rtrn;
    float fee;
};

struct user rent[MAX];
struct user rtrn[MAX];

void rentBook()
{
    struct user user;
    printf("Please enter the renter's surname: ");
    scanf("%s", user.surname);
    printf("Please enter the renter's 5-digit ID: ");
    scanf("%d", &user.userID);
    printf("Please enter the book's 5-digit ID: ");
    scanf("%d", &user.bookID);
    getchar();
    if(user.userID > 99999)
    {
        printf("Error: User ID invalid.\n");
        return;
    }else if(user.bookID > 99999)
    {
        printf("Error: Book ID invalid.\n");
        return;
    }
    for(int x = 0; x < rentbookinc; x++)
    {
        if(rent[x].bookID == user.bookID && rent[x].rtrn == 0)
        {
            printf("Error: Book is already being rented.\n");
            return;
        }

        if(rent[x].strikes == 3)
        {
            printf("Sorry, this user is blacklisted.\n");
            return;
        }
    }
    user.rtrn = 0;
    user.strikes = 0;
    for(int x = 0; x < returnbookinc; x++)
    {
        if(rtrn[x].userID == user.userID)
        {
            user.strikes = rtrn[x].strikes;
        }
    }
    if(rentbookinc >= MAX)
    {
        printf("Error: Rental records are full.\n");
        return;
    }
    rent[rentbookinc] = user;
    rentbookinc++;
    printf("Book rented successfully.\n\n");
    return;
}

float calcLateFee(int days)
{
    float latepercent = 2.50, latefee;
    int rate = days - 14;
    latefee = rate*latepercent;
    return latefee;
}

void returnBook()
{
    struct user user;
    int daysrented, found=0, penalty = 0;
    printf("Please enter the renter's surname: ");
    scanf("%s", user.surname);
    printf("Please enter the renter's 5-digit ID: ");
    scanf("%d", &user.userID);
    printf("Please enter the book's 5-digit ID: ");
    scanf("%d", &user.bookID);
    getchar();
    printf("Please enter the number of days this book was rented for: ");
    scanf("%d", &daysrented);
    getchar();
    if(user.userID > 99999)
    {
        printf("Error: User ID invalid.\n");
        return;
    }else if(user.bookID > 99999)
    {
        printf("Error: Book ID invalid.\n");
        return;
    }

    for(int x=0;x<rentbookinc;x++)
    {
        if(strcmp(user.surname, rent[x].surname) == 0 && rent[x].userID == user.userID && rent[x].bookID == user.bookID && rent[x].rtrn == 0)
        {
            found = 1;
            rent[x].rtrn = 1;
            if(returnbookinc >= MAX)
            {
                printf("Error: Return records are full.\n");
                return;
            }
            if(daysrented>14)
            {
                user.fee = calcLateFee(daysrented);
                printf("User %s must pay $%.2f\n", user.surname, user.fee);
                penalty = 1;
            } else {
                user.fee = 0;
            }
            strcpy(rtrn[returnbookinc].surname, user.surname);
            rtrn[returnbookinc].userID = user.userID;
            rtrn[returnbookinc].bookID = user.bookID;
            rtrn[returnbookinc].rtrn = 1;
            rtrn[returnbookinc].fee = rtrn[returnbookinc].fee + user.fee;

            if(penalty == 1)
            {
                rtrn[returnbookinc].strikes = rent[x].strikes + 1;
            } else
            {
                rtrn[returnbookinc].strikes = rent[x].strikes;
            }
            returnbookinc++;
            printf("Book returned successfully.\n\n");
            break;
        }
    }
    if(found == 0)
    {
        printf("Error: User not found. \n");
    }

}

void generateReport()
{
    int rep_choice;
    FILE*fptr;
    fptr = fopen("library.txt", "w");
    if(fptr == NULL)
    {
        printf("Error, could not open file\n");
        return;
    }
    printf("=====================\n");
    printf("  REPORT GENERATION\n");
    printf("=====================\n");
    printf("     GENERATE:\n");
    printf(" 1. Rental Report\n");
    printf(" 2. Blacklist Report\n");
    printf(" Input: ");
    scanf("%d", &rep_choice);
    getchar();
    printf("\n");
    if (rep_choice == 2)
    {
        printf("=====================\n");
        printf("      BLACKLIST      \n");
        printf("=====================\n");
        fprintf(fptr, "=====================\n");
        fprintf(fptr, "      BLACKLIST      \n");
        fprintf(fptr, "=====================\n");
        for(int c=0;c<returnbookinc;c++)
        {
            if(rtrn[c].strikes >= 3)
            {
                printf("User: %s\nID: %d\nStrikes: %d\nAmt. Owed: %.2f\n", rtrn[c].surname, rtrn[c].userID, rtrn[c].strikes, rtrn[c].fee);
                fprintf(fptr, "User: %s\nID: %d\nStrikes: %d\nAmt. Owed: %.2f\n", rtrn[c].surname, rtrn[c].userID, rtrn[c].strikes, rtrn[c].fee);
                if(c+1!=returnbookinc)
                {
                    printf("\n");
                    fprintf(fptr, "\n");
                }
            }
        }
        printf("=====================\n");
    }else if(rep_choice == 1)
    {
        printf("=====================\n");
        printf("   CURRENT RENTALS   \n");
        printf("=====================\n");
        fprintf(fptr, "=====================\n");
        fprintf(fptr, "   CURRENT RENTALS   \n");
        fprintf(fptr, "=====================\n");
        for(int c=0;c<rentbookinc;c++)
        {
            if(rent[c].rtrn == 0)
            {
                printf("User: %s\nID: %d\nBook ID: %d\nStrikes: %d\nAmt. Owed: %.2f\n", rent[c].surname, rent[c].userID, rent[c].bookID, rent[c].strikes, rent[c].fee);
                fprintf(fptr, "User: %s\nID: %d\nBook ID: %d\nStrikes: %d\nAmt. Owed: %.2f\n", rent[c].surname, rent[c].userID, rent[c].bookID, rent[c].strikes, rent[c].fee);
                if(c+1!=rentbookinc)
                {
                    printf("\n");
                    fprintf(fptr, "\n");
                }
            }
        }
        printf("=====================\n");
        printf("\n\n");
        printf("=====================\n");
        printf("       RETURNS       \n");
        printf("=====================\n");
        fprintf(fptr, "=====================\n");
        fprintf(fptr, "\n\n");
        fprintf(fptr, "=====================\n");
        fprintf(fptr, "       RETURNS       \n");
        fprintf(fptr, "=====================\n");
        for(int c=0;c<returnbookinc;c++)
        {
            if(rtrn[c].rtrn == 1)
            {
                printf("User: %s\nID: %d\nBook ID: %d\nStrikes: %d\nAmt. Owed: $%.2f\n", rtrn[c].surname, rtrn[c].userID, rtrn[c].bookID, rtrn[c].strikes, rtrn[c].fee);
                fprintf(fptr, "User: %s\nID: %d\nBook ID: %d\nStrikes: %d\nAmt. Owed: $%.2f\n", rtrn[c].surname, rtrn[c].userID, rtrn[c].bookID, rtrn[c].strikes, rtrn[c].fee);
                if(c+1!=returnbookinc)
                {
                    printf("\n");
                    fprintf(fptr, "\n");
                }
            }
        }
        printf("=====================\n");
        fprintf(fptr, "=====================\n");
    }
    fclose (fptr);
}

int main()
{
    int choice = 0;
    choice = 0;

    while(choice != 4){
        printf("====================================\n");
        printf(" National Library Management System\n");
        printf("====================================\n");
        printf("      Would you like to:\n");
        printf("      1. Rent a book\n");
        printf("      2. Return a book\n");
        printf("      3. Generate report\n");
        printf("      4. Close program\n");
        printf("====================================\n");
        printf("Please enter your choice.\n");
        scanf("%d", &choice);
        getchar();
        printf("\n");
        switch (choice) {
            case 1:
                rentBook();
                break;
            case 2:
                returnBook();
                break;
            case 3:
                generateReport();
                break;
            case 4:
                printf("Closing...\n");
                break;
            default:
                printf("Invalid choice, try again\n");
        }
    }
    return 0;
}
