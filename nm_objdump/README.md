# ELF Format Exploration

## Overview
This project is a detailed exploration of the **ELF (Executable and Linkable Format)**, focusing on how it is used in Unix-like operating systems for executables, object code, and shared libraries. This repository includes code examples, command usage, and explanations about how to inspect and work with ELF files using common tools like `readelf`, `nm`, and `objdump`.

## Blog Post
As part of the project, I have written a blog post detailing the ELF format. You can read the post at the following link:

[Read the Blog Post on Medium](#)

## Tools Covered
1. **readelf**: Used to inspect ELF headers and sections.
2. **nm**: Used to list symbols in an ELF file.
3. **objdump**: Used to disassemble executable code and dump section contents.

## Commands Overview

### 1. `readelf`
The `readelf` command is used to view the ELF file headers, program headers, section headers, and more.

#### Example:
```bash
readelf -h <file>

