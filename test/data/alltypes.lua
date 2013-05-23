--[[
	Test file for all the different kind of values that
	luadata can handle.
]]--
time_value = function() return os.date("%A %B %d %Y, at %I:%M:%S%p") end
float_value = 3.14159
integer_value = 42
string_value = "test"
boolean_value = true
nil_value = nil
table_empty_value = {}
table_list_string_value = {"a", "b", "c"}
table_list_mixed_value = {1, "a", true}
table_assoc_value = {a="A", b="B", c="C"}
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
function_with_arg_value = function(x)
	return x * x
end
