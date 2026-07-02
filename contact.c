#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"
int check_number(char *new_num,AddressBook *addressBook);
int check_mail(char *new_mail,AddressBook *addressBook);
int check_name(char *new_name);
int match_index[100];

int gi=-1;
int gm=0;
void header()
{
    printf("\n%-10s %-20s %-20s %-25s\n","S.no","Name","Phone","Email");
}
void displayrow(AddressBook *addressBook,int i)
{
    printf("%-10d %-20s %-20s %-25s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
}
void listContacts(AddressBook *addressBook) //To list all the contact in the address book
{
    if(addressBook->contactCount==0)
    {
        printf("\n Address book is empty\n");
        return;
    }
    //printf("%-10s %-20s %-20s %-20s\n","S.no","Name","Phone","Email");
    header();
    for(int i=0;i<addressBook->contactCount;i++)
    {
        //printf("\n%-10d %-20s %-20s %-20s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        displayrow(addressBook,i); //passing the arguments to print the contents
    }
    // Sort contacts based on the chosen criteria
    
}

void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0;
    
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) 
{
      saveContactsToFile(addressBook); // Save contacts to file
      printf("Exiting program...............\n");
      exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
    int verify_name,verify_num,verify_mail;
    int index=addressBook->contactCount;
    int attempts = 0; 
    char new_name[100],new_num[100],new_mail[100];

    //for entering the Name
    do
    {
        printf("Enter the Name: ");
        scanf(" %[^\n]",new_name);
        
        verify_name = check_name(new_name);
        
        if (verify_name != 1)
        {
            attempts++; 
            
            if (attempts >= 5)
            {
                printf("\nError: Mximum 5 failed attempts reached. Terminating program...\n");
                exit(1); // Forcefully closes the entire program
            }

            printf("Invalid name! (Min 5 chars, no leading digits).\n");
            printf("Attempts left: %d\n", 5 - attempts);
        }

    } while (verify_name != 1);

    //for entering the phone number
    attempts=0;
    do
    {
        printf("Enter the number: ");
        scanf(" %[^\n]",new_num);
        verify_num=check_number(new_num,addressBook);
        if(verify_num != 1)
        {
            attempts++;

            if(attempts>=5)
            {
                printf("Too many failed attempts for Number");
                exit(1);
            }
            printf("Attempts left:%d\n",5-attempts);
        }
    }while(verify_num != 1);

    attempts=0;
    do
    {
        printf("Enter the Email: ");
        scanf(" %[^\n]",new_mail);
        verify_mail=check_mail(new_mail,addressBook);
        if(verify_mail != 1)
        {
            attempts++;

            if(attempts>=5)
            {
                printf("Too many failed attempts for Mail\n");
                exit(1);
            }
            printf("Attempts left:%d\n",5-attempts);
        }
    }while(verify_mail != 1);


    strcpy(addressBook->contacts[index].name, new_name);
    strcpy(addressBook->contacts[index].phone, new_num);
    strcpy(addressBook->contacts[index].email, new_mail);


    addressBook->contactCount++;

    printf("Contact successfully saved!\n");
}

//for checking the Name

int check_name(char *new_name)
{
    int len = strlen(new_name);

    if (isdigit(new_name[0]))
    {
        printf("First Character should not be a Digit!\n");
        return 0;
    }

    if (len < 5)
    {
        printf("Minimum 5 Characters required!\n");
        return 0;
    }

    for (int i = 0; i < len; i++)
    {
        if (i < len - 1)     //finding the last character
        {
            // Body: Letters and spaces only
            if (!(isalpha(new_name[i]) || new_name[i] == ' '))
            {
                printf("Digit can only be present at the end!\n");
                return 0;
            }
        }
        else
        {
            // Last char: Letters, spaces, OR digits allowed
            if (!(isalpha(new_name[i]) || isdigit(new_name[i]) || new_name[i] == ' '))
            {
                printf("No special Characters are allowded at the End!\n");
                return 0;
            }
        }
    }
    return 1;
}

//for checking the mobile number

