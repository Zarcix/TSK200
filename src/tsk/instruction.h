#ifndef INSTRUCTION
#define INSTRUCTION

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
	LABEL,
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
} Port;

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

#endif
