--[[
	Test file for all the different kind of values that
	luadata can handle.
]]--
float_value = 3.14159
integer_value = 42
string_value = "test"
boolean_value = true
nil_value = nil
table_empty_value = {}
table_list_string_value = {"a", "b", "c"}
table_list_mixed_value = {12, "a", true}
table_assoc_value = {a="A", b="B", c="C"}
table_two_layers = {one = {a = "1", b = "2", c = "3"}, two = {d = "4", e = "5", f = "6"}}
function_value = function()
	return 42
end
function_returns_a_function = function()
	f = function()
		return 666
	end
	return f
end
function_three_layers = function()
	return function()
		return function()
			return 777
		end
	end
end
function_returns_table = function()
	return {
		foo = "bar",
		fuu = "baz"
	}
end
table_with_functions = {
	pom = function() return "pim" end,
	pam = function() return "poum" end
}
table_with_functions_mixed = {
	one = function()
		return {a = 1, b = 2, c = 3}
	end,
	two = {
		foo =
		function()
			return {
				fu = 42,
				fo = "toto"
			}
		end,
		bar = 55
	}
}
mixed_table = { foo = "bar", "baz" }
function_with_arg_value = function(x, y)
	return (2 * x) + (4 * y)
end
time_value = function()
	integer_value = integer_value + 1
	return os.date("%A %B %d %Y, at %I:%M:%S%p")
end
