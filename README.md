**ROPTool** - *A useful tool for ROP*

ROPTool is an interesting technical tool that allows developers to utilise a simple scripting language to create ROP application. Normally, ROP is used to disable write/execute protections either by completely disabling the feature or using allocatable executable memory typically used in JIT devices. Due to this, ROP isn't really considered a development environment itself and thus there is a lack of tools to do general purpose application development.

This README will act as a short tutorial explaining how to use ROPTool and the components it consists of.

ROPTool in its most primitive of usages requires a source code, an output file and a target.

###**[Targets]**
Targets in ROPTool are currently a collection of XML files that describe the gadget and gadgetmaps available to the code generator. ROPTool gets its targets from the pkg directory adjacent to the executable. In their each target is within its own folder.

Example layout

    roptool.exe
    pkg/
        mytarget/
                manifest.xml
                gadgets/
                        gadget1.xml
                gadgetmaps/
                        gadgetmap1.xml
                        

The above example layout describes a typical target layout. The target must include a manifest.xml which details the architecture requirements of the generated code. Then gadgets and gadgetmaps are within their own folders. The layout of these XML files are detailed below.

###**[Manifest]**
The manifest.xml file details the target architecture. It has 4 required tags: `<version>`, `<arch_bitlen>`, `<stack_alignment>` and `<caller_gadget>`.

`<version>` is simply the version of the target layout. Currently, this should always be 1.0 and is implemented so it can be extended later, if needed.

`<arch_bitlen>` is the number of bits of the word length. This allows roptool to support upto 64 bit systems.

`<stack_alignment>` allows roptool to automatically pad the stack to a certain alignment in addressing. As long as the payload is loaded to this aligned address it is guaranteed that every gadgetmap's stack address will initially be aligned.

`<caller_gadget>` is the glue of the application. This can be the "NOP" operation that does nothing except execute the next address on the stack. On most systems this will be some variant of "POP PC". The actual data is the name of the gadget file in the gadgets/ folder minus the .xml extension.

Example:

    <version>1.0</version>
    <arch_bitlen>32</arch_bitlen>
    <stack_alignment>0x8</stack_alignment>
    <caller_gadget>pop_pc</caller_gadget>

###**[Gadgets]**
ROP defines gadgets as useful snippets of code that does a short piece of work. An example can be this:

    POP { PC }

This gadget doesn't perform anything useful but provides what is essentially a NOP instruction. This gadget can be used to pad out the stack and work as a *caller_gadget* in the manifest.xml. In ROPTool gadgets are simply defined like this:

    <gadget>
    	<address>0x12345678</address>
    </gadget>

Gadgets are referenced by filename minus the extension. That is to say, when you need to reference a gadget when creating a target you would identify "*pop_pc.xml*" as "*pop_pc*".

###**[Gadgetmaps]**
Gadgetmaps are the core of operation of roptool. They describe how gadgets are linked together and what functionality this linkage provides. They also describe the layout of the stack.
An example of a gadget map is shown below:

    <gadgetmap>
    	<!-- Match any amount of arguements that are values -->
    	<regex>.*(\(v{0,9}\))</regex>
    	<stack>
            <data>GADGET_ADDRESS(pop_pc)</data>
            <data>GADGET_ADDRESS(pop_r4_r9)</data>
    		<data>ARG1</data> 		    								<!-- R4 -->
    		<data>ARG0</data>							 		        <!-- R5 -->
    		<data>GADGET_ADDRESS(jump_r7_pop_r4_r8)</data>			    <!-- R6 -->
    		<data>FUNC_ADDRESS</data>		                            <!-- R7 -->
    		<data>ARG3</data>											<!-- R8 -->
    		<data>ARG2</data>											<!-- R9 -->
    		<data>GADGET_ADDRESS(jump_r6_set_arg_r0_r3)</data>			<!-- PC -->
    		<data>ARG4</data>												<!-- R4 -->
    		<data>ARG5</data>												<!-- R5 -->
    		<data>ARG6</data>												<!-- R6 -->
    		<data>ARG7</data>												<!-- R7 -->
    		<data>ARG8</data>												<!-- R8 -->
    		<data>RETURN_ADDRESS</data>									<!-- PC return -->
    	</stack>
    </gadgetmap>

The `<regex>` string defines the parameters that the gadgetmap can handle. In this case the regex will match any function name with upto a maximum of 9 "v" in parenthesis. This is different to appreciate without knowing how the scripting language works. In basic:
    v = any sort of constant value. String, numbers fall into this category.
    r = the return value. Only 1 of these are allowed the regex.
    l = in-line load. This value must be dereferenced before operation.

So the above gadgetmap supports any function as long as it only as upto a maximum of 9 constant values. A gadgetmap doesn't need to be vastly generic and support everything, its recommended to start off with generic-like maps but once tools are developed more specific gadgets can be generated. In cases where a function like: "*myfunc(vv)*" have two applicable gadgetmaps, the compiler will pick the smallest map.

