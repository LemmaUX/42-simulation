# ft_select - Terminal Selection Menu

## Project Overview

ft_select is a terminal-based selection menu program that allows users to interactively select items from a list passed as command-line arguments. It uses termcaps for terminal control and raw terminal mode for keyboard input handling.

## How It Works

### 1. **Terminal Control (`terminal.c`)**
- **setup_terminal()**: Enters raw mode by:
  - Disabling ICANON (canonical input) so characters are read immediately
  - Disabling ECHO so keypresses aren't displayed
  - Using termcaps to enter alternate terminal mode ("ti") and clear screen ("cl")
  - Stores original terminal settings to restore later

- **restore_terminal()**: Recovers the terminal:
  - Exits alternate terminal mode ("te")
  - Restores original termios settings
  - Ensures terminal is usable after program exit

### 2. **Signal Handling (`signals.c`)**
- **SIGWINCH**: Window resize detection - recalculates layout
- **SIGTSTP**: Ctrl+Z suspend - temporarily restores terminal, suspends process
- **SIGCONT**: Process resumption - re-enters raw mode
- **SIGINT/SIGTERM**: Clean exit - restores terminal and exits gracefully

### 3. **Data Structures (`items.c`)**
- **t_item**: Doubly-linked circular list node:
  - `str`: The text to display
  - `selected`: Whether user selected it
  - `deleted`: Marked for deletion (backspace/delete key)
  - Uses circular linking for easy navigation around the list

- **t_display**: Layout information:
  - Column/row dimensions
  - Calculated text width for padding

- **t_select**: Main program state:
  - Points to items list and current cursor position
  - Terminal settings storage
  - Window dimensions

### 4. **Input Handling (`input.c`)**
- **read_key()**: Reads raw keyboard input and interprets escape sequences:
  - Arrow keys come as `ESC [ A/B/C/D`
  - Delete key is `ESC [ 3 ~`
  - Space, Enter, Backspace recognized directly
  - Returns standardized key codes (KEY_UP, KEY_DOWN, etc.)

### 5. **Layout Calculation (`layout.c`)**
- **calculate_layout()**: Determines multi-column display:
  - Counts non-deleted items
  - Finds longest item string
  - Calculates how many columns fit on screen
  - Determines items per column
  - Handles terminal resizing

### 6. **Rendering (`render.c`)**
- **render_items()**: Displays the selection menu:
  - Uses `tgetstr("so", NULL)` for inverse video (selected items)
  - Uses `tgetstr("us", NULL)` for underline (cursor position)
  - Combines both for selected+cursor position
  - Positions items in calculated columns/rows
  - Pads items to align columns

- **print_item()**: Applies formatting to individual items:
  - Selected: inverse video
  - Cursor: underline
  - Selected + Cursor: inverse video + underline
  - Pads each item to column width

### 7. **Key Handling (`handle_key.c`)**
Navigation:
  - Arrow keys navigate the list (circular)
  - Left/Right arrows move between columns
  - Up/Down arrows move within a column

Operations:
  - Space: Toggle selection, move to next item
  - Delete/Backspace: Remove item from list
  - Enter: Submit selection
  - Escape: Cancel without submitting
  - Ctrl+Z: Suspend/resume properly

### 8. **Main Program (`main.c`)**
- Initializes items from argv
- Sets up terminal, signals, and display
- Main event loop reading keys
- On Enter: outputs selected items space-separated
- On Escape: outputs nothing
- Properly restores terminal and cleans up memory

## Compilation

This project requires:
- A Unix/Linux environment
- GCC or Clang compiler
- termcap development library (`libtermcap-dev` or similar)

**On Linux/Mac:**
```bash
make
./ft_select item1 item2 item3
```

**On Windows with WSL/MinGW:**
```bash
make
./ft_select.exe item1 item2 item3
```

## Usage Examples

```bash
# Simple selection
./ft_select file1.c file2.c file3.c

# In shell command substitution
rm `./ft_select *.txt`

# Shell variables
set response = `./ft_select choice1 choice2 choice3`
```

## Key Features

✓ Circular navigation (wrap around list)
✓ Multi-column layout with automatic resizing
✓ Search via arrow keys
✓ Multiple selections with space key
✓ Delete items with backspace/delete
✓ Proper terminal restoration on exit/signals
✓ Suspend/resume with Ctrl+Z
✓ No memory leaks
✓ Proper error handling

## File Structure

```
ft_select/
├── Makefile          # Main build file
├── author            # Username file
├── ft_select.h       # Main header with structures
├── main.c            # Entry point
├── terminal.c        # Terminal setup/restore
├── signals.c         # Signal handlers
├── input.c           # Key reading
├── items.c           # Linked list operations
├── layout.c          # Display layout calculation
├── render.c          # Rendering logic
├── handle_key.c      # Input processing
└── libft/            # Helper library
    ├── Makefile
    ├── libft.h
    └── ft_*.c        # String utilities
```

## Design Decisions

1. **Circular Linked List**: Allows seamless navigation around the list without special cases for boundaries.

2. **Deletion via Flagging**: Instead of physically removing items, we mark them deleted. This preserves the list structure and makes recovery possible if needed.

3. **Layout Recalculation on Every Render**: Simple and ensures correct display after window resize (SIGWINCH).

4. **Global g_select Pointer**: Required for signal handlers (which can't take context parameters) to access program state.

5. **Raw Terminal Mode**: Necessary for immediate character processing and arrow key support - standard cooked mode only provides line buffering.

6. **Termcaps Over ncurses**: Project requirement, but also demonstrates lower-level terminal control which is more educational.

## Compilation Notes

- The project follows the 42 Norm coding standards
- All functions are prototyped in headers
- Static functions for file-local scope
- No global variables except g_select (required for signals)
- Proper memory management with malloc/free
- Comprehensive error handling

---

To compile on a proper Unix/Linux system with make and gcc installed:

```bash
cd ft_select
make
make clean      # Remove object files
make fclean     # Remove all generated files
make re         # Rebuild everything
```
