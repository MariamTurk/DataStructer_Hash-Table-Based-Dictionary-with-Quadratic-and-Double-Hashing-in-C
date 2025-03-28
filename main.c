#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 127

typedef struct
{
    char *word;
    char *meaning1;
    char *meaning2;
    char *meaning3;

} Hashing;



int Q_collision = 0; // collision for
int D_collision = 0; // collision for Double hash



Hashing *TQ_Heah[127]; // Hash table for
Hashing *TD_Heah[127]; // Hash table for Double hash



int isPrime(int size);
int prim_num(int size);
int hash(char *str, int Table_Size);
void Insert_to_Q_Hash(int T_size, char *word, char *M1, char *M2, char *M3);
void Insert_to_D_Hash(int T_size, char *word, char *M1, char *M2, char *M3);
void load_factor(Hashing *Table[], int size);
int count_elements(Hashing *table[], int size);
int search_Quadratic(int T_size, Hashing *Q_Table[], char *word);
int search_Duadratic(int T_size, Hashing *D_Table[], char *word);
void Search(int T_size, Hashing *Q_Table[], Hashing *D_Table[], char *word);
void Delet_Q(int T_size, Hashing *Q_Table[], char *word);
void Delet_D(int T_size, Hashing *D_Table[], char *word);
void collision();
void print_hash_table(Hashing *H_Table[], int T_size);
int countLinesInFile();
int SizeOfTable(int num_meanings);
void print_hash();
void readFromFile(const char *filename, int tableSize, Hashing *Q_Table[], Hashing *D_Table[]);
void saveToFile(int tableSize, Hashing *Q_Table[], Hashing *D_Table[]);
void print_menu();


