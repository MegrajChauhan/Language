global Temp
{
    // add to the Temp namespace
}

// what about using assembly(Merry's assembly not the other ones yet)?
// we could

$assembly (../Merry/lib/stdinit.asm) // just a test

// The compiler will not actually include the assembly but remember that it was included
// How do i access the code inside?

// the compiler will generate extra code to put in the necessary arguments
// based on the convention, if the function returns in Ma, anything may use it.
asm fn init ( @The arguments here exactly as in a function definition@ );
asm var x: u8; // The variable in the assembly code
asm const y: bool; // the constant in the assembly code.

// we cannot use the values defined as "dc" yet

// In the assembly code, "dc" doesn't define a mutable or immutable variable.
// it is more like #define in C and so everything is mutable and that cannot be changed.
// but if something needs to be immutable then the use of const here will ensure that.
// But what if someone else wrote a separate assembly code and changed the variable in it
// and proceeded to make it mutable via a setter function like above?
// Then we cannot do anything really.
// We either solve this issue in the assembler itself or just trust everybody
