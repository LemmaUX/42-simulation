# get_next_line - Complete Project Guide

This document explains the `get_next_line` project in simple English, as if we were studying it in class.

## 0. What Is the Project?

`get_next_line` is a C project where you build a function that reads text from a file descriptor one line at a time.

The goal is not just to read bytes. The goal is to control the flow of data:

- read a piece of the file,
- find the next line,
- return that line,
- and keep the remaining text for the next call.

This project teaches memory management, string handling, and state preservation in C.

## 1. The Main Challenge

In C, `read()` gives raw data, not ready-made lines.

That means your function must:

- keep reading until it finds a newline,
- build one complete line,
- remember everything after that newline,
- and continue from there on the next call.

So the real problem is not reading. The real problem is splitting and saving data correctly.

## 2. The Official Idea

The project usually asks you to implement a function like this:

```c
char *get_next_line(int fd);
```

Each call should return:

- the next line from the file,
- including the newline character if it exists,
- or `NULL` when there is nothing left to read.

## 3. Why This Project Matters

`get_next_line` is important because it forces you to practice three basic C skills at the same time:

- `read()` system calls,
- dynamic memory allocation,
- keeping data between function calls.

That last point is the key idea. The function must remember unfinished text from the previous call, so it usually uses a `static` variable.

## 4. The Core Mental Model

Imagine you are reading a book with a flashlight. You do not always see a full sentence at once. Sometimes you see half a sentence, sometimes two sentences, and sometimes only part of a word.

`get_next_line` works the same way:

1. It reads a chunk of text into a buffer.
2. It checks whether that chunk contains a newline.
3. If it does, it returns the line up to the newline.
4. It saves the remaining text for the next call.

## 5. The Role of the Buffer

The buffer is a temporary area where `read()` stores data.

Example:

```text
BUFFER_SIZE = 4
File content = "Hello\nWorld\n"
```

The program may read:

- `Hell`
- `o\nWo`
- `rld\n`

So the function must be able to assemble the final line from several partial reads.

## 6. The Role of the Static Variable

A `static` variable keeps its value between calls.

This is perfect for `get_next_line` because:

- one call may stop in the middle of a line,
- the next call must continue from that exact point.

Without a static variable, the leftover text would be lost.

Example idea:

```c
static char *stash;
```

`stash` stores what we still have not returned yet.

## 7. Typical Function Flow

Most solutions are built from small helper functions. A common structure is:

### `get_next_line`
This is the main function.

It usually:

- checks the input parameters,
- reads more data if needed,
- extracts one line,
- updates the saved remainder,
- returns the line.

### Helper 1: read and append
This helper keeps reading until either:

- a newline is found, or
- the file ends.

It appends new data to the saved text.

### Helper 2: extract the line
This helper creates the string that will be returned.

If the newline exists, the returned line includes it.

### Helper 3: save the remainder
After returning the line, the function must store everything after the newline for the next call.

## 8. Step-by-Step Example

Let us say the file contains:

```text
First line\nSecond line\nThird line
```

### First call

- The program reads until it finds `\n`.
- It returns `First line\n`.
- It stores `Second line\nThird line`.

### Second call

- It continues from the stored text.
- It returns `Second line\n`.
- It stores `Third line`.

### Third call

- It reads the last part.
- There is no newline at the end.
- It returns `Third line`.
- The stash becomes empty.

### Fourth call

- There is nothing left.
- The function returns `NULL`.

## 9. The Complete Project Structure in Your Head

When you build `get_next_line`, think about these layers:

- input validation,
- buffered reading,
- line detection,
- string extraction,
- leftover storage,
- cleanup and error handling.

If one layer fails, the whole function can break.

## 10. Important Edge Cases

You must think about these cases carefully:

- invalid file descriptor,
- `BUFFER_SIZE <= 0`,
- empty file,
- file that does not end with a newline,
- very long lines,
- multiple calls in a row,
- `read()` returning `-1`.

## 11. What Can Go Wrong?

Common mistakes are:

- forgetting to free memory,
- losing the leftover text,
- returning the wrong line length,
- not handling the final line correctly,
- not checking `read()` errors,
- leaking memory when the file ends,
- mixing up the line and the stash.

## 12. Bonus Version

Some versions of the project ask you to support more than one file descriptor.

In that case, one single `static` variable is not enough.

You need separate saved data for each file descriptor, so each open file keeps its own state.

That makes the project more advanced because now your function must remember multiple streams at the same time.

## 13. How to Think About the Implementation

Think of `get_next_line` as a small machine with two jobs:

1. Build one complete line.
2. Keep the rest for later.

If your code does those two things correctly, the project is working.

## 14. How to Test It

A good test set should include:

