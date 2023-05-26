# TIRD

TIRD is a very simple esolang. Its main data structure is a Binary Tree of Values, where each Value can be a Number or String.
There is also the Tree Pointer, which points to a single node in the binary tree.

## Instructions

Operations: `)`, `(`, `^`, `):`, `(:`, `^:`, `.`, `,`, `#`, `[...|...]`, `{...|...}`, `+`, `-`, `%`, `=`, `!`.

Numeric constants are defined as matching the regex `/[\d.]+/`. strings match `/"[^"]*"/`.

| Instruction    | Description                                                                                                                                                                                                   |
| -------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `)`            | Moves the tree pointer to the right branch of the node it's currently pointing to. If it doesn't exist, create one.                                                                                           |
| `(`            | Moves the tree pointer to the left branch of the node it's currently pointing to. If it doesn't exist, create one.                                                                                            |
| `^`            | Moves the tree pointer to the parent node of the node it's currently pointing to. If it doesn't exist, don't do anything.                                                                                     |
| `):`           | Replace the current node's value with the value of the node in its right branch, If there is no right branch, do nothing.                                                                                     |
| `(:`           | Replace the current node's value with the value of the node in its left branch, If there is no left branch, do nothing.                                                                                       |
| `.`            | Print the current node's value.                                                                                                                                                                               |
| `,`            | Get input as a string and put it into the current node.                                                                                                                                                       |
| `#`            | Convert the current node's value to a number.                                                                                                                                                                 |
| `[x\|y]`       | If the node pointed to after executing `x` (condition) has a truthy value, execute `y` (body). Note that any edits to the tree in `x` are reversed before executing `y`.                                      |
| `{cond\|body}` | While the node pointed to after executing `x` (condition) has a truthy value, keep executing `y` (body).                                                                                                      |
| `+`            | Add the left and right branches of the current node's values, then put the result in the current node. If one of the branches doesn't exist, its value is treated as being the current node's value.
| `-`            | Subtract the right branch of the current node's value from the left branch's, then put the result in the current node. If one of the branches doesn't exist, its value is treated as being the current node's value.
| `%`            | Modulo the left branch's value by the right branch's value, then put the result in the current node. If one of the branches doesn't exist, its value is treated as being the current node's value.
| `=`            | Check if the left and right branches of the current node's values' are equal, then put the result in the current node. `1.0` if true,
| `0.0`          | If false. If one of the branches doesn't exist, its value is treated as being the current node's value.
| `!`            | Binary not the current node's value. `0.0` is false, and anything but is true. The output is either `0.0` or `1.0`.

## Example programs

### Count from 1 to 10, exclusive 

```
1{)10^<|.)1^+}
```

### Fizzbuzz 

```
1{)100^<|[)15^%)0^=|["FizzBuzz".|]][)15^%)0^=!|[)5^%)0^=|["Buzz".|]][)5^%)0^=!|[)3^%)0^=|["Fizz".|]][)3^%)0^=!|.]]])1^+}
```

### Truth-machine

```
,#[!|.]{|.}
```
