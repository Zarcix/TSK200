#ifndef OP_CONSTANTS
#define OP_CONSTANTS

/**
 * Operation codes
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

	// Other
	OPLBL,
} OPCode;

/**
 * Directional locations for operations
 * 
 * Three Types:
 * - Directionals: Used for moving elements around
 * - Specials: Special pseudo operations
 * - Register: Interact with register
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
} DirectionalLocation;

static const int AnyOrderCount = 4;

static const DirectionalLocation AnyOrder[4] = {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

typedef enum {
	VALUE,
	LOCATION,
	LABEL
} DataType;

typedef struct {
	union {
		char* label;
		int dataValue;
		DirectionalLocation nodeValue;
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

DirectionalLocation opposite_of_directional(DirectionalLocation originalDirection);

OPCode string_to_opcode(char* inputString);
DirectionalLocation string_to_direction(char* inputString);

bool opcode_is_jump_instruction(OPCode operation);

#endif
