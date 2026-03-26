# Build Instructions for ft_select

## Prerequisites

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install build-essential
sudo apt-get install libtermcap-dev
```

### Linux (Fedora/RedHat)
```bash
sudo dnf install gcc make
sudo dnf install libtermcap-devel
```

### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# termcap is usually included with macOS
```

### Windows with WSL (Windows Subsystem for Linux)
```bash
# Inside WSL Ubuntu/Debian
sudo apt-get install build-essential libtermcap-dev
```

### Windows with MinGW
Download and install MinGW from https://mingw-w64.org/
Make sure to include:
- gcc compiler
- make utility
- libtermcap development libraries

## Compilation Steps

### Standard Build
```bash
cd ft_select
make
```

This will:
1. Compile the libft library first
2. Compile all ft_select source files
3. Link everything together into `ft_select` executable

### Clean Build
```bash
make re
```

This will completely clean and rebuild the entire project.

### Debug Build (if needed)
To add debug symbols:
```bash
CFLAGS="-Wall -Wextra -Werror -g" make
```

## Verifying Installation

After successful compilation, test the program:

```bash
./ft_select hello world test
```

You should see:
- List displayed in your terminal
- Arrow keys move the cursor (underlined)
- Space key changes selection (inverse video)
- When you press Enter, selected items appear on stdout

## Compilation Issues and Solutions

### Issue: "tgetent, tgetstr not found"
**Cause**: termcap library not linked
**Solution**:
```bash
# Edit Makefile and ensure LDFLAGS includes -ltermcap
LDFLAGS = -ltermcap
```

### Issue: "tcgetattr, tcsetattr not found"
**Cause**: termios header or function not available
**Solution**: Ensure you're building on a Unix-like system (Linux, macOS, BSD)
- Windows requires WSL or MinGW

### Issue: "Segmentation fault on startup"
**Causes**:
- TERM environment variable not set
- Terminal capabilities database (.terminfo) not found
**Solutions**:
```bash
# Set terminal type
export TERM=xterm-256color
# or
export TERM=linux

# Then run
./ft_select item1 item2 item3
```

### Issue: Terminal stays in raw mode after crash
**Solution**: Restore terminal manually
```bash
# Type (you won't see what you type):
reset
# Press Enter
# Terminal should return to normal
```

## Advanced Compilation

### With optimization flags:
```bash
CFLAGS="-Wall -Wextra -Werror -O2" make
```

### For different architectures:
The Makefile supports cross-compilation via CFLAGS:
```bash
# 32-bit compilation
CFLAGS="-Wall -Wextra -Werror -m32" make
```

### Verbose compilation (see all commands):
```bash
make clean && make V=1
# Or modify Makefile to remove the @ symbol before commands
```

## Project Structure During Build

```
ft_select/
├── ft_select           # Final executable (after compilation)
├── *.o                # Object files (cleaned with 'make clean')
├── libft/
│   ├── libft.a        # Static library (cleaned with 'make fclean')
│   └── *.o            # Object files
```

## Testing After Compilation

### Basic functionality test:
```bash
./ft_select apple banana cherry

# Try:
# - Arrow keys to navigate
# - Space to select
# - Backspace to delete
# - Enter to confirm selection
# - Escape to cancel
```

### In shell scripts:
```bash
#!/bin/bash

selected=$(./ft_select "Option 1" "Option 2" "Option 3")
if [ $? -eq 0 ]; then
    echo "You selected: $selected"
else
    echo "Selection cancelled"
fi
```

### With file selection:
```bash
./ft_select *.c

# Select which C files to process
```

## Target Specific Build Notes

### Linux
- Full termcap support
- TIOCGWINSZ ioctl supported
- SIGWINCH for window resize works perfectly
- No special configuration needed

### macOS
- termcap deprecated but still available
- May need to set TERM correctly
- Works with Terminal.app, iTerm2, etc.

### WSL on Windows
- Works as Linux
- May have terminal size detection issues (use `stty cols 120 rows 40`)

### MinGW/MSYS2 on Windows
- Requires termcap library
- Limited terminal capability database
- May need explicit TERM setting

---

## Submitting to 42

When ready to submit:
```bash
make clean        # Remove all object files
make fclean       # Remove all generated files including binary
# Ensure author file exists with username
git add .
git commit -m "Submit ft_select project"
git push
```

The correctors will then:
```bash
cd ft_select
make
./ft_select test1 test2 test3
```

And evaluate your implementation.
