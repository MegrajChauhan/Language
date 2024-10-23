var a: u8 = 12 + 13;
var b: u8[1-2+4*18/3] = 1111 + 2222 - 3233 - -12 + +11;

@
                +
              /   \
            1111   +
                  / \
                 -   +
                / \   \
            2222   -   11
                  / \
              3233   -
                      \
                      12
@

// var c: u8 = fjd; // error
// var d: u8 = a ++ b; // error

var e: u16 = 2 * 3 / (17 - 2);
@
      *
     / \
    2   /
       / \
      3  SUB_EXPR
@

// var f: u8 = (2 * 2; // error
// var g: u64 = 33); // error