In addition, gadgetmaps can have an optional <function> tag. This means that scripts can directly invoke a gadgetmap if it performs a very specific function.

The <stack> elements contains a number of <data> elements and describes the memory layout of the gadget. each <data> element can either contain: a constant number, a **GADGET_ADDRESS**, a **FUNC_ADDRESS**, a **RETURN_ADDRESS** or an argument in the form of **ARGX**, where X is the argument number.

**GADGET_ADDRESS** is how you will directly reference a gadget. When you do this, the address of the gadget take its place during compilation.

**FUNC_ADDRESS** is there to represent the address of the function that is to be called. This is not used when the gadgetmap has the `<function>` field available.

**RETURN_ADDRESS** is where the compiler will insert the address of the caller_gadget to link up the next map.

**ARGX** represents the arguments passed through. ARG0 would be the first argument passed, ARG1 the second and so forth. If there is not an argument available a default value will take its place.

###**[ropscript]**
The scripting language for roptool is relatively straightforward. Currently a file is laid out as follows:

    [DATA]
        [FUNCTIONS]
        [SYMBOLS]
    [/DATA]
    [CODE]
        [FUNCTION CALLS]
    [/CODE]

Starting with the data section, the syntax is simply:

    data
    {
        [FUNCTIONS]
        [SYMBOLS]
    }

The `[FUNCTIONS]` is the list of function addresses to be used. If you want to invoke printf and it is located at address 0xDEADBEEF it is laid out as:

    func printf = 0xDEADBEEF;

The `[SYMBOLS]` is the list of symbols that define constants. This is useful to name memory locations and const values.

    symbol MYINT = 1;
    symbol SOME_HARDWARE_ADDRESS = 0xBADCAFE;

The symbols support assignment to any expression. This include other symbols.

    symbol MYINT2 = MYINT + 1;

Any sort of numerical arithmetic can be used as long as the values are constant.

Below is an example of the entire data section:

    data
    {
        func printf = 0xDEADBEEF;
        symbol MYINT = 1;
        symbol SOME_HARDWARE_ADDRESS = 0xBADCAFE;
        symbol MYINT2 = MYINT + 1;
    }

####**[Code section]**

The code section is very straightforward. The opening brace is simply:

    code : entry
    {
        [FUNCTION CALLS]
    }

The `: entry` is currently used for naming functions, but multiple ropscript functions are not *yet* supported.

####**[Function Calls]**

Function calls in ropscript are purposely kept simple. There is no assignment in the language.

Each function call uses a function from a gadgetmap or data section. Using the data section described above a function call to printf would simply like:

    printf("MYSTRING %i\n", MYINT);
    
The compiler will automatically resolve everything into gadgetmaps and take the string and put it into a Data Section. This is not the same as the ropscript data section, the Data Section is a bunch of data that must be loaded to an address specified in the command lines --link parameter. Strings are then substituted for this address and treated as a "v" value to the gadgetmap.

####**[Return values]**
Since there is no assignment in ropscript, return values have their own keyword `RET`. Consider we have the functions `add(x, y)`, `mult(x, y)` and `send(x)`. In C, if we wanted to add and multiply a value to send we'd simply do this:

    send((x+y)*z);
    
Whilst this is true for ropscript, it is only true if x, y and z are **ALL** constant. If any one of them is calculated from another function we need to have another approach. This is how ropscript would deal with it if z is not constant:

    mult(x+y, LOAD[z]);
    send(RET);


notice the use of `LOAD[z]`, this is because symbols can only represent constant values. If you use symbol to represent an address to store the value, you can use LOAD[symbol] to treat it like a variable.

as you can see, the keyword `RET` is passed to send. This represents the return from the call before. As such, `RET` is undefined on the first function call in the code section and the duration that `RET` is valid is only 1 call. If you need to preserve a return value you must save it to memory or use it in the next instruction. This is invalid:

    mult(x+y, LOAD[z]);
    somethingelse();
    send(RET);

Even if `somethingelse()` formally doesn't return any values, the value of `RET` is _undefined_ for send. A way to deal with this is:

    mult(x+y, LOAD[z]);
    store(RET, sum);
    somethingelse();
    send(LOAD[sum]);
    
So entirely a ropscript would look like:

    data
    {
        func printf = 0xDEADBEEF;
        symbol MYINT = 1;
        symbol SOME_HARDWARE_ADDRESS = 0xBADCAFE;
        symbol MYINT2 = MYINT + 1;
    }
    code : entry
    {
        printf("HELLO WORLD %i\n", MYINT);
    }

###**[Extras]**
`--sled` option: allows to prepend a sled consisting of caller_gadgets. You define the size of the sled and the code generator will produce this.

`--aslr`: WIP