int check_number(char *new_num,AddressBook *addressBook)
{
    int len=strlen(new_num);
    if(len!=10)
    {
        printf("Error : Number must contain 10 digit only.\n");
        return 0;
    }
    if(new_num[0]<'6' || new_num[0]>'9')
    {
        printf("Error : Number Must start with 6-9 only.\n");
        return 0;
    }
    for(int i=0;i<len;i++)
    {
        if(!isdigit(new_num[i]))
        {
            printf("Error : Only Digits allowed\n");
            return 0;
        }
    }
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(strcmp(new_num,addressBook->contacts[i].phone)==0)
        {
            printf("Error : Number is already exists\n");
            return 0;
        }
    }
    return 1;
}

int check_mail(char *new_mail,AddressBook *addressBook)
{
    int len=strlen(new_mail);
    char *at_ptr=strchr(new_mail,'@');
    char *com_pointer=strstr(new_mail,".com");

    if(isdigit(new_mail[0]))
    {
        printf("First Character can't be digit\n");
        return 0;
    }
    for(int i=0;i<len;i++)
    {
     if(isupper(new_mail[i]))
      {
        printf("Mail id can't be in Upper case\n");
        return 0;
      }
    }

    //to check whether @ and .com present or not
    if(at_ptr==NULL || com_pointer==NULL)
    {
        printf("Mail id should contain '@' and '.com' \n");
        return 0;
    }
    //Checking that there are more than 1 @ is present
    if(strchr(at_ptr+1,'@')!=NULL)
    {
        printf("Error: '@' must appear only once\n");
        return 0;
    }
    //Checking that there are more than 1 .com is present
    if(strstr(com_pointer+1,".com")!=NULL)
    {
        printf("Error: '.com' must appear only once\n");
        return 0;
    }
    // Invalid: no .com or wrong 
    if(com_pointer==NULL || strcmp(com_pointer,".com")!=0)
    {
        printf("No other Characters are allowed after .com\n");
        return 0;
    }
    // .com must come after the @ character 
    if(com_pointer<at_ptr)
    {
        printf("Error : '.com' must come after '@'\n");
        return 0;
    }

    if((at_ptr-new_mail)<5)
    {
        printf("Error: Minimum 5 Characters required before '@'\n");
        return 0;
    }

    int checkchar=com_pointer-(at_ptr+1);
    if(checkchar<4)
    {
        printf("Error : Minimum 4 alpha characters required for domain\n");
        return 0;
    }

    for(char *p=at_ptr+1;p<com_pointer;p++)
    {
        if(!isalpha(*p))
        {
            return 0;
        }
    }

    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(strcmp(new_mail,addressBook->contacts[i].email)==0)
        {
            printf("Error : Email already exists\n");
            return 0;
        }
    }
    return 1;

}

void searchContact(AddressBook *addressBook) 
{
    gi=-1;
    gm=0;
    if(addressBook->contactCount==0)
    {
        printf("Address book is Empty.Nothing to search");
        return;
    }
    int choice;
    char target[100];

    printf("\n search by:\n1.Name\n2.Phone number\n3.Email id\n4.Exit\n Enter the choice: ");
    scanf("%d",&choice);
        if(choice==4)
        {
            return;
        }
        if(choice>4)
        {
            printf("Invalid Choice Entered........\n");
            return;
        }
    printf("Enter the search term: ");
    scanf(" %[^\n]",target);
    for(int i=0;i<addressBook->contactCount;i++)
    {
        int ismatch=0;
        if(choice==1 && strcmp(addressBook->contacts[i].name,target)==0)
        {
            ismatch=1;
        }
        if(choice==2 && strcmp(addressBook->contacts[i].phone,target)==0)
        {
            ismatch=1;
        }
        if(choice==3 && strcmp(addressBook->contacts[i].email,target)==0)
        {
            ismatch=1;
        }
        if(ismatch)
        {
            if(gm==0)
            {
                header(); //That title need to print only once
            }
            match_index[gm]=i;
            gm++;
            printf("%-10d %-20s %-20s %-25s\n", 
                   gm, 
                   addressBook->contacts[i].name, 
                   addressBook->contacts[i].phone, 
                   addressBook->contacts[i].email); //to print all contents which is matched
            gi=i;
        }

    }
    if(gm==0)
    {
        printf("\n No record found matching:%s,please try again\n",target);
    }

}

