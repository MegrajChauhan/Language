// var a: u8 = 12 + 13;
@
   +
  / \
 12 13
@

// var b: u8[1-2+4*18/3] = 1111 + 2222 - 3233 - -12 + +11;
@
               +
            /    \
           -      *
          / \    / \
         1   2  4   /
                   / \
                  18  3

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

// var c: u8 = fjd;
@
  fjd
@

// var d: u8 = a ++ b; // error

// var e: u16 = 2 * 3 / (17 - 2);
@
      *
     / \
    2   /
       / \
      3   -
         / \
        17  2
@

// var f: u8 = (2 * 2; // error
// var g: u64 = 33); // error

// var h: u8 = a;
@
  a
@

var i: u16 = test[12 * 32 - x];
@
     test
      |
      -
     / \
    *   x
   / \
  12 32
@

var j = xyz[ pqr[12] * mno[ abc[b * (c - 12) ] / 23 ] & count] / (12 + 17) * 2;
@
             *
            / \
           \   2
        /    \
       xyz    +
        |    / \
        &   12 17
     /    \
    *    count
  /  \
 pqr mno
  |   |
 12   /
    /  \
  abc  23
   |
   *
  / \
 b   -
    / \
   c  12

@

var k = (12 + 3 * 4) / 2;
@
      \
     / \
    +   2
   / \
  12  *
     / \
    3   4
@

// var l = (2 + 3 * (4 - 5); // error, missing closing parentheses

var m = arr1[arr2[4 * 2] + arr3[(5 + 1) * 3]];
@
             arr1
              |
              +
         /        \
       arr2       arr3
        |          |
        *          *
       / \        / \
      4   2      +   3
                / \
               5   1
@

var n = 5 | (2 & 3) ^ 7;
@
     |
    / \
   5   ^
      / \
     &   7
    / \
   2   3
@

// var o:  u8[2 * (3 - 4) = {12,34}; // error

var p = 12 + (a * 15) / 4;
@
      +
     / \
    12  /
       / \
      *   4
     / \
    a  15
@

// var q = ++a; // error

var r = a++;
@
    ++
   /
  a
@

var s = 1 << (3 * 2);
@
     <<
     /\
    1  *
      / \
     3   2
@

// var t:u8[5 * (3 + 1] = 22; // error

var u:bool = (5 < 10) && (2 == 2) || !false;
@
      ||
      /\
     && !
    / \  \
   <  ==  false
  /\  /\
 5 10 2 2
@

// var v = arr["string"] + 10; // error

var w: u16 = (4 + 5) * (6 - 2) / 3;

// var x: u8 = 2 = 3; // error

var y: u16 = 2 & (3 ^ 2);

// var z: u8[4 * (5 + 3)]] = 20; // error

var aa: u8 = (8 & 3) + (4 | 2);

// var ab: u8 = 5 &| 3; // error

var ac: u16 = (3 + 2) * 4 / (1 + 2);

// var ad: u8 = 300~; // error

var ae: u32 = array1[(array2[5] + 12) * (array3[2] - 1)];

var af: u8 = ~4;

var ag: bool = (a > 2) && ((b < 5) || (c == d));

// var ah: u8 = 12 //; // error

var ai: bool = ((5 * 2) - 3) >= (10 / 2 + 1);

// var aj = ; // error