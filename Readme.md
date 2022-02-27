# IPK Project 1 - Server info

The first project for the BUT FIT course IPK year 21/22. A simple TCP server that provides basic information about the server.

Author: Tadeáš Kozub _xkozub06_

Contact: xkozub06@fit.vutbr.cz


### Installation

Run the Makefile to compile the project:

```$ make```

This creates a file called `hinfosvc`

or if you want to also start the server:

```$ make run```

for cleanup use:

```$ make clean```

## Usage

Start the server using the following command:


```$ ./hinfosvc```

Optionally, you can provide the port on which the server will listen (the default port is _8000_):

```$ ./hinfosvc 12345```

If you want to use the same terminal, you can either send the current process to background by pausing
the program using `Ctrl + Z` and using the `bg` command or you can start the server in the background like this:

```$ ./hinfosvc 12345 &```

You can then switch back to the program using the `fg` command and terminate it with `Ctrl + C`.

### List of possible commands

+ hostname  -   returns the hostname of the server machine
+ cpu-name  -   return the CPU manufacturer and model data
+ load      -   returns the current CPU load of the server in the format **integer**%

## Examples

You can access the information either from the web browser or by using tools like curl or GET.

```
$ curl http://localhost:12345/hostname
$ curl http://localhost:12345/cpu-name
$ curl http://localhost:12345/load
```

note: substitute _12345_ with the server's port. You can use the same address in your web browser.
