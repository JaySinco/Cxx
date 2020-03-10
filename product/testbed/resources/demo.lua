-- defines a factorial function
function fact(n)
    if n == 0 then
        return 1
    else
        return n * fact(n - 1)
    end
end

log(fact(5))
return {fact(6), fact(7), {a = 10, b = "test"}}
