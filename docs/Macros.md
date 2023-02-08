# Macros/Language "Extensions"

Macros pave the way fpr some really cool functions. Some macros provided by mclib looks like language extensions
and should make the c code more readable and give it a more modern look. But all of them are totally optional!.

## Pointer Macros

The `&` and `*` operators for pointer can be replaced by `refof` and `valof`. For example the following code
example shows how a standard C code with pointers code be rewritten by using the mentioned macros.

```c
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void main() {
    int x = 10;
    int y = 20;
    swap(&x, &y);
}
```

The Version with macros:
```c
void swap(ref(int) a, ref(int) b) {
    int temp = valof a;
    valof a = valof b;
    valof b = temp;
}

void main() {
    int x = 10;
    int y = 20;
    swap(refof x, refof y);
}
```

Which looks a bit more readable in my eyes but obviously also more verbose.

## Deferred Function Call

With `defer` you can provide a begin function and an end function (e.g. for allocation and freeing), which 
are automatically executed (or inserted in source code) at the right time.

For example, this code...

```c
gui_begin();

gui_draw_rectangle(...);

gui_end();
```

...can be rewritten in the following way.

```c
defer(gui_begin(), gui_end()) {
    gui_draw_rectangle();
}
```

But after the preprocessor it will look like the code example above. But for some eyes it might be more readable
 and it could prevent forgetting to call the `gui_end` function. Furthermore, you can define your own macro like:

```c
#define gui defer(gui_begin(), gui_end())
```

And use it like:

```c
gui {
    gui_draw_rectangle();
}
```

And this looks totally like a language extension of C. This style is used for various `using_xxx` macros 
in the mclib library (e.g. dynamic array or string builder).

If you don't have a begin function but just an end function, you can use the `scope` macro, which takes
only a finalizer as parameter

```c
FILE* file = fopen(...);

scope(free(file)) {
    // Logic needed fo the file
}
```

Find out more about that in the Modern C Talk: https://www.youtube.com/watch?v=QpAhX-gsHMs

## Resources and Inspiration
* https://accu.org/conf-docs/PDFs_2021/luca_sass_modern_c_and_what_we_can_learn_from_it.pdf
* https://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr