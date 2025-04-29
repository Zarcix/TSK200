#ifndef INSTRUCTION
#define INSTRUCTION

/**
	OP Code Definition
		- All opcodes are defined below
		- If an opcode is added or the order changes, make sure to change the string equivalent and the count
		- ORDER MATTERS HERE
*/
typedef enum {
	// Math
	SUB,
	ADD,
	NEG,
	NOP,

	// Jump
	JEZ,
	JMP,
	JNZ,
	JGZ,
	JLZ,
	JRO,

	// Register
	SAV,
	SWP,

	// Movement
	MOV,
	LABEL,

	// Other
	EXT,
} OPCode;

static const char* OPCODE_AS_STR[] = {
	"SUB",
	"ADD",
	"NEG",
	"NOP",
	"JEZ",
	"JMP",
	"JNZ",
	"JGZ",
	"JLZ",
	"JRO",
	"SAV",
	"SWP",
	"MOV",
	"LABEL",
	"EXT"
};

static const int OPCODE_COUNT = 15;

/**
 * Ports for a given operation
 * 
 * Three Types:
 * - Directionals: Used for moving elements around
 * - Specials: Special pseudo operations
 * - Register: Interact with register
 * 
 * If a change is done, make sure to reflect that change in the string definitions and count. 
 * ORDER MATTERS
 */
typedef enum {
	// Directionals
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ANY,

	// Special
	NIL,
	LAST,

	// Register
	ACC,
} Port;

static const Port EXTERNAL_PORT_LIST[] = {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

static const int EXTERNAL_PORT_COUNT = 4;

static const char *PORT_AS_STR[] = {
	"LEFT",
	"RIGHT",
	"UP",
	"DOWN",
	"ANY",

	"NIL",
	"LAST",

	"ACC"
};

static const int PORT_COUNT = 8;

/**
	Available Data Types
		- This covers all available data types for use in <SRC> or <DEST> in a given command
		- When adding a new data type, make sure to update the union too
*/
typedef enum {
	NUMBER,
	PORT,
	STRING,
} DataType;

typedef struct {
	union {
		char* dataStr;
		int dataVal;
		Port dataPort;
	} value;
	DataType type;
} Data;

/**
 * Represents a valid instruction
 * 
 * @param operation: Represents an operation that will be performed
 * 
 * @param src: Represents an object that will be read from
 * 
 * @param dest: Represents an object that will be written to
 */
typedef struct {
	OPCode operation;
	Data src;
	Data dest;
} Instruction;

OPCode str_to_opcode(char* str);
Data str_to_data(char* str);
Port str_to_port(char* str);

Port reverse_port(Port port);

#endif
