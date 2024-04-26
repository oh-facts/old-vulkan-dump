### style guide

This codebase uses modern C++ features. Please respect that.

I am not justifying myself anymore since no one askd. Maybe an old commit mentions it. Feel free to open an issue to discuss them.

These are rules that contributors need to abide by.
I am keeping them brief and removing snide to save time.

## Don'ts

- Don't use const.
- Don't use the crt.
- Don't use the stl.
- Don't use templates
- Don't use virtual.
- Don't use inheritance.
- Don't use exceptions.
- Don't use RAII.
- Don't use namespaces
- Don't use malloc / calloc / realloc / free

## Do's

- Use tabs. 1 tab = 2 spaces.
- Don't return early from functions if you can help it.
- switch is written like
```.cpp
switch(type)
{
    case type_a:
    {
        printf("The type is a on this one");
    }break;
    case type_b:
    {
        printf("The type is b on this one");
    }break;

    default:
    {
        printf("WUHAAAAT");
    }break;
}
```
- single assignments / statements per line 
- use designated initializers whenever possible. put everything on a separate line

- get line wrap, then wonder if you want to break.

- if/elses/functions are always written with brackets like this

```
if(something)
{
    // 1 or more lines
}
else
{

}
```

- MACRO_CONSTANTS_ARE_SCREAMING_SNAKE_CASE
- macroFunctionsLikeThis()
- snake_case_for_everything_else 

enum hi_type
{
	hi_uninitialized,
  hi_hello,
  hi_bye,
  hi_num
};

- no typedefs for pointers and structs
- stdint types are typedef'd. Check `include/common.h`
- unions inside of structs are anonymous

- Use local blocks { } and write //comments above it to hold blocks of code that you want to semanticaly divide.
- i will delete gratituous functions

- avoid goto. Sometimes they are needed for elegant quitting. But are avoidable in 99% of cases. Keyword - "avoid"

- Use note(name) or todo(name) to annotate as appropriate
- mention unhandled edge cases / bugs / quirks / something misc that could be helpful to the next programmer that works on the code.
- this isn't an invitation to comment gratituosly.

- ctrl f Arena and ArenaTemp to learn how memory is managed in this codebase.


- booleans are b32 (stdint int32 type). true is 1, false is 0

- unity build.
- only math is operator overloaded
- all (99%) functions are internal. globals are global and local static variables are local_persist. These are all just `static` that have been `#define`'d over