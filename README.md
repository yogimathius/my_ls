# Welcome to My Ls
***

## Task
Write a programm called my_ls. Following the specifications of this man page.

Build my own ls program handling specific flags (-t, -ta, -a)

## Description
For each operand that names a file of a type other than directory, my_ls displays its name as well as any requested, associated information. For each operand that names a file of type directory, my_ls displays the names of files contained within that directory, as well as any requested, associated information.

If no operands are given, the contents of the current directory are displayed. If more than one operand is given, non-directory operands are displayed first; directory and non-directory operands are sorted separately and in lexicographical order.

The following options are available:

-a Include directory entries whose names begin with a dot (.).
-t Sort by time modified (most recently modified first) before sorting the operands by lexicographical order.


## Installation

use Makefile to make the program. No other installation necessary. Once installed you can run it

## Usage

```
$>./my_ls > my_ls.output
$>ls -1 > ls.output
$>diff my_ls.output ls.output
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