void editContact(AddressBook *addressBook)
{
    searchContact(addressBook);
    if(gm==0)
    {
        return;
    }
    if(gm>1)
    {
        int serial,attempts=0;
        while(1)
        {
        printf("\nMultiple Matches Found.Enter the serial number to edit: ");
        scanf("%d",&serial);
        if(serial>0 && serial<=gm)
        {
            gi=match_index[serial-1]; //To access the that index we need to edit
            break;
        }
    else
    {
        attempts++;
        if(attempts>=2) //to handle if user entered wrong number
        {
            printf("Error: Invalid Selection.\n");
            return;
        }
        printf("Error: pick a number between 1 to %d.Attempts left:%d\n",gm,2-attempts);
    }
   }
}

    int echoice,attempts_edit=0,verify=0;
    char new_data[100];
    printf("1. Edit Name\n2. Edit Phone\n3. Edit Email\nEnter your Choice: ");
    scanf("%d",&echoice);
    switch(echoice)
    {
        case 1:
           do
           {
           printf("Enter new Name: ");
           scanf(" %[^\n]",new_data);
           verify=check_name(new_data);

            if(verify != 1)
            {
              attempts_edit++;
              
              if(attempts_edit>=5)
              {
                printf("\n Error: Maximum attempts have been reached. Update cancelled.\n");
                return;
              }
              printf("Attempts left:%d\n",5-attempts_edit);
            }
            }while(verify!=1);

            strcpy(addressBook->contacts[gi].name,new_data);
            printf("Name Updated!\n");
            break;

        case 2:
        do
        {
           printf("Enter new Phone: ");
           scanf(" %[^\n]",new_data);
           verify=check_number(new_data,addressBook);
           if(verify != 1)
           {
             attempts_edit++;

            if(attempts_edit>=5)
            {
                printf("Maximum attempts have been reached. Update cancelled.\n");
                return;
            }
            printf("Attempts left:%d\n",5-attempts_edit);
            }
        }while(verify!=1); 

        strcpy(addressBook->contacts[gi].phone,new_data);
        printf("Phone number is Updated\n");  
        break;

        case 3:
           do{
           printf("Enter new Email: ");
           scanf(" %[^\n]",new_data);
           verify=check_mail(new_data,addressBook);
           if(verify != 1)
           {
            attempts_edit++;

            if(attempts_edit>=5)
            {
                printf("Maximum attempts have been reached. Update cancelled.\n");
                return;
            }
            printf("Attempts left:%d\n",5-attempts_edit);
            }
        }while(verify!=1);
        
        strcpy(addressBook->contacts[gi].email,new_data);
        printf("Email is Updated\n");
        break;
        default:
           printf("Invaild Choice Entered.\n");   

    }
}

void deleteContact(AddressBook *addressBook)
{
    searchContact(addressBook);
    if(gm==0)
    {
        return;
    }
    if(gm>1)
    {
        int serial,attempts=0;
        while(1)
        {
        printf("Multiple matches found. So Enter the S.no to delete: ");
        scanf("%d",&serial);
        if(serial>0 && serial<=gm)
        {
            gi=match_index[serial-1]; //To know which index to delete
            break;
        }
        else
        {
            attempts++;
            if(attempts>=2)
            {
                printf("Error: Invalid Selection.\n");
                return;
            }
            printf("Error: pick a number between 1 to %d.\n Attempts left:%d\n",gm,2-attempts);
        }
    }
}
    char c;
    printf("Are you sure you want to delete [y/n]:"); //Again asking the user to chech if he really wants to delete
    scanf(" %c",&c);

    if(c=='n' || c=='N')
    {
        printf("Deletion Cancelled\n");
        return;
    }
    for(int i=gi;i<addressBook->contactCount-1;i++)
    {
        addressBook->contacts[i]=addressBook->contacts[i+1];
    }
    addressBook->contactCount--;
    printf("\n Contact deleted Successfully!\n");
   
}