int main()
{
    int option;
    char word[MAX_LENGTH];
    int lines = countLinesInFile();
    int new_size_t = SizeOfTable(lines);



    // Initialize tables
    for (int i = 0; i < 127; i++)
    {
        TQ_Heah[i] = NULL;
        TD_Heah[i] = NULL;
    }

    // Read from file and insert into separate hash tables
    readFromFile("words.txt", new_size_t, TQ_Heah, TD_Heah);

    while (1)
    {
        print_menu();
        printf("------------------------------------------------------------------------------------\n");
        printf("Enter your choice 1-8 : ");
        scanf("%d", &option);
        printf("\n");
        switch (option)
        {
        case 0:
            return 0;
        case 1:
            printf("Print hashed tables:\n");
            printf("\n");
            printf("Quadratic Hash Table:\n");
            print_hash_table(TQ_Heah, new_size_t);
            printf("\n");
            printf("\nDouble Hash Table:\n");
            print_hash_table(TD_Heah, new_size_t);
            printf("\n");
            break;
        case 2:
            printf("Print out table size and the load factor:\n");
            printf("\n");
            load_factor(TQ_Heah,new_size_t);
            printf("\n");
            load_factor(TD_Heah,new_size_t);
            break;
        case 3:
            printf("Print out the used hash functions:\n");
            print_hash();
             printf("\n");
              printf("\n");
            break;

case 4:
    printf("Insert a new record to hash table:\n");
    char new_word[MAX_LENGTH], new_meaning1[MAX_LENGTH], new_meaning2[MAX_LENGTH], new_meaning3[MAX_LENGTH];

    printf("Enter the word: ");
    scanf("%s", new_word);

    // Check if the word already exists in the hash tables
    if (search_Quadratic(new_size_t, TQ_Heah, new_word) != -1 || search_Duadratic(new_size_t, TD_Heah, new_word) != -1)
    {
        printf("The word already exists in the hash tables.\n");
        break;
    }

    printf("Enter Meaning 1: ");
    scanf("%s", new_meaning1);

    printf("Do you want to insert Meaning 2? (yes/no): ");
    char num[3];
    scanf("%s", num);
    if (strcmp(num, "yes") == 0)
    {
        printf("Enter Meaning 2: ");
        scanf("%s", new_meaning2);

        // Since Meaning 2 is inserted, ask for Meaning 3
        printf("Do you want to insert Meaning 3? (yes/no): ");
        scanf("%s", num);
        if (strcmp(num, "yes") == 0)
        {
            printf("Enter Meaning 3: ");
            scanf("%s", new_meaning3);
        }
        else
        {
            // If not inserting Meaning 3, set it to an empty string or any default value
            new_meaning3[0] = '\0';
        }
    }
    else
    {
        // If not inserting Meaning 2, set both Meaning 2 and Meaning 3 to empty string or any default value
        new_meaning2[0] = '\0';
        new_meaning3[0] = '\0';
    }

    // Insert into hash tables only if the table is not full
    if (count_elements(TQ_Heah, new_size_t) < new_size_t)
    {
        Insert_to_Q_Hash(new_size_t, new_word, new_meaning1, new_meaning2, new_meaning3);
        Insert_to_D_Hash(new_size_t, new_word, new_meaning1, new_meaning2, new_meaning3);
        printf("Record has been inserted successfully.\n");
    }
    else
    {
        printf("Both hash tables are full. Cannot insert the record.\n");
    }

    printf("\n");
    break;

        case 5:
            printf("Search for a specific word:\n");
            Search(new_size_t, TQ_Heah, TD_Heah, word);
            printf("\n");
            break;
        case 6:
            printf("Delete a specific word:\n");
            printf("Enter the word to delete: ");
            scanf("%s", word);
            Delet_Q(new_size_t, TQ_Heah, word);
            Delet_D(new_size_t, TD_Heah, word);
            break;
        case 7:
            printf("Compare between the two methods in terms of the number of collisions occurred:\n");
            collision();
            printf("\n");
            break;

case 8:
    printf("Save hash table back to a file named saved_words.txt (of the double hashing):\n");
    saveToFile(new_size_t, TQ_Heah, TD_Heah);
    printf("Save is done! Exiting...\n");
    return 0;  // This line will exit the program

            break;
        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}



//Function to check if the number is prim or not
int isPrime(int size)
{
    if (size <= 1)
        return 0;

    for (int i = 2; i * i <= size; i++)
    {
        if (size % i == 0)
            return 0;
    }

    return 1;
}



//Function to return the first prim number that less than th size of the table size
int prim_num(int size)
{
     //i well check the size of table -1 because i want the first prim number < size of table
    int new_size = size - 1;

    //if the new size not a prim number sub it with 1
    while (!isPrime(new_size))
    {
        new_size--;
    }

    //return the first prim number < table size
    return new_size;
}




//Function to make hashing in string to sort it in a hash table
int hash(char *str, int Table_Size)
{
    //to the over flawo
    long long string_elemnt_hash = 0;
    while (*str != '\0')
    {
        string_elemnt_hash = ((string_elemnt_hash << 5) + *str++) % Table_Size;
    }

    return string_elemnt_hash;
}



void Insert_to_Q_Hash(int T_size, char *word, char *M1, char *M2, char *M3)
{
    int value = hash(word, T_size);
    int  hash_i = (value + 0) % T_size;
    int i = 1;

    while (TQ_Heah[hash_i] != NULL)
    {
        // Collision occurred, increment collision counter
        Q_collision++;
        printf("Collision happened in Quadratic Table!\n");

        // Quadratic hash equation
        hash_i = (value + i * i) % T_size;
        i++;

        // If the table is full, print a message and return
        if (i > T_size)
        {
            printf("The Quadratic hash table is full. Cannot insert %s.\n", word);
            return;
        }
    }

    // Allocate memory for the new word and its meanings
    Hashing *new_word = (Hashing *)malloc(sizeof(Hashing));
    new_word->word = strdup(word);
    new_word->meaning1 = strdup(M1);
    new_word->meaning2 = strdup(M2);
    new_word->meaning3 = strdup(M3);

    TQ_Heah[hash_i] = new_word;
}



void Insert_to_D_Hash(int T_size, char *word, char *M1, char *M2, char *M3)
{
    int value = hash(word, T_size);

    int i = 1;
    int new_size = prim_num(T_size);
   int hash_i = (value + 0 * (new_size - (value % new_size))) % new_size;

    while (TD_Heah[hash_i] != NULL)
    {
        // Collision occurred, increment collision counter
        D_collision++;
        printf("Collision happened in Double Table!\n");

        // Double hash equation
        hash_i = (value + i * (new_size - (value % new_size))) % new_size;
        i++;

        // If the table is full, print a message and return
        if (i > T_size)
        {
            printf("The Double hash table is full. Cannot insert %s.\n", word);
            return;
        }
    }

    // Allocate memory for the new word and its meanings
    Hashing *new_word = (Hashing *)malloc(sizeof(Hashing));
    new_word->word = strdup(word);
    new_word->meaning1 = strdup(M1);
    new_word->meaning2 = strdup(M2);
    new_word->meaning3 = strdup(M3);

    TD_Heah[hash_i] = new_word;
}



//Function to calculate how words in the table , and the size of the table
void load_factor(Hashing *Table[], int size)
{
    int load = 0;
    for (int i = 0; i < size; i++)
    {
        // If Table[i] != NULL, there is a word, so load++
        if (Table[i] != NULL)
        {
            load++;
        }
    }

    // Calculate the load factor
    float loadFactor = (float)load / size;

    printf("Size of table = %d\n", size);
    printf("Load Factor = %.2f\n", loadFactor);
}




int count_elements(Hashing *table[], int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (table[i] != NULL)
        {
            count++;
        }
    }
    return count;
}



// Function to search about an word in Quadratic hash table
int search_Quadratic(int T_size, Hashing *Q_Table[], char *word) {
    int value = hash(word, T_size);
    int hash_i = value % T_size;
    int i = 0;

    //if the table not empty compare between the word in the table and the word we want to search of it
    //if they are equal to each other print the word
    while (Q_Table[hash_i] != NULL && i < T_size) {
        if (strcmp(Q_Table[hash_i]->word, word) == 0) {
            printf("Word '%s' found at index %d: %s:%s#%s#%s\n", word, hash_i, Q_Table[hash_i]->word, Q_Table[hash_i]->meaning1, Q_Table[hash_i]->meaning2, Q_Table[hash_i]->meaning3);
            return i;  // Return the number of iterations
        }
        //in case the word save in the table by Collision use the Quadratic hash equation
        i++;
        hash_i = (value + i * i) % T_size;
    }
// the word is not in the table
    printf("The word is not in the Quadratic hash table.\n");
     // Return -1 to indicate the word is not found
    return -1;
}



//Function to search about an word in Double hash table
int search_Duadratic(int T_size, Hashing *D_Table[], char *word) {
    int value = hash(word, T_size);
    int hash_i = value % T_size;
    int i = 0;
    int new_size = prim_num(T_size);


    //if the table not empty compare between the word in the table and the word we want to search of it
    //if they are equal to each other print the word
    while (D_Table[hash_i] != NULL && i < T_size) {
        if (strcmp(D_Table[hash_i]->word, word) == 0) {
            printf("Word '%s' found at index %d: %s:%s#%s#%s\n", word, hash_i, D_Table[hash_i]->word, D_Table[hash_i]->meaning1, D_Table[hash_i]->meaning2, D_Table[hash_i]->meaning3);
            return i;  // Return the number of iterations
        }

        //in case the word save in the table by Collision use the double hash equation
        i++;
        hash_i = (value + (i * (new_size - (hash_i % new_size)))) % T_size;
    }

    // the word is not in the table
    printf("The word is not in the Double hash table.\n");
    // Return -1 to indicate the word is not found
    return -1;
}



//function for tow function the user will inseart what ward he want and from which table
void Search(int T_size, Hashing *Q_Table[], Hashing *D_Table[], char *word)
{
    int choice;

    printf("Please enter the word you want to search: ");
    //the word want to search
    scanf("%s", word);
    printf("Enter (1) for Quadratic hash table or (2) for Double hash table: ");
    //to know wich table we well serch on it
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        search_Quadratic(T_size, Q_Table, word);
        break;
    case 2:
        search_Duadratic(T_size, D_Table, word);
        break;
    default:
        printf("Invalid choice.\n");
    }
}



