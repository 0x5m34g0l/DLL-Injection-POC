# DLL Injection Proof of Concept

## DISCLAIMER
**FOR EDUCATIONAL AND RESEARCH PURPOSES ONLY**

This repository contains a proof-of-concept DLL injector for Windows. 
Only use on systems you own or have explicit permission to test.

## Description
A simple DLL injector demonstrating the classic `CreateRemoteThread` technique. 
The injector loads a DLL into a remote process by:
1. Allocating memory in the target process
2. Writing the DLL path
3. Creating a remote thread calling `LoadLibraryA`

## Features
- Process handle acquisition with proper privileges
- Remote memory allocation and writing
- Thread creation in foreign processes
- Clean resource management
- Error handling and logging

# Build example DLL (harmless MessageBox)
cl /LD /Fe:example.dll src/example_dll.cpp user32.lib

# Usage
loader.exe [dll_path] [target_pid]

# Example using notepad
loader.exe example.dll [notepad_pid]
