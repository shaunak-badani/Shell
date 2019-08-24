# Shell

## About

> This is a C implementation of the unix CLI (Command Line Interface).

## How to run

First time compilation :
```
make
./shell
```

If attempting to re-run :

```
make clean
make
./shell
```

## Commands implemented :

Command  | Flags | Description
------------- | ---- | -----------
ls  | -[l/a/la/al/] [. / .. / \<directory\>] | Lists files in specified folder in long / short format depending on the flags passed to it.   
pwd | None | Prints the current working directory of the specified folder.
cd | \<folder-name\>  | Changes the working directory of the current process.
echo | any | Prints the arguments passed to it.
pinfo | None / \<pid\> | Gets process info of process id passed as first argument. If no argument is passed, then gets the process info of current process.

Remaining commands are passed to execvp, so the shell supports most of the unix supported commands as well.