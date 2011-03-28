-- Generate binding code
-- Written by Waldemar Celes
-- TeCGraf/PUC-Rio
-- Jul 1998
-- Last update: Apr 2003
-- $Id: $


-- This code is free software; you can redistribute it and/or modify it.
-- The software provided hereunder is on an "as is" basis, and
-- the author has no obligation to provide maintenance, support, updates,
-- enhancements, or modifications.

function parse_extra()

    for k,v in ipairs(_extra_parameters or {}) do
        
        local b,e,name,value = string.find(v, "^([^=])=(.*)$")
        if b then
            _extra_parameters[name] = value
        else
            _extra_parameters[v] = true
        end
    end
end

function doit ()
    -- define package name, if not provided
    if not flags.n then
        if flags.f then
            flags.n = gsub(flags.f,"%..*$","")
            _,_,flags.n = string.find(flags.n, "([^/\\]*)$")
        else
            error("#no package name nor input file provided")
        end
    end

    -- parse table with extra paramters
    parse_extra()

    -- get potential working directory
    if not flags.w then
        flags.w = ''
    end

    -- do this after setting the package name
    if flags.L then
        if string.sub(flags.L, 1, 1) == '/' or string.sub(flags.L, 1, 1) == '\\' or (string.len(flags.L) > 1 and string.sub(flags.L, 2, 2) == ':') then
            dofile(flags.L)
        else
            dofile(flags.w..'/'..flags.L)
        end
    end

    -- add cppstring
    if not flags.S then
        _basic['string'] = 'cppstring'
        _basic['std::string'] = 'cppstring'
        _basic_ctype.cppstring = 'const char*'
    end

    -- proccess package
    local file
    if flags.f then
        if string.sub(flags.f, 1, 1) == '/' or string.sub(flags.f, 1, 1) == '\\' or (string.len(flags.f) > 1 and string.sub(flags.f, 2, 2) == ':') then
            file = flags.f
        else
            file = flags.w..'/'..flags.f
        end
    else
        file = flags.f
    end
    local p  = Package(flags.n, file)

    if flags.p then
        return        -- only parse
    end

    if flags.o then
        local file
        if string.sub(flags.o, 1, 1) == '/' or string.sub(flags.o, 1, 1) == '\\' or (string.len(flags.o) > 1 and string.sub(flags.o, 2, 2) == ':') then
            file = flags.o
        else
            file = flags.w..'/'..flags.o
        end
        local st,msg = writeto(file)
        if not st then
            error('#'..msg)
        end
    end

    p:decltype()
    if flags.P then
        p:print()
    else
        p:preamble()
        p:supcode()
        p:register()
        push(p)
        post_output_hook(p)
        pop()
    end

    if flags.o then
        writeto()
    end

    -- write header file
    if not flags.P then
        if flags.H then
            local file
            if string.sub(flags.H, 1, 1) == '/' or string.sub(flags.H, 1, 1) == '\\' or (string.len(flags.H) > 1 and string.sub(flags.H, 2, 2) == ':') then
                file = flags.H
            else
                file = flags.w..'/'..flags.H
            end
            local st,msg = writeto(file)
            if not st then
                error('#'..msg)
            end
            p:header()
            writeto()
        end
    end
end

