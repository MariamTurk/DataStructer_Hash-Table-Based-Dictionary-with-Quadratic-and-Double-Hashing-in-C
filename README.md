
# 📘 Hash Table-Based Dictionary with Quadratic and Double Hashing in C


## 🧠 Overview

This C project implements a **dictionary management system** using two open addressing hashing techniques:

- 🔁 **Quadratic Probing**
- 🔁 **Double Hashing**

It allows for insertion, search, deletion, collision tracking, and persistence of dictionary records.

---

## 📂 File Structure

. ├── main.c # Full source code ├── words.txt # Input file containing dictionary entries ├── saved_words.txt # Output file to save dictionary records └── README.md # Project documentation

pgsql
Copy
Edit

---

## 📚 Features

- 📥 Load words and meanings from a file (`words.txt`)
- 🧮 Store data in two separate hash tables (Quadratic and Double hashing)
- ➕ Insert new records with up to 3 meanings
- 🔍 Search for words in either hash table
- ❌ Delete records from both hash tables
- ⚖️ Compare the number of collisions between hashing methods
- 💾 Save records from Double Hash table to `saved_words.txt`
- 📊 View load factors and used hashing functions

---

## 🧪 Sample Input (`words.txt`)

apple:fruit#technology#brand car:vehicle#machine#transport book:pages#read#library

yaml
Copy
Edit

---

## 🚀 How to Compile and Run

### Step 1: Compile

```bash
gcc main.c -o hash_dictionary
Step 2: Run
bash
Copy
Edit
./hash_dictionary
🧭 Menu Options
pgsql
Copy
Edit
1. Print hashed tables (including empty spots).
2. Print out table size and the load factor.
3. Print out the used hash functions.
4. Insert a new record to hash table (insertion will be done on both hash tables).
5. Search for a specific word (specify which table to search in).
6. Delete a specific word (from both tables).
7. Compare between the two methods in terms of the number of collisions occurred.
8. Save hash table back to a file named saved_words.txt (of the double hashing).
0. Exit.
🧩 Data Structure Summary
Component	Purpose
Hashing struct	Stores each word and up to 3 meanings
Hash Tables	Two arrays: one for Quadratic Hashing, one for Double Hashing
hash()	String hashing function
Insert_to_*	Handles collisions and stores entries
search_*	Searches by collision resolution strategy
Delet_*	Deletes entries from tables
🔬 Hashing Techniques Used
1. Primary Hash Function (for all strategies)
hash = ((hash << 5) + character) % Table_Size;
2. Quadratic Probing
index = (hash + i^2) % Table_Size;
3. Double Hashing
index = (hash + i * (prime - (hash % prime))) % Table_Size;

📊 Load Factor
The load factor is calculated as:

mathematica
Load Factor = (Number of Elements) / (Table Size)
This metric helps assess how full the table is and how likely collisions are to occur.

💾 File Save Format (saved_words.txt)

Word: car
Meaning 1: vehicle
Meaning 2: machine
Meaning 3: transport

Word: book
Meaning 1: pages
Meaning 2: read
Meaning 3: library


⚠️ Notes
If a word already exists in either hash table, it will not be inserted again.

If both hash tables are full, new records cannot be inserted.

The table size is dynamically computed based on input file size.

