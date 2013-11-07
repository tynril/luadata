table_empty = {}
table_simple = {1, 2, 3}
table_assoc = {a = "A", b = "B", c = "C"}
table_nested = {{1, 2}, {3, 4}, {5, 6}}
table_nested_assoc = {a = {"a", "A"}, b = {"b", "B"}, c = {"c", "C"}}
table_deep = {
	foo = {
		bar = {
			baz = "qux"
		}
	}
}
table_deep_mixed = {
	{
		foo = { 1, 2, 3 },
		bar = { 1.23, 4.56, 7.89 },
		baz = {
			qux = "QUX",
			quux = "QUUX",
			corge = { 2, 4, 6, 8 },
			grault = { 42 }
		}
	},
	{
		"A", "B", "C"
	}
}
