# kprintf
It is just my cross-plaform variant of printf.
Unlike in standard printf, `%l...` and `%ll...` placeholders dont depend on size of `long int` and `long long int`. And you can change terminal colors by unix codes (`\e[92m`) even on Windows.

| type           | placeholder             |
|----------------|-------------------------|
| i8 / i16 / i32 | %i / %d                 |
| i64            | %li / %ld / %lld / %lli |
| u8 / u16 / u32 | %u                      |
| u64            | %lu / %llu              |
| f32 / f64      | %f                      |
| char           | %c                      |
| char[]         | %s                      |
| void\*         | %p                      |
| 32bit or less  | %x                      |
| 64bit          | %lx                     |

<br>

# kprint
I don't really like printf function (and its variants), so i made safer and more convinient replacement.

| function | returns | arguments |
|----------|---------|-----------|
| kprint   | void/throw   | kp_fmt, void\*, kp_fmt, void\*... |
| ksprint  | Maybe<char\*>| kp_fmt, void\*, kp_fmt, void\*... |
| kfprint  | Maybe<void>  | FILE\*, kp_fmt, void\*, kp_fmt, void\*... |

## how to use it:
+ **format construction:**  
    ```
    kp_fmt fmt= kp_fgColor | kp_bgColor | kp_dataFmt | flags | ktid;
    ```
    [more about `kp_fmt`](kp_fmt.md)
    + fgColor and bgColor can be set to change console output color
    + you should set dataFormat for `int`/`uint`/`float`/`char\*` arguments and ktid for other types 
    + flags can be set to modify TypeDescriptor.toString() behavior
    + don't forget to set TypeDescriptor.toString when registering type, or kprint will crash

+ **using base type arguments:**   
    you can just put them into a function
    ```
    kprint(kp_h|kp_upper|kp_prefix, 255);
    ```
    output: 0xFF
+ **using other registered types:**  
    should be sent as pointers
    ```
    Maybe m=MaybeNull;
    kprint(kp_fgBlue|kp_s, "Maybe: ", kp_fgGreen|ktid_MaybePtr, &m);
    ```  
    output: <span style="color:blue">Maybe:</span> <span style="color:lightgreen">{value: { Pointer, 0x0 }}</span>
