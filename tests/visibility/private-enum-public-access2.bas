' TEST_MODE : COMPILE_ONLY_FAIL

type T
	as integer i
	private:
	enum E
		MYCONST = 123
	end enum
end type

dim as integer i
i = T.E.MYCONST