//function to delete a word from the Quadratic hash table
void Delet_Q(int T_size, Hashing *Q_Table[], char *word)
{
    int value = hash(word, T_size);
    int hash_i = value % T_size;
    int i = 0;

    // if the tow word equal to each other in the table and the word from user free it (delete it)
    while (Q_Table[hash_i] != NULL && i < T_size)
    {
        if (strcmp(Q_Table[hash_i]->word, word) == 0)
        {
            //free to delete it from the memory ,  and to delete the meanings of the word
            free(Q_Table[hash_i]->word);
            free(Q_Table[hash_i]->meaning1);
            free(Q_Table[hash_i]->meaning2);
            free(Q_Table[hash_i]->meaning3);
            free(Q_Table[hash_i]);
            Q_Table[hash_i] = NULL;
            printf("The word has been deleted.\n");
            return;
        }
        //in case the word save in the table by Collision use the Quadratic hash equation
        i++;
        hash_i = (value + (i * i)) % T_size;
    }

    printf("The word was not found.\n");
}




//function to delete a word from the Double hash table
void Delet_D(int T_size, Hashing *D_Table[], char *word)
{
    int value = hash(word, T_size);
    int hash_i = value % T_size;
    int i = 0;
    int new_size = prim_num(T_size);

    // if the tow word equal to each other in the table and the word from user free it (delete it)

    while (D_Table[hash_i] != NULL && i < T_size)
    {
        if (strcmp(D_Table[hash_i]->word, word) == 0)
        {
            //free to delete it from the memory ,  and to delete the meanings of the word
            free(D_Table[hash_i]->word);
            free(D_Table[hash_i]->meaning1);
            free(D_Table[hash_i]->meaning2);
            free(D_Table[hash_i]->meaning3);
            free(D_Table[hash_i]);
            D_Table[hash_i] = NULL;
            printf("The word has been deleted.\n");
            printf("\n");
            return;
        }
         //in case the word save in the table by Collision use the Quadratic hash equation
        i++;
        hash_i = (value + (i * (new_size - (hash_i % new_size)))) % T_size;
    }

    printf("The word was not found.\n");
}



