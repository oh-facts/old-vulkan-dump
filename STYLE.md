### style guide

### 1 Indentation:
- Use tabs. 1 tab = 2 spaces.
- Multiple indentations even upto 8 are fine. Put body in a function only if you find yourself repeating code or if the function is absurdly large.
- Don't return early from functions if you can help it. Code flow is easier to read if you have a defined entry and exit. Multithreading is also easier in the future.

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

### 2 Breaking long lines and strings

- my award winning editor has line wrap so I don't know what to say. If its easy to follow, break it, if not, don't

### 3 Placing Braces and Spaces

- if/elses/functions are always written with brackets like this

```
if(something)
{
    // 1 or more lines
}
```

### 3.1
https://www.kernel.org/doc/html/v4.10/process/coding-style.html#spaces

### 4
- MACRO_CONSTANTS_ARE_SCREAMING_SNAKE_CASE
- macroFunctionsLikeThis()
- snake_case_for_everything_else 

enum hi
{
	hi_uninitialized,
  hi_hello,
  hi_bye,
  hi_count
};

### 5
- no typedefs for pointers and structs
- stdint types are typedef'd. Check `include/common.h`
- enums and unions are typedef'd
- unions inside of structs are anonymous

Note: If this is a C codebase, we do

```
struct foo *foo;
```

if C++, we do

```
foo *foo;
```

### 6
- functions are as long as need be. Use local blocks { } and write //comments above it to hold blocks of text that you want to semanticaly divide. Make a function only if you find yourself writing the same code. Or if its an easier representation (consider local macros for this. My award winning language will have proper aliases). 

### 7
- avoid goto. Sometimes they are needed for elegant quitting. But are avoidable in 99% of cases.

### 8
- Use note(name) or todo(name) to annotate as appropriate
- always mention unhandled edge cases / bugs / quirks / something misc that could be helpful to the next programmer that works on the code. Heck, if you were stumped by a problem while implementing it, share why you did what.
- somethings are complex and some things aren't. if someone puts everything inside the self documenting code banner, they have never done anything remotely complex.
- that being said, don't comment unless required. If you write something, I'll assume its worth my time. If its not, I will take snatch that time from your lifespan to restore my own.

### 9 - 14 (not relevant / covered elsewhere)

### 14
- only one allocation is made, which is zero initialized, and then that memory is given to whoever needs it. This memory is never freed.
- for times when the upper limit for allocations cannot be predicted (as is case for parsing files that are several thousands of lines long), a different strategy will be used. We haven't reached this state, so we won't be exploring it.

### 15
- inline doesn't do anything. modern compilers ignore it. Don't use it.

### 16

- booleans are b32. true is 1, false is 0

### misc
- Files include nothing, except for two cases - 1) `main.cpp` includes everything it needs (with < >). 2) `xxx_inc.h` includes other files inside its directory (with " ")
- mark all functions with internal linkage as internal and all globals with global and all local static vars with local_persist
- header gaurds in header files.
- Don't use const.
- Don't use the crt.
- Don't use the stl.
- Don't use templates
- Don't use virtual.
- Don't use exceptions.
- Don't use new / delete
- I don't see enough value in namespaces. This is subject to change.
- only math is operator overloaded