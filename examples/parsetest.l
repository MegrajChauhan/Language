var a: u8 = 12; // valid
const b: u16 = 11; // valid but for the parser only
var c: u8[] = {12, 23, 234,52}; // valid
var d: u8[3] = {'a', 'b', 'c'};
const e = 123; // auto type deduction(yet to be implemented)

var name: u8[] = "Hello World!";
var char: u8 = '2';

@
struct Example
{
    field1: bool; 
}

var inval3: Example = ' ';
var inval: Example = " This is invalid";
var inval2: Example = 'A';
var inval4: Example = " multi-line string
                                         
                   but this is an error";
@

@
 Array length must be known at compile-time.
 Expressions containing constant variables are allowed as length expressions.
@