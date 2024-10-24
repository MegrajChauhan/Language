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

var c: u8 = fjd;

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

var h: u8 = a;

var i: u16 = test[12 * 32 - x];

var j = xyz[pqr[12] * mno[abc[b * (c - 12)] / 23 ] & count] / (12 + 17)* 2;
