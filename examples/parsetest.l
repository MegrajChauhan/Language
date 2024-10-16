var inval3: Example = ' ';
var a: u8 = 12; // valid
const b: u8 u16 u32 u64 = 11; // valid but for the parser only
var c: u8[] = {12, 23, 234,52}; // valid
var d: u8[a @some operators@ b] = {'a', 'b', 'c'};
const e = 123; // auto type deduction(yet to be implemented)

var name: u8[] = "Hello World!";

@
struct Example
{
    field1: bool; 
}
@

var inval: Example = " This is invalid";
var inval2: Example = 'A';
var inval4: Example = " multi-line string
                                         
                   but this is an error";