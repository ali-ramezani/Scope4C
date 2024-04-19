# Scope for C
Unlike C++ we don't destructor concept in C which is called when the object is out of the scope. 
The main issue is that user must be careful any allocated data must be freed. 
Moreover if for any reason you leave the function you must fee all already allocated data before leaving (RAII and unwinding).
The following easy tricks will help developers to:

- Have arbitrary scope that you can add your allocated memory address to it and it will free all of them when you leave the scope.
- By introducing Scope macros you can be sure the scope will be freed.
- If you allocate memory for a variable more than once it will assure that no dangled memory created. 
- You can have scope and sub scopes.
- Basically you just allocated and it will be deleted in the end of scope 

Simple usage would be like this:
~~~C++

    X_SCOPE_BEGIN

        int* a;
        X_malloc_local(int, a, 10);

        X_malloc_local(int, a, 10);//Previous memory of a already kept in the scope

        float* b;
        X_malloc_local(float, b, 10);

        
        X_SCOPE_BEGIN_FOR(1)
            X_malloc_local(float, b, 10);//address of b newly allocated and deallocated
        X_SCOPE_END

        b = X_SCOPE_LAST(0);//It will recover the memory of b

        float* d = GetAllocatedData(5);
        X_free_local(d);

    X_SCOPE_END

~~~

You can also use the scope without higher level macros like following. It is useful when 
~~~ 