- a file with one short line,
- a file with several lines,
- a file with no final newline,
- an empty file,
- a very long line,
- repeated calls after the file ends,
- invalid input cases.

If the output is correct in all of those situations, your function is probably solid.

## 15. Short Summary

`get_next_line` is not only about reading text. It is about learning how to control memory, handle partial data, and preserve state across function calls.

If you understand the flow of:

- read,
- search for newline,
- split line and remainder,
- save remainder,

then you understand the whole project.

## 16. File-by-File Explanation

### `get_next_line.h`
This is the header file.

It declares the function that the outside world can use and includes the headers needed for `malloc`, `free`, and `read`.

It also defines `BUFFER_SIZE` if it is not already provided by the build system.

Think of this file as the project's public interface. It tells other files what exists.

### `get_next_line.c`
This is the main implementation file.

It contains the `get_next_line` function and the helper functions that control the full reading process.

Its job is to:

- keep a saved stash between calls,
- read until a newline or end of file,
- extract one line,
- and update the leftover text for the next call.

This is the core logic of the project.

### `get_next_line_utils.c`
This file contains the small utility functions used by the main implementation.

These helpers replace common string functions that you are expected to write yourself in 42 projects.

They handle tasks such as:

- calculating string length,
- duplicating a string,
- joining two strings,
- finding a character,
- and creating a substring.

Without these helpers, the main function would be too large and harder to understand.

### `Makefile`
This file describes how to build the project.

It tells the compiler which source files to use, how to create the static library, and how to clean the build artifacts.

In a real 42 project, the Makefile is important because it makes the project easy to compile, rebuild, and deliver.

### `README.md`
This file is the teaching document.

It does not control the program, but it explains the project in English so that a student can understand the goal, the logic, and the structure before reading the code.

## 17. Function-by-Function Explanation

### `ft_strlen_gnl`
This function counts how many characters are in a string.

It starts from the first character and moves forward until it finds the `\0` terminator.

Why it exists:

- to know how much memory to allocate,
- to calculate string sizes before joining or copying,
- to avoid repeating the same length logic in several places.

If the input string is `NULL`, the function returns `0`.

### `ft_strdup_gnl`
This function creates a new copy of a string.

It first measures the length, then allocates enough memory, then copies every character one by one, and finally adds the `\0` terminator.

Why it exists:

- to safely preserve leftover text,
- to create an independent string that can be freed later,
- to avoid sharing the same memory block between different steps.

If the input string is `NULL`, the function returns `NULL`.

### `ft_strchr_gnl`
This function searches for a character inside a string.

It moves through the string until it finds the target character or the end of the string.

In `get_next_line`, this is mainly used to check whether the stash already contains a newline.

If the character is `\0`, the function returns the end of the string.

If the string is `NULL`, the function returns `NULL`.

### `ft_strjoin_gnl`
This function joins two strings into a new one.

It calculates the length of both strings, allocates enough memory for both plus the final `\0`, then copies the first string and the second string into the new memory block.

Why it exists:

- to merge the previous leftover text with new data from `read()`,
- to rebuild the full text piece by piece,
- to keep the main function simple.

If one of the inputs is `NULL`, the function treats it like an empty string.

### `ft_substr_gnl`
This function creates a substring from an existing string.

It starts at a chosen index and copies a limited number of characters into a new allocated string.

In `get_next_line`, this is used to extract:

- the line that will be returned,
- or the remaining text that stays in the stash.

If the start position is beyond the end of the string, the function returns an empty string.

If the input string is `NULL`, the function returns `NULL`.

### `read_until_newline`
This helper keeps reading from the file descriptor until it finds a newline or reaches the end of the file.

It allocates a temporary buffer, reads into it, joins the new data to the saved stash, and repeats the process while the stash still does not contain a newline.

This is the part that handles the real streaming behavior of the project.

If `read()` fails, the function frees the temporary buffer and returns `NULL`.

### `extract_line`
This helper takes the current stash and returns the next full line.

It scans the stash until it reaches a newline or the end of the string, then returns a substring from the beginning up to that point.

If the line includes a newline, the newline is included in the returned result.

If the stash is empty or `NULL`, the function returns `NULL`.

### `update_stash`
This helper prepares the leftover text for the next call.

It finds the first newline in the stash and copies everything after it into a new string.

If there is no newline left, the old stash is freed and the function returns `NULL`.

This step is important because it makes sure the next call starts exactly where the previous one stopped.

### `get_next_line`
This is the public function that the user calls.

It controls the whole process:

- it checks whether the file descriptor and `BUFFER_SIZE` are valid,
- it reads more data when needed,
- it extracts one line,
- it updates the stash,
- and it returns the final result.

The static stash is what allows the function to remember unfinished text between calls.

If there is no more data to return, the function gives back `NULL`.