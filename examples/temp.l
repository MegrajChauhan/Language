// The name of the language itself is "Language"
// This is a comment(one liner)
@
  Multi-line comments
@

// I don't want macros
// I like the types from Zig but we can't have something like u4 or u31 which feels weird.
var a: u8 = 12;
const num: u32 = 9727412.35453.555;

// Similary 'f32', 'f64' and 'i8', '16' etc for signed
var name: u8[] = {}; // Error(unknown length)
var name1: u8[13] = {}; // allowed(random values)
var name2: u8[] = "Hello Yo"; // allowed(auto length detection) and also adds a null terminator

// var ptr: *u8 = &name; // allowed
// var ptr1: **u8 = &ptr; // pointer to a pointer
// var ptr2: const *u8 = &num; // a pointer to a constant value
// const ptr2again: const *u8 = &num; // a pointer to a constant value but it cannot point to anything else
// const constptr: *const *u8 = &ptr2again; // pointer to a pointer of a constant value

// const a_null: *void = null; // NULL pointer

// const variables must be assigned immediately

// instead of macros, we will introduce built-in functions

$include (examples/example.l) // this is included right away

#
  // the code here is executed at compile time and we may make this more versatile with time
  // perform things platform-specific
  if (platformOs == "linux") 
    $include (examples/example.l)
  
  // we keep this simple so no else and else if
  // we only need this to include platform-specific thing so no extra needs
  // we could expand in future.
#

// Namespaces may be exported and used.

// The compiler will also be able to deduce the type by itself.

Defs
{
  // private namespace
  enum Error_t
  {
    FALSE
    TRUE = 0
    AMBIGIOUS
  };
}

global Temp
{
  // Anything here can be accessed as Temp:: including the variables

  struct Name
  {
    hello: u8[12];
    is_okay: bool;
@
    fn init(this: *Name, msg: u8[], state: bool) void
    {
      this->hello = msg;
      this->is_okay = state;
    }

    fn errorIfFalse(self: *Name) bool
    {
      if (self->is_okay == false)
         return false;
      return true;
    }
@ 
  };

  var instance: Name = Name(); // instance of Name
  instance.hello = "Hello!";
  // There will be no single quotes.
  instance.is_okay = true;
}