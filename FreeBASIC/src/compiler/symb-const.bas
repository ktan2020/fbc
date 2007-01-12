''	FreeBASIC - 32-bit BASIC Compiler.
''	Copyright (C) 2004-2007 The FreeBASIC development team.
''
''	This program is free software; you can redistribute it and/or modify
''	it under the terms of the GNU General Public License as published by
''	the Free Software Foundation; either version 2 of the License, or
''	(at your option) any later version.
''
''	This program is distributed in the hope that it will be useful,
''	but WITHOUT ANY WARRANTY; without even the implied warranty of
''	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
''	GNU General Public License for more details.
''
''	You should have received a copy of the GNU General Public License
''	along with this program; if not, write to the Free Software
''	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA.


'' symbol table module for constants
''
'' chng: sep/2004 written [v1ctor]
''		 jan/2005 updated to use real linked-lists [v1ctor]


#include once "inc\fb.bi"
#include once "inc\fbint.bi"
#include once "inc\hash.bi"
#include once "inc\list.bi"
#include once "inc\ir.bi"

'':::::
function symbAddConst _
	( _
		byval symbol as zstring ptr, _
		byval dtype as integer, _
		byval subtype as FBSYMBOL ptr, _
		byval value as FBVALUE ptr, _
		byval attrib as integer = FB_SYMBATTRIB_NONE _
	) as FBSYMBOL ptr static

    dim as FBSYMBOL ptr sym

    function = NULL

    sym = symbNewSymbol( FB_SYMBOPT_DOHASH, _
    					 NULL, _
    					 NULL, NULL, _
    					 FB_SYMBCLASS_CONST, _
    				   	 symbol, NULL, _
    				   	 dtype, subtype, 0, attrib )
	if( sym = NULL ) then
		exit function
	end if

	sym->con.val = *value

	function = sym

end function

'':::::
function symbAllocFloatConst _
	( _
		byval value as double, _
		byval dtype as integer _
	) as FBSYMBOL ptr static

    static as zstring * FB_MAXINTNAMELEN+1 id, id_alias
	dim as FBSYMBOL ptr s
	dim as FBARRAYDIM dTB(0)
	dim as string svalue

	function = NULL

	'' can't use STR() because GAS doesn't support the 1.#INF notation
	svalue = hFloatToStr( value, dtype )

	id = "{fbnc}"
	id += svalue

	'' preserve case, 'D', 'd', 'E', 'e' will become 'e' in lexer
	s = symbLookupByNameAndSuffix( @symbGetGlobalNamespc( ), _
								   @id, dtype, TRUE )
	if( s <> NULL ) then
		return s
	end if

	id_alias = *hMakeTmpStr( FALSE )

	'' it must be declare as SHARED, because even if currently inside an
	'' proc, the global symbol tb should be used, so just one constant
	'' will be ever allocated over the module
	s = symbAddVarEx( @id, @id_alias, dtype, NULL, 0, 0, 0, dTB(), _
					  FB_SYMBATTRIB_SHARED or FB_SYMBATTRIB_LITCONST, _
					  FB_SYMBOPT_MOVETOGLOB or FB_SYMBOPT_ADDSUFFIX or FB_SYMBOPT_PRESERVECASE )

	''
	s->var_.littext = ZstrAllocate( len( svalue ) )
	*s->var_.littext = svalue

	function = s

end function

'':::::
function symbAllocStrConst _
	( _
		byval sname as zstring ptr, _
		byval lgt as integer _
	) as FBSYMBOL ptr static

    static as zstring * FB_MAXINTNAMELEN+1 id, id_alias
	dim as FBSYMBOL ptr s
	dim as FBARRAYDIM dTB(0)
	dim as integer strlen

	function = NULL

	'' the lgt passed isn't the real one because it doesn't
	'' take into acount the escape characters
	strlen = len( *sname )
	if( lgt < 0 ) then
		lgt = strlen
	end if

	if( strlen <= FB_MAXNAMELEN-6 ) then
		id = "{fbsc}"
		id += *sname
	else
		id = *hMakeTmpStr( FALSE )
	end if

	''
	s = symbLookupByNameAndClass( @symbGetGlobalNamespc( ), _
								  @id, FB_SYMBCLASS_VAR, TRUE )
	if( s <> NULL ) then
		return s
	end if

	id_alias = *hMakeTmpStr( FALSE )

	'' lgt += the null-char (rtlib wrappers will take it into account)

	'' it must be declare as SHARED, see symbAllocFloatConst()
	s = symbAddVarEx( @id, @id_alias, FB_DATATYPE_CHAR, NULL, _
					  0, lgt + 1, 0, dTB(), _
					  FB_SYMBATTRIB_SHARED or FB_SYMBATTRIB_LITCONST, _
					  FB_SYMBOPT_MOVETOGLOB or FB_SYMBOPT_PRESERVECASE )

	''
	s->var_.littext = ZstrAllocate( strlen )
	*s->var_.littext = *sname

	function = s

