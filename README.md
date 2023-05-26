TIRD is a very simple esolang. its main data structure is a Binary Tree of Values, where each Value can be a Number or String.
there is also the Tree Pointer, which points to a single node in the binary tree.

operations: `)`, `(`, `^`, `):`, `(:`, `^:`, `.`, `,`, `#`, `[...|...]`, `{...|...}`, `+`, `-`, `%`, `=`, `!`.

numeric constants are defined as matching the regex `/[\d.]+/`. strings match `/"[^"]*"/`.

- `)`: moves the tree pointer to the right branch of the node it's currently pointing to. if it doesn't exist, create one.
- `(`: moves the tree pointer to the left branch of the node it's currently pointing to. if it doesn't exist, create one.
- `^`: moves the tree pointer to the parent node of the node it's currently pointing to. if it doesn't exist, don't do anything.
- `):`: replace the current node's value with the value of the node in its right branch, if there is no right branch, do nothing.
- `(:`: replace the current node's value with the value of the node in its left branch, if there is no left branch, do nothing.
- `.`: print the current node's value.
- `,`: get input as a string and put it into the current node.
- `#`: convert the current node's value to a number.
- `[cond|body]`: if the node pointed to after executing `cond` has a truthy value, execute `body`. note that any edits to the tree
  in `cond` are reversed before executing `body`.
- `{cond|body}`: while the node pointed to after executing `cond` has a truthy value, keep executing `body`.
- `+`: add the left and right branches of the current node's values, then put the result in the current node. if one of the branches doesn't
  exist, its value is treated as being the current node's value.
- `-`: subtract the right branch of the current node's value from the left branch's, then put the result in the current node. if one of the
  branches doesn't exist, its value is treated as being the current node's value.
- `%`: modulo the left branch's value by the right branch's value, then put the result in the current node. if one of the branches doesn't
  exist, its value is treated as being the current node's value.
- `=`: check if the left and right branches of the current node's values' are equal, then put the result in the current node. `1.0` if true,
  `0.0` if false. if one of the branches doesn't exist, its value is treated as being the current node's value.
- `!`: binary not the current node's value. `0.0` is false, and anything but is true. the output is either `0.0` or `1.0`.

## example programs
- count from 1 to 10, exclusive: `1{)10^<|.)1^+}`
- fizzbuzz: `1{)100^<|[)15^%)0^=|["FizzBuzz".|]][)15^%)0^=!|[)5^%)0^=|["Buzz".|]][)5^%)0^=!|[)3^%)0^=|["Fizz".|]][)3^%)0^=!|.]]])1^+}`
- truth machine: `,#[!|.]{|.}`
