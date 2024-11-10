# Assembler and Emulator Project

This project includes an **assembler** and an **emulator** for developing, testing, and executing assembly programs. It supports assembling instructions, managing labels and mnemonics, and running the assembled code with options for memory inspection and tracing.

---

## Project Structure

### 1. Assembler

The assembler is designed with the following features to ensure efficiency, readability, and accuracy:

- **Data Structures**:  
  - **Label Table**: Stores labels with their respective addresses.
  - **Mnemonic Table**: Maintains mnemonics with expected operand formats.
  
- **Single Routine Pass**: Uses a single routine to perform both assembly passes for streamlined execution.

- **Error Detection**: Detects label errors for improved program correctness.

- **Consistent Formatting**: 
  - Code is formatted for readability, with sensible variable, function, and type names.
  - Includes explanatory comments for clarity.
  
- **Listing File**: Generates an advanced listing file to aid in debugging and program analysis.

- **Test Program Compatibility**: Capable of assembling test programs to verify functionality.

- **SET Instruction**: Supports and demonstrates the `SET` instruction for enhanced assembly control.

### 2. Emulator

The emulator provides a virtual environment to execute the assembled code, with debugging and analysis tools:

- **Data Structures**:
  - **Memory**: Stores program data and instructions for execution.
  - **Mnemonic Table**: Holds mnemonics with expected operand formats.
  
- **Object File Loader**: Loads object files generated by the assembler.

- **Memory Dump Options**:
  - `-before` / `-after`: Dumps memory contents before or after execution.

- **Trace Option**: 
  - `-trace`: Prints detailed execution results to track program flow.
  
- **Error Detection**: Identifies errant programs and outputs relevant information.

---

## Usage

### 1. Assembling a Program

To assemble a test program, pass it to the assembler. The assembler detects label errors and outputs a listing file.

Example:
$ assembler example_program.asm

---

### 2. Running the Emulator

The emulator can load the assembled object file and provides options to inspect memory and trace execution:

- **Memory Dump Before/After Execution**:  
  Use the `-before` or `-after` options to dump memory contents before or after execution.

$ emulator -before example_object.obj
$ emulator -after example_object.obj

- **Execution Trace**:  
  Use the `-trace` option to print detailed results for each executed instruction, showing the program's flow and modifications to memory.

$ emulator -trace example_object.obj

---

## Testing

The assembler has been tested using two example programs to confirm its functionality and error-detection capabilities. These tests verify the assembler’s handling of labels, mnemonics, and the SET instruction.

---
