--[[
	Test file for all the different kind of values that
	luadata can handle.
]]--
floating_value = 3.14159
double_value = 3.1415926535897932384626433832795
integer_value = 42
string_value = "test"
boolean_value = true
nil_value = nil
table_empty_value = {}
table_list_string_value = {"a", "b", "c"}
table_list_mixed_value = {1, "a", true}
table_assoc_value = {a="A", b="B", c="C"}
function_value = function() return 42 end
function_with_arg_value = function(x) return x * x end
