# Fastlog: A Lightweight Logging Library for C

Fastlog is a simple and efficient logging library for C, designed to provide flexible logging capabilities with minimal overhead. It supports logging to both files and syslog, making it suitable for a wide range of applications.

## Features

- **File Logging**: Log messages to a file with automatic file handling.
- **Syslog Integration**: Fallback to syslog if file logging fails or is disabled.
- **Thread-Safe**: Designed to be used in multi-threaded environments.
- **Customizable**: Supports custom log paths, error handling, and message formatting.
- **Lightweight**: Minimal dependencies and low resource usage.
- **Cross-Platform**: Works on Linux and macOS (including Apple Silicon).

## Installation

### Prerequisites

- A C compiler (e.g., `gcc`, `clang`)
- `make` (for building the project)

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/fastlog.git
   cd fastlog
   ```
2. Build the library and test program:
   ```bash
   make
   ```
3. (Optional) Install the library system-wide:
   ```bash
   sudo make install
   ```

    On macOS, the library and headers will be installed to `/usr/local/lib` and `/usr/local/include/flog`, respectively.
  
4. Run the test program to verify the installation:
   ```
   ./test
   ```

## Usage

### Basic Logging

To use Fastlog in your project, include the `flog.h` header and link against the `libflog` library.

   ```c
   #include "flog.h"

    int main(void) {
        // Set the log file path
        logpath("/tmp/myapp.log");

        // Log a message
        flog("Application started.");

        // Log formatted messages
        flog("User ID: %d, Username: %s", 1234, "john_doe");

        // Close the log file
        closelogfile();

        return 0;
    }
   ```

## Advanced Features
 - Fallback to Syslog: If the log file cannot be opened, messages are automatically written to syslog.

 - Error Handling: Check the return value of logpath to handle errors gracefully.

 - Custom Log Paths: Dynamically change the log file path at runtime.

### Example: Error Handling
  ```c
  if (logpath("/invalid/path/to/logfile.log") == LOG_ERROR) {
      flog("Failed to open log file. Falling back to syslog.");
  }
  ```

  ## API Reference

  ### Functions

  - `void flog(const char* output, ...)`

    Logs a formatted message to the current log file or syslog.

  - `int logpath(const char* logfile)`

    Sets the log file path. Returns NO_ERROR on success or LOG_ERROR on failure.

  - `void closelogfile(void)`

    Closes the current log file.

  - `char* getlogpath(void)`

    Returns the current log file path.

  - `void flushlog(void)`
    Flushes the log file buffer.

  - `int openlogfile(char* logfilename)`

    Opens a log file. Used internally by logpath.

  - `void writesyslog(const char* format, ...)`

    Writes a message to syslog. Used internally as a fallback.