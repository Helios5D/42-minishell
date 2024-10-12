# 42-Minishell, by Maxime Rochedy and Hélios Daher

Welcome to our implementation of **Minishell**, a core project from the **École 42** common core. This repository contains our own version of a minimalist Unix shell, developed by **Maxime Rochedy** (mrochedy) and **Hélios Daher** (hdaher). The project challenges us to recreate essential features of a shell, helping us deepen our understanding of process control, system calls, and command parsing. Our **Minishell** is compatible with both **Linux** and **macOS** systems.

This project earned a score of **110/100**, thanks to a **complete mandatory** implementation and bonus features such as **wildcard handling**.

<img width="198" alt="110/100 grade" src="https://github.com/user-attachments/assets/4be14bc6-0746-4bcb-9cd6-ee4f319ab093">

## About the Project

**Minishell** is a custom-built Unix shell designed to mimic the behavior of major shells like **bash**, while staying minimal and focused. This project was a collaborative effort, following the principles taught at **42**, and it was crucial in enhancing our skills in C programming, process management, and inter-process communication.

Our **Minishell** supports core features such as:

- Running executables located in system paths.
- Handling environment variables and creating/exporting them.
- Implementing **pipes** and **redirections** (input/output).
- Handling built-in commands like `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- Managing signals like `CTRL+C`, `CTRL+D`, and `CTRL+\` to ensure a user-friendly shell experience.

We have also added a bonus feature: **wildcard handling**. Our **Minishell** supports patterns with the `*` character, allowing for flexible matching, such as `*.md`, `README*`, `REA*E.md`, `R*E*A*D*M***E*.*m*d`, and more. Please note that wildcards do work only inside the **current directory**.

The project adheres to the POSIX standard, and special care was taken to ensure compatibility between **Linux** and **macOS** environments, making our Minishell versatile across different platforms.

Please note that while this code is provided for **learning purposes**, you should not copy it for your own submissions. Please respect the 42 school's policies on plagiarism.

## Getting Started

To compile and use **Minishell**, you will need to install the **Readline** library, which provides line-editing and command history capabilities, similar to other popular shells.

### Linux

First install Readline:

```bash
sudo apt install libreadline-dev
```

Then, you should be able to install and compile Minishell:

```bash
git clone https://github.com/Helios5D/42-minishell.git
cd 42-minishell
make
./minishell
```

### MacOS

Ensure you have **Homebrew** installed, then install Readline:

```bash
sudo brew install readline
```

At this point, you should be able to install and compile minishell:

```bash
git clone https://github.com/Helios5D/42-minishell.git
cd 42-minishell
make macos
./minishell_macos
```

From here, you can start using **Minishell** like any other Unix shell. It supports command execution, piping (`|`), redirections (`>`, `<`, `>>`), heredocs (`<<`) and basic shell operations.

## Contributing

If you found this project interesting or helpful, feel free to ⭐️ **star** ⭐️ the repository. Contributions are welcome through pull requests, but please respect the **42 school's policies** on collaboration and plagiarism.

We welcome bug reports and suggestions for improvements. If you encounter any issues or have ideas on how to enhance **Minishell**, don't hesitate to open an issue.

Thank you for visiting and happy coding!