end function

'':::::
function symbAllocWStrConst _
	( _
		byval sname as wstring ptr, _
		byval lgt as integer _
	) as FBSYMBOL ptr static

    static as zstring * FB_MAXINTNAMELEN+1 id, id_alias
	dim as FBSYMBOL ptr s
	dim as FBARRAYDIM dTB(0)
	dim as integer strlen

	function = NULL

	'' the lgt passed isn't the real one because it doesn't
	'' take into acount the escape characters
	strlen = len( *sname )
	if( lgt < 0 ) then
		lgt = strlen
	end if

	'' hEscapeW() can use up to 4 chars p/ unicode char (\ooo)
	if( strlen * (3+1) <= FB_MAXNAMELEN-6 ) then
		id = "{fbwc}"
		id += *hEscapeW( sname )
	else
		id = *hMakeTmpStr( FALSE )
	end if

	''
	s = symbLookupByNameAndClass( @symbGetGlobalNamespc( ), _
								  @id, FB_SYMBCLASS_VAR, TRUE )
	if( s <> NULL ) then
		return s
	end if

	id_alias = *hMakeTmpStr( FALSE )

	'' lgt = (lgt + null-char) * sizeof( wstring ) (see parser-decl-symbinit.bas)
	'' it must be declare as SHARED, see symbAllocFloatConst()
	s = symbAddVarEx( @id, @id_alias, FB_DATATYPE_WCHAR, NULL, _
					  0, (lgt+1) * len( wstring ), 0, dTB(), _
					  FB_SYMBATTRIB_SHARED or FB_SYMBATTRIB_LITCONST, _
					  FB_SYMBOPT_MOVETOGLOB or FB_SYMBOPT_PRESERVECASE )

	''
	s->var_.littextw = WstrAllocate( strlen )
	*s->var_.littextw = *sname

	function = s

end function

'':::::
sub symbDelConst _
	( _
		byval s as FBSYMBOL ptr _
	)

    if( s = NULL ) then
    	exit sub
    end if

    '' if it's a string, the symbol attached will be deleted be delVar()

	symbFreeSymbol( s )

end sub

'':::::
function symbGetConstValueAsStr _
	( _
		byval s as FBSYMBOL ptr _
	) as string

  	select case as const symbGetType( s )
  	case FB_DATATYPE_STRING, FB_DATATYPE_FIXSTR, FB_DATATYPE_CHAR
  		function = *symbGetConstValStr( s )->var_.littext

  	case FB_DATATYPE_LONGINT
  		function = str( symbGetConstValLong( s ) )

  	case FB_DATATYPE_ULONGINT
  	    function = str( cunsg( symbGetConstValLong( s ) ) )

  	case FB_DATATYPE_SINGLE, FB_DATATYPE_DOUBLE
  		function = str( symbGetConstValFloat( s ) )

  	case FB_DATATYPE_LONG
  		if( FB_LONGSIZE = len( integer ) ) then
  			function = str( symbGetConstValInt( s ) )
  		else
  			function = str( symbGetConstValLong( s ) )
  		end if

  	case FB_DATATYPE_ULONG
  	    if( FB_LONGSIZE = len( integer ) ) then
  	    	function = str( cunsg( symbGetConstValInt( s ) ) )
  	    else
  	    	function = str( cunsg( symbGetConstValLong( s ) ) )
  	    end if

  	case FB_DATATYPE_UBYTE, FB_DATATYPE_USHORT, FB_DATATYPE_UINT
  		function = str( cunsg( symbGetConstValInt( s ) ) )

  	case else
  		function = str( symbGetConstValInt( s ) )
  	end select

end function

'':::::
function symbCloneConst _
	( _
		byval sym as FBSYMBOL ptr _
	) as FBSYMBOL ptr

	'' no need to make a copy of fbvalue.str, if it's a literal,
	'' it will be a non-local var

	function = symbAddConst( NULL, _
							 symbGetType( sym ), _
							 symbGetSubType( sym ), _
							 @sym->con.val, _
							 symbGetAttrib( sym ) )

end function
