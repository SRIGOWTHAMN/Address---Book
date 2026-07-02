#include <stdio.h>
#include "file.h"


void saveContactsToFile(AddressBook *addressBook) 
{
  FILE *fp = fopen("contacts.txt", "w");

  if(fp == NULL)
  {
    printf("Error:Unable to open file for saving");
    return;
  }

  // Save contact count first
  fprintf(fp, "%d\n", addressBook->contactCount);

  // Save each contact
  for (int i = 0; i < addressBook->contactCount; i++)
  {
    fprintf(fp, "%s,%s,%s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
  }
  
  fclose(fp);

}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contacts.txt", "r");

    if(fp == NULL)
    {
        return;
    }

    // Read contact count first
    fscanf(fp, "%d\n", &addressBook->contactCount);

    // Read each contact
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(fp, "%[^,],%[^,],%[^\n]\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }

    fclose(fp);
}