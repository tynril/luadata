function_simple = function()
	return 42
end
function_nested = function()
	f = function()
		return 43
	end
	return f
end
function_nested_deeper = function()
	return function()
		return function()
			return function()
				return function()
					return function()
						return 44
					end
				end
			end
		end
	end
end
function_returns_table = function()
	return { "A", "B", "C" }
end
function_in_table = {
	pom = function() return "pim" end,
	pam = function() return "poum" end
}
function_with_arg = function(x, y)
	return (2 * x) + (4 * y)
end
global_test = 0
function_with_global = function()
	global_test = global_test + 1
	return global_test
end
function_within_deep_table = {
	one = function()
		return {a = 1, b = 2, c = 3}
	end,
	two = {
		foo = function()
			return { fu = 42, fo = "toto" }
		end,
		bar = 55
	}
}
