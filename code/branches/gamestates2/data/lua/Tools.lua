function handleDefArg(argument, defaultValue)
    if argument == nil then
        return defaultValue
    else
        return argument
    end
end

function table.findIndex(table, value)
    for i, v in ipairs(table) do
        if v == value then
            return i
        end
    end
    return 0
end