//Function to print the number of collision in each table
void collision()
{
    if (Q_collision != 0)
    {
        printf("The collision that happened in Quadratic hash table = %d\n", Q_collision);
    }
    if (D_collision != 0)
    {
        printf("The collision that happened in Double hash table = %d\n", D_collision);
    }

    if (Q_collision == 0 && D_collision == 0)
    {
        printf("No collisions occurred in both hash tables.\n");
    }
}



//function to print the Quadratic and Double hash table
void print_hash_table(Hashing *H_Table[], int T_size)
{
    for (int i = 0; i < T_size; i++)
    {
        if (H_Table[i] != NULL)
        {
            printf("index[%d] : %s : %s # %s # %s.\n", i, H_Table[i]->word, H_Table[i]->meaning1, H_Table[i]->meaning2, H_Table[i]->meaning3);
        }
        //if the index of the table is empty print ---------------
        else
        {
            printf("index[%d] : ______________.\n", i);
        }
    }
}



//this function to calculate the size of the table , Depends on the number of lines in the file
int countLinesInFile()
{
    FILE *file = fopen("words.txt", "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return -1;
    }

    int lineCount = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            lineCount++;
        }
    }

    fclose(file);
    return lineCount;
}





//function to calculate the best prim size number for the tables
int SizeOfTable(int num_meanings)
{
    //number of lines in the file
     num_meanings = countLinesInFile() + 1;
     //this is the best equation to calculated the size of tables
    int prim_size = (2 * num_meanings) + 50;
    if (!isPrime(prim_size))
    {
        prim_size++;
    }
    return prim_size;
}



//this for case 3
void print_hash()
{
    printf("1. Hash for string ==> Key = ((Key << 5)  + str++) %% Table Size\n");
    printf("2. Quadratic Hash  ==> Key = (Key + i*i) %% Table Size\n");
    printf("3. Double Hash  ==> Key = ((Key %% Table Size) + (i * (prim - (Key %% prim)))) %% Table Size\n");
}




void readFromFile(const char *filename, int tableSize, Hashing *Q_Table[], Hashing *D_Table[])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        char temp[MAX_LENGTH];
        strcpy(temp, line);

        char *word = strtok(temp, ":");
        char *meanings = strtok(NULL, "\n");

        if (word != NULL && meanings != NULL)
        {
            // Split meanings into individual parts
            char *meaning1 = strtok(meanings, "#");
            char *meaning2 = strtok(NULL, "#");
            char *meaning3 = strtok(NULL, "#");

            // Trim leading whitespaces
            word = strtok(word, " \t\r\n");
            meaning1 = strtok(meaning1, " \t\r\n");
            meaning2 = (meaning2 != NULL) ? strtok(meaning2, " \t\r\n") : "--------";
            meaning3 = (meaning3 != NULL) ? strtok(meaning3, " \t\r\n") : "--------";

            // Insert into hash tables
            Insert_to_Q_Hash(tableSize, word, meaning1, meaning2, meaning3);
            Insert_to_D_Hash(tableSize, word, meaning1, meaning2, meaning3);
        }
        else
        {
            printf("Error reading line: %s\n", line);
        }
    }

    fclose(file);
}



void saveToFile(int tableSize, Hashing *Q_Table[], Hashing *D_Table[])
{
    FILE *file = fopen("saved_words.txt", "w");
    if (file == NULL)
    {
        printf("Failed to create the output file.\n");
        return;
    }

    for (int i = 0; i < tableSize; i++)
    {
        Hashing *qNode = Q_Table[i];
        Hashing *dNode = D_Table[i];

        if (qNode != NULL && dNode != NULL)
        {
            fprintf(file, "Word: %s\n", qNode->word);
            fprintf(file, "Meaning 1: %s\n", qNode->meaning1);
            fprintf(file, "Meaning 2: %s\n", qNode->meaning2);
            fprintf(file, "Meaning 3: %s\n", qNode->meaning3);
        }
    }

    fclose(file);
}




void print_menu()
{
    printf("\n");
    printf("1. Print hashed tables (including empty spots).\n");
    printf("2. Print out table size and the load factor.\n");
    printf("3. Print out the used hash functions.\n");
    printf("4. Insert a new record to hash table (insertion will be done on both hash tables).\n");
    printf("5. Search for a specific word (specify which table to search in).\n");
    printf("6. Delete a specific word (from both tables).\n");
    printf("7. Compare between the two methods in terms of the number of collisions occurred.\n");
    printf("8. Save hash table back to a file named saved_words.txt (of the double hashing).\n");
}


