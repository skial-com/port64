#if defined __sm_port64_ext_included
	#endinput
#endif
#define __sm_port64_ext_included

const NumberType NumberType_Int64 = view_as<NumberType>(3);

// we can't use enum structs in natives, "any var[2]" will be compatible with this though
enum struct Address64
{
	int low;
	int high;
}

// returns how many bytes a pointer has
native int Port64_PointerBytes();

native void Port64_GetEntityAddress(int entity, any outputAddr[2]);

// conversions with pseudoaddress
native void Port64_FromPseudoAddress(Address pseudo, any outputAddr[2]);
native Address Port64_ToPseudoAddress(any addr[2]);

// load and store to address. offset is a 32 bit int for pointer arithmetic purposes
native void Port64_LoadFromAddress(any addr[2], int offset, NumberType type, any outputValue[2]);
native void Port64_StoreToAddress(any addr[2], int offset, NumberType type, any input[2]);

// operations
native void Port64_Add(any a[2], any b[2], any output[2]);
native void Port64_Sub(any a[2], any b[2], any output[2]);
native void Port64_Mul(any a[2], any b[2], any output[2]);
native void Port64_Div(any a[2], any b[2], any output[2]);

public Extension __ext_sm_port64 = 
{
	name = "port64",
	file = "port64.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public void __ext_sm_port64_SetNTVOptional()
{
    MarkNativeAsOptional("Port64_PointerBytes");
    MarkNativeAsOptional("Port64_GetEntityAddress");
    MarkNativeAsOptional("Port64_FromPseudoAddress");
    MarkNativeAsOptional("Port64_ToPseudoAddress");
    MarkNativeAsOptional("Port64_LoadFromAddress");
    MarkNativeAsOptional("Port64_StoreToAddress");
    MarkNativeAsOptional("Port64_Add");
    MarkNativeAsOptional("Port64_Sub");
    MarkNativeAsOptional("Port64_Mul");
    MarkNativeAsOptional("Port64_Div");
}
#endif
