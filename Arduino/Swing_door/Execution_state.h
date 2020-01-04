
#ifndef Execution_state_h
#define Execution_state_h

/// <summary>
/// Enum with the different expected execution states that can be returned by specific methods or procedures
/// </summary>
typedef enum ExecutionState {
	EXE_ERROR,
	EXE_OK,
	EXE_ABORT,
	EXE_WARNING,
	EXE_WAIT,
} ExecutionState;
//----------------------------------------------------------------------------------------------

#endif /* Execution_state_types_h */